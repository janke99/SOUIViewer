// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <Windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <assert.h>
#include <ShellAPI.h>
#include "config.h"

using namespace std;
typedef std::vector<BYTE> ByteVector;
#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring TString;
typedef std::wstringstream TStream;
#else
typedef std::string TString;
typedef std::stringstream TStream; //typedef std::istringstream TStream;
#endif

#define  _CRT_SECURE_NO_WARNINGS
#define	 DLL_SOUI
#include <souistd.h>
#include <core/SHostDialog.h>
#include <control/SMessageBox.h>
#include <control/souictrls.h>
#include <res.mgr/sobjdefattr.h>
#include <com-cfg.h>
#include <event/NotifyCenter.h>
#include "resource.h"
#define R_IN_CPP	//定义这个开关来
#include "res\resource.h"
using namespace SOUI;