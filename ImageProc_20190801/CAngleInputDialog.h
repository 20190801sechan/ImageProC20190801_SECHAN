#pragma once
#include "afxdialogex.h"


// CAngleInputDialog 대화 상자

class CAngleInputDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAngleInputDialog)

public:
	CAngleInputDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAngleInputDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_iAngle;
};
