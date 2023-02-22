// CaponDlg.h : header file
//

#if !defined(AFX_CAPONDLG_H__BB174410_1309_4B9A_B59A_F296F6BF6DF3__INCLUDED_)
#define AFX_CAPONDLG_H__BB174410_1309_4B9A_B59A_F296F6BF6DF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCaponDlg dialog

class CCaponDlg : public CDialog
{
// Construction
public:
	CCaponDlg(CWnd* pParent = NULL);	// standard constructor


// Dialog Data
	//{{AFX_DATA(CCaponDlg)
	enum { IDD = IDD_CAPON_DIALOG };
	double	m_SNR;
	double	m_theta1;
	double	m_theta2;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaponDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;


	// Generated message map functions
	//{{AFX_MSG(CCaponDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDrawCapon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAPONDLG_H__BB174410_1309_4B9A_B59A_F296F6BF6DF3__INCLUDED_)
