#pragma once
#include "LimitEdit.h"

typedef struct _CAN_CONF_T_
{
	UINT mBgnid;
	UINT mEndid;
	BOOL mFilterEn;
	UINT mCanChnl;
}CanConf_t;

// COpenDevDlg �Ի���

class COpenDevDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpenDevDlg)

public:
	COpenDevDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COpenDevDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OPENDEV };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CLimitHexEdit m_EditExBgnid;
	CLimitHexEdit m_EditExEndid;

	CString m_EditBgnid;
	CString m_EditEndid;

	CComboBox m_combchnl;
	CComboBox m_combbaud;
public:

	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
