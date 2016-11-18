// OpenDevDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCAutoLinkCan.h"
#include "OpenDevDlg.h"
#include "afxdialogex.h"

#include "CanUtil.h"
#include "ControlCAN.h"
// COpenDevDlg �Ի���
CanConf_t CanConf;

IMPLEMENT_DYNAMIC(COpenDevDlg, CDialogEx)

COpenDevDlg::COpenDevDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_OPENDEV, pParent)
{

}

COpenDevDlg::~COpenDevDlg()
{
}

void COpenDevDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_CHNL, m_combchnl);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_combbaud);

	DDX_Text(pDX, IDC_EDIT_BGNID, m_EditBgnid);
	DDX_Text(pDX, IDC_EDIT_ENDID, m_EditEndid);
}


BEGIN_MESSAGE_MAP(COpenDevDlg, CDialogEx)
END_MESSAGE_MAP()


// COpenDevDlg ��Ϣ�������


BOOL COpenDevDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_combchnl.SetCurSel(0);//Ԥ��CAN1
	m_combbaud.SetCurSel(2);//Ԥ��500K


	m_EditBgnid = _T("700");
	m_EditEndid = _T("7FF");
	GetDlgItem(IDC_EDIT_BGNID)->SetWindowText(m_EditBgnid);
	GetDlgItem(IDC_EDIT_ENDID)->SetWindowText(m_EditEndid);

	m_EditExBgnid.SubclassWindow(GetDlgItem(IDC_EDIT_BGNID)->m_hWnd);
	m_EditExEndid.SubclassWindow(GetDlgItem(IDC_EDIT_ENDID)->m_hWnd);

	m_EditExBgnid.SetLimitLen(3);
	m_EditExEndid.SetLimitLen(3);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void COpenDevDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	UpdateData(true);//���¿ؼ�����

	CString baudrate;//�����ַ���
	CString databit;
	INT Index;
	BYTE id_bgn[10] = { 0 };
	BYTE id_end[10] = { 0 };
	BYTE temp_buf[50];
	LONG temp_len;
	UINT filter_code;

	if (m_combchnl.GetCurSel() == 0)
		CanConf.mCanChnl = CANCH_CAN1;
	else
		CanConf.mCanChnl = CANCH_CAN2;

	Index = m_combbaud.GetCurSel();
	m_combbaud.GetLBText(Index, baudrate);//ȡ����ѡ���ַ������������baudrate����

	temp_len = CanUtil::str2char(m_EditBgnid, temp_buf) - 1;
	CanUtil::str2HEX(temp_buf, id_bgn);

	temp_len = CanUtil::str2char(m_EditEndid, temp_buf) - 1;
	CanUtil::str2HEX(temp_buf, id_end);


	if (CanConf.mFilterEn)
	{
		UINT temp_id;
		CanConf.mBgnid = (UINT)id_bgn[0] << 8;
		CanConf.mBgnid |= (UINT)id_bgn[1] << 0;

		CanConf.mEndid = (UINT)id_end[0] << 8;
		CanConf.mEndid |= (UINT)id_end[1] << 0;

		if (CanConf.mEndid < CanConf.mBgnid)
		{
			temp_id = CanConf.mBgnid;
			CanConf.mBgnid = CanConf.mEndid;
			CanConf.mEndid = temp_id;
		}
	}
	else
	{
		CanConf.mBgnid = 0x000;
		CanConf.mEndid = 0x7FF;
	}

	filter_code = 0;
	filter_code = (CanConf.mBgnid & 0x00000F00) << 21;
	filter_code |= (CanConf.mEndid & 0x00000F00) << 5;

	DWORD Reserved = 0;
	//���豸
	if (VCI_OpenDevice(VCI_USBCAN2, CAN_DEVINDEX, Reserved) != 1)
	{
		MessageBox(_T("open usbcan failed"));
		return;
	}
	VCI_INIT_CONFIG InitInfo[1];
	if (Index == 0)
	{
		InitInfo->Timing0 = 0x03;
		InitInfo->Timing1 = 0x1C;
	}
	else if (Index == 1)
	{
		InitInfo->Timing0 = 0x01;
		InitInfo->Timing1 = 0x1C;
	}
	else
	{
		InitInfo->Timing0 = 0x00;
		InitInfo->Timing1 = 0x1C;
	}
	InitInfo->Filter = 0;
	InitInfo->AccCode = filter_code;
	InitInfo->AccMask = (~filter_code);
	InitInfo->Mode = 0;
	//��ʼ��CAN1
	if (VCI_InitCAN(VCI_USBCAN2, CAN_DEVINDEX, CanConf.mCanChnl, InitInfo) != 1)
	{
		MessageBox(_T("Init-CAN1 failed!"));
		return;
	}
	Sleep(100);
	//��ʼ��CAN1
	if (VCI_StartCAN(VCI_USBCAN2, CAN_DEVINDEX, CanConf.mCanChnl) != 1)
	{
		MessageBox(_T("Start-CAN1 failed!"));
		return;
	}

	MessageBox(_T("Open successful!\n Start CAN OK!"));


	CDialogEx::OnOK();
}
