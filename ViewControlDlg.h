#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CViewControlDlg 对话框
class CCurve;
class CGeometryForFunView;
class CViewControlDlg : public CDialog
{
	DECLARE_DYNAMIC(CViewControlDlg)

public:
	CViewControlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewControlDlg();

// 对话框数据
	enum { IDD = IDD_VIEW_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CGeometryForFunView*	m_pView;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNMReleasedcaptureSliderHorizontal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderVertical(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckVertical();
	afx_msg void OnBnClickedCheckHorizontal();
	afx_msg void OnEnChangeEditVertical();
	afx_msg void OnEnChangeEditHorizontal();
	afx_msg void OnBnClickedCheckMoveCenter();
	afx_msg void OnBnClickedCheckScale();
	afx_msg void OnEnChangeEditScale();
	afx_msg void OnBnClickedButtonCalculate();
	afx_msg void OnBnClickedButtonCalculate2d();
	afx_msg void OnBnClickedCancel();
public:
	void	m_SetParent(CWnd* pParent);
	void	m_ShowAttrib(CCurve& curve);
//	CString szExpress,CString szFormulaY,double fHorizontal,double fVertical,
//							double fScale,double fRangBegin,double fRangEnd,UINT nResolution );
private:
	CSliderCtrl m_sliderHorizontal;
	CSliderCtrl m_sliderVertical;
	CButton m_checkVertical;
	CButton m_checkHorizontal;
	CButton m_checkMoveCenter;
	CButton m_checkScale;
	CButton m_checkSymmetryY;
	CButton m_checkSymmetryX;
	CButton m_checkSymmetryZ;
	CEdit m_editVertical;
	CEdit m_editHorizontal;
	CEdit m_editScale;
	CEdit m_editExpress;
	CEdit m_editFormulaX;
	CEdit m_editFormulaY;
	CEdit m_editFormulaZ;
	CEdit m_editRangeBegin;
	CEdit m_editRangeEnd;
	CComboBox m_comboResolution;
	double m_fVertical;
	double m_fHorizontal;
	double m_fScale;
public:
};
