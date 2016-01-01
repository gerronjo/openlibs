// Atapi.cpp : 实现文件
//

#include "stdafx.h"
#include "3600safeview.h"
#include "Atapi.h"

void QueryAtapiHook(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list)
{
	DWORD dwReadByte;
	int ItemNum = m_list->GetItemCount();
	int i=0;

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描Atapi/Dispatch，请稍后...");

	if (AtapiDispatchBakUp)
	{
		VirtualFree(AtapiDispatchBakUp,(sizeof(ATAPIDISPATCHBAKUP)+sizeof(ATAPIDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2,MEM_RESERVE | MEM_COMMIT);
		AtapiDispatchBakUp = 0;
	}
	AtapiDispatchBakUp = (PATAPIDISPATCHBAKUP)VirtualAlloc(0,(sizeof(ATAPIDISPATCHBAKUP)+sizeof(ATAPIDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (AtapiDispatchBakUp)
	{
		memset(AtapiDispatchBakUp,0,(sizeof(ATAPIDISPATCHBAKUP)+sizeof(ATAPIDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2);

		ReadFile((HANDLE)LIST_ATAPI_HOOK,AtapiDispatchBakUp,(sizeof(ATAPIDISPATCHBAKUP)+sizeof(ATAPIDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2,&dwReadByte,0);

		for (i=0;i< (int)AtapiDispatchBakUp->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",AtapiDispatchBakUp->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzNumber[256] = {0};
			WCHAR lpwzHookType[256] = {0};
			WCHAR lpwzFunction[256] = {0};
			WCHAR lpwzHookModuleImage[256] = {0};
			WCHAR lpwzCurrentAtapiDispatch[256] = {0};
			WCHAR lpwsAtapiDispatch[256] = {0};

			WCHAR lpwzHookModuleBase[256] = {0};
			WCHAR lpwzHookModuleSize[256] = {0};

			memset(lpwzNumber,0,sizeof(lpwzNumber));
			memset(lpwzHookType,0,sizeof(lpwzHookType));

			memset(lpwzFunction,0,sizeof(lpwzFunction));
			memset(lpwzHookModuleImage,0,sizeof(lpwzHookModuleImage));
			memset(lpwzCurrentAtapiDispatch,0,sizeof(lpwzCurrentAtapiDispatch));
			memset(lpwsAtapiDispatch,0,sizeof(lpwsAtapiDispatch));

			memset(lpwzHookModuleBase,0,sizeof(lpwzHookModuleBase));
			memset(lpwzHookModuleSize,0,sizeof(lpwzHookModuleSize));

			switch (AtapiDispatchBakUp->AtapiDispatch[i].Hooked)
			{
			case 0:
				StrCatW(lpwzHookType,L"-");
				break;
			case 1:
				StrCatW(lpwzHookType,L"atapi hook");
				break;
			case 2:
				StrCatW(lpwzHookType,L"atapi Inline hook");
				break;
			}

			wsprintfW(lpwzNumber,L"%d",AtapiDispatchBakUp->AtapiDispatch[i].ulNumber);

			//wsprintfW(lpwzHookModuleImage,L"%ws",AtapiDispatchBakUp->AtapiDispatch[i].lpszBaseModule);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				AtapiDispatchBakUp->AtapiDispatch[i].lpszBaseModule,
				-1, 
				lpwzHookModuleImage, 
				strlen(AtapiDispatchBakUp->AtapiDispatch[i].lpszBaseModule)
				);
			wsprintfW(lpwzFunction,L"%ws",AtapiDispatchBakUp->AtapiDispatch[i].lpwzAtapiDispatchName);
			wsprintfW(lpwzCurrentAtapiDispatch,L"0x%08X",AtapiDispatchBakUp->AtapiDispatch[i].ulCurrentAtapiDispatch);
			wsprintfW(lpwsAtapiDispatch,L"0x%08X",AtapiDispatchBakUp->AtapiDispatch[i].ulAtapiDispatch);
			wsprintfW(lpwzHookModuleBase,L"0x%X",AtapiDispatchBakUp->AtapiDispatch[i].ulModuleBase);
			wsprintfW(lpwzHookModuleSize,L"0x%X",AtapiDispatchBakUp->AtapiDispatch[i].ulModuleSize);

			if (AtapiDispatchBakUp->AtapiDispatch[i].Hooked == 0)
			{
				m_list->InsertItem(i,lpwzNumber,RGB(77,77,77));

			}else
			{
				m_list->InsertItem(i,lpwzNumber,RGB(255,20,147));
			}
			m_list->SetItemText(i,1,lpwzFunction);
			m_list->SetItemText(i,2,lpwzCurrentAtapiDispatch);
			m_list->SetItemText(i,3,lpwsAtapiDispatch);
			m_list->SetItemText(i,4,lpwzHookModuleImage);
			m_list->SetItemText(i,5,lpwzHookModuleBase);
			m_list->SetItemText(i,6,lpwzHookModuleSize);
			m_list->SetItemText(i,7,lpwzHookType);
		}
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"Atapi/Dispatch扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);
}
void AtapiHookResetOne(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list)
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

	if (wcsstr(str,L"atapi") != NULL)
	{
		//取序号
		Num.Format(L"%s",m_list->GetItemText(Item,0));

		memset(lpszNum,0,sizeof(lpszNum));
		memset(lpwzNum,0,sizeof(lpwzNum));
		wcscat_s(lpwzNum,Num);
		WideCharToMultiByte (CP_OEMCP,NULL,lpwzNum,-1,lpszNum,wcslen(lpwzNum),NULL,FALSE);
		ReadFile((HANDLE)INIT_SET_ATAPI_HOOK,0,atoi(lpszNum),&dwReadByte,0);

		for (int i = 0;i< (int)AtapiDispatchBakUp->ulCount;i++)
		{
			if (atoi(lpszNum) == AtapiDispatchBakUp->AtapiDispatch[i].ulNumber)
			{
				ReadFile((HANDLE)SET_ATAPI_HOOK,0,AtapiDispatchBakUp->AtapiDispatch[i].ulAtapiDispatch,&dwReadByte,0);
				break;
			}
		}

		//删除当前选择的一行
		m_list->DeleteAllItems();
		QueryAtapiHook(m_hWnd,ID,m_list);
	}
}
void CopyAtapiDataToClipboard(HWND m_hWnd,CClrListCtrl *m_list)
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