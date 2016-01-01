#include "stdafx.h"
#include "ShadowSSDT.h"
#include "3600safeView.h"

VOID QueryShadowSSDTHook(HWND m_hWnd,ULONG ID,int IntType,CClrListCtrl *m_list)
{
	DWORD dwReadByte;
	int i=0;
	int ItemNum = m_list->GetItemCount();

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描ShadowSSDT，请稍后...");

	ShadowSSDTInfo = (PSHADOWSSDTINFO)VirtualAlloc(0, (sizeof(SHADOWSSDTINFO)+sizeof(SHADOWSSDTINFO_INFORMATION))*910,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (ShadowSSDTInfo)
	{
		memset(ShadowSSDTInfo,0,(sizeof(SHADOWSSDTINFO)+sizeof(SHADOWSSDTINFO_INFORMATION))*910);
		if (IntType == 0)
		{
			ReadFile((HANDLE)LIST_SHADOWSSDT,ShadowSSDTInfo,(sizeof(SHADOWSSDTINFO)+sizeof(SHADOWSSDTINFO_INFORMATION))*910,&dwReadByte,0);
		}else
			ReadFile((HANDLE)LIST_SHADOWSSDT_ALL,ShadowSSDTInfo,(sizeof(SHADOWSSDTINFO)+sizeof(SHADOWSSDTINFO_INFORMATION))*910,&dwReadByte,0);

		for ( i=0;i< (int)ShadowSSDTInfo->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",ShadowSSDTInfo->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzNumber[256] = {0};
			WCHAR lpwzHookType[256] = {0};
			WCHAR lpwzFunction[256] = {0};
			WCHAR lpwzHookModuleImage[256] = {0};
			WCHAR lpwzMemoryFunctionBase[256] = {0};
			WCHAR lpwzHookModuleBase[256] = {0};
			WCHAR lpwzHookModuleSize[256] = {0};

			memset(lpwzNumber,0,sizeof(lpwzNumber));
			memset(lpwzHookType,0,sizeof(lpwzHookType));

			memset(lpwzFunction,0,sizeof(lpwzFunction));
			memset(lpwzHookModuleImage,0,sizeof(lpwzHookModuleImage));
			memset(lpwzMemoryFunctionBase,0,sizeof(lpwzMemoryFunctionBase));
			memset(lpwzHookModuleBase,0,sizeof(lpwzHookModuleBase));
			memset(lpwzHookModuleSize,0,sizeof(lpwzHookModuleSize));

			if (ShadowSSDTInfo->SSDT[i].ulNumber == 0xffffffff)
			{
				StrCatW(lpwzNumber,L"*");
			}
			else
				wsprintfW(lpwzNumber,L"%d",ShadowSSDTInfo->SSDT[i].ulNumber);

			MultiByteToWideChar(
				CP_ACP,
				0, 
				ShadowSSDTInfo->SSDT[i].lpszFunction,
				-1, 
				lpwzFunction, 
				strlen(ShadowSSDTInfo->SSDT[i].lpszFunction)
				);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				ShadowSSDTInfo->SSDT[i].lpszHookModuleImage,
				-1, 
				lpwzHookModuleImage, 
				strlen(ShadowSSDTInfo->SSDT[i].lpszHookModuleImage)
				);
			//wsprintfW(lpwzFunction,L"%s",SSDTInfo->SSDT[i].lpszFunction);
			//wsprintfW(lpwzHookModuleImage,L"%s",SSDTInfo->SSDT[i].lpszHookModuleImage);

			wsprintfW(lpwzMemoryFunctionBase,L"0x%08X",ShadowSSDTInfo->SSDT[i].ulMemoryFunctionBase);
			wsprintfW(lpwzHookModuleBase,L"0x%08X",ShadowSSDTInfo->SSDT[i].ulHookModuleBase);
			wsprintfW(lpwzHookModuleSize,L"%d KB",ShadowSSDTInfo->SSDT[i].ulHookModuleSize/1024);

			switch (ShadowSSDTInfo->SSDT[i].IntHookType)
			{
			case 0:
				StrCatW(lpwzHookType,L"-");
				break;
			case 1:
				StrCatW(lpwzHookType,L"ShadowSSDT hook");
				break;
			case 2:
				StrCatW(lpwzHookType,L"ShadowSSDT Inline hook");
				break;
			case 3:
				StrCatW(lpwzHookType,L"Inline hook");
				break;
			}
			if (ShadowSSDTInfo->SSDT[i].IntHookType == 0)
			{
				m_list->InsertItem(0,lpwzNumber,RGB(77,77,77));

			}else
			{
				m_list->InsertItem(0,lpwzNumber,RGB(255,20,147));
			}
			m_list->SetItemText(0,1,lpwzMemoryFunctionBase);
			m_list->SetItemText(0,2,lpwzFunction);
			m_list->SetItemText(0,3,lpwzHookModuleImage);
			m_list->SetItemText(0,4,lpwzHookModuleBase);
			m_list->SetItemText(0,5,lpwzHookModuleSize);
			m_list->SetItemText(0,6,lpwzHookType);
		}
		VirtualFree(ShadowSSDTInfo,(sizeof(SHADOWSSDTINFO)+sizeof(SHADOWSSDTINFO_INFORMATION))*910,MEM_RESERVE | MEM_COMMIT);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"ShadowSSDT扫描完毕，共有 %d 个数据.",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}
VOID UnHookShadowSSDT(HWND m_hWnd,CClrListCtrl *m_list)
{
	DWORD dwReadByte;
	CString str;
	CString Num;

	CString FunctionStr;

	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	str.Format(L"%s",m_list->GetItemText(Item,6));

	if (!wcslen(str))
	{
		return;
	}
	if (wcsstr(str,L"-") != NULL)
	{
		AfxMessageBox(_T("该函数不需要恢复！"));
		return;
	}
	WCHAR lpwzNum[10];
	char lpszNum[10];

	if (wcsstr(str,L"Inline") != NULL)
	{
		//取服务号
		Num.Format(L"%s",m_list->GetItemText(Item,0));

		memset(lpszNum,0,sizeof(lpszNum));
		memset(lpwzNum,0,sizeof(lpwzNum));
		wcscat_s(lpwzNum,Num);

		WideCharToMultiByte (CP_OEMCP,NULL,lpwzNum,-1,lpszNum,wcslen(lpwzNum),NULL,FALSE);

		ReadFile((HANDLE)SET_SHADOWSSDT_INLINE_HOOK,0,atoi(lpszNum),&dwReadByte,0);

		//删除当前选择的一行
		m_list->DeleteAllItems();
		QueryShadowSSDTHook(m_hWnd,IDC_DebugStatus,0,m_list);
	}
	if (wcsstr(str,L"SSDT") != NULL)
	{
		//取服务号
		Num.Format(L"%s",m_list->GetItemText(Item,0));

		memset(lpszNum,0,sizeof(lpszNum));
		memset(lpwzNum,0,sizeof(lpwzNum));
		wcscat_s(lpwzNum,Num);

		WideCharToMultiByte (CP_OEMCP,NULL,lpwzNum,-1,lpszNum,wcslen(lpwzNum),NULL,FALSE);

		//MessageBoxA(0,lpszNum,0,0);
		ReadFile((HANDLE)SET_ONE_SHADOWSSDT,0,atoi(lpszNum),&dwReadByte,0);

		//删除当前选择的一行
		m_list->DeleteAllItems();
		QueryShadowSSDTHook(m_hWnd,IDC_DebugStatus,0,m_list);
	}

}
VOID UnHookShadowSSDTAll(HWND m_hWnd,CClrListCtrl *m_list)
{
	DWORD dwReadByte;

	m_list->DeleteAllItems();

	ReadFile((HANDLE)SET_ALL_SHADOWSSDT,0,0,&dwReadByte,0);

	QueryShadowSSDTHook(m_hWnd,IDC_DebugStatus,0,m_list);
}
void CopyShadowSSDTDataToClipboard(HWND hWnd,CClrListCtrl *m_list)
{
	CString FunctionName;
	int ItemNum = m_list->GetItemCount();
	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	FunctionName.Format(L"%s",m_list->GetItemText(Item,2));

	WCHAR lpwzFunctionName[260];

	memset(lpwzFunctionName,0,sizeof(lpwzFunctionName));
	wcscat_s(lpwzFunctionName,FunctionName);
	CHAR lpszFunctionName[1024];
	char *lpString = NULL;

	memset(lpwzFunctionName,0,sizeof(lpwzFunctionName));
	memset(lpszFunctionName,0,sizeof(lpszFunctionName));
	wcscat_s(lpwzFunctionName,FunctionName);
	WideCharToMultiByte( CP_ACP,
		0,
		lpwzFunctionName,
		-1,
		lpszFunctionName,
		wcslen(lpwzFunctionName)*2,
		NULL,
		NULL
		);
	lpString = setClipboardText(lpszFunctionName);
	if (lpString)
	{
		MessageBoxW(hWnd,L"操作成功！",0,MB_ICONWARNING);
	}
}