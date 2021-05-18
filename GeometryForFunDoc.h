// GeometryForFunDoc.h :  CGeometryForFunDoc ��Ľӿ�
//


#pragma once

class CGeometryForFunDoc : public CDocument
{
protected: // �������л�����
	CGeometryForFunDoc();
	DECLARE_DYNCREATE(CGeometryForFunDoc)

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CGeometryForFunDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

public:
	CString		m_szExpress;
	CString		m_szFormulaX;
	CString		m_szFormulaY;
	CString		m_szFormulaZ;
	UINT		m_bSymmetryX;
	UINT		m_bSymmetryY;
	UINT		m_bSymmetryZ;
	double		m_fHorizontal;
	double		m_fVertical;
	double		m_fScale;
	CPoint		m_oCenter;
	double		m_fRangBegin;
	double		m_fRangEnd;
	UINT		m_nResolution;
};


