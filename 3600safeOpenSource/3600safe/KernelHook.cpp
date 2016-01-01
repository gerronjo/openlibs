#include "stdafx.h"
#include "kernelhook.h"
#include "3600safeView.h"

VOID QueryKernelHook(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list)
{
	DWORD dwReadByte;
	int i=0;
	int ItemNum = m_list->GetItemCount();

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描内核Hook，请稍后...");

	InlineHookInfo = (PINLINEHOOKINFO)VirtualAlloc(0, (sizeof(INLINEHOOKINFO)+sizeof(INLINEHOOKINFO_INFORMATION))*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (InlineHookInfo)
	{
		ReadFile((HANDLE)LIST_INLINEHOOK,InlineHookInfo,(sizeof(INLINEHOOKINFO)+sizeof(INLINEHOOKINFO_INFORMATION))*1025,&dwReadByte,0);

		for (i=0;i< (int)InlineHookInfo->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",InlineHookInfo->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzHookType[50] = {0};
			WCHAR lpwzMemoryFunctionBase[256] = {0};
			WCHAR lpwzMemoryHookBase[256] = {0};
			WCHAR lpwzFunction[256] = {0};

			WCHAR lpwzHookModuleImage[256] = {0};
			WCHAR lpwzHookModuleBase[256] = {0};
			WCHAR lpwzHookModuleSize[256] = {0};

			memset(lpwzMemoryFunctionBase,0,sizeof(lpwzMemoryFunctionBase));
			memset(lpwzMemoryHookBase,0,sizeof(lpwzMemoryHookBase));
			memset(lpwzFunction,0,sizeof(lpwzFunction));
			memset(lpwzHookModuleImage,0,sizeof(lpwzHookModuleImage));
			memset(lpwzHookModuleBase,0,sizeof(lpwzHookModuleBase));
			memset(lpwzHookModuleSize,0,sizeof(lpwzHookModuleSize));

			memset(lpwzHookType,0,sizeof(lpwzHookType));

			wsprintfW(lpwzMemoryFunctionBase,L"0x%08X",InlineHookInfo->InlineHook[i].ulMemoryFunctionBase);
			//OutputDebugStringW(lpwzMemoryFunctionBase);

			wsprintfW(lpwzMemoryHookBase,L"0x%08X",InlineHookInfo->InlineHook[i].ulMemoryHookBase);

			MultiByteToWideChar(
				CP_ACP,
				0, 
				InlineHookInfo->InlineHook[i].lpszFunction,
				-1, 
				lpwzFunction, 
				strlen(InlineHookInfo->InlineHook[i].lpszFunction)
				);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				InlineHookInfo->InlineHook[i].lpszHookModuleImage,
				-1, 
				lpwzHookModuleImage, 
				strlen(InlineHookInfo->InlineHook[i].lpszHookModuleImage)
				);
			//wsprintfW(lpwzMemoryHookBase,L"0x%08X",InlineHookInfo->InlineHook[i].ulMemoryHookBase);
			wsprintfW(lpwzHookModuleBase,L"0x%08X",InlineHookInfo->InlineHook[i].ulHookModuleBase);
			wsprintfW(lpwzHookModuleSize,L"0x%08X",InlineHookInfo->InlineHook[i].ulHookModuleSize);

			switch (InlineHookInfo->InlineHook[i].ulHookType)
			{
			case 0:
				wcscat_s(lpwzHookType,L"Inline Hook");
				break;
			case 1:
				wcscat_s(lpwzHookType,L"Eat Hook");
				break;
			}
			m_list->InsertItem(0,lpwzMemoryFunctionBase,RGB(255,20,147));
			m_list->SetItemText(0,1,lpwzFunction);
			m_list->SetItemText(0,2,lpwzMemoryHookBase);
			m_list->SetItemText(0,3,lpwzHookModuleImage);
			m_list->SetItemText(0,4,lpwzHookModuleBase);
			m_list->SetItemText(0,5,lpwzHookModuleSize);
			m_list->SetItemText(0,6,lpwzHookType);
		}
		VirtualFree(InlineHookInfo,(sizeof(INLINEHOOKINFO)+sizeof(INLINEHOOKINFO_INFORMATION))*1025,MEM_RESERVE | MEM_COMMIT);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"内核Hook扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}
VOID UnHookKernelHookSelect(HWND m_hWnd,CClrListCtrl *m_list)
{
	DWORD dwReadByte;
	CString HookType;
	CString FunctionStr;

	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	WCHAR lpwzFunction[100];

	memset(lpwzFunction,0,sizeof(lpwzFunction));
	FunctionStr.Format(L"%s",m_list->GetItemText(Item,1));
	if (!wcslen(FunctionStr))
	{
		return;
	}
	if (wcsstr(FunctionStr,L"/NextCallHook"))
	{
		MessageBoxW(m_hWnd,L"暂时不支持未导出函数！",0,MB_ICONWARNING);
		return;
	}
	HookType.Format(L"%s",m_list->GetItemText(Item,6));
	if (!wcslen(HookType))
	{
		return;
	}
	if (wcsstr(HookType,L"Inline"))
	{
		wcscat_s(lpwzFunction,FunctionStr);
		ReadFile((HANDLE)SET_INLINE_HOOK,lpwzFunction,wcslen(lpwzFunction),&dwReadByte,0);
	}
	if (wcsstr(HookType,L"Eat"))
	{
		char lpszFunciton[256] = {0};

		wcscat_s(lpwzFunction,FunctionStr);

		WideCharToMultiByte( CP_ACP,
			0,
			lpwzFunction,
			-1,
			lpszFunciton,
			wcslen(lpwzFunction)*2,
			NULL,
			NULL);
		ReadFile((HANDLE)SET_EAT_HOOK,lpszFunciton,strlen(lpszFunciton),&dwReadByte,0);
	}
	m_list->DeleteAllItems();
	QueryKernelHook(m_hWnd,IDC_DebugStatus,m_list);
}
VOID ByPassKernelHook(HWND m_hWnd,CClrListCtrl *m_list)
{
	DWORD dwReadByte;
	CString HookType;
	CString FunctionStr;

	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	WCHAR lpwzFunction[100];

	memset(lpwzFunction,0,sizeof(lpwzFunction));
	FunctionStr.Format(L"%s",m_list->GetItemText(Item,1));
	if (!wcslen(FunctionStr))
	{
		return;
	}
	if (wcsstr(FunctionStr,L"/NextCallHook"))
	{
		MessageBoxW(m_hWnd,L"暂时不支持未导出函数！",0,MB_ICONWARNING);
		return;
	}
	HookType.Format(L"%s",m_list->GetItemText(Item,6));
	if (!wcslen(HookType))
	{
		return;
	}
	if (wcsstr(HookType,L"Eat"))
	{
		MessageBoxW(m_hWnd,L"暂时不支持绕过Eat Hook！",0,MB_ICONWARNING);
		return;
	}
	WCHAR lpwzForMat[256];
	memset(lpwzForMat,0,sizeof(lpwzForMat));
	wsprintfW(lpwzForMat,L"绕过\"%ws\"函数的hook有可能导致对方软件部分功能失效，是否继续？\r\n",FunctionStr);

	if (MessageBoxW(m_hWnd,lpwzForMat,0,MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		wcscat_s(lpwzFunction,FunctionStr);
		ReadFile((HANDLE)ANTI_INLINEHOOK,lpwzFunction,wcslen(lpwzFunction),&dwReadByte,0);

		MessageBoxW(m_hWnd,L"操作已经成功！",0,MB_ICONWARNING);
	}
	return;
}
void CopyKernelHookDataToClipboard(HWND m_hWnd,CClrListCtrl *m_list)
{
	CString KernelHook;
	int ItemNum = m_list->GetItemCount();
	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	KernelHook.Format(L"%s",m_list->GetItemText(Item,1));

	WCHAR lpwzKernelHook[260];

	memset(lpwzKernelHook,0,sizeof(lpwzKernelHook));
	wcscat_s(lpwzKernelHook,KernelHook);
	CHAR lpszKernelHook[1024];
	char *lpString = NULL;

	memset(lpwzKernelHook,0,sizeof(lpwzKernelHook));
	memset(lpszKernelHook,0,sizeof(lpszKernelHook));
	wcscat_s(lpwzKernelHook,KernelHook);
	WideCharToMultiByte( CP_ACP,
		0,
		lpwzKernelHook,
		-1,
		lpszKernelHook,
		wcslen(lpwzKernelHook)*2,
		NULL,
		NULL
		);
	lpString = setClipboardText(lpszKernelHook);
	if (lpString)
	{
		MessageBoxW(m_hWnd,L"操作成功！",0,MB_ICONWARNING);
	}
}