// GeometryForFunDoc.cpp :  CGeometryForFunDoc ���ʵ��
//

#include "stdafx.h"
#include "GeometryForFun.h"

#include "GeometryForFunDoc.h"
#include "GeometryForFunView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGeometryForFunDoc

IMPLEMENT_DYNCREATE(CGeometryForFunDoc, CDocument)

BEGIN_MESSAGE_MAP(CGeometryForFunDoc, CDocument)
END_MESSAGE_MAP()


// CGeometryForFunDoc ����/����

CGeometryForFunDoc::CGeometryForFunDoc()
{
	m_fHorizontal	= 0.0f;
	m_fVertical		= 0.0f;
	m_fScale		= 1.0f;
	m_oCenter		= CPoint(0,0);
	m_fRangBegin	= -1.0f;
	m_fRangEnd		= 1.0f;
	m_nResolution	= 10;
	m_bSymmetryX	= 0;
	m_bSymmetryY	= 0;
	m_bSymmetryZ	= 0;
}

CGeometryForFunDoc::~CGeometryForFunDoc()
{
}

BOOL CGeometryForFunDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CGeometryForFunDoc ���л�

void CGeometryForFunDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_szExpress;
		ar << m_bSymmetryX;
		ar << m_szFormulaX;
		ar << m_bSymmetryY;
		ar << m_szFormulaY;
		ar << m_bSymmetryZ;
		ar << m_szFormulaZ;
		ar << m_fHorizontal;
		ar << m_fVertical;
		ar << m_fScale;
		ar << m_oCenter;
		ar << m_fRangBegin;
		ar << m_fRangEnd;
		ar << m_nResolution;
	}
	else
	{
		ar >> m_szExpress;
		ar >> m_bSymmetryX;
		ar >> m_szFormulaX;
		ar >> m_bSymmetryY;
		ar >> m_szFormulaY;
		ar >> m_bSymmetryZ;
		ar >> m_szFormulaZ;
		ar >> m_fHorizontal;
		ar >> m_fVertical;
		ar >> m_fScale;
		ar >> m_oCenter;
		ar >> m_fRangBegin;
		ar >> m_fRangEnd;
		ar >> m_nResolution;
	}
}


// CGeometryForFunDoc ���

#ifdef _DEBUG
void CGeometryForFunDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGeometryForFunDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGeometryForFunDoc ����