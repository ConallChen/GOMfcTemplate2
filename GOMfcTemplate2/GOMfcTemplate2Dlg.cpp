
//////////////////////////////////////////////////////////////////////////////
//���ƣ�GOMfcTemplate2
//���ܣ�opencv+mfc ���
//���ߣ�jsxyhelu(1755311380@qq.com http://jsxyhelu.cnblogs.com)
//��֯��GREENOPEN
//���ڣ�2016-10-30
/////////////////////////////////////////////////////////////////////////////

// GOMfcTemplate2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GOMfcTemplate2.h"
#include "GOMfcTemplate2Dlg.h"
#include "afxdialogex.h"
using namespace std;
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//����ͷ��ʾѭ��,���й��ڲɼ��Ĳ�����ͨ�����̴߳��ݿ��Ʊ������ɼ��̣߳������ɲɼ��߳���ɵ�
DWORD WINAPI CaptureThread(LPVOID lpParameter)
{
	CGOMfcTemplate2Dlg* pDlg = (CGOMfcTemplate2Dlg*)lpParameter;
	while (true)
	{
		if (pDlg->b_closeCam)//�˳�ѭ��
			break;
		//��directX�л�õ�ǰͼ����ʾ����
		IplImage* queryframe  = pDlg->cameraDs.QueryFrame();
		//��2.0�汾�п���ǿת����3.0����Ҫʹ�ú���
		Mat camframe = cvarrToMat(queryframe);
		pDlg->showImage(camframe,IDC_CAM);
		//�ɼ�����ʾͼ��?
		//ע�⣡�����ҽ��ɼ�����ͼ�����ʾд�����߳��У������ܹ�ǿ�ƴ����ػ� jsxyhelu 2016��10��30�� 
		if (pDlg->b_takeApic )
		{
			pDlg->m_mainframe = camframe.clone();
			pDlg->b_takeApic = false;
		}
		if (pDlg->m_mainframe.rows > 0)
		{
			pDlg->showImage(pDlg->m_mainframe,IDC_PIC);
		}
		 
	}

	return 0;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGOMfcTemplate2Dlg �Ի���



CGOMfcTemplate2Dlg::CGOMfcTemplate2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGOMfcTemplate2Dlg::IDD, pParent)
	, m_nCamCount(0)
	, m_iCamNum(0)
	, b_takeApic(false)
	, b_closeCam(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGOMfcTemplate2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CAM, m_CBNCamList);
}

BEGIN_MESSAGE_MAP(CGOMfcTemplate2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPENCAM, &CGOMfcTemplate2Dlg::OnBnClickedBtnOpencam)
	ON_CBN_SELCHANGE(IDC_COMBO_CAM, &CGOMfcTemplate2Dlg::OnCbnSelchangeComboCam)
	ON_BN_CLICKED(IDC_BTN_TAKEPIC, &CGOMfcTemplate2Dlg::OnBnClickedBtnTakepic)
	ON_BN_CLICKED(IDC_BTN_CONFIG, &CGOMfcTemplate2Dlg::OnBnClickedBtnConfig)
	ON_BN_CLICKED(IDC_BTN_CLOSECAM, &CGOMfcTemplate2Dlg::OnBnClickedBtnClosecam)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_RATIO, &CGOMfcTemplate2Dlg::OnBnClickedBtnRatio)
END_MESSAGE_MAP()


// CGOMfcTemplate2Dlg ��Ϣ�������

BOOL CGOMfcTemplate2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_nCamCount = CCameraDS::CameraCount();//����ͷ����
	//�������ͷ��Ŀ
	char camera_name[1024];
	char istr[25];
	for(int i=0; i < m_nCamCount; i++)
	{  
		int retval = CCameraDS::CameraName(i, camera_name, sizeof(camera_name) );
		sprintf_s(istr, " # %d", i);
		strcat_s(camera_name,istr );  
		CString camstr(camera_name);
		if(retval >0)
			m_CBNCamList.AddString(camstr);
		else
			AfxMessageBox(_T("���ܻ�ȡ����ͷ������"));
	} 
	//��ʼ����ʾ�ؼ�
	CRect rect;
	GetDlgItem(IDC_CAM)->GetClientRect(&rect);
	m_mainframe = Mat::zeros(rect.Height(),rect.Width(),CV_8UC3);
	GetDlgItem(IDC_PIC)->GetClientRect(&rect);
	m_takepic  = Mat::zeros(rect.Height(),rect.Width(),CV_8UC3);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGOMfcTemplate2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGOMfcTemplate2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//����ͼƬ��ʾ����
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGOMfcTemplate2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGOMfcTemplate2Dlg::OnBnClickedBtnOpencam()
{
	if (m_nCamCount>=1)//����Ƶ�����߳�
	{	
		 HANDLE hThread = NULL;
		 DWORD dwThreadID = 0;
		 OnBnClickedBtnClosecam();//���ȹر���������ͷ
		 bool bret = cameraDs.OpenCamera(m_iCamNum,false,640,480); //���Դ�����ͷ
		 if (bret)
		 {  
			 b_closeCam = false;
			 hThread = CreateThread(NULL, 0, CaptureThread, this, 0, &dwThreadID);
		 }
		
	}
	else
	{
		AfxMessageBox(_T("��ȷ������������ͷ������"));
	}
}


void CGOMfcTemplate2Dlg::OnCbnSelchangeComboCam()
{
	m_iCamNum = m_CBNCamList.GetCurSel();
}


void CGOMfcTemplate2Dlg::showImage(Mat& src, UINT ID)
{
	if (src.empty())
		return;
	CRect rect;
	Mat dst = src.clone();
	GetDlgItem(ID) ->GetClientRect( &rect );	// ��ȡ�ؼ��ߴ�λ��
	if (dst.channels() == 1)
		cvtColor(dst, dst, CV_GRAY2BGR);
	CDC* pDC = GetDlgItem( ID ) ->GetDC();   
	HDC hDC = pDC ->GetSafeHdc();				// ��ȡ HDC(�豸���) �����л�ͼ����
	CvvImage cimg;
	IplImage cpy = dst;
	cimg.CopyOf( &cpy );						// ����ͼƬ
	cimg.DrawToHDC( hDC, &rect );				// ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	ReleaseDC( pDC );
}


void CGOMfcTemplate2Dlg::OnBnClickedBtnTakepic()
{
	b_takeApic = true;
	Sleep(100);//�ȴ��ɼ��̷߳��� jsxyhelu
	//imwrite("abc.jpg",m_mainframe);
}


void CGOMfcTemplate2Dlg::OnBnClickedBtnConfig()
{
	cameraDs.DisplayFilterProperties();
}


void CGOMfcTemplate2Dlg::OnBnClickedBtnClosecam()
{
	 //���Թر�����ͷ
	b_closeCam = true;
	Sleep(100);
	cameraDs.CloseCamera();
}


void CGOMfcTemplate2Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	OnBnClickedBtnClosecam();//��������֮ǰ���Թر���������ͷ���߳�
}



void CGOMfcTemplate2Dlg::OnBnClickedBtnRatio()
{
	OnBnClickedBtnClosecam();
	if (m_nCamCount>=1)//����Ƶ�����߳�
	{	
		HANDLE hThread = NULL;
		DWORD dwThreadID = 0;
		OnBnClickedBtnClosecam();//���ȹر���������ͷ
		bool bret = cameraDs.OpenCamera(m_iCamNum,true,640,480); //���Դ�����ͷ
		if (bret)
		{  
			b_closeCam = false;
			hThread = CreateThread(NULL, 0, CaptureThread, this, 0, &dwThreadID);
		}

	}
	else
	{
		AfxMessageBox(_T("��ȷ������������ͷ������"));
	}
}
