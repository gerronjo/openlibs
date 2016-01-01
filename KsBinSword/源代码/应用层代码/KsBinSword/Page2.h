#pragma once
#include "afxwin.h"
#include "EditEx.h"

// CPage2 对话框

class CPage2 : public CDialog
{
	DECLARE_DYNAMIC(CPage2)

public:
	CPage2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage2();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool IsPMon;//进程监视
	bool IsRMon;//注册表监视
	bool IsMMon;//模块监视

	CButton m_PMon;

	CButton m_RMon;

	CButton m_Mmon;

	afx_msg void OnBnClickedOk();
	BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedButton1();

	afx_msg void OnBnClickedExeUSBDiskCheck();

	afx_msg void OnBnClickedCheckASP();
};
