///////////////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Demo for CSE687 Project #4, Spring 2015                         //
// - build as static library showing how C++\CLI client can use native code channel  //
// - MockChannel reads from sendQ and writes to recvQ                                //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "MockChannel.h"
#include"../HttpSender/HttpSender.h"
#include"../HttpReceiver/HttpReceiver.h"
#include <string>
#include <thread>
#include <iostream>

using BQueue = BlockingQueue<Message>;
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
  void postMessage(const Message& msg);
  BQueue& queue();
private:
  BQueue sendQ_;
};

void Sendr::postMessage(const Message& msg)
{
  sendQ_.enQ(msg);
}

BQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
  Message getMessage();
  BQueue& queue();
private:
  BQueue recvQ_;
};

Message Recvr::getMessage()
{
  return recvQ_.deQ();
}

BQueue& Recvr::queue()
{
  return recvQ_;
}
/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
  MockChannel(ISendr* pSendr, IRecvr* pRecvr);
  void startSender(string url);
  void startReceiver(Message recvPth, string url);
  void stop();
private:
  string connectedServerUrl;
  std::thread threadSender_;
  std::thread threadReceiver_;
  ISendr* pISendr_;
  IRecvr* pIRecvr_;
  bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------
MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr)
{
	
}
//----< creates thread to read from sendQ and echo back to the recvQ >-------
void MockChannel::startSender(string url){
	std::cout << "\n  Sender MockChannel starting up: "<< url <<"\n";
	threadSender_ = std::thread(
		[this,url] {
		HttpSender sndr(url);
		sndr.startHttpSender();
		Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
		Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
		if (pSendr == nullptr || pRecvr == nullptr)
			 return;
		BQueue& sendQ = pSendr->queue();
		while (!stop_){
			std::cout << "\n  channel deQing message";
			Message msg = sendQ.deQ();  // will block here so send quit message when stopping
			if (msg.find("File Upload")!=std::string::npos){
				std::cout << "\n  Uploading the files";
				vector<string> files = StringHelper::split(StringHelper::split(msg, '|')[1], ';');
				string categoryName = StringHelper::split(msg, '|')[2];
				std::cout << "\n  got the catgory "<<categoryName;
				for (string file : files){
					std::cout << "\n  Uploading file: "<<file;
					sndr.sendMsg(file, true, connectedServerUrl, categoryName);}
				Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
				BQueue& recvQ = pRecvr->queue();
				recvQ.enQ(msg);}
			else if (msg.find("Server Connect") != std::string::npos){
				std::cout << "\n  Connecting to server";
				string url = StringHelper::split(msg, '|')[1];
				sndr.Connect(url);
				if (sndr.isConnected(url)){
					std::cout << "\n  Connected";
					connectedServerUrl = url;
					sndr.sendMsg(msg, false, connectedServerUrl);
					Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
					BQueue& recvQ = pRecvr->queue();
					recvQ.enQ(msg);}}
			else if (msg.find("DownLoad File") != std::string::npos){
				std::cout << "\n  Downloading the File";
				sndr.sendMsg(msg, false, connectedServerUrl);}
			else if (msg.find("Delete File") != std::string::npos){
				std::cout << "\n  Deleteing the File";
				sndr.sendMsg(msg, false, connectedServerUrl);}
			else if (msg.find("Deletehtml File") != std::string::npos){
				std::cout << "\n  Deleteing the File";
				sndr.sendMsg(msg, false, connectedServerUrl);}
			else
				sndr.sendMsg(msg, false, connectedServerUrl);}
		std::cout << "\n  Server stopping\n\n";});
}

void MockChannel::startReceiver(string url, Message recvPth)
{
	std::cout << "\n  Receiver MockChannel starting up : "<< url;
	threadReceiver_ = std::thread(
		[this, &recvPth, url] {
		HttpReceiver rcv(url, recvPth);
		rcv.startHttpReceiver();
		while (true)
		{
			HttpMessage msg = rcv.deQ();
			std::cout << "\n\n  server recvd message contents:\n" << msg.bodyString();
			std::string msgBody = msg.bodyString();
			if (msgBody.find("DownLoad File") != std::string::npos)
			{
				std::cout << "\n  File Downloaded.";
				Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
				BQueue& recvQ = pRecvr->queue();
				recvQ.enQ(msgBody);
			}
			else if (msgBody.find("Treeview") != std::string::npos || msgBody.find("HtmlFileView") != std::string::npos)
			{
				std::cout << "\n  File Downloaded.";
				Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
				BQueue& recvQ = pRecvr->queue();
				recvQ.enQ(msgBody);
			}
		}});
}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr) 
{ 
  return new MockChannel(pISendr, pIRecvr); 
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
