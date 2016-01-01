// Kbdclass.cpp : 实现文件
//

#include "stdafx.h"
#include "3600safeview.h"
#include "Kbdclass.h"

void QueryKbdclassHook(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list)
{
	DWORD dwReadByte;
	int i=0;
	int ItemNum = m_list->GetItemCount();

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描Kbdclass/Dispatch，请稍后...");

	if (KbdclassDispatchBakUp)
	{
		VirtualFree(KbdclassDispatchBakUp,(sizeof(KBDCLASSDISPATCHBAKUP)+sizeof(KBDCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2,MEM_RESERVE | MEM_COMMIT);
		KbdclassDispatchBakUp = 0;
	}
	KbdclassDispatchBakUp = (PKBDCLASSDISPATCHBAKUP)VirtualAlloc(0,(sizeof(KBDCLASSDISPATCHBAKUP)+sizeof(KBDCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (KbdclassDispatchBakUp)
	{
		memset(KbdclassDispatchBakUp,0,(sizeof(KBDCLASSDISPATCHBAKUP)+sizeof(KBDCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2);

		ReadFile((HANDLE)LIST_KBDCLASS_HOOK,KbdclassDispatchBakUp,(sizeof(KBDCLASSDISPATCHBAKUP)+sizeof(KBDCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2,&dwReadByte,0);

		for (i=0;i< (int)KbdclassDispatchBakUp->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",KbdclassDispatchBakUp->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzNumber[256] = {0};
			WCHAR lpwzHookType[256] = {0};
			WCHAR lpwzFunction[256] = {0};
			WCHAR lpwzHookModuleImage[256] = {0};
			WCHAR lpwzCurrentKbdclassDispatch[256] = {0};
			WCHAR lpwsKbdclassDispatch[256] = {0};

			WCHAR lpwzHookModuleBase[256] = {0};
			WCHAR lpwzHookModuleSize[256] = {0};

			memset(lpwzNumber,0,sizeof(lpwzNumber));
			memset(lpwzHookType,0,sizeof(lpwzHookType));

			memset(lpwzFunction,0,sizeof(lpwzFunction));
			memset(lpwzHookModuleImage,0,sizeof(lpwzHookModuleImage));
			memset(lpwzCurrentKbdclassDispatch,0,sizeof(lpwzCurrentKbdclassDispatch));
			memset(lpwsKbdclassDispatch,0,sizeof(lpwsKbdclassDispatch));

			memset(lpwzHookModuleBase,0,sizeof(lpwzHookModuleBase));
			memset(lpwzHookModuleSize,0,sizeof(lpwzHookModuleSize));

			switch (KbdclassDispatchBakUp->KbdclassDispatch[i].Hooked)
			{
			case 0:
				StrCatW(lpwzHookType,L"-");
				break;
			case 1:
				StrCatW(lpwzHookType,L"kbd hook");
				break;
			case 2:
				StrCatW(lpwzHookType,L"kbd Inline hook");
				break;
			}

			wsprintfW(lpwzNumber,L"%d",KbdclassDispatchBakUp->KbdclassDispatch[i].ulNumber);

			//wsprintfW(lpwzHookModuleImage,L"%ws",KbdclassDispatchBakUp->KbdclassDispatch[i].lpszBaseModule);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				KbdclassDispatchBakUp->KbdclassDispatch[i].lpszBaseModule,
				-1, 
				lpwzHookModuleImage, 
				strlen(KbdclassDispatchBakUp->KbdclassDispatch[i].lpszBaseModule)
				);
			wsprintfW(lpwzFunction,L"%ws",KbdclassDispatchBakUp->KbdclassDispatch[i].lpwzKbdclassDispatchName);
			wsprintfW(lpwzCurrentKbdclassDispatch,L"0x%08X",KbdclassDispatchBakUp->KbdclassDispatch[i].ulCurrentKbdclassDispatch);
			wsprintfW(lpwsKbdclassDispatch,L"0x%08X",KbdclassDispatchBakUp->KbdclassDispatch[i].ulKbdclassDispatch);
			wsprintfW(lpwzHookModuleBase,L"0x%X",KbdclassDispatchBakUp->KbdclassDispatch[i].ulModuleBase);
			wsprintfW(lpwzHookModuleSize,L"0x%X",KbdclassDispatchBakUp->KbdclassDispatch[i].ulModuleSize);

			if (KbdclassDispatchBakUp->KbdclassDispatch[i].Hooked == 0)
			{
				m_list->InsertItem(i,lpwzNumber,RGB(77,77,77));

			}else
			{
				m_list->InsertItem(i,lpwzNumber,RGB(255,20,147));
			}
			m_list->SetItemText(i,1,lpwzFunction);
			m_list->SetItemText(i,2,lpwzCurrentKbdclassDispatch);
			m_list->SetItemText(i,3,lpwsKbdclassDispatch);
			m_list->SetItemText(i,4,lpwzHookModuleImage);
			m_list->SetItemText(i,5,lpwzHookModuleBase);
			m_list->SetItemText(i,6,lpwzHookModuleSize);
			m_list->SetItemText(i,7,lpwzHookType);
		}
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"Kbdclass/Dispatch 扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);
}
void KbdclassHookResetOne(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list)
{
	DWORD dwReadByte;
	CString str;
	CString Num;
	CString Address;

	CString FunctionStr;

	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	str.Format(L"%s",m_list->GetItemText(Item,7));

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

	if (wcsstr(str,L"kbd") != NULL)
	{
		//取序号
		Num.Format(L"%s",m_list->GetItemText(Item,0));

		memset(lpszNum,0,sizeof(lpszNum));
		memset(lpwzNum,0,sizeof(lpwzNum));
		wcscat_s(lpwzNum,Num);
		WideCharToMultiByte (CP_OEMCP,NULL,lpwzNum,-1,lpszNum,wcslen(lpwzNum),NULL,FALSE);
		ReadFile((HANDLE)INIT_SET_KBDCLASS_HOOK,0,atoi(lpszNum),&dwReadByte,0);

		for (int i = 0;i< (int)KbdclassDispatchBakUp->ulCount;i++)
		{
			if (atoi(lpszNum) == KbdclassDispatchBakUp->KbdclassDispatch[i].ulNumber)
			{
				ReadFile((HANDLE)SET_KBDCLASS_HOOK,0,KbdclassDispatchBakUp->KbdclassDispatch[i].ulKbdclassDispatch,&dwReadByte,0);
				break;
			}
		}

		//删除当前选择的一行
		m_list->DeleteAllItems();
		QueryKbdclassHook(m_hWnd,ID,m_list);
	}
}
void CopyKbdclassDataToClipboard(HWND m_hWnd,CClrListCtrl *m_list)
{
	CString FunctionName;
	int ItemNum = m_list->GetItemCount();
	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	FunctionName.Format(L"%s",m_list->GetItemText(Item,1));

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
		MessageBoxW(m_hWnd,L"操作成功！",0,MB_ICONWARNING);
	}
}