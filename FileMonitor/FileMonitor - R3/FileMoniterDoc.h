// FileMoniterDoc.h : interface of the CFileMoniterDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMONITERDOC_H__EC5F81B1_6E18_4BF6_A0CE_AD7D762B4824__INCLUDED_)
#define AFX_FILEMONITERDOC_H__EC5F81B1_6E18_4BF6_A0CE_AD7D762B4824__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFileMoniterDoc : public CDocument
{
protected: // create from serialization only
	CFileMoniterDoc();
	DECLARE_DYNCREATE(CFileMoniterDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileMoniterDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFileMoniterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFileMoniterDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEMONITERDOC_H__EC5F81B1_6E18_4BF6_A0CE_AD7D762B4824__INCLUDED_)
