#pragma once
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
#include <cstdlib>

class CFaceRecognitionFinalDlg;
// Modal 대화 상자

class Modal : public CDialogEx
{
	DECLARE_DYNAMIC(Modal)

public:
	Modal(CWnd* pParent = nullptr, CFaceRecognitionFinalDlg* p = nullptr);
	virtual ~Modal();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	CFaceRecognitionFinalDlg* m_pFace;			// Main Dialog Class의 멤버변수에 접근하기 위한 포인터
	CStatic m_ctrlCam;									// Modal Dialog에 있는 Picture Control의 컨트롤 변수
	CRect rect;											// Modal Dialog의 Picture Control의 영역을 저장하는 변수
	cv::Mat m_matImg;								// 카메라의 한 프레임을 저장하는 Mat 변수
	CImage m_cImg;									// 프로그램 상에서 보이는 Cimage 변수
	CProgressCtrl m_prgs;							// 프로그래스바를 컨트롤 하기 위한 컨트롤 변수
	int m_nPrgsStatus;									// 프로그래스바의 진행도를 저장하는 변수
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
