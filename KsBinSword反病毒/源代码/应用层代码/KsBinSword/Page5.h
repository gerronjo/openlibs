#pragma once
#include "afxcmn.h"
#include "EditEx.h"

// CPage5 对话框

class CPage5 : public CDialog
{
	DECLARE_DYNAMIC(CPage5)

public:
	CPage5(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage5();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
	CListCtrl m_List;
	HANDLE hDriver;
public:
	afx_msg void OnReSSDTAndThrowSpilth();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	void ShowSSDT(void);
public:
	afx_msg void OnReShowSSDT();
public:
	afx_msg void OnReSSDT();
public:
	afx_msg void OnDrawColorForMyList(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMRclickList3(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnMenuRusSSDT();
};
