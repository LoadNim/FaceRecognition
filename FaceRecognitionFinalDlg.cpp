
// FaceRecognitionFinalDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "FaceRecognitionFinal.h"
#include "FaceRecognitionFinalDlg.h"
#include "Modal.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFaceRecognitionFinalDlg 대화 상자



CFaceRecognitionFinalDlg::CFaceRecognitionFinalDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FACERECOGNITIONFINAL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFaceRecognitionFinalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CROP, m_ctrlCrop);
}

BEGIN_MESSAGE_MAP(CFaceRecognitionFinalDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFaceRecognitionFinalDlg 메시지 처리기

BOOL CFaceRecognitionFinalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	// 카메라 및 픽처 컨트롤 영역 설정
	m_vCam = new cv::VideoCapture(0);
	if (!m_vCam->isOpened())
	{
		AfxMessageBox(_T("카메라가 연결되지 않았습니다."), MB_OK | MB_ICONWARNING);
		return -1;
	}
	m_ctrlCrop.GetClientRect(&rect);
	m_winSize = cv::Size(rect.right, rect.bottom);
	m_vCam->set(cv::CAP_PROP_FRAME_WIDTH, 1920);
	m_vCam->set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

	// Mat과 CImage 생성
	m_vCam->read(m_matImg);
	m_cImg.Create(m_winSize.width, m_winSize.height, 24);

	// Haarcascade 모델 설정
	if (m_model.load("haarcascade_frontalface_default.xml"));
	else
	{
		AfxMessageBox(_T("Haarcascade xml 경로가 잘못되었습니다."), MB_OK | MB_ICONWARNING);
		return -1;
	}

	// confidence 값을 가져올 파일 객체 초기화
	file.open("log.txt");
	if (!file.is_open())
	{
		AfxMessageBox(_T("log.txt가 누락되었습니다."), MB_OK | MB_ICONWARNING);
		return -1;
	}
	m_fConfidence = 0.0;
	
	// 한 프레임씩 화면에 출력하게 타이머 설정
	SetTimer(0, 33, NULL);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFaceRecognitionFinalDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFaceRecognitionFinalDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CFaceRecognitionFinalDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFaceRecognitionFinalDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	GetFrame();
	CropFace(FALSE);
	MatToCImage(m_matImg, m_cImg, rect);
	ShowImg(m_ctrlCrop, m_cImg);
	CDialogEx::OnTimer(nIDEvent);
}


cv::Mat CFaceRecognitionFinalDlg::GetFrame()
{
	// TODO: 여기에 구현 코드 추가.
	m_vCam->read(m_matImg);
	return m_matImg;
}


void CFaceRecognitionFinalDlg::CropFace(bool is_Modal)
{
	// TODO: 여기에 구현 코드 추가.
	cv::Mat gray;
	cv::cvtColor(m_matImg, gray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(gray, gray);
	std::vector<cv::Rect> faces;
	m_model.detectMultiScale(m_matImg, faces, 1.1, 3, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(300, 300));

	for (int i = 0; i < faces.size(); i++) {
		cv::Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
		cv::Point tr(faces[i].x, faces[i].y);
		cv::rectangle(m_matImg, lb, tr, cv::Scalar(0, 255, 0), 3, 4, 0);
		if (!is_Modal)
		{
			KillTimer(0);
			Modal dlg(this, this);
			dlg.DoModal();
			CreateImg();
			ViewResult();
			SetTimer(0, 33, NULL);
		}
	}
}


void CFaceRecognitionFinalDlg::MatToCImage(cv::Mat& mat, CImage& cImg, CRect& rct)
{
	// TODO: 여기에 구현 코드 추가.
	cv::Mat resizeMat;
	cv::resize(mat, resizeMat, cv::Size(rct.Width(), rct.Height()), 0, 0, cv::INTER_CUBIC);
	int bpp = resizeMat.channels() * 8; // Bit per pixel (8 for gray, 24 for BGR, 32 for BGRA)
	cImg.Destroy();  // 기존에 생성된 CImage가 있다면 해제
	cImg.Create(resizeMat.cols, resizeMat.rows, bpp); // 새로운 CImage 생성

	// CImage의 포인터 얻기
	uchar* pDest = (uchar*)cImg.GetBits();
	int step = cImg.GetPitch(); // 한 줄의 바이트 수 (음수일 수도 있음)

	// cv::Mat 데이터를 CImage로 복사
	for (int y = 0; y < resizeMat.rows; y++)
	{
		memcpy(pDest + y * step, resizeMat.ptr<uchar>(y), resizeMat.cols * resizeMat.channels());
	}
}


void CFaceRecognitionFinalDlg::ShowImg(CStatic& ctrl, CImage& cImg)
{
	// TODO: 여기에 구현 코드 추가.
	HDC dc = ::GetDC(ctrl.m_hWnd);
	cImg.BitBlt(dc, 0, 0);
	::ReleaseDC(ctrl.m_hWnd, dc);
}


void CFaceRecognitionFinalDlg::CreateImg()
{
	// TODO: 여기에 구현 코드 추가.
	m_matImg = GetFrame();
	MatToCImage(m_matImg, m_cImg, rect);
	ShowImg(m_ctrlCrop, m_cImg);
	std::string filename = "LastImg.jpg";
	cv::imwrite(filename, m_matImg);
	int ret = system("comp.py");
	if (ret != 0) {
		AfxMessageBox(_T("파이썬 파일이 연동되지 않았습니다."), MB_OK | MB_ICONERROR);
	}
}


void CFaceRecognitionFinalDlg::ViewResult()
{
	// TODO: 여기에 구현 코드 추가.
	file.seekg(0);
	file >> m_fConfidence;
	CString msg;
	if (file.fail())
	{
		AfxMessageBox(_T("파일에서 값을 읽는 데 실패했습니다."), MB_OK | MB_ICONWARNING);
	}
	else if (m_fConfidence > 99.5f)
	{
		msg.Format(_T("문이 열립니다. 인식률은 %.2f%% 입니다."), m_fConfidence);
		AfxMessageBox(msg);
	}
	else if (m_fConfidence >= 0.0f)
	{
		msg.Format(_T("등록된 사용자가 아닙니다. 인식률은 %.2f%% 입니다."), m_fConfidence);
		AfxMessageBox(msg, MB_OK | MB_ICONWARNING);
	}
	else
	{
		AfxMessageBox(_T("얼굴이 인식되지 않았습니다."), MB_OK | MB_ICONWARNING);
	}
}