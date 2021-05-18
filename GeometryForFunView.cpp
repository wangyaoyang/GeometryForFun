// GeometryForFunView.cpp : CGeometryForFunView 类的实现
//

#include "stdafx.h"
#include "GeometryForFun.h"

#include "GeometryForFunDoc.h"
#include "GeometryForFunView.h"
#include ".\geometryforfunview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGeometryForFunView

IMPLEMENT_DYNCREATE(CGeometryForFunView, CView)

BEGIN_MESSAGE_MAP(CGeometryForFunView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
//	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
//	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

// CGeometryForFunView 构造/析构

CGeometryForFunView::CGeometryForFunView()
{
	// TODO: 在此处添加构造代码
	m_bMouseVertical = false;
	m_bMouseHorizontal = false;
	m_bMouseCaptured = false;
	m_bMouseOrigin = false;
	m_bMouseScale = false;
}

CGeometryForFunView::~CGeometryForFunView()
{
}

BOOL CGeometryForFunView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// CGeometryForFunView 绘制

void CGeometryForFunView::OnDraw(CDC* pCDC)
{
	CGeometryForFunDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	m_curve.m_Draw(pCDC);
}


// CGeometryForFunView 打印

BOOL CGeometryForFunView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGeometryForFunView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CGeometryForFunView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CGeometryForFunView 诊断

#ifdef _DEBUG
void CGeometryForFunView::AssertValid() const
{
	CView::AssertValid();
}

void CGeometryForFunView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGeometryForFunDoc* CGeometryForFunView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGeometryForFunDoc)));
	return (CGeometryForFunDoc*)m_pDocument;
}
#endif //_DEBUG


// CGeometryForFunView 消息处理程序

void CGeometryForFunView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CRect	rect;

	GetClientRect(rect);
	m_curve.m_SetExpress(							GetDocument()->m_szExpress);
	m_curve.m_SetFormula(							GetDocument()->m_szFormulaX,
													GetDocument()->m_szFormulaY,
													GetDocument()->m_szFormulaZ);
	m_curve.m_SetSymmetry(							GetDocument()->m_bSymmetryX,
													GetDocument()->m_bSymmetryY,
													GetDocument()->m_bSymmetryZ);
	m_curve.m_GetCoordinate().m_SetAngleH(			GetDocument()->m_fHorizontal);
	m_curve.m_GetCoordinate().m_SetAngleV(			GetDocument()->m_fVertical);
	m_curve.m_GetCoordinate().m_SetScale(			GetDocument()->m_fScale);
	m_curve.m_GetCoordinate().m_SetWindowOrigin(	GetDocument()->m_oCenter);
	m_curve.m_SetViewRange(							GetDocument()->m_fRangBegin,
													GetDocument()->m_fRangEnd,
													GetDocument()->m_nResolution );
	m_viewCtrl.SetWindowText(_T("视图控制━") + GetDocument()->GetTitle());
	m_viewCtrl.m_ShowAttrib(m_curve);
	//m_viewCtrl.MoveWindow(0,rect.bottom-100,rect.right,100);
}

BOOL CGeometryForFunView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect; 
	CBrush brush; 
	brush.CreateSolidBrush(RGB(0,0,0)); 
	GetClientRect(rect);
	pDC->FillRect(rect,&brush); 

	return TRUE;
}

int CGeometryForFunView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_viewCtrl.Create(IDD_VIEW_CONTROL,this) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_viewCtrl.m_SetParent(this);

	return 0;
}

void CGeometryForFunView::OnDestroy()
{
	CView::OnDestroy();
	
	m_viewCtrl.DestroyWindow();
}

void CGeometryForFunView::m_SetZoom(double scale)
{
	GetDocument()->m_fScale = scale;
	m_curve.m_GetCoordinate().m_SetScale(scale);
	m_curve.m_PreDraw();
	Invalidate();
}

void CGeometryForFunView::m_SetHorizontal(double angle)
{
	GetDocument()->m_fHorizontal = angle;
	m_curve.m_GetCoordinate().m_SetAngleH(angle);
	m_curve.m_PreDraw();
	Invalidate();
}

void CGeometryForFunView::m_SetVertical(double angle)
{
	GetDocument()->m_fVertical = angle;
	m_curve.m_GetCoordinate().m_SetAngleV(angle);
	m_curve.m_PreDraw();
	Invalidate();
}

void CGeometryForFunView::m_SetCenter(CPoint center)
{
	GetDocument()->m_oCenter = center;
	m_curve.m_GetCoordinate().m_SetWindowOrigin(center);
	m_curve.m_PreDraw();
	Invalidate();
}

void CGeometryForFunView::m_SetRangeResolution(double fBegin,double fEnd,UINT nResolution)
{
	if( fBegin == fEnd )
	{
		GetDocument()->m_nResolution = nResolution;
		m_curve.m_SetResolution(nResolution);
	}
	else
	{
		GetDocument()->m_fRangBegin = fBegin;
		GetDocument()->m_fRangEnd = fEnd;
		GetDocument()->m_nResolution = nResolution;
		m_curve.m_SetViewRange(fBegin,fEnd,nResolution);
	}
}

void CGeometryForFunView::m_SetExpress(CString szExpress)
{
	GetDocument()->m_szExpress = szExpress;
	m_curve.m_SetExpress(szExpress);
}

void CGeometryForFunView::m_SetFormula(	CString szFormulaX,UINT bSymmetryX,
										CString szFormulaY,UINT bSymmetryY,
										CString szFormulaZ,UINT bSymmetryZ,bool b3D)
{
	CWaitCursor		wait;
	GetDocument()->m_szFormulaX = szFormulaX;
	GetDocument()->m_szFormulaY = szFormulaY;
	GetDocument()->m_szFormulaZ = szFormulaZ;
	GetDocument()->m_bSymmetryX = bSymmetryX;
	GetDocument()->m_bSymmetryY = bSymmetryY;
	GetDocument()->m_bSymmetryZ = bSymmetryZ;
	m_curve.m_Calualate(szFormulaX,bSymmetryX,
						szFormulaY,bSymmetryY,
						szFormulaZ,bSymmetryZ,b3D);
	m_curve.m_PreDraw();
	Invalidate();
}

void CGeometryForFunView::m_SetMouseHorizontal(bool b)
{
	m_bMouseHorizontal = b;
}

void CGeometryForFunView::m_SetMouseVertical(bool b)
{
	m_bMouseVertical = b;
}

void CGeometryForFunView::m_SetMouseOrigin(bool b)
{
	m_bMouseOrigin = b;
}

void CGeometryForFunView::m_SetMouseScale(bool b)
{
	m_bMouseScale = b;
}

void CGeometryForFunView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bMouseCaptured = true;
	m_oMouseZoom = point;
	m_oMouseVertical = point;
	m_oMouseHorizontal = point;
	CView::OnLButtonDown(nFlags, point);
}

void CGeometryForFunView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bMouseCaptured = false;
	m_viewCtrl.m_ShowAttrib(m_curve);
	CView::OnLButtonUp(nFlags, point);
}

void CGeometryForFunView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if( m_viewCtrl.IsWindowVisible() )
	{
		m_viewCtrl.ShowWindow(SW_HIDE);
		SetFocus();
	}
	else
	{
		m_viewCtrl.ShowWindow(SW_SHOW);
	}
	CView::OnRButtonDown(nFlags, point);
}

void CGeometryForFunView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if( m_bMouseCaptured )
	{
		CRect	rect;
		GetClientRect( rect );
		if( m_bMouseHorizontal )
		{
			double	x = point.x - m_oMouseHorizontal.x;
			double	w = rect.right;
			double	angleH = m_curve.m_GetCoordinate().m_GetAngleH();
			angleH += x*360.0f/w;
			m_curve.m_GetCoordinate().m_SetAngleH(angleH);
			m_oMouseHorizontal = point;
		}
		if( m_bMouseVertical )
		{
			double	y = m_oMouseVertical.y - point.y;
			double	w = rect.bottom;
			double	angleV = m_curve.m_GetCoordinate().m_GetAngleV();
			angleV += y*360.0f/w;
			m_curve.m_GetCoordinate().m_SetAngleV(angleV);
			m_oMouseVertical = point;
		}
		if( m_bMouseScale )
		{
			double	delta = point.y - m_oMouseZoom.y;
			double	w = rect.bottom;
			double	scale = m_curve.m_GetCoordinate().m_GetScale();
			scale *= 1+delta/w;
			m_curve.m_GetCoordinate().m_SetScale(scale);
			m_oMouseZoom = point;
		}
		if( m_bMouseOrigin )
		{
			m_curve.m_GetCoordinate().m_SetWindowOrigin(point);
		}
		if( m_bMouseHorizontal || m_bMouseVertical || m_bMouseOrigin || m_bMouseScale )
		{
			GetDocument()->m_oCenter = m_curve.m_GetCoordinate().m_GetWindowOrigin();
			GetDocument()->m_fHorizontal = m_curve.m_GetCoordinate().m_GetAngleH();
			GetDocument()->m_fVertical = m_curve.m_GetCoordinate().m_GetAngleV();
			GetDocument()->m_fScale = m_curve.m_GetCoordinate().m_GetScale();
			m_curve.m_PreDraw();
			Invalidate();
		}
	}
	CView::OnMouseMove(nFlags, point);
}

//void CGeometryForFunView::OnKillFocus(CWnd* pNewWnd)
//{
//	m_viewCtrl.ShowWindow(SW_HIDE);
//
//	CString	szExpress;
//	m_curve.m_GetExpress(szExpress);
//	TRACE0("\n"+szExpress+"Lost Focus");
//	CView::OnKillFocus(pNewWnd);
//}

void CGeometryForFunView::OnSetFocus(CWnd* pOldWnd)
{
	CString	szExpress;
	m_curve.m_GetExpress(szExpress);
	TRACE(_T("\n") + szExpress + _T("Get Focus"));
	CView::OnSetFocus(pOldWnd);
}

void CGeometryForFunView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_viewCtrl.MoveWindow(cx-700,cy-70,700,70);
}