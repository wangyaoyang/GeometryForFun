// GeometryForFunDoc.h :  CGeometryForFunDoc 类的接口
//


#pragma once

class CGeometryForFunDoc : public CDocument
{
protected: // 仅从序列化创建
	CGeometryForFunDoc();
	DECLARE_DYNCREATE(CGeometryForFunDoc)

// 属性
public:

// 操作
public:

// 重写
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CGeometryForFunDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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


