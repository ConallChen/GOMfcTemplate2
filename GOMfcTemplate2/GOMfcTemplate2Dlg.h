
//////////////////////////////////////////////////////////////////////////////
//���ƣ�GOMfcTemplate2
//���ܣ�opencv+mfc ���
//���ߣ�jsxyhelu(1755311380@qq.com http://jsxyhelu.cnblogs.com)
//��֯��GREENOPEN
//���ڣ�2016-10-30
/////////////////////////////////////////////////////////////////////////////
// GOMfcTemplate2Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "camerads.h"
using namespace std;
using namespace cv;

// CGOMfcTemplate2Dlg �Ի���
class CGOMfcTemplate2Dlg : public CDialogEx
{
// ����
public:
	CGOMfcTemplate2Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GOMFCTEMPLATE2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	int m_nCamCount;
public:
	CComboBox m_CBNCamList;
	afx_msg void OnBnClickedBtnOpencam();
	Mat m_mainframe;
	Mat m_takepic;
	CCameraDS cameraDs;
	int m_iCamNum;
	afx_msg void OnCbnSelchangeComboCam();
	void showImage(Mat& src, UINT ID);
	afx_msg void OnBnClickedBtnTakepic();
	afx_msg void OnBnClickedBtnConfig();
	afx_msg void OnBnClickedBtnClosecam();
	bool b_closeCam;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnRatio();
	bool b_takeApic;
};
