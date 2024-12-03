// Modal.cpp: 구현 파일
//

#include "pch.h"
#include "resource.h"
#include "FaceRecognitionFinalDlg.h"
#include "afxdialogex.h"
#include "Modal.h"
#include <fstream>
#include <string>
#include <ctime>


// Modal 대화 상자

IMPLEMENT_DYNAMIC(Modal, CDialogEx)

Modal::Modal(CWnd* pParent, CFaceRecognitionFinalDlg* p)
	: CDialogEx(IDD_DIALOG_MODAL, pParent), m_pFace(p)
{

}

Modal::~Modal()
{
}

void Modal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_prgs);
	DDX_Control(pDX, IDC_STATIC_CAM, m_ctrlCam);
}


BEGIN_MESSAGE_MAP(Modal, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Modal 메시지 처리기

BOOL Modal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 픽처 컨트롤 영역 설정
	m_ctrlCam.GetClientRect(&rect);

	// Mat과 CImage 생성
	m_matImg = m_pFace->GetFrame();
	cv::Size m_winSize(rect.right, rect.bottom);
	m_cImg.Create(m_winSize.width, m_winSize.height, 24);

	// 프로그래스 바 초기화
	m_nPrgsStatus = 0;
	m_prgs.SetRange(0, 100);
	m_prgs.SetPos(0);
	
	// 한 프레임씩 화면에 출력하게 타이머 설정
	SetTimer(0, 33, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void Modal::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_matImg = m_pFace->GetFrame();
	m_pFace->CropFace(TRUE);
	m_pFace->MatToCImage(m_matImg, m_cImg, rect);
	m_pFace->ShowImg(m_ctrlCam, m_cImg);

	if (m_nPrgsStatus++ == 100)
	{
		KillTimer(0);
		EndDialog(0);
	}
	else
	{
		m_prgs.SetPos(m_nPrgsStatus);
	}

	CDialogEx::OnTimer(nIDEvent);
}