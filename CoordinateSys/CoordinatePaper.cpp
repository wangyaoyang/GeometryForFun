#include "StdAfx.h"
#include <math.h>
#include <float.h>
#include ".\coordinatepaper.h"

///////////////////////////////////////////////////////////////////////////
CCurvePoints::CCurvePoints(UINT nSpace,UINT symmetry)
{
	m_nSymmetry	= symmetry;
	m_nSpace	= nSpace;
	m_nValid	= 0;
	m_fX		= new double[nSpace];
	m_fY		= new double[nSpace];
	m_fZ		= new double[nSpace];
}

CCurvePoints::~CCurvePoints(void)
{
	if( m_fX ) { delete []m_fX;	m_fX = NULL; }
	if( m_fY ) { delete []m_fY;	m_fY = NULL; }
	if( m_fZ ) { delete []m_fZ;	m_fZ = NULL; }
}

bool CCurvePoints::m_Append(double x,double y,double z)
{
	if( m_nValid >= m_nSpace ) return false;	//空间不足
	m_fX[m_nValid] = x;
	m_fY[m_nValid] = y;
	m_fZ[m_nValid] = z;
	m_nValid ++;
	return true;
}

bool CCurvePoints::m_GetAt(UINT nPoint,double& x,double& y,double& z)
{
	if( nPoint > m_nValid ) return false;		//nPoint超出有效范围
	if(_finite(m_fX[nPoint])==0) return false;
	if(_finite(m_fY[nPoint])==0) return false;
	if(_finite(m_fZ[nPoint])==0) return false;
	x = m_fX[nPoint];
	y = m_fY[nPoint];
	z = m_fZ[nPoint];
	return true;
}
///////////////////////////////////////////////////////////////////////////
CTracePoints::CTracePoints(CCurvePoints& curve,CCoordinateAxis& axis)
{
	UINT		nPoint = curve.m_GetCount();
	UINT		nSymmetry = curve.m_GetSymmetry();
	m_nValid	= 0;
	m_nSpace	= 0;
	m_oPoint	= NULL;

	switch(nSymmetry)
	{
	case SYMMETRY_ONE:	m_nSpace = nPoint;		break;
	case SYMMETRY_X_2:
	case SYMMETRY_Y_2:
	case SYMMETRY_Z_2:	m_nSpace = 2*nPoint+1;	break;
	default:			return;
	}
	m_oPoint	= new CPoint[m_nSpace];
	for( UINT n = 0; n < nPoint; n ++ )
	{
		double	x=0.0f,y=0.0f,z=0.0f;
		if( !curve.m_GetAt(n,x,y,z) ) m_Append(CPoint(-1,-1));
			else m_Append(axis.m_GetWindowPoint(x,y,z));
	}
	if(nSymmetry==SYMMETRY_ONE) return;
	m_Append(CPoint(-1,-1));	//在Y轴正负之间插入一个无效点,表示不连续
	for( UINT n = 0; n < nPoint; n ++ )
	{
		double	x=0.0f,y=0.0f,z=0.0f;
		if( curve.m_GetAt(n,x,y,z) )
		{
			switch(nSymmetry)
			{
			case SYMMETRY_X_2:	m_Append(axis.m_GetWindowPoint(-x,y,z));	break;
			case SYMMETRY_Y_2:	m_Append(axis.m_GetWindowPoint(x,-y,z));	break;
			case SYMMETRY_Z_2:	m_Append(axis.m_GetWindowPoint(x,y,-z));	break;
			default:;
			}			
		}
		else m_Append(CPoint(-1,-1));
	}
}

CTracePoints::~CTracePoints(void)
{
	if( m_oPoint ) { delete []m_oPoint;	m_oPoint = NULL; }
}


bool CTracePoints::m_Append(CPoint point)
{
	if( m_nValid >= m_nSpace ) return false;	//空间不足
	m_oPoint[m_nValid] = point;
	m_nValid ++;
	return true;
}

bool CTracePoints::m_GetAt(UINT nPoint,CPoint& point)
{
	if( nPoint > m_nValid ) return false;		//nPoint超出有效范围
	if( m_oPoint[nPoint] == CPoint(-1,-1) ) return false;
	point = m_oPoint[nPoint];
	return true;
}
///////////////////////////////////////////////////////////////////////////
CCurve::CCurve(void)
{
	m_bSymmetryX = 0;
	m_bSymmetryY = 0;
	m_fBegin = -10;
	m_fEnd = 10;
	m_fStep = 1;
}

CCurve::~CCurve(void)
{
	m_ClearTrace();
	m_ClearCurve();
}

void CCurve::m_ClearCurve()
{
	while( !m_oCurvesYx.IsEmpty() )
	{
		CCurvePoints*	pCurve = (CCurvePoints*)m_oCurvesYx.RemoveHead();
		if( pCurve ) { delete pCurve;	pCurve = NULL; }
	}
	while( !m_oCurvesXz.IsEmpty() )
	{
		CCurvePoints*	pCurve = (CCurvePoints*)m_oCurvesXz.RemoveHead();
		if( pCurve ) { delete pCurve;	pCurve = NULL; }
	}
	while( !m_oCurvesZy.IsEmpty() )
	{
		CCurvePoints*	pCurve = (CCurvePoints*)m_oCurvesZy.RemoveHead();
		if( pCurve ) { delete pCurve;	pCurve = NULL; }
	}
}

void CCurve::m_ClearTrace()
{
	while( !m_oTraceYx.IsEmpty() )
	{
		CTracePoints*	pTrace = (CTracePoints*)m_oTraceYx.RemoveHead();
		if( pTrace ) { delete pTrace;	pTrace = NULL; }
	}
	while( !m_oTraceXz.IsEmpty() )
	{
		CTracePoints*	pTrace = (CTracePoints*)m_oTraceXz.RemoveHead();
		if( pTrace ) { delete pTrace;	pTrace = NULL; }
	}
	while( !m_oTraceZy.IsEmpty() )
	{
		CTracePoints*	pTrace = (CTracePoints*)m_oTraceZy.RemoveHead();
		if( pTrace ) { delete pTrace;	pTrace = NULL; }
	}
}

void CCurve::m_SetExpress(CString szExpress)
{
	m_szExpress = szExpress;
}

void CCurve::m_GetExpress(CString& szExpress)
{
	szExpress = m_szExpress;
}

void CCurve::m_SetFormula(CString szFormulaX,CString szFormulaY,CString szFormulaZ)
{
	m_szFormulaX = szFormulaX;
	m_szFormulaY = szFormulaY;
	m_szFormulaZ = szFormulaZ;
}

void CCurve::m_GetFormula(CString& szFormulaX,CString& szFormulaY,CString& szFormulaZ)
{
	szFormulaX = m_szFormulaX;
	szFormulaY = m_szFormulaY;
	szFormulaZ = m_szFormulaZ;
}

void CCurve::m_SetSymmetry(UINT bSymmetryX,UINT bSymmetryY,UINT bSymmetryZ)
{
	m_bSymmetryX = bSymmetryX;
	m_bSymmetryY = bSymmetryY;
	m_bSymmetryZ = bSymmetryZ;
}
void CCurve::m_GetSymmetry(UINT& bSymmetryX,UINT& bSymmetryY,UINT& bSymmetryZ)
{
	bSymmetryX = m_bSymmetryX;
	bSymmetryY = m_bSymmetryY;
	bSymmetryZ = m_bSymmetryZ;
}

void CCurve::m_SetResolution(UINT nResolution)
{
	if( nResolution > 0 )
	{
		m_fStep = abs(m_fEnd-m_fBegin)/nResolution;
		m_fStep = abs(m_fEnd-m_fBegin)/nResolution;
	}
}

void CCurve::m_SetViewRange(double fBegin,double fEnd,UINT nResolution)
{
	m_fBegin = fBegin;
	m_fEnd = fEnd;
	if( nResolution > 0 )
	{
		m_fStep = abs(m_fEnd-m_fBegin)/nResolution;
	}
}

void CCurve::m_GetViewRange(double& fBegin,double& fEnd,UINT& nResolution)
{
	fBegin = m_fBegin;
	fEnd = m_fEnd;
	nResolution = (UINT)(abs(m_fEnd-m_fBegin)/m_fStep);
}

void CCurve::m_CalualateY(double z)
{
	double			yy			= sqrt(-1.0);	// INF value
	double			y			= 0.0f;
	UINT			nSpace		= (UINT)(abs(m_fEnd - m_fBegin)/m_fStep + 2);
	UINT			nSymmetry	= 0;

	switch( m_bSymmetryY )
	{
	case VALIDATE_ONE:	nSymmetry = SYMMETRY_ONE;	break;
	case VALIDATE_TWO:	nSymmetry = SYMMETRY_Y_2;	break;
	case VALIDATE_NON:
	default:			return;
	}
	CCurvePoints*	pCurve	= new CCurvePoints(nSpace,nSymmetry);
	for( double x = m_fBegin; x <= m_fEnd; x += m_fStep )
	{	//如果所有值都无效，则放弃该曲线，释放刚申请的内存
		CCalculater			cal(TYPE_FLOAT);
		cal.m_SourceString	= m_szFormulaY;
		BOOL bInvalidate	= cal.Calculate( &y,x,0,z );
		if( bInvalidate == 0 || _finite(yy) )
		{	//当前一个值有效时，记录当前无效值是为了绘图时知道曲线在那里断开
			pCurve->m_Append( x,y,z );	//当前值有效或前一个值有效
			yy = y;						//保存当前值，无论其是否有效
		}	//当前值和前一个值都无效，忽略
	}
	if( pCurve->m_GetCount() > 0 ) m_oCurvesYx.AddTail(pCurve);
	else { delete pCurve; pCurve = NULL; }
}

void CCurve::m_CalualateX(double y)
{
	double			xx		= sqrt(-1.0);	// INF value
	double			x		= 0.0f;
	UINT			nSpace	= (UINT)(abs(m_fEnd - m_fBegin)/m_fStep + 2);
	UINT			nSymmetry	= 0;

	switch( m_bSymmetryX )
	{
	case VALIDATE_ONE:	nSymmetry = SYMMETRY_ONE;	break;
	case VALIDATE_TWO:	nSymmetry = SYMMETRY_X_2;	break;
	case VALIDATE_NON:
	default:			return;
	}
	CCurvePoints*	pCurve	= new CCurvePoints(nSpace,nSymmetry);
	for( double z = m_fBegin; z <= m_fEnd; z += m_fStep )
	{
		CCalculater			cal(TYPE_FLOAT);
		cal.m_SourceString	= m_szFormulaX;
		BOOL bInvalidate	= cal.Calculate( &x,0,y,z );
		if( bInvalidate == 0 || _finite(xx) )
		{	//当前一个值有效时，记录当前无效值是为了绘图时知道曲线在那里断开
			pCurve->m_Append( x,y,z );	//当前值有效或前一个值有效
			xx = x;						//保存当前值，无论其是否有效
		}	//当前值和前一个值都无效，忽略
	}
	if( pCurve->m_GetCount() > 0 ) m_oCurvesXz.AddTail(pCurve);
	else { delete pCurve; pCurve = NULL; }
}

void CCurve::m_CalualateZ(double x)
{
	double			zz		= sqrt(-1.0);	// INF value
	double			z		= 0.0f;
	UINT			nSpace	= (UINT)(abs(m_fEnd - m_fBegin)/m_fStep + 2);
	UINT			nSymmetry	= 0;

	switch( m_bSymmetryZ )
	{
	case VALIDATE_ONE:	nSymmetry = SYMMETRY_ONE;	break;
	case VALIDATE_TWO:	nSymmetry = SYMMETRY_Z_2;	break;
	case VALIDATE_NON:
	default:			return;
	}
	CCurvePoints*	pCurve	= new CCurvePoints(nSpace,nSymmetry);
	for( double y = m_fBegin; y <= m_fEnd; y += m_fStep )
	{
		CCalculater			cal(TYPE_FLOAT);
		cal.m_SourceString	= m_szFormulaZ;
		BOOL bInvalidate	= cal.Calculate( &z,x,y,0 );
		if( bInvalidate == 0 || _finite(zz) )
		{	//当前一个值有效时，记录当前无效值是为了绘图时知道曲线在那里断开
			pCurve->m_Append( x,y,z );	//当前值有效或前一个值有效
			zz = z;						//保存当前值，无论其是否有效
		}	//当前值和前一个值都无效，忽略
	}
	if( pCurve->m_GetCount() > 0 ) m_oCurvesZy.AddTail(pCurve);
	else { delete pCurve; pCurve = NULL; }
}

void CCurve::m_Calualate(	CString szFormulaX,UINT bSymmetryX,
							CString szFormulaY,UINT bSymmetryY,
							CString szFormulaZ,UINT bSymmetryZ,bool b3D)
{
	m_SetFormula(szFormulaX,szFormulaY,szFormulaZ);
	m_SetSymmetry(bSymmetryX,bSymmetryY,bSymmetryZ);
	m_ClearCurve();
	m_CalualateX(0);
	m_CalualateY(0);
	m_CalualateZ(0);
	if( b3D )
	{
		for( double zx = m_fBegin; zx <= m_fEnd; zx += m_fStep ) m_CalualateX(zx);
		for( double zy = m_fBegin; zy <= m_fEnd; zy += m_fStep ) m_CalualateY(zy);
		for( double xy = m_fBegin; xy <= m_fEnd; xy += m_fStep ) m_CalualateZ(xy);
	}
}

void CCurve::m_PreDraw()
{
	m_ClearTrace();
	for( POSITION posZ = m_oCurvesYx.GetHeadPosition(); posZ; m_oCurvesXz.GetNext(posZ) )
	{
		CCurvePoints*	pCurve = (CCurvePoints*)m_oCurvesYx.GetAt(posZ);
		CTracePoints*	pTrace = new CTracePoints(*pCurve,m_axis);
		if( pTrace->m_GetCount()>0 ) m_oTraceYx.AddTail(pTrace); else delete pTrace;
	}
	for( POSITION posX = m_oCurvesXz.GetHeadPosition(); posX; m_oCurvesXz.GetNext(posX) )
	{
		CCurvePoints*	pCurve = (CCurvePoints*)m_oCurvesXz.GetAt(posX);
		CTracePoints*	pTrace = new CTracePoints(*pCurve,m_axis);
		if( pTrace->m_GetCount()>0 ) m_oTraceXz.AddTail(pTrace); else delete pTrace;
	}
	for( POSITION posY = m_oCurvesZy.GetHeadPosition(); posY; m_oCurvesZy.GetNext(posY) )
	{
		CCurvePoints*	pCurve = (CCurvePoints*)m_oCurvesZy.GetAt(posY);
		CTracePoints*	pTrace = new CTracePoints(*pCurve,m_axis);
		if( pTrace->m_GetCount()>0 ) m_oTraceZy.AddTail(pTrace); else delete pTrace;
	}
}

void CCurve::m_Draw(CDC* pCDC)
{
	m_axis.m_DrawCoordinate(pCDC);
	CPen	hPenX( PS_SOLID,1,RGB(127,127,127) );
	CPen	hPenY( PS_SOLID,1,RGB(192,192,192) );
	CPen	hPenZ( PS_SOLID,1,RGB(255,255,255) );

	pCDC->SelectObject(hPenX);
	for( POSITION posZ = m_oTraceYx.GetHeadPosition(); posZ; m_oTraceYx.GetNext(posZ) )
	{
		CPoint			pointCurr = CPoint(0,0);
		CTracePoints*	pTrace = (CTracePoints*)m_oTraceYx.GetAt(posZ);
		UINT			nCount = pTrace->m_GetCount();
		bool			bInContinuous = true;		//当前点与前一点不连续
		for( UINT n = 0; n < nCount; n ++ )
		{
			if( pTrace->m_GetAt(n,pointCurr) )
			{
				if( bInContinuous )
				{
					pCDC->MoveTo(pointCurr);
					bInContinuous = false;
				}
				else pCDC->LineTo(pointCurr);
			}
			else bInContinuous = true;
		}
		/*
		pCDC->PolylineTo(pTrace->m_GetPoints(),nCount);
		*/
	}
	pCDC->SelectObject(hPenY);
	for( POSITION posY = m_oTraceXz.GetHeadPosition(); posY; m_oTraceXz.GetNext(posY) )
	{
		CPoint			pointCurr = CPoint(0,0);
		CTracePoints*	pTrace = (CTracePoints*)m_oTraceXz.GetAt(posY);
		UINT			nCount = pTrace->m_GetCount();
		bool			bInContinuous = true;		//当前点与前一点不连续
		for( UINT n = 0; n < nCount; n ++ )
		{
			if( pTrace->m_GetAt(n,pointCurr) )
			{
				if( bInContinuous )
				{
					pCDC->MoveTo(pointCurr);
					bInContinuous = false;
				}
				else pCDC->LineTo(pointCurr);
			}
			else bInContinuous = true;
		}
		/*
		pCDC->PolylineTo(pTrace->m_GetPoints(),nCount);
		*/
	}
	pCDC->SelectObject(hPenZ);
	for( POSITION posX = m_oTraceZy.GetHeadPosition(); posX; m_oTraceZy.GetNext(posX) )
	{
		CPoint			pointCurr = CPoint(0,0);
		CTracePoints*	pTrace = (CTracePoints*)m_oTraceZy.GetAt(posX);
		UINT			nCount = pTrace->m_GetCount();
		bool			bInContinuous = true;		//当前点与前一点不连续
		for( UINT n = 0; n < nCount; n ++ )
		{
			if( pTrace->m_GetAt(n,pointCurr) )
			{
				if( bInContinuous )
				{
					pCDC->MoveTo(pointCurr);
					bInContinuous = false;
				}
				else pCDC->LineTo(pointCurr);
			}
			else bInContinuous = true;
		}
		/*
		pCDC->PolylineTo(pTrace->m_GetPoints(),nCount);
		*/
	}
}

///////////////////////////////////////////////////////////////////////////
CCoordinateAxis::CCoordinateAxis(void)
{
	m_fAngleH	= 0.0f;		//Z轴沿X轴旋转的角度
	m_fAngleV	= 0.0f;		//Z轴沿Y轴旋转的角度
	m_fScale	= 100.0f;	//缩放比例，数学单位1对应的屏幕像素个数
	m_oWindowOrigin	= CPoint(0,0);
	m_oWindowBound	= CRect(0,0,0,0);
	m_pCDC		= NULL;
}

CCoordinateAxis::~CCoordinateAxis(void)
{
}

void CCoordinateAxis::m_SetAngleV(double angle)
{
	m_fAngleV = angle;
	m_mappingTool.m_SetAngle(m_fAngleH,m_fAngleV);
}

void CCoordinateAxis::m_SetAngleH(double angle)
{
	m_fAngleH = angle;
	m_mappingTool.m_SetAngle(m_fAngleH,m_fAngleV);
}

double CCoordinateAxis::m_GetAngleV()	{ return m_fAngleV; }
double CCoordinateAxis::m_GetAngleH()	{ return m_fAngleH; }

CPoint CCoordinateAxis::m_GetWindowPoint(double X,double Y,double Z)
{
	CPoint point = m_mappingTool.m_GetWindowPoint(m_fScale,X,Y,Z);
	point += m_oWindowOrigin;
	return point;
}

void CCoordinateAxis::m_DrawCoordinate(CDC* pCDC)
{
	if( pCDC != NULL ) m_pCDC = pCDC; else return;
	double	LO	= -200/m_fScale;
	double	OL	= 200/m_fScale;
	CPoint	AxisXO = m_mappingTool.m_GetWindowPoint(m_fScale,LO,0,0);
	CPoint	AxisOX = m_mappingTool.m_GetWindowPoint(m_fScale,OL,0,0);
	CPoint	AxisYO = m_mappingTool.m_GetWindowPoint(m_fScale,0,LO,0);
	CPoint	AxisOY = m_mappingTool.m_GetWindowPoint(m_fScale,0,OL,0);
	CPoint	AxisZO = m_mappingTool.m_GetWindowPoint(m_fScale,0,0,LO);
	CPoint	AxisOZ = m_mappingTool.m_GetWindowPoint(m_fScale,0,0,OL);
	AxisXO += m_oWindowOrigin;	AxisOX += m_oWindowOrigin;
	AxisYO += m_oWindowOrigin;	AxisOY += m_oWindowOrigin;
	AxisZO += m_oWindowOrigin;	AxisOZ += m_oWindowOrigin;

	CPoint	u1 = m_mappingTool.m_GetWindowPoint(m_fScale,LO,LO,OL);
	CPoint	u2 = m_mappingTool.m_GetWindowPoint(m_fScale,LO,OL,OL);
	CPoint	u3 = m_mappingTool.m_GetWindowPoint(m_fScale,OL,OL,OL);
	CPoint	u4 = m_mappingTool.m_GetWindowPoint(m_fScale,OL,LO,OL);
	CPoint	d1 = m_mappingTool.m_GetWindowPoint(m_fScale,LO,LO,LO);
	CPoint	d2 = m_mappingTool.m_GetWindowPoint(m_fScale,LO,OL,LO);
	CPoint	d3 = m_mappingTool.m_GetWindowPoint(m_fScale,OL,OL,LO);
	CPoint	d4 = m_mappingTool.m_GetWindowPoint(m_fScale,OL,LO,LO);
	u1 += m_oWindowOrigin;	d1 += m_oWindowOrigin;
	u2 += m_oWindowOrigin;	d2 += m_oWindowOrigin;
	u3 += m_oWindowOrigin;	d3 += m_oWindowOrigin;
	u4 += m_oWindowOrigin;	d4 += m_oWindowOrigin;

	pCDC->SetBkColor(RGB(0,0,0));
	pCDC->SetTextColor(RGB(255,255,255));
	pCDC->TextOut(m_oWindowOrigin.x,m_oWindowOrigin.y,_T("O"));

	COLORREF	colorX = RGB(255,0,0);
	COLORREF	colorY = RGB(0,0,255);
	COLORREF	colorZ = RGB(0,255,0);
	COLORREF	colorW = RGB(255,255,255);
	CPen*	hOld = NULL;
	CPen	hPenX( PS_SOLID,1,colorX );
	CPen	hPenY( PS_SOLID,1,colorY );
	CPen	hPenZ( PS_SOLID,1,colorZ );
	CPen	hPenW( PS_SOLID,1,colorW );
	CString	szUnit;

	pCDC->SetTextColor( colorX);			szUnit.Format(_T("X=%4.3f"),OL);	pCDC->TextOut(AxisOX.x,AxisOX.y,szUnit);
	pCDC->SetTextColor( colorY);			szUnit.Format(_T("Y=%4.3f"),OL);	pCDC->TextOut(AxisOY.x,AxisOY.y,szUnit);
	pCDC->SetTextColor( colorZ);			szUnit.Format(_T("Z=%4.3f"),OL);	pCDC->TextOut(AxisOZ.x,AxisOZ.y,szUnit);
	hOld = pCDC->SelectObject( &hPenX );	pCDC->MoveTo(AxisXO);				pCDC->LineTo(AxisOX);
	pCDC->SelectObject( &hPenY );			pCDC->MoveTo(AxisYO);				pCDC->LineTo(AxisOY);
	pCDC->SelectObject( &hPenZ );			pCDC->MoveTo(AxisZO);				pCDC->LineTo(AxisOZ);
	pCDC->SelectObject( &hPenW );

	pCDC->TextOut(u1.x,u1.y,_T("u1"));
	pCDC->TextOut(u2.x,u2.y,_T("u2"));
	pCDC->TextOut(u3.x,u3.y,_T("u3"));
	pCDC->TextOut(u4.x,u4.y,_T("u4"));
	pCDC->TextOut(d1.x,d1.y,_T("d1"));
	pCDC->TextOut(d2.x,d2.y,_T("d2"));
	pCDC->TextOut(d3.x,d3.y,_T("d3"));
	pCDC->TextOut(d4.x,d4.y,_T("d4"));
	pCDC->MoveTo(u1);		pCDC->LineTo(u2);		pCDC->LineTo(u3);		pCDC->LineTo(u4);		pCDC->LineTo(u1);
	pCDC->LineTo(d1);		pCDC->LineTo(d2);		pCDC->LineTo(d3);		pCDC->LineTo(d4);		pCDC->LineTo(d1);
	pCDC->MoveTo(u2);		pCDC->LineTo(d2);
	pCDC->MoveTo(u3);		pCDC->LineTo(d3);
	pCDC->MoveTo(u4);		pCDC->LineTo(d4);
	pCDC->SelectObject( hOld );
}

///////////////////////////////////////////////////////////////////////////
CCoordinateMapping::CCoordinateMapping(void)
{
	m_PI = 2*acos(0.0f);
	m_PIE = m_PI/180;
	m_angleH = 0.0f;
	m_angleV = 0.0f;
}

CCoordinateMapping::~CCoordinateMapping(void)
{
}

void CCoordinateMapping::m_SetAngle(double H,double V)
{
	m_angleH = H*m_PIE;
	m_angleV = V*m_PIE;
}

CPoint CCoordinateMapping::m_GetWindowPoint(double scale,double X,double Y,double Z)
{	//根据缩放比例、坐标系旋转角度，将指定的空间坐标映射为屏幕窗口内的二维坐标（单位为像素）
	double	yz = sqrt(Y*Y+Z*Z);
	double	XYZ = X*X+Y*Y+Z*Z;
	double	vPinkZy = atan2(Y,Z);
	double	vBlueZy = vPinkZy + m_angleV;
	double	wGreen = cos(vBlueZy)*yz;
	double	v = sin(vBlueZy)*yz;
	double	h = sin(atan2(X,wGreen)+m_angleH) * sqrt(abs(XYZ-v*v));

	int		nH = (int)(h*scale);
	int		nV = (int)(v*scale)*(-1);
	return CPoint(nH,nV);
}

///////////////////////////////////////////////////////////////////////////
/*
CPoint CCoordinateMapping::m_GetWindowPoint(double angleH,double angleV,double scale,double X,double Y,double Z)
{	//根据缩放比例、坐标系旋转角度，将指定的空间坐标映射为屏幕窗口内的二维坐标（单位为像素）
	double	m_PIE = 2*acos(0.0f);
	double	xy = sqrt(X*X+Y*Y);
	double	yz = sqrt(Y*Y+Z*Z);
	double	xz = sqrt(X*X+Z*Z);
	double	xyz = sqrt(X*X+Y*Y+Z*Z);
	double	vPinkZy = atan2(Y,Z)*180/m_PIE;
	double	vBlueZy = vPinkZy + angleV;
	double	wGreen = cos(vBlueZy*m_PIE/180)*yz;
	double	hBlue = sin(vBlueZy*m_PIE/180)*yz;
	double	wBlue = sqrt(abs(xyz*xyz-hBlue*hBlue));
	double	vBlueZx0 = atan2(X,wGreen)*180/m_PIE;
	double	vBlueZx1 = vBlueZx0 + angleH;
	double	h = sin(vBlueZx1*m_PIE/180)*wBlue;
	double	v = hBlue;

	int		nH = (int)(h*scale);
	int		nV = (int)(v*scale)*(-1);
	return CPoint(nH,nV);
}
*/
///////////////////////////////////////////////////////////////////////////
