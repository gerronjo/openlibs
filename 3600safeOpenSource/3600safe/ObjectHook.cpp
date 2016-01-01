#include "stdafx.h"
#include "ObjectHook.h"
#include "3600safeview.h"

VOID QueryObjectHook(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list)
{
	DWORD dwReadByte;
	int i=0;
	SetDlgItemTextW(m_hWnd,ID,L"正在扫描ObjectHook，请稍后...");

	ObjectHookInfo = (POBJECTHOOKINFO)VirtualAlloc(0, (sizeof(OBJECTHOOKINFO)+sizeof(OBJECTHOOKINFO_INFORMATION))*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (ObjectHookInfo)
	{
		memset(ObjectHookInfo,0,(sizeof(OBJECTHOOKINFO)+sizeof(OBJECTHOOKINFO_INFORMATION))*1025);

		ReadFile((HANDLE)LIST_OBJECT_HOOK,ObjectHookInfo, (sizeof(OBJECTHOOKINFO)+sizeof(OBJECTHOOKINFO_INFORMATION))*1025,&dwReadByte,0);

		for ( i=0;i< (int)ObjectHookInfo->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",ObjectHookInfo->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);


			WCHAR lpwzMemoryFunctionBase[256] = {0};
			WCHAR lpwzMemoryHookBase[256] = {0};
			WCHAR lpwzFunction[256] = {0};
			WCHAR lpwzHookModuleImage[256] = {0};
			WCHAR lpwzObjectTypeBase[256] = {0};
			WCHAR lpwzObjectTypeName[256] = {0};
			WCHAR lpwzHookType[256] = {0};

			memset(lpwzMemoryFunctionBase,0,sizeof(lpwzMemoryFunctionBase));
			memset(lpwzMemoryHookBase,0,sizeof(lpwzMemoryHookBase));
			memset(lpwzFunction,0,sizeof(lpwzFunction));
			memset(lpwzHookModuleImage,0,sizeof(lpwzHookModuleImage));
			memset(lpwzObjectTypeBase,0,sizeof(lpwzObjectTypeBase));
			memset(lpwzObjectTypeName,0,sizeof(lpwzObjectTypeName));
			memset(lpwzHookType,0,sizeof(lpwzHookType));

			wsprintfW(lpwzMemoryHookBase,L"0x%08X",ObjectHookInfo->ObjectHook[i].ulMemoryHookBase);

			if (ObjectHookInfo->ObjectHook[i].ulMemoryFunctionBase != 0)
			
				wsprintfW(lpwzMemoryFunctionBase,L"0x%08X",ObjectHookInfo->ObjectHook[i].ulMemoryFunctionBase);
			else
				wsprintfW(lpwzMemoryFunctionBase,L"%ws",L"-");

			MultiByteToWideChar(
				CP_ACP,
				0, 
				ObjectHookInfo->ObjectHook[i].lpszFunction,
				-1, 
				lpwzFunction, 
				strlen(ObjectHookInfo->ObjectHook[i].lpszFunction)
				);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				ObjectHookInfo->ObjectHook[i].lpszHookModuleImage,
				-1, 
				lpwzHookModuleImage, 
				strlen(ObjectHookInfo->ObjectHook[i].lpszHookModuleImage)
				);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				ObjectHookInfo->ObjectHook[i].lpszObjectTypeName,
				-1, 
				lpwzObjectTypeName, 
				strlen(ObjectHookInfo->ObjectHook[i].lpszObjectTypeName)
				);
			wsprintfW(lpwzObjectTypeBase,L"0x%08X",ObjectHookInfo->ObjectHook[i].ulObjectTypeBase);

			switch (ObjectHookInfo->ObjectHook[i].ulHookType)
			{
			case 0:
				m_list->InsertItem(i,lpwzMemoryHookBase,RGB(77,77,77));
				wcscat_s(lpwzHookType,L"-");
				break;
			case 1:
				m_list->InsertItem(i,lpwzMemoryHookBase,RGB(255,20,147));
				wcscat_s(lpwzHookType,L"ObjectHook");
				break;
			case 2:
				m_list->InsertItem(i,lpwzMemoryHookBase,RGB(255,20,147));
				wcscat_s(lpwzHookType,L"ObjectInlineHook");
				break;
			}
			m_list->SetItemText(i,1,lpwzMemoryFunctionBase);
			m_list->SetItemText(i,2,lpwzFunction);
			m_list->SetItemText(i,3,lpwzObjectTypeName);
			m_list->SetItemText(i,4,lpwzObjectTypeBase);
			m_list->SetItemText(i,5,lpwzHookModuleImage);
			m_list->SetItemText(i,6,lpwzHookType);
		}
		VirtualFree(ObjectHookInfo,(sizeof(OBJECTHOOKINFO)+sizeof(OBJECTHOOKINFO_INFORMATION))*1025,MEM_RESERVE | MEM_COMMIT);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"ObjectHook扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}