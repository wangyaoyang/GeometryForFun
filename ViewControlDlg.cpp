// ViewControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GeometryForFun.h"
#include "ViewControlDlg.h"
#include ".\viewcontroldlg.h"
#include "GeometryForFunView.h"
#include "math.h"

// CViewControlDlg 对话框

IMPLEMENT_DYNAMIC(CViewControlDlg, CDialog)
CViewControlDlg::CViewControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewControlDlg::IDD, pParent)
	, m_fVertical(0)
	, m_fHorizontal(0)
	, m_fScale(0)
{
	m_pView = NULL;
}

CViewControlDlg::~CViewControlDlg()
{
}

void CViewControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_HORIZONTAL, m_sliderHorizontal);
	DDX_Control(pDX, IDC_SLIDER_VERTICAL, m_sliderVertical);
	DDX_Control(pDX, IDC_CHECK_VERTICAL, m_checkVertical);
	DDX_Control(pDX, IDC_CHECK_HORIZONTAL, m_checkHorizontal);
	DDX_Control(pDX, IDC_EDIT_VERTICAL, m_editVertical);
	DDX_Control(pDX, IDC_EDIT_HORIZONTAL, m_editHorizontal);
	DDX_Text(pDX, IDC_EDIT_VERTICAL, m_fVertical);
	DDX_Text(pDX, IDC_EDIT_HORIZONTAL, m_fHorizontal);
	DDX_Control(pDX, IDC_CHECK_MOVE_CENTER, m_checkMoveCenter);
	DDX_Control(pDX, IDC_CHECK_SCALE, m_checkScale);
	DDX_Control(pDX, IDC_EDIT_SCALE, m_editScale);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_fScale);
	DDV_MinMaxDouble(pDX, m_fScale, 0.00000000001, 10000000000);
	DDX_Control(pDX, IDC_EDIT_EXPRESS, m_editExpress);
	DDX_Control(pDX, IDC_EDIT_FORMULAY, m_editFormulaY);
	DDX_Control(pDX, IDC_EDIT_RANGE_BEGIN, m_editRangeBegin);
	DDX_Control(pDX, IDC_EDIT_RANGE_END, m_editRangeEnd);
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_comboResolution);
	DDX_Control(pDX, IDC_EDIT_FORMULAX, m_editFormulaX);
	DDX_Control(pDX, IDC_CHECK_SYMMETRY_Y, m_checkSymmetryY);
	DDX_Control(pDX, IDC_CHECK_SYMMETRY_X, m_checkSymmetryX);
	DDX_Control(pDX, IDC_CHECK_SYMMETRY_Z, m_checkSymmetryZ);
	DDX_Control(pDX, IDC_EDIT_FORMULAZ, m_editFormulaZ);
}


BEGIN_MESSAGE_MAP(CViewControlDlg, CDialog)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_HORIZONTAL, OnNMReleasedcaptureSliderHorizontal)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_VERTICAL, OnNMReleasedcaptureSliderVertical)
	ON_BN_CLICKED(IDC_CHECK_VERTICAL, OnBnClickedCheckVertical)
	ON_BN_CLICKED(IDC_CHECK_HORIZONTAL, OnBnClickedCheckHorizontal)
	ON_EN_CHANGE(IDC_EDIT_VERTICAL, OnEnChangeEditVertical)
	ON_EN_CHANGE(IDC_EDIT_HORIZONTAL, OnEnChangeEditHorizontal)
	ON_BN_CLICKED(IDC_CHECK_MOVE_CENTER, OnBnClickedCheckMoveCenter)
	ON_BN_CLICKED(IDC_CHECK_SCALE, OnBnClickedCheckScale)
	ON_EN_CHANGE(IDC_EDIT_SCALE, OnEnChangeEditScale)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, OnBnClickedButtonCalculate)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE2D, OnBnClickedButtonCalculate2d)
END_MESSAGE_MAP()


// CViewControlDlg 消息处理程序
void CViewControlDlg::m_SetParent(CWnd* pParent)
{
	m_pView = (CGeometryForFunView*)pParent;
}

BOOL CViewControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HWND	hWnd = GetSafeHwnd();
	SetWindowLong(	hWnd,GWL_EXSTYLE,
					GetWindowLong(hWnd,GWL_EXSTYLE)^0x80000);
	HINSTANCE hInst = LoadLibrary(_T("User32.DLL")); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		//取得SetLayeredWindowAttributes函数指针 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if( fun )
			fun(hWnd,0,158,2); 
		FreeLibrary(hInst); 
	}

	m_sliderVertical.SetPos(0);		m_sliderVertical.SetTicFreq(10);	m_sliderVertical.SetRange(-180,180,TRUE);
	m_sliderVertical.SetPos(0);		m_sliderVertical.SetTicFreq(10);	m_sliderVertical.SetRange(-180,180,TRUE);
	m_sliderHorizontal.SetPos(0);	m_sliderHorizontal.SetTicFreq(10);	m_sliderHorizontal.SetRange(-180,180,TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CViewControlDlg::OnNMReleasedcaptureSliderHorizontal(NMHDR *pNMHDR, LRESULT *pResult)
{
	if( m_pView )
		m_pView->m_SetHorizontal( m_sliderHorizontal.GetPos() );
	*pResult = 0;
}

void CViewControlDlg::OnNMReleasedcaptureSliderVertical(NMHDR *pNMHDR, LRESULT *pResult)
{
	if( m_pView )
		m_pView->m_SetVertical( m_sliderVertical.GetPos() );
	*pResult = 0;
}

void CViewControlDlg::OnBnClickedCheckVertical()
{
	if( m_pView )
		m_pView->m_SetMouseVertical( m_checkVertical.GetCheck()?true:false );
}

void CViewControlDlg::OnBnClickedCheckHorizontal()
{
	if( m_pView )
		m_pView->m_SetMouseHorizontal( m_checkHorizontal.GetCheck()?true:false );
}

void CViewControlDlg::OnBnClickedCheckMoveCenter()
{
	if( m_pView )
		m_pView->m_SetMouseOrigin( m_checkMoveCenter.GetCheck()?true:false );
}

void CViewControlDlg::OnBnClickedCheckScale()
{
	if( m_pView )
		m_pView->m_SetMouseScale( m_checkScale.GetCheck()?true:false );
}

void CViewControlDlg::OnEnChangeEditVertical()
{
	CString	szAngle;
	double	angle = 0.0f;
	int		nPi = 0;

	m_editVertical.GetWindowText(szAngle);
	uniscanf( szAngle,_T("%lf"),&angle );
	nPi = angle/360;
	angle -= nPi*360;
	m_sliderVertical.SetPos(angle);
	if( m_pView )
		m_pView->m_SetVertical( angle );

}

void CViewControlDlg::OnEnChangeEditHorizontal()
{
	CString	szAngle;
	double	angle = 0.0f;
	int		nPi = 0;

	m_editHorizontal.GetWindowText(szAngle);
	uniscanf( szAngle,_T("%lf"),&angle );
	nPi = angle/360;
	angle -= nPi*360;
	m_sliderHorizontal.SetPos(angle);
	if( m_pView )
		m_pView->m_SetHorizontal( angle );
}

void CViewControlDlg::OnEnChangeEditScale()
{
	CString	szScale;
	double	scale = 0.0f;

	m_editScale.GetWindowText(szScale);
	uniscanf( szScale,_T("%lf"),&scale );
	if( scale == 0 ) scale = 1;
	else if( scale < 0 ) scale = 1/abs(scale);
	if( m_pView )
		m_pView->m_SetZoom( scale );
}

void CViewControlDlg::OnBnClickedButtonCalculate2d()
{
	CString		szExpress;
	CString		szFormulaX;
	CString		szFormulaY;
	CString		szFormulaZ;
	CString		szRange0;
	CString		szRange1;
	CString		szResolution;
	double		fRange0 = -1.0f;
	double		fRange1 = 1.0f;
	UINT		nResolution = 10;
	UINT		bSymmetryX = m_checkSymmetryX.GetCheck();
	UINT		bSymmetryY = m_checkSymmetryY.GetCheck();
	UINT		bSymmetryZ = m_checkSymmetryZ.GetCheck();
	m_editExpress.GetWindowText(szExpress);
	m_editFormulaX.GetWindowText(szFormulaX);
	m_editFormulaY.GetWindowText(szFormulaY);
	m_editFormulaZ.GetWindowText(szFormulaZ);
	m_editRangeBegin.GetWindowText(szRange0);
	m_editRangeEnd.GetWindowText(szRange1);
	m_comboResolution.GetWindowText(szResolution);
	uniscanf( szRange0,_T("%lf"),&fRange0 );
	uniscanf( szRange1,_T("%lf"),&fRange1 );
	uniscanf( szResolution,_T("%u"),&nResolution );
	if( m_pView )
	{
		m_pView->m_SetRangeResolution(fRange0,fRange1,nResolution);
		m_pView->m_SetExpress(szExpress);
		m_pView->m_SetFormula(	szFormulaX,bSymmetryX,
								szFormulaY,bSymmetryY,
								szFormulaZ,bSymmetryZ,false);
	}
}

void CViewControlDlg::OnBnClickedButtonCalculate()
{
	CString		szExpress;
	CString		szFormulaX;
	CString		szFormulaY;
	CString		szFormulaZ;
	CString		szRange0;
	CString		szRange1;
	CString		szResolution;
	double		fRange0 = -1.0f;
	double		fRange1 = 1.0f;
	UINT		nResolution = 10;
	UINT		bSymmetryX = m_checkSymmetryX.GetCheck();
	UINT		bSymmetryY = m_checkSymmetryY.GetCheck();
	UINT		bSymmetryZ = m_checkSymmetryZ.GetCheck();
	m_editExpress.GetWindowText(szExpress);
	m_editFormulaX.GetWindowText(szFormulaX);
	m_editFormulaY.GetWindowText(szFormulaY);
	m_editFormulaZ.GetWindowText(szFormulaZ);
	m_editRangeBegin.GetWindowText(szRange0);
	m_editRangeEnd.GetWindowText(szRange1);
	m_comboResolution.GetWindowText(szResolution);
	uniscanf( szRange0,_T("%lf"),&fRange0 );
	uniscanf( szRange1,_T("%lf"),&fRange1 );
	uniscanf( szResolution,_T("%u"),&nResolution );
	if( m_pView )
	{
		m_pView->m_SetRangeResolution(fRange0,fRange1,nResolution);
		m_pView->m_SetExpress(szExpress);
		m_pView->m_SetFormula(	szFormulaX,bSymmetryX,
								szFormulaY,bSymmetryY,
								szFormulaZ,bSymmetryZ);
	}
}

void CViewControlDlg::m_ShowAttrib(	CCurve& curve )
								   /*CString szExpress,CString szFormulaY,
									double fHorizontal,double fVertical,double fScale,
									double fRangeBegin,double fRangeEnd,UINT nResolution )*/
{
	CString	szExpress,szFormulaX,szFormulaY,szFormulaZ;
	CString	szHorizontal,szVertical,szScale;
	CString	szRangeBegin,szRangeEnd,szResolution;
	double	fBegin = 0.0f,fEnd = 0.0f;
	UINT	bSymmetryX = false,bSymmetryY = false,bSymmetryZ = false;
	UINT	nResolution = 10;

	curve.m_GetExpress(szExpress);
	curve.m_GetFormula(szFormulaX,szFormulaY,szFormulaZ);
	curve.m_GetSymmetry(bSymmetryX,bSymmetryY,bSymmetryZ);
	curve.m_GetViewRange(fBegin,fEnd,nResolution);
	szHorizontal.Format(_T("%lf"),curve.m_GetCoordinate().m_GetAngleH());
	szVertical.Format(_T("%lf"),curve.m_GetCoordinate().m_GetAngleV());
	szScale.Format(_T("%lf"),curve.m_GetCoordinate().m_GetScale());
	szRangeBegin.Format(_T("%lf"),fBegin);
	szRangeEnd.Format(_T("%lf"),fEnd);
	szResolution.Format(_T("%u"),nResolution);

	m_editExpress.SetWindowText( szExpress );
	m_editFormulaX.SetWindowText( szFormulaX );
	m_editFormulaY.SetWindowText( szFormulaY );
	m_editFormulaZ.SetWindowText( szFormulaZ );
	m_checkSymmetryX.SetCheck( bSymmetryX );
	m_checkSymmetryY.SetCheck( bSymmetryY );
	m_checkSymmetryZ.SetCheck( bSymmetryZ );
	m_editHorizontal.SetWindowText( szHorizontal );
	m_editVertical.SetWindowText( szVertical );
	m_editScale.SetWindowText( szScale );
	m_editRangeBegin.SetWindowText( szRangeBegin );
	m_editRangeEnd.SetWindowText( szRangeEnd );
	m_comboResolution.SelectString( 0,szResolution );
}

void CViewControlDlg::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
}
