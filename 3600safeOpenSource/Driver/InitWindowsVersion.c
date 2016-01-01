//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         获取系统内核版本
//**************************************************************************
#include "InitWindowsVersion.h"

 WIN_VER_DETAIL GetWindowsVersion()
{
	UNICODE_STRING ustrFuncName = { 0 }; 
	RTL_OSVERSIONINFOEXW osverinfo = { sizeof(osverinfo) }; 
	PFN_RtlGetVersion pfnRtlGetVersion = NULL;

	if (WinVersion)
		return WinVersion;

	RtlInitUnicodeString(&ustrFuncName, L"RtlGetVersion"); 
	pfnRtlGetVersion = MmGetSystemRoutineAddress(&ustrFuncName); 

	if (pfnRtlGetVersion)
	{ 
		pfnRtlGetVersion((PRTL_OSVERSIONINFOW)&osverinfo); 
	} 
	else 
	{

		PsGetVersion(&osverinfo.dwMajorVersion, &osverinfo.dwMinorVersion, &osverinfo.dwBuildNumber, NULL);
	}

// 	KdPrint(("[xxxxxxxx] OSVersion NT %d.%d:%d sp%d.%d\n", 
// 		osverinfo.dwMajorVersion, osverinfo.dwMinorVersion, osverinfo.dwBuildNumber, 
// 		osverinfo.wServicePackMajor, osverinfo.wServicePackMinor));

	if (osverinfo.dwMajorVersion == 5 && osverinfo.dwMinorVersion == 0) 
	{
		WinVersion = WINDOWS_VERSION_2K;
	} 
	else if (osverinfo.dwMajorVersion == 5 && osverinfo.dwMinorVersion == 1) 
	{
		WinVersion = WINDOWS_VERSION_XP;
	} 
	else if (osverinfo.dwMajorVersion == 5 && osverinfo.dwMinorVersion == 2) 
	{
		if (osverinfo.wServicePackMajor==0) 
		{ 
			WinVersion = WINDOWS_VERSION_2K3;
		} 
		else 
		{
			WinVersion = WINDOWS_VERSION_2K3_SP1_SP2;
		}
	} 
	else if (osverinfo.dwMajorVersion == 6 && osverinfo.dwMinorVersion == 0) 
	{
		WinVersion = WINDOWS_VERSION_2K3_SP1_SP2;
	}
	else if (osverinfo.dwMajorVersion == 6 && osverinfo.dwMinorVersion == 1)
	{
		WinVersion = WINDOWS_VERSION_7;
	}

	return WinVersion;
}
