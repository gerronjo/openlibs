#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"
#include <windows.h>
#include <powrprof.h>
#include <cstdlib>
#include <stdio.h>
#include <Commctrl.h>

#include "..\\Driver\\objchk_win7_x86\\i386\\KernelModule.h"

#pragma comment(lib,"ntdll.Lib") 

#define STATUS_SUCCESS        0x00000000 
typedef LONG NTSTATUS;

//#define SAFE_SYSTEM 0x11111111

typedef struct _UNICODE_STRING {
	USHORT  Length;     //UNICODE占用的内存字节数，个数*2；
	USHORT  MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING ,*PUNICODE_STRING;

extern "C" NTSTATUS __stdcall  NtLoadDriver(
	IN PUNICODE_STRING  DriverServiceName
	);

extern "C" VOID __stdcall
	RtlInitUnicodeString(
	IN OUT PUNICODE_STRING  DestinationString,
	IN PCWSTR  SourceString
	);

extern "C" ULONG __stdcall
	RtlNtStatusToDosError(
	IN NTSTATUS  Status
	); 

BOOL Install(HWND hwndDlg);