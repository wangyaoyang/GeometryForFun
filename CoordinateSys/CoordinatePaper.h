#pragma once

#include "Calculat.h"

#define	SYMMETRY_ONE	0
#define	SYMMETRY_X_2	1
#define	SYMMETRY_Y_2	2
#define	SYMMETRY_Z_2	3

#define	VALIDATE_NON	0	//check box = empty
#define	VALIDATE_ONE	2	//check box = gray
#define	VALIDATE_TWO	1	//check box = selected

class CCoordinateMapping
{
private:
	double		m_PI;
	double		m_PIE;
	double		m_angleH;
	double		m_angleV;
public:
	void		m_SetAngle(double H,double V);
	CPoint		m_GetWindowPoint(double scale,double X,double Y,double Z);
public:
	CCoordinateMapping(void);
	~CCoordinateMapping(void);
};

class CCoordinateAxis
{
private:
	CCoordinateMapping	m_mappingTool;
	double		m_fAngleH;	//X轴沿Z轴旋转的角度
	double		m_fAngleV;	//X轴沿Y轴旋转的角度
	double		m_fScale;	//缩放比例，数学单位1对应的屏幕像素个数
	CPoint		m_oWindowOrigin;
	CRect		m_oWindowBound;
	CDC*		m_pCDC;
public:
	void		m_SetScale(double scale) { m_fScale = scale; }
	void		m_SetAngleV(double angle);
	void		m_SetAngleH(double angle);
	double		m_GetScale() { return m_fScale; }
	double		m_GetAngleV();
	double		m_GetAngleH();
	void		m_SetWindowOrigin(CPoint point) { m_oWindowOrigin = point; }
	CPoint		m_GetWindowOrigin() { return m_oWindowOrigin; }
	void		m_SetWindowBound(CRect rect) { m_oWindowBound = rect; }
	CRect		m_GetWindowBound() { return m_oWindowBound; }
public:
	CCoordinateAxis(void);
	~CCoordinateAxis(void);
	void	m_DrawCoordinate(CDC* pCDC);
	CPoint	m_GetWindowPoint(double X,double Y,double Z);
};

class CCurvePoints : public CObject
{
private:
	UINT		m_nSymmetry;
	UINT		m_nSpace;
	UINT		m_nValid;
	double*		m_fX;
	double*		m_fY;
	double*		m_fZ;
public:
	CCurvePoints(UINT nSpace,UINT symmetry);
	~CCurvePoints(void);
	bool		m_Append(double x,double y,double z);
	bool		m_GetAt(UINT nPoint,double& x,double& y,double& z);
	UINT		m_GetCount() { return m_nValid; }
	UINT		m_GetSymmetry() { return m_nSymmetry; }
};

class CTracePoints : public CObject
{
private:
	UINT		m_nSpace;
	UINT		m_nValid;
	CPoint*		m_oPoint;
private:
	bool			m_Append(CPoint point);
public:
	CTracePoints(CCurvePoints& curve,CCoordinateAxis&axis);
	~CTracePoints(void);
	bool			m_GetAt(UINT nPoint,CPoint& point);
	UINT			m_GetCount() { return m_nValid; }
	POINT*			m_GetPoints() { return m_oPoint; }
};

class CCurve
{
private:
	CCoordinateAxis	m_axis;
	CString		m_szExpress;
	CString		m_szFormulaX;
	CString		m_szFormulaY;
	CString		m_szFormulaZ;
	CObList		m_oCurvesYx;	//Y0X平面上的所有区线
	CObList		m_oCurvesXz;	//X0Z平面上的所有区线
	CObList		m_oCurvesZy;	//ZOY平面上的所有区线
	CObList		m_oTraceYx;		//窗口Y0X平面上的所有区线
	CObList		m_oTraceXz;		//窗口X0Z平面上的所有区线
	CObList		m_oTraceZy;		//窗口ZOY平面上的所有区线
	UINT		m_bSymmetryX;
	UINT		m_bSymmetryY;
	UINT		m_bSymmetryZ;
private:
	double		m_fBegin;
	double		m_fEnd;
	double		m_fStep;
private:
	void		m_ClearCurve();
	void		m_ClearTrace();
	void		m_CalualateX(double y);
	void		m_CalualateY(double z);
	void		m_CalualateZ(double x);
public:
	CCurve(void);
	~CCurve(void);
	CCoordinateAxis&	m_GetCoordinate() { return m_axis; }
	void		m_SetExpress(CString szExpress);
	void		m_GetExpress(CString& szExpress);
	void		m_SetFormula(CString szFormulaX,CString szFormulaY,CString szFormulaZ);
	void		m_GetFormula(CString& szFormulaX,CString& szFormulaY,CString& szFormulaZ);
	void		m_SetSymmetry(UINT bSymmetryX,UINT bSymmetryY,UINT bSymmetryZ);
	void		m_GetSymmetry(UINT& bSymmetryX,UINT& bSymmetryY,UINT& bSymmetryZ);
	void		m_SetResolution(UINT nResolution);
	void		m_SetViewRange(double fBegin,double fEnd,UINT nResolution=0);
	void		m_GetViewRange(double& fBegin,double& fEnd,UINT& nResolution);
	void		m_Calualate(CString szFormulaX,UINT bSymmetryX,
							CString szFormulaY,UINT bSymmetryY,
							CString szFormulaZ,UINT bSymmetryZ,
							bool b3D);
	void		m_PreDraw();
	void		m_Draw(CDC* pCDC);
};

//#define		PI		3.14159265358979323846f
//#define		PI		acos(0.0f)