
// MFCAutoLinkCanDlg.h : 头文件
//

#pragma once
#include "ColoredListCtrl.h"
#include "EditMsgDlg.h"
#include "ControlCAN.h"

#define MSG_LEN        8
#define TIMOUT_CAN_MS  10

typedef struct _CAN_MSG_T_
{
	UINT Id;
	BYTE Dlc;
	UINT Cycle;
	BYTE Data[MSG_LEN];
	UINT Count;
	UINT FrameNum;
	UINT LastTime;
	UINT TimeCnt;
	BOOL PreSet;
	BYTE PreData[MSG_LEN];
}CanMsg_t, *CanMsg_ptr;



// CMFCAutoLinkCanDlg 对话框
class CMFCAutoLinkCanDlg : public CDialogEx
{
// 构造
public:
	CMFCAutoLinkCanDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAUTOLINKCAN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


private:
	CTabCtrl m_Table;
	CMenu m_Menu;
	CColoredListCtrl m_XmitList;
	//CColoredListCtrl m_RecvList;
	//CListCtrl m_XmitList;
	CListCtrl m_RecvList;

	UINT m_XmitRow;
	UINT m_RecvRow;

	CArray<CanMsg_t> m_XmitArray;
	CArray<CanMsg_t> m_RecvArray;

	CLimitHexEdit m_EditPre;
	BOOL m_SameId;
	BOOL m_GetPre;
	UINT m_Item;

	BOOL m_XmitEnable;
	BOOL m_RecvEnable;
public:
	afx_msg void OnMenuOpendev();
	afx_msg void OnNMRClickListEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuAddmsg();
	afx_msg void OnMenuDelmsg();
	afx_msg void OnMenuClemsg();
	afx_msg void OnNMDblclkListXmit(NMHDR *pNMHDR, LRESULT *pResult);
	// Convert a EditMsg to CanMsg
	CanMsg_t MsgConvert(EditMsg_t EditMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	INT TransmitCanmsg(VCI_CAN_OBJ * SendObj);
	afx_msg void OnMenuRecvclr();
	afx_msg void OnNMRClickListRecv(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuClosedev();
	afx_msg void OnBnClickedCheckSameid();
	afx_msg void OnMenuPreset();
	afx_msg void OnMenuEdit();
	virtual void OnOK();
	afx_msg void OnMenuApply();
	afx_msg void OnMenuCancel();
	afx_msg void OnBnClickedCheckXmit();
	afx_msg void OnBnClickedCheckRecv();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void XmitMsgAdd();
};
