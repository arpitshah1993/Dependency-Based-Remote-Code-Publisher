///////////////////////////////////////////////////////////////
// Cpp11-HttpSender.cpp - Sender attached with socket         //
// ver 1.0                                                   //
// Arpit Shah, CSE687 - Object Oriented Design, Spring 2017 //
//////////////////////////////////////////////////////////////
#include "HttpSender.h"
using Attribute = HttpMessage::Attribute;
using Value = HttpMessage::Value;

HttpSender::HttpSender()
{
}

//HttpSender consturctor
HttpSender::HttpSender(string localURl)
{
	std::cout << "\n starting sender at port: " << localURl;
	localUrl = localURl;
	setAction();
}

//Initiate HttpSender with local Url
void HttpSender::InitiateHttpSender(string localURlt)
{
	localUrl = localURlt;
	setAction();
}

HttpSender::~HttpSender()
{
}

// check wther socket is connected or not
bool HttpSender::isConnected(string remoteUrl)
{
	return socketStore.find(remoteUrl) != socketStore.end();
}

//return the connect socket with remote Url
Socket& HttpSender::Connect(string remoteUrl)
{
	if (isConnected(remoteUrl))
		return socketStore[remoteUrl];
	Attribute attrib = HttpMessage::parseAttribute(remoteUrl);
	std::stringstream sstream(attrib.second);
	size_t portNumber;
	sstream >> portNumber;
	while (!si.connect("localhost", portNumber))
	{
		std::cout << "\n client waiting to connect";
		::Sleep(100);
	}
	socketStore[remoteUrl] = std::move(si);
	return std::move(si);
}

//set up sender thread on sender queue
void HttpSender::setAction()
{
	sendAction = [&]() {
		while (true)
		{
			try
			{
				bool shouldClose = false;
				HttpMessage smsg = sendQ.deQ();
				//std::string attrib = "toAddr";
				Value remoteUrl = smsg.findValue("toAddr");
				std::string str(smsg.body().begin(), smsg.body().end());
				if (str == "quit")
				{
					shouldClose = true;
				}
				Socket& sck = Connect(remoteUrl);
				std::string msgString = smsg.toString();
				sck.send(msgString.size(), (Socket::byte*)msgString.c_str());
				if (shouldClose)
					break;
			}

			catch (...)
			{
				continue;
			}
		}};
}

//start Http sender with its thread
void HttpSender::startHttpSender()
{
	std::thread HttpSenderThread(sendAction);
	HttpSenderThread.detach();
}

//send given content to url by wrapping up into HttpMessage
bool HttpSender::sendMsg(string content, bool file, string remoteUrl,string category)
{
	if (file)
	{
		std::string fqname = content;
		FileSystem::FileInfo fi(fqname);
		size_t fileSize = fi.size();
		std::string sizeString = Converter<size_t>::toString(fileSize);
		FileSystem::File file(fqname);
		file.open(FileSystem::File::in, FileSystem::File::binary);
		if (!file.isGood())
			return false;
		HttpMessage msg = makeMessage(1, "", remoteUrl);
		string fileName = category == "" ? FileSystem::Path::getName(content) : (category + "/" + FileSystem::Path::getName(content));
		msg.addAttribute(HttpMessage::Attribute("file", fileName));
		msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
		std::string msgString = msg.toString();
		Socket& socket = Connect(remoteUrl);
		socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
		const size_t BlockSize = 2048;
		Socket::byte buffer[BlockSize];
		while (true)
		{
			FileSystem::Block blk = file.getBlock(BlockSize);
			if (blk.size() == 0)
				break;
			for (size_t i = 0; i < blk.size(); ++i)
				buffer[i] = blk[i];
			socket.send(blk.size(), buffer);
			if (!file.isGood())
				break;
		}
		file.close();
		return true;
	}
	else
	{
		HttpMessage msg = makeMessage(1, content, remoteUrl);
		sendQ.enQ(msg);
	}
	return true;
}

//create message by wrapping up the content
HttpMessage HttpSender::makeMessage(size_t n, const std::string& body, const EndPoint& ep, string msgType)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	switch (n)
	{
	case 1:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("POST", "Message"));
		msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
		msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
		msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + localUrl));
		if (msgType != "")
			msg.addAttribute(HttpMessage::parseAttribute("Type:" + msgType));
		msg.addBody(body);
		if (body.size() > 0)
		{
			attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
			msg.addAttribute(attrib);
		}
		break;
	default:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
	}
	return msg;
}

//shut down the sender
void HttpSender::shutdown()
{
	HttpMessage msg;
	msg = makeMessage(1, "quit", localUrl);
	sendQ.enQ(msg);
}

#ifdef TEST_CLEINTSND
int main()
{
	HttpSender snd("localhost:8081");
	snd.startHttpSender();
	snd.sendMsg("Test Message", false, "localhost:8082");
}
#endif