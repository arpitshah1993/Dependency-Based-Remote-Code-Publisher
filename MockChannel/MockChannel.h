#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
///////////////////////////////////////////////////////////////////////////////////////
// MockChannel.h - Demo for CSE687 Project #4, Spring 2015                           //
// - build as static Library showing how C++\CLI client can use native code channel  //
// - MockChannel reads from sendQ and writes to recvQ                                //
//                                                                                   //
// Arpit Shah												                         //
///////////////////////////////////////////////////////////////////////////////////////

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
using Message = std::string;

struct ISendr
{
  virtual void postMessage(const Message& msg) = 0;
};

struct IRecvr
{
  virtual std::string getMessage() = 0;
};

struct IMockChannel
{
public:
	virtual void startSender(std::string url) = 0;
	virtual void startReceiver(Message recvPth, std::string url) = 0;
  virtual void stop() = 0;
};

extern "C" {
  struct ObjectFactory
  {
    DLL_DECL ISendr* createSendr();
    DLL_DECL IRecvr* createRecvr();
    DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr);
  };
}

#endif


