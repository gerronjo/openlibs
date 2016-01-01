#pragma once
#include  <Afxtempl.h>

// CMyList

class CMyList : public CListCtrl
{
	DECLARE_DYNAMIC(CMyList)

public:
	CMyList();
	CMap<DWORD , DWORD& , COLORREF , COLORREF&> MapItemColor;
	virtual ~CMyList();
	void SetItemColor(DWORD  iItem, COLORREF color);
	bool InitSystemImageLists(HWND hwndList);
	int  GetFileIcon(LPCTSTR lpFileName);
protected:
	DECLARE_MESSAGE_MAP()
};


