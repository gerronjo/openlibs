// KsBinSword.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CKsBinSwordApp:
// 有关此类的实现，请参阅 KsBinSword.cpp
//
typedef  BOOL (__stdcall *PFNUSkinInit)( PCHAR lpszUserName,PCHAR lpszRegCode,PCHAR lpszFileName );

typedef BOOL  (__stdcall * PFNUSkinExit)();
typedef BOOL  (__stdcall * PFNUSkinLoadSkin)(PCHAR lpszFileName);
class CKsBinSwordApp : public CWinApp
{
public:
	CKsBinSwordApp();

// 重写
public:	HMODULE hModuleUSkin ;

		PFNUSkinInit USkinInit;
        PFNUSkinExit USkinExit;
		PFNUSkinLoadSkin USkinLoadSkin;
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

};





extern CKsBinSwordApp theApp;