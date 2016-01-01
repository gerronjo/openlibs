// KernelThread.cpp : 实现文件
//

#include "stdafx.h"
#include "3600safeview.h"
#include "KernelThread.h"

void QueryKernelThread(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list)
{
	DWORD dwReadByte;
	int i = 0;
	SetDlgItemTextW(m_hWnd,ID,L"正在扫描内核线程，请稍后...");

	if (KernelThread)
	{
		VirtualFree(KernelThread,(sizeof(KERNEL_THREAD_INFO)+sizeof(KERNEL_INFO))*1025,MEM_RESERVE | MEM_COMMIT);
		KernelThread = NULL;
	}
	KernelThread = (PKERNEL_THREAD_INFO)VirtualAlloc(0, (sizeof(KERNEL_THREAD_INFO)+sizeof(KERNEL_INFO))*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (KernelThread)
	{
		memset(KernelThread,0,(sizeof(KERNEL_THREAD_INFO)+sizeof(KERNEL_INFO))*1025);

		ReadFile((HANDLE)LIST_KERNEL_THREAD,KernelThread, (sizeof(KERNEL_THREAD_INFO)+sizeof(KERNEL_INFO))*1025,&dwReadByte,0);

		for (i=0;i<KernelThread->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",KernelThread->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzHideType[256];           //0 正常模块，在driver_object中有   1 隐藏模块
			WCHAR lpwzStatus[256];             //线程状态，0运行，1退出
			WCHAR lpwzKernelThread[256];         //ETHREAD
			WCHAR lpwzThreadStart[256];          //开始
			WCHAR lpwzThreadModule[256];

			memset(lpwzHideType,0,sizeof(lpwzHideType));
			memset(lpwzStatus,0,sizeof(lpwzStatus));
			memset(lpwzKernelThread,0,sizeof(lpwzKernelThread));
			memset(lpwzThreadStart,0,sizeof(lpwzThreadStart));
			memset(lpwzThreadModule,0,sizeof(lpwzThreadModule));

			wsprintfW(lpwzKernelThread,L"0x%08X",KernelThread->KernelThreadInfo[i].KernelThread);
			wsprintfW(lpwzThreadStart,L"0x%08X",KernelThread->KernelThreadInfo[i].ThreadStart);
			
			MultiByteToWideChar(
				CP_ACP,
				0,
				KernelThread->KernelThreadInfo[i].lpszThreadModule,
				strlen(KernelThread->KernelThreadInfo[i].lpszThreadModule),
				lpwzThreadModule,
				sizeof(lpwzThreadModule)
				);

			if (KernelThread->KernelThreadInfo[i].ulStatus == 1)
				wsprintfW(lpwzStatus,L"%ws",L"终止");
			else
				wsprintfW(lpwzStatus,L"%ws",L"运行");

			if (KernelThread->KernelThreadInfo[i].ulHideType == 1)
			{
				wsprintfW(lpwzHideType,L"%ws",L"隐藏");
				m_list->InsertItem(i,lpwzKernelThread,RGB(255,20,147));
			}
			else
			{
				wsprintfW(lpwzHideType,L"%ws",L"正常");
				m_list->InsertItem(i,lpwzKernelThread,RGB(77,77,77));
			}
			m_list->SetItemText(i,1,lpwzThreadStart);
			m_list->SetItemText(i,2,lpwzThreadModule);
			m_list->SetItemText(i,3,lpwzStatus);
			m_list->SetItemText(i,4,lpwzHideType);
		}
		VirtualFree(KernelThread,(sizeof(KERNEL_THREAD_INFO)+sizeof(KERNEL_INFO))*1025,MEM_RESERVE | MEM_COMMIT);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"内核线程扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);
}
void ClearKernelThreadLog(CClrListCtrl *m_list)
{
	DWORD dwReadByte;

	m_list->DeleteAllItems();
	ReadFile((HANDLE)CLEAR_KERNEL_THREAD,0,0,&dwReadByte,0);
}