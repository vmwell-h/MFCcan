#if !defined(AFX_COLOREDLISTCTRL_H__86FEBE8E_F6FA_429F_B740_76F1769C81C6__INCLUDED_)
#define AFX_COLOREDLISTCTRL_H__86FEBE8E_F6FA_429F_B740_76F1769C81C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColoredListCtrl.h : header file
//
extern int nextrow;
/////////////////////////////////////////////////////////////////////////////
// CColoredListCtrl window

class CColoredListCtrl : public CListCtrl
{
// Construction
public:
	CColoredListCtrl();

// Attributes
public:
	enum { EmCol0 = 0, EmCol1 = 1, EmCol2 = 2, EmCol3 = 3};
	COLORREF m_colRow0;
	COLORREF m_colRow1;
	COLORREF m_colRow2;
	COLORREF m_colRow3; 
	COLORREF m_colRow4;
// Operations
public:
	unsigned char ItemColorFlag[60000];
	unsigned char ItemTextColor[60000];
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColoredListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColoredListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColoredListCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	
	//}}AFX_MSG
	afx_msg void CColoredListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOREDLISTCTRL_H__86FEBE8E_F6FA_429F_B740_76F1769C81C6__INCLUDED_)
