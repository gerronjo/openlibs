// Process.cpp : 实现文件
//

#include "stdafx.h"
#include "3600safeview.h"
#include "Process.h"

LPSTR ExtractFileName(LPSTR lpcFullFileName)
{
	lpcFullFileName = strrchr(lpcFullFileName, '\\') + 1;
	return lpcFullFileName;
}
HANDLE RunAProcess(char *comline)
{
	STARTUPINFOA   si;   
	memset(&si,0 ,sizeof(LPSTARTUPINFOA));   
	si.cb   =   sizeof(   LPSTARTUPINFOA   );   
	si.dwFlags   =   STARTF_USESHOWWINDOW;   
	si.wShowWindow   =   SW_SHOW;   
	PROCESS_INFORMATION   pi;   
	CreateProcessA(NULL,comline,   NULL,   NULL,   FALSE,   CREATE_NO_WINDOW,   NULL,   NULL,   &si,   &pi);
	//Sleep(888);
	//WaitForSingleObject(pi.hProcess, INFINITE);  //等待信号执行结束
	return pi.hProcess;
}
VOID QuerySystemProcess(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list)
{
	DWORD dwReadByte;
	int ItemNum = m_list->GetItemCount();
	HANDLE hProcess;
	int i=0;
	//触发最后一个进程，不然无法列举最后一个执行的进程
	//hProcess = RunAProcess("ping 127.0.0.1 -n 100");
	//WinExec("ping 127.0.0.1 -n 5",SW_HIDE);

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描系统进程，请稍后...");

	if (NormalProcessInfo)
	{
		VirtualFree(NormalProcessInfo,(sizeof(PROCESSINFO)+sizeof(SAFESYSTEM_PROCESS_INFORMATION))*900,MEM_RESERVE | MEM_COMMIT);
		NormalProcessInfo = NULL;
	}

	NormalProcessInfo = (PPROCESSINFO)VirtualAlloc(0, (sizeof(PROCESSINFO)+sizeof(SAFESYSTEM_PROCESS_INFORMATION))*900,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (NormalProcessInfo)
	{
		//MessageBoxW(0,0,0);
		memset(NormalProcessInfo,0,(sizeof(PROCESSINFO)+sizeof(SAFESYSTEM_PROCESS_INFORMATION))*900);
		ReadFile((HANDLE)LIST_PROCESS,NormalProcessInfo,(sizeof(PROCESSINFO)+sizeof(SAFESYSTEM_PROCESS_INFORMATION))*900,&dwReadByte,0);

		for ( i=0;i<NormalProcessInfo->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",NormalProcessInfo->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzProcName[100];
			WCHAR lpwzPid[50];
			WCHAR lpwzInheritedPid[50];
			WCHAR lpwzFullProcName[256];

			WCHAR lpwzEProcess[100];
			WCHAR lpwzStatus[50];

			WCHAR lpwzFileServices[256];
			WCHAR lpwzTrue[256];

			memset(lpwzProcName,0,sizeof(lpwzProcName));
			memset(lpwzPid,0,sizeof(lpwzPid));
			memset(lpwzInheritedPid,0,sizeof(lpwzInheritedPid));
			memset(lpwzFullProcName,0,sizeof(lpwzFullProcName));
			memset(lpwzEProcess,0,sizeof(lpwzEProcess));
			memset(lpwzStatus,0,sizeof(lpwzStatus));
			memset(lpwzFileServices,0,sizeof(lpwzFileServices));

			if (NormalProcessInfo->ProcessInfo[i].ulPid < 0 &&
				NormalProcessInfo->ProcessInfo[i].ulPid > 123456)
			{
				continue;
			}
			if (NormalProcessInfo->ProcessInfo[i].ulInheritedFromProcessId < 0 &&
				NormalProcessInfo->ProcessInfo[i].ulInheritedFromProcessId > 123456)
			{
				continue;
			}
			if (NormalProcessInfo->ProcessInfo[i].EProcess == 0)
			{
				continue;
			}
			//提取进程DOS路径
			WCHAR lpwzWinDir[256];
			WCHAR lpwzSysDisk[10];


			char  lpszString[256];
			char  lpszFullString[5024];
			WCHAR lpwzFullString[256];
			memset(lpszString,0,sizeof(lpszString));
			memset(lpszFullString,0,sizeof(lpszFullString));
			memset(lpwzFullString,0,sizeof(lpwzFullString));

			memset(lpwzTrue,0,sizeof(lpwzTrue));
			memset(lpwzWinDir,0,sizeof(lpwzWinDir));
			memset(lpwzSysDisk,0,sizeof(lpwzSysDisk));

			if (NormalProcessInfo->ProcessInfo[i].ulPid == 4)
			{
				wcscat(lpwzFullString,L"System");
				wcscat(lpwzProcName,L"System");
				goto Next;
			}

			if (wcsstr(NormalProcessInfo->ProcessInfo[i].lpwzFullProcessPath,L"\\Device\\") != NULL)
			{
				//开始处理dos路径
				NtFilePathToDosFilePath(NormalProcessInfo->ProcessInfo[i].lpwzFullProcessPath,lpwzFullString);
			}else
				wcsncat(lpwzFullString,NormalProcessInfo->ProcessInfo[i].lpwzFullProcessPath,wcslen(NormalProcessInfo->ProcessInfo[i].lpwzFullProcessPath));
			//----------------------
			WideCharToMultiByte( CP_ACP,
				0,
				lpwzFullString,
				-1,
				lpszFullString,
				wcslen(lpwzFullString)*2,
				NULL,
				NULL);
			char *p = strstr(lpszFullString,"\\");
			if (p)
			{
				wsprintfA(lpszString,"%s",ExtractFileName(lpszFullString));
				MultiByteToWideChar(
					CP_ACP,
					0, 
					lpszString,
					-1, 
					lpwzProcName, 
					strlen(lpszString)
					);
			}

			FILE * fp=fopen(lpszFullString,"rb");
			if(fp)
			{
				MD5VAL val;
				val = md5File(fp);
				wsprintfW(lpwzFileServices,L"%08x%08x%08x%08x",conv(val.a),conv(val.b),conv(val.c),conv(val.d));
				FileVerify(lpszFullString,lpwzFileServices,lpwzTrue);

				fclose(fp);
			}
			//MessageBoxW(NormalProcessInfo->ProcessInfo[i].lpwzFullProcessPath,lpwzFullProcName,0);
Next:
			wsprintfW(lpwzPid,L"%d",NormalProcessInfo->ProcessInfo[i].ulPid);
			wsprintfW(lpwzInheritedPid,L"%d",NormalProcessInfo->ProcessInfo[i].ulInheritedFromProcessId);
			wsprintfW(lpwzEProcess,L"0x%08X",NormalProcessInfo->ProcessInfo[i].EProcess);
			//wsprintfW(lpwzStatus,L"%d",NormalProcessInfo->ProcessInfo[i].ulKernelOpen);

			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE,NormalProcessInfo->ProcessInfo[i].ulPid);
			if (hProcess)
			{
				wcscat(lpwzStatus,L"Yes/");
				CloseHandle(hProcess);
			}else
				wcscat(lpwzStatus,L"No/");

			if (NormalProcessInfo->ProcessInfo[i].ulKernelOpen == 1)
			{
				wcscat(lpwzStatus,L"Yes");
			}else
				wcscat(lpwzStatus,L"No");

			if (NormalProcessInfo->ProcessInfo[i].IntHideType == 1)
			{
				m_list->InsertItem(i,lpwzPid,RGB(255,20,147));  //隐藏
				memset(lpwzStatus,0,sizeof(lpwzStatus));
				wcscat(lpwzStatus,L"隐藏进程");
			}
			else
			{
				if (_wcsnicmp(lpwzTrue,L"无法确认文件来源",wcslen(L"无法确认文件来源")) == 0)
				{
					m_list->InsertItem(i,lpwzPid,RGB(238,118,0));

				}else
				{
					if (!wcslen(lpwzProcName))
					{
						wcscat(lpwzFullString,L"* (Warning:进程文件已被移动)");
						wcscat(lpwzProcName,L"*");
						m_list->InsertItem(i,lpwzPid,RGB(255,20,147));

					}else
						m_list->InsertItem(i,lpwzPid,RGB(77,77,77));

				}
			}

			//m_list->InsertItem(ItemNum,lpwzHideType);
			m_list->SetItemText(i,1,lpwzInheritedPid);
			m_list->SetItemText(i,2,lpwzProcName);
			m_list->SetItemText(i,3,lpwzFullString);
			m_list->SetItemText(i,4,lpwzEProcess);
			m_list->SetItemText(i,5,lpwzStatus);
			m_list->SetItemText(i,6,lpwzTrue);

			//以下为图标，强子添加
			/*SHFILEINFO shfileinfo;
			CImageList ImageListBmp;
			//ImageListBmp.Create(16,16, ILC_COLOR32, 0, 1);
			//m_list->SetImageList(&ImageListBmp,LVSIL_SMALL);
			if(SUCCEEDED(::SHGetFileInfo(lpwzFullString,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON)))
			{
				ImageListBmp.Add(shfileinfo.hIcon);
				//m_list->InsertItem(i,lpwzFullString,1);
				m_list->SetItem(i, 3, LVIF_IMAGE, NULL, 0, 0, 0, 0);
				ImageListBmp.DeleteImageList();
			} */
		}
		//VirtualFree(NormalProcessInfo,sizeof(NormalProcessInfo)*1050*200,MEM_RESERVE | MEM_COMMIT);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"系统进程扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}
void Processmodule()
{
}
extern BOOL IsNumber(char *lpszString);

void KillProcess(HWND hWnd,ULONG ID,CClrListCtrl *m_list)
{
	// TODO: 在此添加命令处理程序代码
	//m_list->DeleteAllItems();
	//Insert();
	CString PidNum;
	DWORD dwReadByte;

	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	PidNum.Format(L"%s",m_list->GetItemText(Item,0));

	WCHAR lpwzNum[50];
	char lpszNum[50];

	memset(lpszNum,0,sizeof(lpszNum));
	memset(lpwzNum,0,sizeof(lpwzNum));
	wcscat(lpwzNum,PidNum);

	WideCharToMultiByte (CP_OEMCP,NULL,lpwzNum,-1,lpszNum,wcslen(lpwzNum)*2,NULL,FALSE);
	if (IsNumber(lpszNum))
	{
		if (MessageBoxW(hWnd,L"结束进程可能导致系统不稳定!!\r\n\r\n是否继续？\r\n",L"SafeSystem",MB_YESNO | MB_ICONWARNING) == IDYES)
		{
			for (int i=0;i<NormalProcessInfo->ulCount;i++)
			{
				if (NormalProcessInfo->ProcessInfo[i].ulPid == atoi(lpszNum))
				{
					ReadFile((HANDLE)KILL_PROCESS_BY_PID,0,NormalProcessInfo->ProcessInfo[i].EProcess,&dwReadByte,0);
					break;
				}
			}
			m_list->DeleteAllItems();
			Sleep(1000);
			QuerySystemProcess(hWnd,ID,m_list);
		}
	}
}
void  KillProcessDeleteFile(HWND hWnd,ULONG ID,CClrListCtrl *m_list)
{
	// TODO: 在此添加命令处理程序代码
	//m_list->DeleteAllItems();
	//Insert();
	CString PidNum;
	DWORD dwReadByte;


	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	PidNum.Format(L"%ws",m_list->GetItemText(Item,4));

	WCHAR lpwzNum[50];
	WCHAR lpszEProcessNum[50];

	memset(lpwzNum,0,sizeof(lpwzNum));
	wcscat(lpwzNum,PidNum);

	if (MessageBoxW(hWnd,L"结束并删除进程文件，可能导致系统不稳定!!\r\n\r\n是否继续？\r\n",L"SafeSystem",MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		for (int i=0;i<NormalProcessInfo->ulCount;i++)
		{
			memset(lpszEProcessNum,0,sizeof(lpszEProcessNum));
			wsprintfW(lpszEProcessNum,L"0x%x",NormalProcessInfo->ProcessInfo[i].EProcess);
			if (_wcsnicmp(lpszEProcessNum,lpwzNum,wcslen(lpwzNum)) == 0)
			{
				ReadFile((HANDLE)KILL_PROCESS_BY_PID,0,NormalProcessInfo->ProcessInfo[i].EProcess,&dwReadByte,0);

				WCHAR lpwzDeletedFile[256];
				memset(lpwzDeletedFile,0,sizeof(lpwzDeletedFile));
				wsprintfW(lpwzDeletedFile,L"%ws",NormalProcessInfo->ProcessInfo[i].lpwzFullProcessPath);

				ReadFile((HANDLE)DELETE_FILE,lpwzDeletedFile,wcslen(lpwzDeletedFile),&dwReadByte,0);
				break;
			}
		}
		m_list->DeleteAllItems();
		Sleep(1000);
		QuerySystemProcess(hWnd,ID,m_list);
	}
}
void ProcessVerify(HWND hWnd,CClrListCtrl *m_list)
{
	CString PidNum;
	CString FilePath;
	DWORD dwReadByte;

	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	//PidNum.Format(L"%s",m_list->GetItemText(Item,0));
	FilePath.Format(L"%s",m_list->GetItemText(Item,3));

	WCHAR lpwzNum[50];
	WCHAR lpwzFilePath[260];
	WCHAR lpwzTrue[260];
	CHAR lpszFilePath[5024];

	memset(lpwzNum,0,sizeof(lpwzNum));
	memset(lpszFilePath,0,sizeof(lpszFilePath));
	memset(lpwzFilePath,0,sizeof(lpwzFilePath));

	wcscat(lpwzFilePath,FilePath);
	if (!wcslen(lpwzFilePath))
	{
		return;
	}
	WideCharToMultiByte (CP_OEMCP,NULL,lpwzFilePath,-1,lpszFilePath,wcslen(lpwzFilePath)*2,NULL,FALSE);

	if (GetFileAttributesA(lpszFilePath) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBoxW(hWnd,L"取文件路径出错！",0,0);
		return;
	}
	FILE * fp=fopen(lpszFilePath,"rb");
	if(fp)
	{
		MD5VAL val;
		val = md5File(fp);
		wsprintfW(lpwzNum,L"%08x%08x%08x%08x",conv(val.a),conv(val.b),conv(val.c),conv(val.d));
		fclose(fp);
	}
	FileVerify(lpszFilePath,lpwzNum,lpwzTrue);

	WCHAR lpwzMessageBox[256] = {0};

	memset(lpwzMessageBox,0,sizeof(lpwzMessageBox));

	if (_wcsnicmp(lpwzTrue,L"不支持当前系统",wcslen(L"不支持当前系统")) == 0)
	{
		wsprintfW(lpwzMessageBox,L"%s\r\n\r\n是否允许\"A盾电脑防护\"收集您的计算机版本以便作为后续版本更新？",L"不支持当前系统");
		if (MessageBoxW(hWnd,lpwzMessageBox,L"错误提示",MB_YESNO | MB_ICONWARNING) == IDYES)
		{
			//开始收集系统信息
		}
	}
	else if (_wcsnicmp(lpwzTrue,L"本地(√)/云端(-)",wcslen(L"本地(√)/云端(-)")) == 0)
	{
		WCHAR lpszSuccess[256];
		memset(lpszSuccess,0,sizeof(lpszSuccess));
		wsprintfW(lpszSuccess,L"文件:%ws\r\n\r\n已经通过云端验证，属于系统原生文件！\r\n",lpwzFilePath);
		AfxMessageBox(lpszSuccess);
	}
	else
	{
		AfxMessageBox(lpwzTrue);
	}
}
void CopyProcessDataToClipboard(HWND hWnd,CClrListCtrl *m_list)
{
	CString ProcessPath;
	int ItemNum = m_list->GetItemCount();
	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	ProcessPath.Format(L"%s",m_list->GetItemText(Item,3));

	WCHAR lpwzProcessPath[260];
	CHAR lpszProcessPath[1024];
	char *lpString = NULL;
	memset(lpwzProcessPath,0,sizeof(lpwzProcessPath));
	memset(lpszProcessPath,0,sizeof(lpszProcessPath));
	wcscat(lpwzProcessPath,ProcessPath);
	WideCharToMultiByte( CP_ACP,
		0,
		lpwzProcessPath,
		-1,
		lpszProcessPath,
		wcslen(lpwzProcessPath)*2,
		NULL,
		NULL
		);
	lpString = setClipboardText(lpszProcessPath);
	if (lpString)
	{
		MessageBoxW(hWnd,L"操作成功！",0,MB_ICONWARNING);
	}
}