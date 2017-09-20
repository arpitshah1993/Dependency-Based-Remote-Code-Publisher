///////////////////////////////////////////////////////////////
// Cpp11-HttpReceiver.cpp - Receiver attached with socket       //
// ver 1.0                                                   //
// Arpit Shah, CSE687 - Object Oriented Design, Spring 2017 //
//////////////////////////////////////////////////////////////

#include "HttpReceiver.h"
using Attribute = HttpMessage::Attribute;
using Value = HttpMessage::Value;

//read the message
HttpMessage HttpReceiver::readMessage(Socket & socket)
{
	connectionClosed_ = false;
	HttpMessage msg;
	while (true){
		std::string attribString = socket.recvString('\n');
		if (attribString.size() > 1){
			HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);
			msg.addAttribute(attrib);
		}
		else
			break;
	}
	if (msg.attributes().size() == 0){
		connectionClosed_ = true;
		return msg;
	}
	if (msg.attributes()[0].first == "POST"){
		std::string filename = msg.findValue("file");
		if (filename != ""){
			size_t contentSize;
			std::string sizeString = msg.findValue("content-length");
			if (sizeString != "")
				contentSize = Converter<size_t>::toValue(sizeString);
			else
				return msg;
			readFile(filename, contentSize, socket);
		}
		if (filename != ""){
			msg.removeAttribute("content-length");
			std::string bodyString = "<file>" + filename + "</file>";
			std::string sizeString = Converter<size_t>::toString(bodyString.size());
			msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
			msg.addBody(bodyString);
		}
		else{
			size_t numBytes = 0, pos = msg.findAttribute("content-length");
			if (pos < msg.attributes().size()){
				numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
				Socket::byte* buffer = new Socket::byte[numBytes + 1];
				socket.recv(numBytes, buffer);
				buffer[numBytes] = '\0';
				std::string msgBody(buffer);
				msg.addBody(msgBody);
				delete[] buffer;
			}
		}
	}
	return msg;
}

//initiate receiver
void HttpReceiver::InitiateReceiver(string localUrl, string localFilePath)
{
	localPath = localFilePath;
	std::cout << "\n  Setting an Action at Receiver";
	Attribute attrib = HttpMessage::parseAttribute(localUrl);
	std::stringstream sstream(attrib.second);
	size_t portNumber;
	sstream >> portNumber;
	sl.setParam(portNumber);
	setAction();
}

//read the given filename from socket
bool HttpReceiver::readFile(const std::string & filename, size_t fileSize, Socket & socket)
{
	std::cout << "\n Receiving file: " << localPath + filename;
	std::string fqname = localPath+filename;
	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
		std::cout << "\n\n  can't open file " << fqname;
		return false;
	}
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	size_t bytesToRead;
	while (true)
	{
		if (fileSize > BlockSize)
			bytesToRead = BlockSize;
		else
			bytesToRead = fileSize;

		socket.recv(bytesToRead, buffer);

		FileSystem::Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);

		file.putBlock(blk);
		if (fileSize < BlockSize)
			break;
		fileSize -= BlockSize;
	}
	file.close();
	return true;
}

//Constructor
HttpReceiver::HttpReceiver(string localUrl, string localFilePath)
{
	std::cout << "\n Starting receiver at port: " << localUrl;
	localPath = localFilePath;
	Attribute attrib = HttpMessage::parseAttribute(localUrl);
	std::stringstream sstream(attrib.second);
	size_t portNumber;
	sstream >> portNumber;
	std::cout << "\n "<<portNumber<<"\n";
	sl.setParam(portNumber);
	setAction();
}

//start the Http receiver
void HttpReceiver::startHttpReceiver()
{
	sl.start(receiveAction);
}

//set the thread action on receiver queue
void HttpReceiver::setAction()
{
	receiveAction = [=](Socket socket) {
		while (true)
		{
			HttpMessage msg = readMessage(socket);


			if (connectionClosed_ || msg.bodyString() == "quit")
			{
				std::cout << "\n\n  clienthandler thread is terminating";
				break;
			}
			receiveQ.enQ(msg);
		}

	};
}

HttpReceiver::~HttpReceiver()
{
}
//enQ the msg in receiveQ
void HttpReceiver::enQ(HttpMessage msg)
{
	receiveQ.enQ(msg);
}

//deQ and return the msg
HttpMessage HttpReceiver::deQ()
{
	return receiveQ.deQ();
}

HttpReceiver::HttpReceiver()
{
}

#ifdef TEST_CLEINtRCV
int main()
{
	HttpReceiver rcv("localhost:8080","/Test");
	rcv.startHttpReceiver();
}
#endif