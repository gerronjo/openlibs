// Page4.cpp : 实现文件
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "Page4.h"
#include "MySystem.h"
//#include <ws2spi.h>
//#pragma comment(lib,"ws2_32")
// CPage4 对话框

IMPLEMENT_DYNAMIC(CPage4, CDialog)

CPage4::CPage4(CWnd* pParent /*=NULL*/)
	: CDialog(CPage4::IDD, pParent)
{

}

CPage4::~CPage4()
{
}

void CPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_MonInfoEdit);
}


BEGIN_MESSAGE_MAP(CPage4, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CPage4::OnBnClickedResMonInfo)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage4::OnBnClickedClearInfo)
END_MESSAGE_MAP()


// CPage4 消息处理程序
HBRUSH CPage4::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	CBrush* brBkgnd = new CBrush();
	brBkgnd->CreateSolidBrush( RGB(250, 249, 250) );
	hbr = *brBkgnd;                // Control bkgnd


	return hbr;
}
void CPage4::OnBnClickedResMonInfo()// 原来这个函数是用来查看LSP，感觉不实用，改成进线程监视。
{
// 	
// 	CString EditPrintf;
// 	WCHAR wPrintf[200]={'\0'};
// 	int Errno;
// 	DWORD dwBufferLength;
// 	LPWSAPROTOCOL_INFOW pProtocolInfo=NULL;
// 
// 	int i,n,c;
// 
// 	WCHAR ProviderDllPath[MAX_PATH];
// 
// 	WSCEnumProtocols(NULL,pProtocolInfo,&dwBufferLength,&Errno);
// 
// 	 pProtocolInfo=(LPWSAPROTOCOL_INFOW)malloc(dwBufferLength);
// 
// 	n=WSCEnumProtocols(NULL,pProtocolInfo,&dwBufferLength,&Errno);
// 
// 	c=dwBufferLength;
// 
// 	for(i=0;i<n;i++)
// 	{
// 
// 		wsprintf(wPrintf,L"Protocol Name:%ws\r\n",pProtocolInfo[i].szProtocol);
//         EditPrintf+=wPrintf;
// 		WSCGetProviderPath((_GUID *)&pProtocolInfo[i].ProviderId,ProviderDllPath,&c,&Errno);
// 
// 		wsprintf(wPrintf,L"DLL File Path:%ws\r\n\r\n\r\n",ProviderDllPath);
// 		EditPrintf+=wPrintf;
// 	}
// 
// 	wsprintf(wPrintf,L"Protocol's count : %d\n",n);
//     EditPrintf+=wPrintf;
// 	free(pProtocolInfo);
// 	m_LSPEdit.SetWindowText(L"");
// 	m_LSPEdit.SetWindowText(EditPrintf);
// 	return ;
	int j=0;
	CString csTem,csEditCont;
	WCHAR wcOutMonInfoArr[MaxLenNum];
	PrintMonInfo(CMySystem:: hMyHandle);
	m_MonInfoEdit.GetWindowText(csEditCont);
	for (j=0;j<MaxMonNum;j++)
	{

        CMySystem::CharToWCHAR(wcOutMonInfoArr,g_OutMonInfoArr[j]);
		if (0==g_OutMonInfoArr[j][0])
		{
			continue;
		}
		csTem.Format(L"%s\r\n",wcOutMonInfoArr);
		csEditCont+=csTem;
	}
	m_MonInfoEdit.SetWindowText(csEditCont);
}
void CPage4::PrintMonInfo(HANDLE hMyHandle)
{
    memset(g_OutMonInfoArr,'\0',sizeof(CHAR)*MaxMonNum*MaxLenNum);
	unsigned long  DeviceRet=0;
	DeviceIoControl(
		hMyHandle  ,                       // handle to volume
		IOCTL_KSBINSWORD_GETMONINFO,                      // dwIoControlCode
		g_OutMonInfoArr,                          // MOVE_FILE_DATA structure
		sizeof(TCHAR)*MaxMonNum*MaxLenNum,     // size of input buffer
		g_OutMonInfoArr,                          // lpOutBuffer
		sizeof(TCHAR)*MaxMonNum*MaxLenNum,     // nOutBufferSize
		&DeviceRet,                        // number of bytes returned
		0);                                // OVERLAPPED structure
	return  ;
}
void CPage4::OnBnClickedClearInfo()
{
	m_MonInfoEdit.SetWindowText(L"");
}
