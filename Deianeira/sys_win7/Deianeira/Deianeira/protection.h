//防止重定义
#ifndef _PROTECTION_
#define _PROTECTION_

#include "common.h"

#define NtUserBuildHwndList_XP       312
#define NtUserDestroyWindow_XP       355
#define NtUserFindWindowEx_XP        378
#define NtUserGetForegroundWindow_XP 404
#define NtUserMessageCall_XP         460
#define NtUserPostMessage_XP         475
#define NtUserPostThreadMessage_XP   476
#define NtUserQueryWindow_XP         483
#define NtUserSetWindowLong_XP       544
#define NtUserShowWindow_XP          555
#define NtUserWindowFromPoint_XP     592

#define NtUserBuildHwndList_WIN7       323
#define NtUserDestroyWindow_WIN7       371
#define NtUserFindWindowEx_WIN7        396
#define NtUserGetForegroundWindow_WIN7 423
#define NtUserMessageCall_WIN7         490
#define NtUserPostMessage_WIN7         508
#define NtUserPostThreadMessage_WIN7   509
#define NtUserQueryWindow_WIN7         515
#define NtUserSetWindowLong_WIN7       578
#define NtUserShowWindow_WIN7          591
#define NtUserWindowFromPoint_WIN7     629

typedef struct _USER_STACK {
	PVOID FixedStackBase;
	PVOID FixedStackLimit;
	PVOID ExpandableStackBase;
	PVOID ExpandableStackLimit;
	PVOID ExpandableStackBottom;
} USER_STACK, *PUSER_STACK;


//shadow ssdt hook
NTSTATUS (*OldNtUserBuildHwndList)
(
 IN HDESK hdesk,
 IN HWND hwndNext,
 IN BOOL fEnumChildren,
 IN DWORD idThread,
 IN UINT cHwndMax,
 OUT HWND *phwndFirst,
 OUT PUINT pcHwndNeeded
 );
NTSTATUS NewNtUserBuildHwndList(
								IN HDESK hdesk,
								IN HWND hwndNext,
								IN BOOL fEnumChildren,
								IN DWORD idThread,
								IN UINT cHwndMax,
								OUT HWND *phwndFirst,
								OUT PUINT pcHwndNeeded);

BOOL (*OldNtUserDestroyWindow)
(
 IN HWND hwnd
 );
BOOL NewNtUserDestroyWindow(
							IN HWND hwnd);


HWND (*OldNtUserFindWindowEx)
(
 IN HWND hwndParent,
 IN HWND hwndChild,
 IN PUNICODE_STRING pstrClassName OPTIONAL,
 IN PUNICODE_STRING pstrWindowName OPTIONAL,
 IN DWORD dwType
 );
HWND NewNtUserFindWindowEx(
						   IN HWND hwndParent,
						   IN HWND hwndChild,
						   IN PUNICODE_STRING pstrClassName OPTIONAL,
						   IN PUNICODE_STRING pstrWindowName OPTIONAL,
						   IN DWORD dwType);


HWND (*OldNtUserGetForegroundWindow)(VOID);
HWND NewNtUserGetForegroundWindow(VOID);


LRESULT (*OldNtUserMessageCall)
(
 IN HWND hwnd,
 IN UINT msg,
 IN WPARAM wParam,
 IN LPARAM lParam,
 IN ULONG_PTR xParam,
 IN DWORD xpfnProc,
 IN BOOL bAnsi);
LRESULT NewNtUserMessageCall(
							 IN HWND hwnd,
							 IN UINT msg,
							 IN WPARAM wParam,
							 IN LPARAM lParam,
							 IN ULONG_PTR xParam,
							 IN DWORD xpfnProc,
							 IN BOOL bAnsi);

BOOL (*OldNtUserPostMessage)
(
 IN HWND hwnd,
 IN UINT msg,
 IN WPARAM wParam,
 IN LPARAM lParam
 );
BOOL NewNtUserPostMessage(
						   IN HWND hwnd,
						   IN UINT msg,
						   IN WPARAM wParam,
						   IN LPARAM lParam
						   );

BOOL (*OldNtUserPostThreadMessage)
(
 IN DWORD id,
 IN UINT msg,
 IN WPARAM wParam,
 IN LPARAM lParam
 );
BOOL NewNtUserPostThreadMessage(
								IN DWORD id,
								IN UINT msg,
								IN WPARAM wParam,
								IN LPARAM lParam);

HANDLE (*OldNtUserQueryWindow)
(
 IN HWND WindowHandle,
 IN ULONG TypeInformation
 );
HANDLE NewNtUserQueryWindow(
							IN HWND WindowHandle,
							IN ULONG TypeInformation);


LONG (*OldNtUserSetWindowLong)
(
 IN HWND hwnd,
 IN int nIndex,
 IN LONG dwNewLong,
 IN BOOL bAnsi);
LONG NewNtUserSetWindowLong(
							IN HWND hwnd,
							IN int nIndex,
							IN LONG dwNewLong,
							IN BOOL bAnsi);


BOOL (*OldNtUserShowWindow)
(
 IN HWND hwnd,
 IN int nCmdShow);
BOOL NewNtUserShowWindow(
						 IN HWND hwnd,
						 IN int nCmdShow);


HWND (*OldNtUserWindowFromPoint)(LONG x, LONG y);
HWND NewNtUserWindowFromPoint(LONG x, LONG y);


//
VOID protection_startInlineHook();

//
VOID protection_stopInlineHook();


//开启shadow ssdt hook
VOID protection_startShadowSSDTHook();


//停止shadow ssdt hook
VOID protection_stopShadowSSDTHook();


#endif //_PROTECTION_