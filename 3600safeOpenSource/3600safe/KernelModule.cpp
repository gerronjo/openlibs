#include "stdafx.h"
#include "3600safeview.h"
#include "KernelModule.h"

#include "Windows7Home_CN.h"
#include "Windows2003SP1_CN.h"
#include "Windows2003SP2_CN.h"
#include "Windows7SP1_CN.h"
#include "WindowsXPSP2_CN.h"
#include "WindowsXPSP3_CN.h"

BOOL FileVerify(char *lpszFullPath,WCHAR *lpwzFileMd5,WCHAR *lpwzTrue)
{
	memset(lpwzTrue,0,sizeof(lpwzTrue));
	wcscat(lpwzTrue,L"无法确认文件来源");

	OSVERSIONINFOEX   osvi;   
	BOOL   bOsVersionInfoEx;   

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));   
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);   
	if(!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi)))   
	{   
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);   
		if(!GetVersionEx((OSVERSIONINFO*)&osvi))     
			return FALSE;   
	}
	for (int x=0;x<2888;x++)
	{
		if (osvi.dwBuildNumber == 2600 &&
			osvi.wServicePackMajor == 3 &&
			GetSystemDefaultLCID() == 2052)  //xpSP3_cn
		{
			if (!wcslen(WindowsXPSP3_CN[x]))
			{
				break;
			}
			if (_wcsnicmp(WindowsXPSP3_CN[x],lpwzFileMd5,wcslen(lpwzFileMd5)) == NULL)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"本地(√)/云端(-)");
				break;
			}
		}else if (osvi.dwBuildNumber == 2600 &&
			osvi.wServicePackMajor == 2 &&
			GetSystemDefaultLCID() == 2052)  //xpSP2_cn
		{
			if (!wcslen(WindowsXPSP2_CN[x]))
			{
				break;
			}
			if (_wcsnicmp(WindowsXPSP2_CN[x],lpwzFileMd5,wcslen(lpwzFileMd5)) == NULL)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"本地(√)/云端(-)");
				break;
			}
		}
		else if (osvi.dwBuildNumber == 3790 &&
			osvi.wServicePackMajor == 1 &&
			GetSystemDefaultLCID() == 2052)  //2003SP1_cn
		{
			if (!wcslen(Windows2003SP1_CN[x]))
			{
				break;
			}
			if (_wcsnicmp(Windows2003SP1_CN[x],lpwzFileMd5,wcslen(lpwzFileMd5)) == NULL)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"本地(√)/云端(-)");
				break;
			}
		}
		else if (osvi.dwBuildNumber == 3790 &&
			osvi.wServicePackMajor == 2 &&
			GetSystemDefaultLCID() == 2052)  //2003SP2_cn
		{
			if (!wcslen(Windows2003SP2_CN[x]))
			{
				break;
			}
			if (_wcsnicmp(Windows2003SP2_CN[x],lpwzFileMd5,wcslen(lpwzFileMd5)) == NULL)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"本地(√)/云端(-)");
				break;
			}
		}
		else if (osvi.dwBuildNumber == 7600 &&
			osvi.wServicePackMajor == 0 &&
			GetSystemDefaultLCID() == 2052)  //win7 home
		{
			if (!wcslen(Windows7Home_CN[x]))
			{
				break;
			}
			if (_wcsnicmp(Windows7Home_CN[x],lpwzFileMd5,wcslen(lpwzFileMd5)) == NULL)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"本地(√)/云端(-)");
				break;
			}
		}
		else if (osvi.dwBuildNumber == 7601 &&
			osvi.wServicePackMajor == 1 &&
			GetSystemDefaultLCID() == 2052)  //win7 旗舰 SP1_CN
		{
			if (!wcslen(Windows7SP1_CN[x]))
			{
				break;
			}
			if (_wcsnicmp(Windows7SP1_CN[x],lpwzFileMd5,wcslen(lpwzFileMd5)) == NULL)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"本地(√)/云端(-)");
				break;
			}
		}
		else
		{
			memset(lpwzTrue,0,sizeof(lpwzTrue));
			wcscat(lpwzTrue,L"不支持当前系统");
			break;
		}
	}
	return TRUE;
}

VOID QueryKernelModule(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list,int IntLookType)
{
	DWORD dwReadByte;
	int ItemNum = m_list->GetItemCount();
	int x = 0;
	int i = 0;

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描内核模块，请稍后...");

	if (SysModuleInfo)
	{
		VirtualFree(SysModuleInfo,(sizeof(SYSINFO)+sizeof(SYSINFO_INFORMATION))*810,MEM_RESERVE | MEM_COMMIT);
		SysModuleInfo = NULL;
	}
	SysModuleInfo = (PSYSINFO)VirtualAlloc(0, (sizeof(SYSINFO)+sizeof(SYSINFO_INFORMATION))*810,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (SysModuleInfo)
	{
		ReadFile((HANDLE)LIST_SYS_MODULE,SysModuleInfo,(sizeof(SYSINFO)+sizeof(SYSINFO_INFORMATION))*810,&dwReadByte,0);

		for (i=0;i< SysModuleInfo->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",SysModuleInfo->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzSysBase[256] = {0};
			WCHAR lpwzSizeOfImage[256] = {0};
			WCHAR lpwzFullSysName[256] = {0};

			WCHAR lpwzBaseSysName[256] = {0};
			WCHAR LoadCount[256] = {0};
			WCHAR IntHideType[256] = {0};

			memset(lpwzSysBase,0,sizeof(lpwzSysBase));
			memset(lpwzSizeOfImage,0,sizeof(lpwzSizeOfImage));

			memset(lpwzFullSysName,0,sizeof(lpwzFullSysName));
			memset(lpwzBaseSysName,0,sizeof(lpwzBaseSysName));

			memset(LoadCount,0,sizeof(LoadCount));
			memset(IntHideType,0,sizeof(IntHideType));

			wsprintfW(lpwzSysBase,L"0x%08X",SysModuleInfo->SysInfo[i].ulSysBase);
			wsprintfW(lpwzSizeOfImage,L"0x%08X",SysModuleInfo->SysInfo[i].SizeOfImage);


			wcscat(lpwzFullSysName,SysModuleInfo->SysInfo[i].lpwzFullSysName);
			wcscat(lpwzBaseSysName,SysModuleInfo->SysInfo[i].lpwzBaseSysName);

			wsprintfW(LoadCount,L"%d",SysModuleInfo->SysInfo[i].LoadCount);

/*			if (SysModuleInfo->SysInfo[i].IntHideType == 1)
			{
				wcscat(IntHideType,L"隐藏驱动");
				m_list->InsertItem(i,lpwzSysBase,RGB(255,20,147));
			}else
			{
				wcscat(IntHideType,L"-");
				m_list->InsertItem(i,lpwzSysBase,RGB(77,77,77));
			}
*/
			/////
			WCHAR lpwzDosFullPath[256];
			WCHAR lpwzWinDir[256];
			WCHAR lpwzSysDisk[256];

			memset(lpwzWinDir,0,sizeof(lpwzWinDir));
			memset(lpwzSysDisk,0,sizeof(lpwzSysDisk));
			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));

			GetWindowsDirectoryW(lpwzWinDir,sizeof(lpwzWinDir));
			memcpy(lpwzSysDisk,lpwzWinDir,4);

			if (wcsstr(lpwzFullSysName,L"\\??\\"))
			{
				//开始这种路径的处理
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcsncpy(lpwzDosFullPath,lpwzFullSysName+wcslen(L"\\??\\"),wcslen(lpwzFullSysName)-wcslen(L"\\??\\"));
				goto Next;
			}
			if (wcsstr(lpwzFullSysName,L"\\WINDOWS\\system32\\"))
			{
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcscat(lpwzDosFullPath,lpwzSysDisk);
				wcscat(lpwzDosFullPath,lpwzFullSysName);
				//MessageBoxW(lpwzDosFullPath,lpwzFullSysName,0);
				goto Next;
			}
			if (wcsstr(lpwzFullSysName,L"\\SystemRoot\\"))
			{
				WCHAR lpwzTemp[256];
				memset(lpwzTemp,0,sizeof(lpwzTemp));
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcscat(lpwzTemp,lpwzSysDisk);
				wcscat(lpwzTemp,L"\\WINDOWS\\");
				wcscat(lpwzDosFullPath,lpwzTemp);
				wcsncpy(lpwzDosFullPath+wcslen(lpwzTemp),lpwzFullSysName+wcslen(L"\\SystemRoot\\"),wcslen(lpwzFullSysName) - wcslen(L"\\SystemRoot\\"));
				goto Next;
			}
			if (wcslen(lpwzFullSysName) == wcslen(lpwzBaseSysName))
			{
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcscat(lpwzDosFullPath,lpwzSysDisk);
				wcscat(lpwzDosFullPath,L"\\WINDOWS\\system32\\drivers\\");
				wcscat(lpwzDosFullPath,lpwzBaseSysName);
				goto Next;
			}
Next:
			/////
			//处理MD5
			//计算md5
			char lpszFullPath[5024] = {0};
			WCHAR lpwzMD5[256];
			WCHAR lpwzTrue[256];

			memset(lpszFullPath,0,sizeof(lpszFullPath));
			memset(lpwzMD5,0,sizeof(lpwzMD5));
			memset(lpwzTrue,0,sizeof(lpwzTrue));
			WideCharToMultiByte( CP_ACP,
				0,
				lpwzDosFullPath,
				-1,
				lpszFullPath,
				wcslen(lpwzDosFullPath)*2,
				NULL,
				NULL);
			FILE * fp=fopen(lpszFullPath,"rb");
			if(fp)
			{
				MD5VAL val;
				val = md5File(fp);
				wsprintfW(lpwzMD5,L"%08x%08x%08x%08x",conv(val.a),conv(val.b),conv(val.c),conv(val.d));
				FileVerify(lpszFullPath,lpwzMD5,lpwzTrue);
				//MessageBoxW(lpwzDosFullPath,lpwzMD5,0);
				fclose(fp);
			}
			//文件不存在
			if (GetFileAttributesA(lpszFullPath) == -1)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"文件不存在，无法验证");
			}
			//只看非认证
			if (IntLookType == 1)
			{
				if (wcsstr(lpwzTrue,L"本地") ||
					wcsstr(lpwzTrue,L"不支持当前系统"))
				{
					continue;
				}
				//m_ModuleList.InsertItem(i,lpwzBase,RGB(238,118,0));
				if (_wcsnicmp(lpwzTrue,L"无法确认文件来源",wcslen(L"无法确认文件来源")) == 0)
				{
					m_list->InsertItem(x,lpwzSysBase,RGB(238,118,0));

				}else if (_wcsnicmp(lpwzTrue,L"文件不存在，无法验证",wcslen(L"文件不存在，无法验证")) == 0)
				{
					m_list->InsertItem(x,lpwzSysBase,RGB(255,20,147));
				}
				m_list->SetItemText(x,1,lpwzSizeOfImage);
				m_list->SetItemText(x,2,lpwzBaseSysName);
				m_list->SetItemText(x,3,lpwzDosFullPath);
				m_list->SetItemText(x,4,LoadCount);
				m_list->SetItemText(x,5,lpwzTrue);
				x++;
				continue;
			}
			if (_wcsnicmp(lpwzTrue,L"无法确认文件来源",wcslen(L"无法确认文件来源")) == 0)
			{
				m_list->InsertItem(i,lpwzSysBase,RGB(238,118,0));

			}else if (_wcsnicmp(lpwzTrue,L"文件不存在，无法验证",wcslen(L"文件不存在，无法验证")) == 0)
			{
				m_list->InsertItem(i,lpwzSysBase,RGB(255,20,147));
			}else
			{
				m_list->InsertItem(i,lpwzSysBase,RGB(77,77,77));
			}

			m_list->SetItemText(i,1,lpwzSizeOfImage);
			m_list->SetItemText(i,2,lpwzBaseSysName);
			m_list->SetItemText(i,3,lpwzDosFullPath);
			m_list->SetItemText(i,4,LoadCount);
			m_list->SetItemText(i,5,lpwzTrue);
		}
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"内核模块扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);
}
//非认证
void KernelModule1(HWND m_hWnd,ULONG ID,CClrListCtrl *m_list)
{
	QueryKernelModule(m_hWnd,ID,m_list,1);
}
void CopyKernelModuleDataToClipboard(HWND m_hWnd,CClrListCtrl *m_list)
{
	CString KernelModule;
	int ItemNum = m_list->GetItemCount();
	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	KernelModule.Format(L"%s",m_list->GetItemText(Item,3));

	WCHAR lpwzKernelModule[260];

	memset(lpwzKernelModule,0,sizeof(lpwzKernelModule));
	wcscat(lpwzKernelModule,KernelModule);
	CHAR lpszKernelModule[1024];
	char *lpString = NULL;

	memset(lpwzKernelModule,0,sizeof(lpwzKernelModule));
	memset(lpszKernelModule,0,sizeof(lpszKernelModule));
	wcscat(lpwzKernelModule,KernelModule);
	WideCharToMultiByte( CP_ACP,
		0,
		lpwzKernelModule,
		-1,
		lpszKernelModule,
		wcslen(lpwzKernelModule)*2,
		NULL,
		NULL
		);
	lpString = setClipboardText(lpszKernelModule);
	if (lpString)
	{
		MessageBoxW(m_hWnd,L"操作成功！",0,MB_ICONWARNING);
	}
}
BOOL SaveToFile(WCHAR *lpwzFilePath,PVOID Buffer,ULONG ulBufferSize)
{
	DWORD dwBytesWrite = 0;
	BOOL bRetOK = FALSE;

	HANDLE	hFile = CreateFileW(lpwzFilePath, GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(hFile, 0, 0, FILE_END);
		WriteFile(hFile,Buffer,ulBufferSize, &dwBytesWrite, NULL);
		CloseHandle(hFile);
		bRetOK = TRUE;
	}
	return bRetOK;
}
void OnDumpmemorydatatofile(HWND hWnd,CClrListCtrl *m_list)
{
	// TODO: 在此添加命令处理程序代码


	CString			m_path;

	CString KernelModuleBase;
	int ItemNum = m_list->GetItemCount();
	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	KernelModuleBase.Format(L"%s",m_list->GetItemText(Item,0));

	WCHAR lpwzKernelModuleBase[260];
	WCHAR lpwzSysBase[260];
	CHAR lpszSysSize[260];

	WCHAR lpwzFilePath[260];
	DWORD dwReadByte;
	WCHAR lpwzForMat[260];

	memset(lpwzForMat,0,sizeof(lpwzForMat));
	memset(lpwzFilePath,0,sizeof(lpwzFilePath));
	memset(lpwzSysBase,0,sizeof(lpwzSysBase));
	memset(lpwzKernelModuleBase,0,sizeof(lpwzKernelModuleBase));

	wcscat(lpwzKernelModuleBase,KernelModuleBase);
	if (!wcslen(lpwzKernelModuleBase))
	{
		return;
	}

	//恢复下PEPROCESS，不然无法因为保护着win7下无法打开对话框
	ReadFile((HANDLE)EXIT_PROCESS,0,0,&dwReadByte,0);

	CFileDialog dlg( FALSE,L"txt",0, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,L"所有文件|*.*");
	dlg.m_ofn.lpstrTitle= L"保存dump";
	if ( dlg.DoModal() == IDOK )
	{
		m_path = dlg.GetPathName();
		wsprintfW(lpwzFilePath,L"\\??\\%ws",m_path);

		for (int i=0;i< (int)SysModuleInfo->ulCount;i++)
		{
			wsprintfW(lpwzSysBase,L"0x%08X",SysModuleInfo->SysInfo[i].ulSysBase);
			wsprintfA(lpszSysSize,"%d",SysModuleInfo->SysInfo[i].SizeOfImage);

			if (_wcsnicmp(lpwzKernelModuleBase,lpwzSysBase,wcslen(lpwzSysBase)) == 0)
			{
				ReadFile((HANDLE)INIT_DUMP_KERNEL_MODULE_MEMORY,lpszSysSize,SysModuleInfo->SysInfo[i].ulSysBase,&dwReadByte,0);
				ReadFile((HANDLE)DUMP_KERNEL_MODULE_MEMORY,lpwzFilePath,wcslen(lpwzFilePath),&dwReadByte,0);

				wsprintfW(lpwzForMat,L"操作成功，dump文件保存至:%ws",m_path);
				MessageBoxW(hWnd,lpwzForMat,0,MB_ICONWARNING);
			}
		}
	}
	//再继续保护自己的进程
	char lpszInit[4];
	memset(lpszInit,0,sizeof(lpszInit));
	ReadFile((HANDLE)SAFE_SYSTEM,lpszInit,4,&dwReadByte,0);
}
