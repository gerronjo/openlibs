// Mouclass.cpp : 实现文件
//

#include "stdafx.h"
#include "3600safeview.h"
#include "Mouclass.h"

void QueryMouclassHook(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list)
{
	DWORD dwReadByte;
	int i=0;
	int ItemNum = m_list->GetItemCount();

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描Mouclass/Dispatch，请稍后...");

	if (MouclassDispatchBakUp)
	{
		VirtualFree(MouclassDispatchBakUp,(sizeof(MOUCLASSDISPATCHBAKUP)+sizeof(MOUCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2,MEM_RESERVE | MEM_COMMIT);
		MouclassDispatchBakUp = 0;
	}
	MouclassDispatchBakUp = (PMOUCLASSDISPATCHBAKUP)VirtualAlloc(0,(sizeof(MOUCLASSDISPATCHBAKUP)+sizeof(MOUCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (MouclassDispatchBakUp)
	{
		memset(MouclassDispatchBakUp,0,(sizeof(MOUCLASSDISPATCHBAKUP)+sizeof(MOUCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2);

		ReadFile((HANDLE)LIST_MOUCLASS_HOOK,MouclassDispatchBakUp,(sizeof(MOUCLASSDISPATCHBAKUP)+sizeof(MOUCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2,&dwReadByte,0);

		for (i=0;i< (int)MouclassDispatchBakUp->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",MouclassDispatchBakUp->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzNumber[256] = {0};
			WCHAR lpwzHookType[256] = {0};
			WCHAR lpwzFunction[256] = {0};
			WCHAR lpwzHookModuleImage[256] = {0};
			WCHAR lpwzCurrentMouclassDispatch[256] = {0};
			WCHAR lpwsMouclassDispatch[256] = {0};

			WCHAR lpwzHookModuleBase[256] = {0};
			WCHAR lpwzHookModuleSize[256] = {0};

			memset(lpwzNumber,0,sizeof(lpwzNumber));
			memset(lpwzHookType,0,sizeof(lpwzHookType));

			memset(lpwzFunction,0,sizeof(lpwzFunction));
			memset(lpwzHookModuleImage,0,sizeof(lpwzHookModuleImage));
			memset(lpwzCurrentMouclassDispatch,0,sizeof(lpwzCurrentMouclassDispatch));
			memset(lpwsMouclassDispatch,0,sizeof(lpwsMouclassDispatch));

			memset(lpwzHookModuleBase,0,sizeof(lpwzHookModuleBase));
			memset(lpwzHookModuleSize,0,sizeof(lpwzHookModuleSize));

			switch (MouclassDispatchBakUp->MouclassDispatch[i].Hooked)
			{
			case 0:
				StrCatW(lpwzHookType,L"-");
				break;
			case 1:
				StrCatW(lpwzHookType,L"mou hook");
				break;
			case 2:
				StrCatW(lpwzHookType,L"mou Inline hook");
				break;
			}

			wsprintfW(lpwzNumber,L"%d",MouclassDispatchBakUp->MouclassDispatch[i].ulNumber);

			//wsprintfW(lpwzHookModuleImage,L"%ws",MouclassDispatchBakUp->MouclassDispatch[i].lpszBaseModule);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				MouclassDispatchBakUp->MouclassDispatch[i].lpszBaseModule,
				-1, 
				lpwzHookModuleImage, 
				strlen(MouclassDispatchBakUp->MouclassDispatch[i].lpszBaseModule)
				);
			wsprintfW(lpwzFunction,L"%ws",MouclassDispatchBakUp->MouclassDispatch[i].lpwzMouclassDispatchName);
			wsprintfW(lpwzCurrentMouclassDispatch,L"0x%08X",MouclassDispatchBakUp->MouclassDispatch[i].ulCurrentMouclassDispatch);
			wsprintfW(lpwsMouclassDispatch,L"0x%08X",MouclassDispatchBakUp->MouclassDispatch[i].ulMouclassDispatch);
			wsprintfW(lpwzHookModuleBase,L"0x%X",MouclassDispatchBakUp->MouclassDispatch[i].ulModuleBase);
			wsprintfW(lpwzHookModuleSize,L"0x%X",MouclassDispatchBakUp->MouclassDispatch[i].ulModuleSize);

			if (MouclassDispatchBakUp->MouclassDispatch[i].Hooked == 0)
			{
				m_list->InsertItem(i,lpwzNumber,RGB(77,77,77));

			}else
			{
				m_list->InsertItem(i,lpwzNumber,RGB(255,20,147));
			}
			m_list->SetItemText(i,1,lpwzFunction);
			m_list->SetItemText(i,2,lpwzCurrentMouclassDispatch);
			m_list->SetItemText(i,3,lpwsMouclassDispatch);
			m_list->SetItemText(i,4,lpwzHookModuleImage);
			m_list->SetItemText(i,5,lpwzHookModuleBase);
			m_list->SetItemText(i,6,lpwzHookModuleSize);
			m_list->SetItemText(i,7,lpwzHookType);
		}
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"Mouclass/Dispatch 扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}
void MouclassHookResetOne(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list)
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

	if (wcsstr(str,L"mou") != NULL)
	{
		//取序号
		Num.Format(L"%s",m_list->GetItemText(Item,0));

		memset(lpszNum,0,sizeof(lpszNum));
		memset(lpwzNum,0,sizeof(lpwzNum));
		wcscat_s(lpwzNum,Num);
		WideCharToMultiByte (CP_OEMCP,NULL,lpwzNum,-1,lpszNum,wcslen(lpwzNum),NULL,FALSE);
		ReadFile((HANDLE)INIT_SET_MOUCLASS_HOOK,0,atoi(lpszNum),&dwReadByte,0);

		for (int i = 0;i< (int)MouclassDispatchBakUp->ulCount;i++)
		{
			if (atoi(lpszNum) == MouclassDispatchBakUp->MouclassDispatch[i].ulNumber)
			{
				ReadFile((HANDLE)SET_MOUCLASS_HOOK,0,MouclassDispatchBakUp->MouclassDispatch[i].ulMouclassDispatch,&dwReadByte,0);
				break;
			}
		}

		//删除当前选择的一行
		m_list->DeleteAllItems();
		QueryMouclassHook(m_hWnd,ID,m_list);
	}
}
void CopyMouclassDataToClipboard(HWND m_hWnd,CClrListCtrl *m_list)
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