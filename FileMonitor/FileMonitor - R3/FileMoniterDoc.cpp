// FileMoniterDoc.cpp : implementation of the CFileMoniterDoc class
//

#include "stdafx.h"
#include "FileMoniter.h"

#include "FileMoniterDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileMoniterDoc

IMPLEMENT_DYNCREATE(CFileMoniterDoc, CDocument)

BEGIN_MESSAGE_MAP(CFileMoniterDoc, CDocument)
	//{{AFX_MSG_MAP(CFileMoniterDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileMoniterDoc construction/destruction

CFileMoniterDoc::CFileMoniterDoc()
{
	// TODO: add one-time construction code here

}

CFileMoniterDoc::~CFileMoniterDoc()
{
}

BOOL CFileMoniterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFileMoniterDoc serialization

void CFileMoniterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFileMoniterDoc diagnostics

#ifdef _DEBUG
void CFileMoniterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFileMoniterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFileMoniterDoc commands
