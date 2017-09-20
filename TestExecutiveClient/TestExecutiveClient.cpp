///////////////////////////////////////////////////////////////
//  TestExecutiveClient.cpp - Demonstartions package    	  //
// ver 1.0                                                   //
// Arpit Shah,  CSE687 - Object Oriented Design, Spring 2017 //
///////////////////////////////////////////////////////////////

#include "TestExecutiveClient.h"


TestExecutiveClient::TestExecutiveClient()
{

}

TestExecutiveClient::~TestExecutiveClient()
{
}

//demonstarting req 1 and calling other demonstartion
void TestExecutiveClient::Requirement1()
{
	StringHelper::title("Demonstrating Req #1");
	std::cout << "\n  Examine code in Visual Studio to verify that I have used Visual Studio 2015 and its C++ Windows\n  console projects, as provided in the ECS computer labs.\n";
	Requirement2();
	Requirement3();
	Requirement4();;
	Requirement5();
	Requirement6();
	Requirement7();
	Requirement8();
	Requirement9();
	Requirement10();
	Requirement11();
}

//demonstarting req 2
void TestExecutiveClient::Requirement2()
{
	StringHelper::title("Demonstrating Req #2");
	std::cout << "\n  Examine code in Visual Studio to verify that I have used the C++ standard library's streams for all\n  I/O and new and delete for all heap-based memory management.\n";
}

//demonstarting req 3
void TestExecutiveClient::Requirement3()
{
	StringHelper::title("Demonstrating Req #3");
	std::cout << "\n  Examine code in Visual Studio to verify I provided a Repository program at server side that provides functionality to publish, as linked web pages, the contents of a set of C++ source code files. Check in your provided path to publish file in server where Server repository is publishing some files I have published as part of demonstrations.\n";
}

//demonstarting req 4
void TestExecutiveClient::Requirement4()
{
	StringHelper::title("Demonstrating Req #4");
	std::cout << "\n  Examine code in Visual Studio in my Server folder which calls the code for the publishing process which satisfied the requirements of CodePublisher developed in Project #3. Check in your provided path to publish files in server where Server repository is publishing some files I have published as part of demonstrations.\n";
}

//demonstarting req 5
void TestExecutiveClient::Requirement5()
{
	StringHelper::title("Demonstrating Req #5");
	std::cout << "\n  Examine the my GUI to verify all the requirements mentioned in Purpose section. I am executing all the functionality as part of DEMONSTRATION PROCESS below using my TestExecutiveClient.\n";
}

//demonstarting req 6
void TestExecutiveClient::Requirement6()
{
	StringHelper::title("Demonstrating Req #6");
	std::cout << "\n Verify my HttpSender and HttpReceiver around socket, which I am using in each client and server for communication purpose.\n";
}

//demonstarting req 7
void TestExecutiveClient::Requirement7()
{
	StringHelper::title("Demonstrating Req #7");
	std::cout << "\n  Verify that I have provided support for passing HTTP style messages using asynchronous messaging I have printed some of the = Httpmessage to demonstrate this requirement in DEMONSTRATION PROCESS below. \n";
}

//demonstarting req 8
void TestExecutiveClient::Requirement8()
{
	StringHelper::title("Demonstrating Req #8");
	std::cout << "\n  Check DEMONSTRATION PROCESs to verify that I am sending files to and from client and sever. Check my code in visual studio to verify that I am sending and receiving files in stream of bytes. \n";
}

//demonstarting req 9
void TestExecutiveClient::Requirement9()
{
	StringHelper::title("Demonstrating Req #9");
	std::cout << "\n Examine code in Visual Studio to verify that it includes an automated unit test \n executive client(TestExecutiveClient for me) that demonstrates I meet all the requirements\n  of this project.\n";
}

//demonstarting req 10
void TestExecutiveClient::Requirement10()
{
	StringHelper::title("Demonstrating Req #10");
	std::cout << "\n  Used a lazy download strategy, when presented with a name of a source code web page, will download that file and all the files it links to. Therfore, I am downloading selected local webpages instead of downloading the entire contents of the Code Publisher. Check DEMONSTRATION PROCESS and folder client repository(TestFileClient) in each Client's folder, which is the place where requested files got downloaded to verify I am using lazy download strategy.\n";
}

//demonstarting req 11
void TestExecutiveClient::Requirement11()
{
	StringHelper::title("Demonstrating Req #11");
	std::cout << "\n  Provided facility to provide publishing path on the commandline, to a virtual directory on the server. My publisher will directly publish files over there and open it in browser using IIS. While this demonstartion, it will open the file from the copy client have. If you want to open it from GUI, you have to put your IIS URL in GUI and click on Download Button in GUI.\n";
}

//demonstrate connection
void TestExecutiveClient::connect(HttpSender& sndr, HttpReceiver& rcv, string& serverAddress)
{
	std::cout << "\n  Client is trying to connect with server " << serverAddress << "\n";
	sndr.Connect(serverAddress);
	sndr.sendMsg("Server Connect", false, serverAddress);
}

//demonstrate uploading of the file
void TestExecutiveClient::upload(HttpSender& sndr, HttpReceiver& rcv, string& serverAddress)
{
	std::vector<std::string> currfiles = FileSystem::Directory::getFiles("TestExecutiveClient/TestFiles/files/", "*.*");
	for (string file : currfiles)
	{
		std::cout << "\n Uploading a file " << file << " to server's repository category \"Category1\".............REQUIREMENT #5\n";
		sndr.sendMsg(FileSystem::Path::getFullFileSpec("TestExecutiveClient/TestFiles/files/"+file), true, serverAddress, "Category1");
	}
}

//demonstrate downloading of the file
void TestExecutiveClient::download(HttpSender& sndr, HttpReceiver& rcv, string& serverAddress)
{
	string serveRepoPath = "Repository/Category1/";
	string fileName = "Grandparent.h";
	std::cout << "\n Sending download request for file " << fileName << " to server's repository category \"Category1\"............. which will download this file as well as its depedent files.REQUIREMENT #5\n";
	sndr.sendMsg("DownLoad File|" + serveRepoPath + fileName, false, serverAddress);
}


//demonstrate deletion of the code files from server
void TestExecutiveClient::deleteFile(HttpSender& sndr, HttpReceiver& rcv, string& serverAddress)
{
	string serveRepoPath = "Repository/Category1/";
	string fileName = "Grandparent.h";
	std::cout << "\n Sending delete request for file " << fileName << " which is in server's repository category \"Category1\".............REQUIREMENT #5\n";
	sndr.sendMsg("Delete File|" + serveRepoPath + fileName, false, serverAddress);
}

//demonstrate deletion of html code files from server
void TestExecutiveClient::deleteHtmlFile(HttpSender& sndr, HttpReceiver& rcv, string& serverAddress)
{
	string fileName = "Grandparent.h.htm;Child.h.htm;";
	std::cout << "\n Sending delete request for html file " << fileName << ".............REQUIREMENT #5\n";
	sndr.sendMsg("Deletehtml File|" + fileName, false, serverAddress);
}

#ifdef TEST_CLEINT
int main(int argc, char* argv[])
{   try {   if (argc = 3) {
			string portNumberClient = argv[1], portNumberServer = argv[2], clientAddress = "localhost:" + portNumberClient, serverAddress = "localhost:" + portNumberServer;
			StringHelper::Title("\n Starting client on port: " + portNumberClient);
			::putline();
			StringHelper::Title("\n Client repository: TestExecutiveClient/TestFilesClient/");
			::putline();
			HttpReceiver rcv("localhost:" + portNumberClient, "TestExecutiveClient/TestFilesClient/");
			rcv.startHttpReceiver();
			HttpSender sndr("localhost:" + portNumberClient);
			sndr.startHttpSender();
			TestExecutiveClient tst;
			StringHelper::Title("Testing - Project4 - S2017");
			StringHelper::title("Arpit Shah");
			::putline();
			tst.Requirement1();
			::putline(); ::putline();
			StringHelper::Title("DEMONSTRATIORN PROCESS Started");
			::putline();
			std::thread ListenThreadFromUI(
				[&](){
				while (true){
					HttpMessage msg = rcv.deQ();
					std::cout << "\n server recvd message contents:\n" << msg.bodyString()<< "\n" ;
				    string msgBody = msg.bodyString();
					Sleep(3000);
					if (msgBody.find("DownLoad File") != std::string::npos) {
						std::cout << "\n Getting call back message after downloading all the HTML files in client:.........Requirement#7 to displaying HTTP messages\n" << msg.toString() << "\n";
						tst.deleteFile(sndr, rcv, serverAddress);
						tst.deleteHtmlFile(sndr, rcv, serverAddress);
						string fileToOpen = "TestExecutiveClient/TestFilesClient/" + StringHelper::split(msgBody, '|')[1] + ".htm";
						std::cout << "\n  Launching requested file which is downloaded in client: " << StringHelper::split(msgBody, '|')[1] << "........\n";
					    std::cout << "\n  As I have mentioned above, If you want to open file from IIS, you have to use GUI and provide your IIS oath before opening it........\n"; 
						string browser("start \"\" \"" + fileToOpen + "\"");
						std::system(browser.c_str());
						Sleep(2000);
					}
				}
			}); ListenThreadFromUI.detach();
			tst.connect(sndr, rcv, serverAddress);
			tst.upload(sndr, rcv, serverAddress);
			tst.download(sndr, rcv, serverAddress);
			Utilities::waitForUser();
		}
		else
			std::cout << "\nPlease provide valid command arguments\n";}
	catch (...){
		std::cout << "\n\n  Caught exception in Executive::main: \n\n";
	}return 0;
}
#endif
