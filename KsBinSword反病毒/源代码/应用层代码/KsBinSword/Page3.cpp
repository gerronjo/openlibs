// Page3.cpp : 实现文件
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "Page3.h"
#include "MySystem.h"

// CPage3 对话框

IMPLEMENT_DYNAMIC(CPage3, CDialog)

CPage3::CPage3(CWnd* pParent /*=NULL*/)
	: CDialog(CPage3::IDD, pParent)
{

}

CPage3::~CPage3()
{
}

void CPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_MonEdit);
}


BEGIN_MESSAGE_MAP(CPage3, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CPage3::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPage3::OnBnClickedFindMod)
END_MESSAGE_MAP()


// CPage3 消息处理程序
HBRUSH CPage3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	CBrush* brBkgnd = new CBrush();
	brBkgnd->CreateSolidBrush( RGB(250, 249, 250) );
	hbr = *brBkgnd;                // Control bkgnd


	return hbr;
}
void CPage3::OnBnClickedOk()
{
	
}

void CPage3::OnBnClickedFindMod()
{
	CString EditPrintf;
    CString EditConvTem;
	WCHAR wPrintf[200]={'\0'};
	NTSTATUS status;
	ULONG size, index;
	PULONG buf;
	PSYSTEM_MODULE_INFORMATION module;
	PVOID driverAddress = 0;
	char *sysname[256];
	char *hljname[256];

    char   pModulePath[256] =   {'\0'};  
	WCHAR  wModulePath[256];  
	int   nLen   =   strlen(pModulePath)   +   1;   
	int   nwLen   =   MultiByteToWideChar(CP_ACP,   0,   pModulePath,   nLen,   NULL,   0);   

	EditPrintf=L"开始列举系统模块\r\n";
	MultiByteToWideChar(CP_ACP,   0,   pModulePath,   nLen,   wModulePath,   nwLen);   

	HMODULE hNTDLL   =   GetModuleHandle(L"ntdll.dll");
	pFnZwQuerySystemInformation ZwQuerySystemInformation = 
		(pFnZwQuerySystemInformation) GetProcAddress (hNTDLL, "ZwQuerySystemInformation");
	ZwQuerySystemInformation(SystemModuleInformation, &size, 0, &size);

	if (NULL == (buf = (PULONG)malloc( size)))
	{
		EditPrintf="failed alloc memory failed \n";
		return ;
	}

	status = ZwQuerySystemInformation(SystemModuleInformation, buf, size, 0);
	if (!NT_SUCCESS(status))
	{
		EditPrintf="failed query\n";
		return ;
	}

	module = (PSYSTEM_MODULE_INFORMATION)((PULONG)buf + 1);

	for (index = 0; index <  *buf; index++)
	{
		driverAddress = module[index].Base;
		wsprintf(wPrintf,L"Module found at:%x\r\n", (DWORD)driverAddress);
		EditPrintf+=wPrintf;

		sysname[index] = module[index].ImageName + module[index].ModuleNameOffset;
		nLen   =   strlen(sysname[index])   +   1;   
		nwLen  =   MultiByteToWideChar(CP_ACP,   0,   sysname[index],   nLen,   NULL,   0);  
		MultiByteToWideChar(CP_ACP,   0,   sysname[index],   nLen,   wModulePath,   nwLen);   
		wsprintf(wPrintf,L"Module found at:%s\r\n", wModulePath);
		EditPrintf+=wPrintf;

		hljname[index] = module[index].ImageName;

		nLen   =   strlen( hljname[index])   +   1;   
		nwLen  =   MultiByteToWideChar(CP_ACP,   0,   hljname[index],   nLen,   NULL,   0);  
		MultiByteToWideChar(CP_ACP,   0,   hljname[index],   nLen,   wModulePath,   nwLen);   
		wsprintf(wPrintf,L"Imagename found at:%s\r\n", wModulePath);
		EditConvTem=wPrintf;
		EditConvTem.MakeLower();

		EditPrintf+=EditConvTem;
		EditPrintf+="\r\n";

	}
	m_MonEdit.SetWindowText(L"");
	m_MonEdit.SetWindowText(EditPrintf);
	free(buf);
	//return ;//driverAddress
	//---------------------------------------------------------------
	//方式二：通过目录对象打开
	//CString EditPrintf;
	//WCHAR wPrintf[200]={'\0'};
	HMODULE hNtdll;
	UNICODE_STRING      usDirPath;
	OBJECT_ATTRIBUTES   oa;
	NTSTATUS            ntStatus;
	HANDLE                hDir;
	PDIRECTORY_BASIC_INFORMATION pBuffer=NULL,pBuffer2;
	ULONG               uLength = 0x800;
	ULONG                context,dwRet;
	RTLINITUNICODESTRING RtlInitUnicodeString;
	ZWOPENDIRECTORYOBJECT ZwOpenDirectoryObject;
	ZWQUERYDIRECTORYOBJECT ZwQueryDirectoryObject;
	ZWCLOSE ZwClose;
	int nHideModNum=0;
	hNtdll = LoadLibrary( L"ntdll.dll" );
	RtlInitUnicodeString = (RTLINITUNICODESTRING)GetProcAddress( hNtdll, "RtlInitUnicodeString");
	ZwOpenDirectoryObject = (ZWOPENDIRECTORYOBJECT)GetProcAddress( hNtdll, "ZwOpenDirectoryObject");
	ZwQueryDirectoryObject = (ZWQUERYDIRECTORYOBJECT)GetProcAddress( hNtdll, "ZwQueryDirectoryObject");
	ZwClose = (ZWCLOSE)GetProcAddress( hNtdll, "ZwClose");

    EditPrintf+=L"\r\n开始列举不属于上述模块的系统目录对象（有可能为被病毒隐藏的模块）\r\n";

	//打开目录对象

	RtlInitUnicodeString(&usDirPath, L"\\Driver");
	InitializeObjectAttributes(&oa,&usDirPath,OBJ_CASE_INSENSITIVE,NULL,NULL);
	printf("ZwOpenDirectoryObject ");
	ntStatus=ZwOpenDirectoryObject(&hDir,DIRECTORY_QUERY,&oa);
	if (ntStatus!=STATUS_SUCCESS)
	{
		EditPrintf=L"\r\nfailed! \r\n";
		goto __exit;
	}
	//EditPrintf="OK! \r\n";
	//EditPrintf+="ZwQueryDirectoryObject\r\n";
	//查询目录对象
	do 
	{
		if (pBuffer) free(pBuffer);
		uLength *= 2;
		pBuffer=(PDIRECTORY_BASIC_INFORMATION)malloc(uLength);
		if (!pBuffer)
		{
			EditPrintf+="malloc failed! ";
			goto __exit;
		}

		ntStatus=ZwQueryDirectoryObject(hDir,pBuffer,uLength,FALSE,TRUE,&context,&dwRet);
		wsprintf(wPrintf,L"dwRet:%d \r\n",dwRet);
		//EditPrintf+=wPrintf;
	} while (ntStatus == STATUS_MORE_ENTRIES || ntStatus == STATUS_BUFFER_TOO_SMALL);

	if (ntStatus==STATUS_SUCCESS)
	{
		EditPrintf+="\r\n";
		//遍历
		pBuffer2=pBuffer;
		while (pBuffer2->ObjectName.Length!=0&&pBuffer2->ObjectTypeName.Length!=0)
		{
			//wsprintf(wPrintf,L"ObjectName: [%s]  ObjectTypeName: [%s] \r\n\r\n",pBuffer2->ObjectName.Buffer,pBuffer2->ObjectTypeName.Buffer);
			wsprintf(wPrintf,L"%s",pBuffer2->ObjectName.Buffer);
			EditConvTem=wPrintf;
		    EditConvTem.MakeLower();
			if (-1==EditPrintf.Find(EditConvTem))
			{
				nHideModNum++;
				EditPrintf+=L" \r\n找到目录对象ObjectName: ";
				EditPrintf+=wPrintf;
				EditPrintf+=L" \r\n";
			}
            
			
			pBuffer2++;
		}
	}
	else
	{
		wsprintf(wPrintf,L"failed! ");
		EditPrintf+=wPrintf;
	}
__exit:
	if (0==nHideModNum)
	{
		EditPrintf+=L" 没有发现隐藏驱动！\r\n";
	}
	
	//m_MonEdit.SetWindowText(L"");
	m_MonEdit.SetWindowText(EditPrintf);
	if (pBuffer) free(pBuffer);
	if (hDir) ZwClose(hDir);
	return ;
}
