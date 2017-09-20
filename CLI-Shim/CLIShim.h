#pragma once
///////////////////////////////////////////////////////////////////////
// CLIShim.h - C++\CLI layer is a proxy for calls into MockChannel   //
//                                                                   //
// Note:                                                             //
//   - build as a dll so C# can load                                 //
//   - link to MockChannel static library                            //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include "../MockChannel/MockChannel.h"
#include <string>
using namespace System;


public ref class Shim
{
public:
  using Message = String;

  void PostMessage(Message^ msg);
  String^ GetMessage();
  String^ stdStrToSysStr(const std::string& str);
  std::string sysStrToStdStr(String^ str);
  Shim(String^ url, String^ path);
private:
  ISendr* pSendr;
  IRecvr* pRecvr;
  IMockChannel* pMockChan;
};



