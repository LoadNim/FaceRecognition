
// FaceRecognitionFinalDlg.h: 헤더 파일
//

#pragma once
#include "opencv2/opencv.hpp"
#include <fstream>
#include "Modal.h"

// CFaceRecognitionFinalDlg 대화 상자
class CFaceRecognitionFinalDlg : public CDialogEx
{
// 생성입니다.
public:
	CFaceRecognitionFinalDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACERECOGNITIONFINAL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ctrlCrop;							// MainDialog에 있는 Picture Control의 컨트롤 변수
	CRect rect;										// MainDialog에 있는 Picture Control의 영역을 저장하는 변수
	cv::VideoCapture* m_vCam;					// 카메라와 연동되어있는 객체
	cv::Mat m_matImg;							// 카메라의 한 프레임을 저장하는 Mat 변수
	CImage m_cImg;								// 프로그램 상에서 보이는 Cimage 변수
	cv::CascadeClassifier m_model;				// 얼굴 감지하는 모델
	cv::Size m_winSize;								// CImage 레이아웃 정의용 변수
	std::ifstream file;								// 인공지능에서 결과 값을 가져오기 위한 파일 객체
	float m_fConfidence;							// 결과 값
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	cv::Mat GetFrame();
	void CropFace(bool is_Modal);
	void MatToCImage(cv::Mat& mat, CImage& cImg, CRect& rct);
	void ShowImg(CStatic& ctrl, CImage& cImg);
	void CreateImg();
	void ViewResult();
};
