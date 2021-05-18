// GeometryForFunView.h : CGeometryForFunView ��Ľӿ�
//


#pragma once

#include ".\CoordinateSys\CoordinatePaper.h"
#include "ViewControlDlg.h"

class CGeometryForFunDoc;
class CGeometryForFunView : public CView
{
protected: // �������л�����
	CGeometryForFunView();
	DECLARE_DYNCREATE(CGeometryForFunView)

// ����
public:
	CGeometryForFunDoc* GetDocument() const;

// ����
public:

// ��д
	public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CGeometryForFunView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
protected:
	CViewControlDlg	m_viewCtrl;
private:
	CCurve			m_curve;
	CPoint			m_oMouseVertical;
	CPoint			m_oMouseHorizontal;
	CPoint			m_oMouseZoom;
	bool			m_bMouseVertical;
	bool			m_bMouseHorizontal;
	bool			m_bMouseOrigin;
	bool			m_bMouseScale;
	bool			m_bMouseCaptured;
friend class CViewControlDlg;
friend class CGeometryForFunDoc;
protected:
	void		m_SetHorizontal(double angle);
	void		m_SetVertical(double angle);
	void		m_SetZoom(double scale);
	void		m_SetCenter(CPoint center);
	void		m_SetExpress(CString szExpress);
	void		m_SetFormula(	CString szFormulaX,UINT bSymmetryX,
								CString szFormulaY,UINT bSymmetryY,
								CString szFormulaZ,UINT bSymmetryZ,bool b3D = true);
	void		m_SetRangeResolution(double fBegin,double fEnd,UINT nResolution);
	///////////////////////////////////////////////////////////////////////
	void		m_SetMouseHorizontal(bool b);
	void		m_SetMouseVertical(bool b);
	void		m_SetMouseOrigin(bool b);
	void		m_SetMouseScale(bool b);
public:
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // GeometryForFunView.cpp �ĵ��԰汾
inline CGeometryForFunDoc* CGeometryForFunView::GetDocument() const
   { return reinterpret_cast<CGeometryForFunDoc*>(m_pDocument); }
#endif

