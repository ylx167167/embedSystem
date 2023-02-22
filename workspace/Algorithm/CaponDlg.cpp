// CaponDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Capon.h"
#include "CaponDlg.h"
#include "My_capon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCaponDlg dialog

CCaponDlg::CCaponDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCaponDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCaponDlg)
	m_SNR = 0.0;
	m_theta1 = 0.0;
	m_theta2 = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCaponDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaponDlg)
	DDX_Text(pDX, IDC_SNR, m_SNR);
	DDV_MinMaxDouble(pDX, m_SNR, 0., 50.);
	DDX_Text(pDX, IDC_theta1, m_theta1);
	DDV_MinMaxDouble(pDX, m_theta1, -90., 89.);
	DDX_Text(pDX, IDC_theta2, m_theta2);
	DDV_MinMaxDouble(pDX, m_theta2, -90., 89.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCaponDlg, CDialog)
	//{{AFX_MSG_MAP(CCaponDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DrawCapon, OnDrawCapon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCaponDlg message handlers

BOOL CCaponDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_SNR=10;
	m_theta1=20;
	m_theta2=-20;
	UpdateData(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCaponDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCaponDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	CDC *dc;
	dc = GetDlgItem(IDC_CaponImage)->GetDC();
    CRect rcclient;
	GetDlgItem(IDC_CaponImage)->GetWindowRect(&rcclient);

	CBrush pBrush;
	pBrush.CreateSolidBrush(RGB(255,255,255));
	dc->FillRect(CRect(0,0,rcclient.Width(),rcclient.Height()),&pBrush);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCaponDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCaponDlg::OnDrawCapon() 
{
	// TODO: Add your control notification handler code here
	double G_capon[360];
	int G[360];
	double nMax=-100000.0, nMin=100000.0;
	int i,j;
	int M=8;               //阵元数

	UpdateData(true);

	//计算capon波束形成器的各角度幅值,计算结果有G_capon返回
	Compute_capon(m_theta1,m_theta2,m_SNR, M, G_capon);  

	for(i=0;i<360;i++)
	{
		if(G_capon[i]>nMax) nMax=G_capon[i];    //找到最大值
		if(G_capon[i]<nMin) nMin=G_capon[i];    //找到最小值
	}
	for(i=0;i<360;i++)
	{
		G_capon[i]+=abs(nMin);     //通过整体平移，把负值都变成正值	
	}
	nMax=nMax+abs(nMin);
	for(i=0;i<360;i++)    //量化成高为250,宽为360的数组
	{
		G[i]=int((double)G_capon[i]/nMax*250) ;
		if(G[i]<=0) G[i]=0;
	}
	

	//////画波束图//////////////////////////
	CDC *dc;
	dc = GetDlgItem(IDC_CaponImage)->GetDC();
    CRect rcclient;
	GetDlgItem(IDC_CaponImage)->GetWindowRect(&rcclient);

	CBrush pBrush;
	pBrush.CreateSolidBrush(RGB(255,255,255));
	dc->FillRect(CRect(0,0,rcclient.Width(),rcclient.Height()),&pBrush);
	
	CPen pen1(PS_SOLID,1,RGB(0,0,255));
	CPen *oldPen;
	oldPen=dc->SelectObject(&pen1);
	for(i=0;i<360;i++)
	{
		for(j=0;j<250;j++)
		{
			if(G[i]==j && i==0)
			{
				dc->MoveTo(i,300-(j+25));
				continue ;
			}
			else if(G[i]==j)
				{
					dc->LineTo(i,300-(j+25));
					continue ;
				}
		}
	}
	pen1.DeleteObject();

	//////画刻度线////////
	CPen pen2(PS_DOT,1,RGB(0,0,0));
	dc->SelectObject(&pen2);
	for(i=0;i<361;)
	{
		dc->MoveTo(i,0);
		dc->LineTo(i,300);
		i+=20;
	}
	pen2.DeleteObject();
	dc->SelectObject(oldPen);

	/////标视横坐标刻度//////
	LOGFONT stFont;
	memset(&stFont,0,sizeof(LOGFONT));
	stFont.lfCharSet=134;
	stFont.lfHeight=10;
	stFont.lfWidth=5;

	CFont myFont;
	CFont *oldFont;
	CString strNum;
	char bb[10];
	myFont.CreateFontIndirect(&stFont);
	oldFont=dc->SelectObject(&myFont);
	dc->SetTextColor(RGB(255,0,0));
	dc->SetBkMode(TRANSPARENT);
	int k=-90;
	for(i=0;i<361;)
	{
		_itoa(k,bb,10);
		strNum=bb;
		dc->TextOut(i-5,300,strNum);
		k+=10;
		i+=20;
	}
	dc->SelectObject(oldFont);
	
}
