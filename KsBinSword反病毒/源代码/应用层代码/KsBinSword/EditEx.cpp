// EditEx.cpp : 实现文件
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "EditEx.h"
#include <ctype.h>
#include <afxole.h>
#include <afxdisp.h>

// CEditEx

IMPLEMENT_DYNAMIC(CEditEx, CEdit)
//WM_RBUTTONP

CEditEx::CEditEx()
{

}

CEditEx::~CEditEx()
{
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_CUT, &CEditEx::OnMenuCut)
	ON_COMMAND(ID_MENU_COPY, &CEditEx::OnMenuCopy)
	ON_COMMAND(ID_MENU_PASTE, &CEditEx::OnMenuPaste)
END_MESSAGE_MAP()


// CEditEx 消息处理程序

void CEditEx::OnRButtonDown(UINT nFlags, CPoint point) 
{

	//AfxMessageBox(L"设置成为只读模式,右键菜单屏蔽!");
	CEdit::OnRButtonDown(nFlags, point);
// 	CMenu *pMenu;
// 	pMenu=new CMenu;
// 	pMenu->LoadMenu(IDR_RIGHTBUTTON);//自己的弹出菜单的ID
// 	CMenu *pMenuPop=pMenu->GetSubMenu(0);
// 	pMenuPop->TrackPopupMenu(TPM_RIGHTALIGN,point.x,point.y,this);
// 	delete pMenu;

	return;

} 

//
void CEditEx::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (point.x == -1 && point.y == -1)
	{
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(RIGHTBUTTON));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	//pPopup->EnableMenuItem(ID_EDIT_UNDO, MF_GRAYED|MF_DISABLED|MF_BYCOMMAND);
	//pPopup->EnableMenuItem(ID_EDIT_CLEAR, MF_GRAYED|MF_DISABLED|MF_BYCOMMAND);
	//pPopup->EnableMenuItem(ID_EDIT_CUT, MF_GRAYED|MF_DISABLED|MF_BYCOMMAND);
	//pPopup->EnableMenuItem(ID_EDIT_COPY, MF_GRAYED|MF_DISABLED|MF_BYCOMMAND);
	
	COleDataObject	obj;	
	if (obj.AttachClipboard()) 
	{
		if(!obj.IsDataAvailable(CF_TEXT) && !obj.IsDataAvailable(RegisterClipboardFormat(L"BinaryData")))
				pPopup->EnableMenuItem(ID_EDIT_PASTE, MF_GRAYED|MF_DISABLED|MF_BYCOMMAND);
	}
	

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		this);
    return;
}


void CEditEx::OnMenuCut()
{
	SendMessage(WM_CUT);
}

void CEditEx::OnMenuCopy()
{
	SendMessage(WM_COPY);
}

void CEditEx::OnMenuPaste()
{
	SendMessage(WM_PASTE); 
}
