#pragma once
#include "afxcmn.h"
#include "DrvFltIp.h"
#include "MyDriver.h"
#include "afxwin.h"
#include "EditEx.h"

#define IDT_TIMER1  1
#define MAXPRINT 50
#define MAXSTR 70
typedef
char FAR *
(WINAPI *pinet_ntoa)(
//struct in_addr in
  DWORD in
	);
typedef
unsigned long
(WINAPI *pinet_addr)(
					 const char FAR * cp
					 );


// CPage8 对话框
typedef struct _RuleInfo
{
	unsigned long sourceIp;
	unsigned long sourceMask;
	unsigned short sourcePort;

	unsigned long destinationIp;
	unsigned long destinationMask;
	unsigned short destinationPort;

	unsigned int protocol;

	int action;
}RuleInfo, *PRuleInfo;
#define MAX_RULES 15
class CPage8 : public CDialog
{
	DECLARE_DYNAMIC(CPage8)

public:
	CPage8(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage8();
    BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_rules;
	RuleInfo rules[MAX_RULES];
	unsigned int nRules;
	BOOL started;
    MyDriver filterDriver;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void UpdateList(void);
	void AddRuleToList(unsigned long srcIp, 
		unsigned long srcMask,
		unsigned short srcPort, 
		unsigned long dstIp, 
		unsigned long dstMask,
		unsigned short dstPort, 
		unsigned int protocol, 
		int action);
	int AddRule(unsigned long srcIp,
		unsigned long srcMask,
		unsigned short srcPort,
		unsigned long dstIp,
		unsigned long dstMask,
		unsigned short dstPort,
		unsigned int protocol,
		int action);
	void ResetRules();

	void DeleteRule(unsigned int position);
	BOOL AddFilterToFw(unsigned long srcIp, 
		unsigned long srcMask,
		unsigned short srcPort, 
		unsigned long dstIp,
		unsigned long dstMask,
		unsigned short dstPort,
		unsigned int protocol,
		int action);
	void OnTimer(UINT nIDEvent) ;

	WCHAR g_OutArr[MAXPRINT][MAXSTR];
	void OutPrint(HANDLE hMyHandle);
	int nMessageCount;
	HMODULE hModulentdll;
	pinet_ntoa inet_ntoa;
	pinet_addr inet_addr;

public:
	afx_msg void OnButtonAdd();
public:
	afx_msg void OnButtonDel();
public:
	afx_msg void OnButtoninStall();
public:
	afx_msg void OnLoadRules();
public:
	afx_msg void OnSaveRules();
public:
	afx_msg void OnButtonStart();
public:
	afx_msg void OnButtonStop();
public:
	afx_msg void OnButtonDesinStall();
public:
	CButton m_Monitor;
public:
	CButton m_StopMonitor;
public:
	afx_msg void OnBnClickedMonitor();
public:
	afx_msg void OnBnClickedStopMonitor();

public:
	CEditEx m_NetData;
public:
	afx_msg void OnBnClickedClearMessage();
	 CString ChangeProtocolToCString(CString csProtocol);
};
