#pragma once


// CEditEx

class CEditEx : public CEdit
{
	DECLARE_DYNAMIC(CEditEx)

public:
	CEditEx();
	virtual ~CEditEx();
void OnRButtonDown(UINT nFlags, CPoint point) ;
void OnContextMenu(CWnd* pWnd, CPoint point);
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMenuCut();
public:
	afx_msg void OnMenuCopy();
public:
	afx_msg void OnMenuPaste();
};


