// GeometryForFun.h : GeometryForFun Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CGeometryForFunApp:
// �йش����ʵ�֣������ GeometryForFun.cpp
//

class CGeometryForFunApp : public CWinApp
{
public:
	CGeometryForFunApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGeometryForFunApp theApp;
