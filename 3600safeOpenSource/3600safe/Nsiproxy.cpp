#include "stdafx.h"
#include "3600safeview.h"
#include "Nsiproxy.h"

void QueryNsipHook(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list)
{
	DWORD dwReadByte;
	int ItemNum = m_list->GetItemCount();
	int i=0;

	if (!IsWindows7())
	{
		SetDlgItemTextW(m_hWnd,ID,L"不支持当前系统Nsiproxy的枚举...");
		return;
	}
	SetDlgItemTextW(m_hWnd,ID,L"正在扫描Nsiproxy/Dispatch，请稍后...");

	if (NsiproxyDispatchBakUp)
	{
		VirtualFree(NsiproxyDispatchBakUp,(sizeof(NSIPROXYDISPATCHBAKUP)+sizeof(NSIPROXYDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2,MEM_RESERVE | MEM_COMMIT);
		NsiproxyDispatchBakUp = 0;
	}
	NsiproxyDispatchBakUp = (PNSIPROXYDISPATCHBAKUP)VirtualAlloc(0,(sizeof(NSIPROXYDISPATCHBAKUP)+sizeof(NSIPROXYDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (NsiproxyDispatchBakUp)
	{
		memset(NsiproxyDispatchBakUp,0,(sizeof(NSIPROXYDISPATCHBAKUP)+sizeof(NSIPROXYDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2);

		ReadFile((HANDLE)LIST_NSIPROXY_HOOK,NsiproxyDispatchBakUp,(sizeof(NSIPROXYDISPATCHBAKUP)+sizeof(NSIPROXYDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION*2,&dwReadByte,0);

		for (i=0;i< (int)NsiproxyDispatchBakUp->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",NsiproxyDispatchBakUp->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzNumber[256] = {0};
			WCHAR lpwzHookType[256] = {0};
			WCHAR lpwzFunction[256] = {0};
			WCHAR lpwzHookModuleImage[256] = {0};
			WCHAR lpwzCurrentNtfsDispatch[256] = {0};
			WCHAR lpwsNtfsDispatch[256] = {0};

			WCHAR lpwzHookModuleBase[256] = {0};
			WCHAR lpwzHookModuleSize[256] = {0};

			memset(lpwzNumber,0,sizeof(lpwzNumber));
			memset(lpwzHookType,0,sizeof(lpwzHookType));

			memset(lpwzFunction,0,sizeof(lpwzFunction));
			memset(lpwzHookModuleImage,0,sizeof(lpwzHookModuleImage));
			memset(lpwzCurrentNtfsDispatch,0,sizeof(lpwzCurrentNtfsDispatch));
			memset(lpwsNtfsDispatch,0,sizeof(lpwsNtfsDispatch));

			memset(lpwzHookModuleBase,0,sizeof(lpwzHookModuleBase));
			memset(lpwzHookModuleSize,0,sizeof(lpwzHookModuleSize));

			switch (NsiproxyDispatchBakUp->NsiproxyDispatch[i].Hooked)
			{
			case 0:
				StrCatW(lpwzHookType,L"-");
				break;
			case 1:
				StrCatW(lpwzHookType,L"nsiprroxy hook");
				break;
			case 2:
				StrCatW(lpwzHookType,L"nsiprroxy Inline hook");
				break;
			}

			wsprintfW(lpwzNumber,L"%d",NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulNumber);

			//wsprintfW(lpwzHookModuleImage,L"%ws",NtfsDispatchBakUp->NtfsDispatch[i].lpszBaseModule);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				NsiproxyDispatchBakUp->NsiproxyDispatch[i].lpszBaseModule,
				-1, 
				lpwzHookModuleImage, 
				strlen(NsiproxyDispatchBakUp->NsiproxyDispatch[i].lpszBaseModule)
				);
			wsprintfW(lpwzFunction,L"%ws",NsiproxyDispatchBakUp->NsiproxyDispatch[i].lpwzNsiproxyDispatchName);
			wsprintfW(lpwzCurrentNtfsDispatch,L"0x%08X",NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulCurrentNsiproxyDispatch);
			wsprintfW(lpwsNtfsDispatch,L"0x%08X",NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulNsiproxyDispatch);
			wsprintfW(lpwzHookModuleBase,L"0x%X",NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulModuleBase);
			wsprintfW(lpwzHookModuleSize,L"0x%X",NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulModuleSize);

			if (NsiproxyDispatchBakUp->NsiproxyDispatch[i].Hooked == 0)
			{
				m_list->InsertItem(i,lpwzNumber,RGB(77,77,77));

			}else
			{
				m_list->InsertItem(i,lpwzNumber,RGB(255,20,147));
			}
			m_list->SetItemText(i,1,lpwzFunction);
			m_list->SetItemText(i,2,lpwzCurrentNtfsDispatch);
			m_list->SetItemText(i,3,lpwsNtfsDispatch);
			m_list->SetItemText(i,4,lpwzHookModuleImage);
			m_list->SetItemText(i,5,lpwzHookModuleBase);
			m_list->SetItemText(i,6,lpwzHookModuleSize);
			m_list->SetItemText(i,7,lpwzHookType);
		}
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"Nsiproxy/Dispatch 扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}
void NsipHookResetOne(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list)
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

	if (wcsstr(str,L"nsiprroxy hook") != NULL)
	{
		//取序号
		Num.Format(L"%s",m_list->GetItemText(Item,0));

		memset(lpszNum,0,sizeof(lpszNum));
		memset(lpwzNum,0,sizeof(lpwzNum));
		wcscat_s(lpwzNum,Num);
		WideCharToMultiByte (CP_OEMCP,NULL,lpwzNum,-1,lpszNum,wcslen(lpwzNum),NULL,FALSE);
		ReadFile((HANDLE)INIT_SET_NSIPROXY_HOOK,0,atoi(lpszNum),&dwReadByte,0);

		for (int i = 0;i<NsiproxyDispatchBakUp->ulCount;i++)
		{
			if (atoi(lpszNum) == NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulNumber)
			{
				ReadFile((HANDLE)SET_NSIPROXY_HOOK,0,NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulNsiproxyDispatch,&dwReadByte,0);
				break;
			}
		}

		//删除当前选择的一行
		m_list->DeleteAllItems();
		QueryNsipHook(m_hWnd,ID,m_list);
	}
}
void CopyNsipDataToClipboard(HWND hWnd,CClrListCtrl *m_list)
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
		MessageBoxW(hWnd,L"操作成功！",0,MB_ICONWARNING);
	}
}