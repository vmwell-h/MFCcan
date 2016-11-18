
// MFCAutoLinkCanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCAutoLinkCan.h"
#include "MFCAutoLinkCanDlg.h"
#include "afxdialogex.h"

#include "OpenDevDlg.h"
#include "EditMsgDlg.h"
#include "CanUtil.h"
#include "ControlCAN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CanConf_t CanConf;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFCAutoLinkCanDlg 对话框



CMFCAutoLinkCanDlg::CMFCAutoLinkCanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAUTOLINKCAN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_XmitRow = 0;
	m_RecvRow = 0;

	m_SameId = FALSE;
	m_GetPre = FALSE;

	m_XmitEnable = TRUE;
	m_RecvEnable = TRUE;
	CanConf = { 0,0,0,0 };
}

void CMFCAutoLinkCanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_XMIT, m_XmitList);
	DDX_Control(pDX, IDC_LIST_RECV, m_RecvList);
	DDX_Control(pDX, IDC_EDIT_PREDATA, m_EditPre);

	DDX_Check(pDX, IDC_CHECK_SAMEID, m_SameId);
	DDX_Check(pDX, IDC_CHECK_XMIT, m_XmitEnable);
	DDX_Check(pDX, IDC_CHECK_RECV, m_RecvEnable);
}

BEGIN_MESSAGE_MAP(CMFCAutoLinkCanDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENU_OPENDEV, &CMFCAutoLinkCanDlg::OnMenuOpendev)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_XMIT, &CMFCAutoLinkCanDlg::OnNMRClickListEdit)
	ON_COMMAND(ID_MENU_ADDMSG, &CMFCAutoLinkCanDlg::OnMenuAddmsg)
	ON_COMMAND(ID_MENU_DELMSG, &CMFCAutoLinkCanDlg::OnMenuDelmsg)
	ON_COMMAND(ID_MENU_CLRMSG, &CMFCAutoLinkCanDlg::OnMenuClemsg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_XMIT, &CMFCAutoLinkCanDlg::OnNMDblclkListXmit)
	ON_WM_TIMER()
	ON_COMMAND(ID_MENU_CLRRCV, &CMFCAutoLinkCanDlg::OnMenuRecvclr)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RECV, &CMFCAutoLinkCanDlg::OnNMRClickListRecv)
	ON_COMMAND(ID_MENU_CLOSEDEV, &CMFCAutoLinkCanDlg::OnMenuClosedev)
	ON_BN_CLICKED(IDC_CHECK_SAMEID, &CMFCAutoLinkCanDlg::OnBnClickedCheckSameid)
	ON_COMMAND(ID_MENU_PRESET, &CMFCAutoLinkCanDlg::OnMenuPreset)
	ON_COMMAND(ID_MENU_EDIT, &CMFCAutoLinkCanDlg::OnMenuEdit)
	ON_COMMAND(ID_MENU_APPLY, &CMFCAutoLinkCanDlg::OnMenuApply)
	ON_COMMAND(ID_MENU_CANCEL, &CMFCAutoLinkCanDlg::OnMenuCancel)
	ON_BN_CLICKED(IDC_CHECK_XMIT, &CMFCAutoLinkCanDlg::OnBnClickedCheckXmit)
	ON_BN_CLICKED(IDC_CHECK_RECV, &CMFCAutoLinkCanDlg::OnBnClickedCheckRecv)
END_MESSAGE_MAP()


// CMFCAutoLinkCanDlg 消息处理程序

BOOL CMFCAutoLinkCanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//添加Menu
	m_Menu.LoadMenu(IDR_MENU_MAIN);
	SetMenu(&m_Menu);

	m_XmitList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);

	m_XmitList.InsertColumn(0, _T("Cycle Time"));
	m_XmitList.SetColumnWidth(0, 75);
	m_XmitList.InsertColumn(1, _T("MSG ID"));
	m_XmitList.SetColumnWidth(1, 55);
	m_XmitList.InsertColumn(2, _T("DLC"));
	m_XmitList.SetColumnWidth(2, 40);
	m_XmitList.InsertColumn(3, _T("Data"));
	m_XmitList.SetColumnWidth(3, 175);
	m_XmitList.InsertColumn(4, _T("Count"));
	m_XmitList.SetColumnWidth(4, 55);
	m_XmitList.InsertColumn(5, _T("Num"));
	m_XmitList.SetColumnWidth(5, 55);


	m_RecvList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	m_RecvList.InsertColumn(0, _T("Cycle Time"));
	m_RecvList.SetColumnWidth(0, 75);
	m_RecvList.InsertColumn(1, _T("MSG ID"));
	m_RecvList.SetColumnWidth(1, 55);
	m_RecvList.InsertColumn(2, _T("DLC"));
	m_RecvList.SetColumnWidth(2, 40);
	m_RecvList.InsertColumn(3, _T("Data"));
	m_RecvList.SetColumnWidth(3, 175);
	m_RecvList.InsertColumn(4, _T("Count"));
	m_RecvList.SetColumnWidth(4, 55);

	//CAN Msg handle Timer
	SetTimer(1, TIMOUT_CAN_MS, NULL);

	m_EditPre.ShowWindow(SW_HIDE);


	m_EditPre.SetLimitText(23);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCAutoLinkCanDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCAutoLinkCanDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCAutoLinkCanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCAutoLinkCanDlg::OnMenuOpendev()
{
	// TODO: 在此添加命令处理程序代码
	COpenDevDlg  Dlg;
	Dlg.DoModal();
}

void CMFCAutoLinkCanDlg::OnMenuClosedev()
{
	// TODO: 在此添加命令处理程序代码
	if (VCI_CloseDevice(VCI_USBCAN2, CAN_DEVINDEX) != 1)
	{
		MessageBox(_T("Close-CAN1 failed!"));
	}
	else
	{
		MessageBox(_T("Clos-CAN1 success!"));
	}
}

void CMFCAutoLinkCanDlg::OnNMRClickListEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CRect rect;
    GetWindowRect(&rect);
    CPoint p;
    GetCursorPos(&p);//鼠标点的屏幕坐标  
	if (rect.PtInRect(p))
    {
        CMenu RMenu;
		RMenu.LoadMenu(IDR_MENU_XMIT);//编辑好的菜单资源  
        CMenu *m_SubMenu = RMenu.GetSubMenu(0);

	    m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);
    }

	*pResult = 0;
}


void CMFCAutoLinkCanDlg::OnMenuAddmsg()
{
	// TODO: 在此添加命令处理程序代码
	XmitMsgAdd();
}

void CMFCAutoLinkCanDlg::OnMenuEdit()
{
	// TODO: 在此添加命令处理程序代码
	POSITION ps;
	INT nIndex;

	ps = m_XmitList.GetFirstSelectedItemPosition();
	nIndex = m_XmitList.GetNextSelectedItem(ps);

	if (nIndex == -1) return;

	EditMsg_t EditMsg;
	CString str;
	CEditMsgDlg Dlg;

	EditMsg.Cycle = m_XmitList.GetItemText(nIndex, 0);
	EditMsg.Data = m_XmitList.GetItemText(nIndex, 3);
	str = m_XmitList.GetItemText(nIndex, 2);
	EditMsg.Dlc = 8;
	EditMsg.Id = m_XmitList.GetItemText(nIndex, 1);
	EditMsg.FrameNum = m_XmitList.GetItemText(nIndex, 5);

	Dlg.pEditMsg = &EditMsg;
	Dlg.DoModal();

	CanMsg_t CanMsg;
	CanMsg = MsgConvert(EditMsg);
	m_XmitArray.SetAt(nIndex, CanMsg);

	m_XmitList.SetItemText(nIndex, 0, EditMsg.Cycle);	//Cycle Time
	m_XmitList.SetItemText(nIndex, 1, EditMsg.Id);	    //Can id
	str.Format(_T("%d"), EditMsg.Dlc);
	m_XmitList.SetItemText(nIndex, 2, str);	            //Can Data Length
	m_XmitList.SetItemText(nIndex, 3, EditMsg.Data);	    //Can Data
	m_XmitList.SetItemText(nIndex, 5, EditMsg.FrameNum);	//Frame Num
}


void CMFCAutoLinkCanDlg::OnMenuDelmsg()
{
	// TODO: 在此添加命令处理程序代码
	POSITION ps;
	INT nIndex;

	ps = m_XmitList.GetFirstSelectedItemPosition();
	nIndex = m_XmitList.GetNextSelectedItem(ps);

	INT ListSize = m_XmitList.GetItemCount();
	for (INT i = nIndex; i < ListSize; i++)
		m_XmitList.ItemColorFlag[i] = m_XmitList.ItemColorFlag[i+1];

	m_XmitList.DeleteItem(nIndex);
	m_XmitArray.RemoveAt(nIndex);
}


void CMFCAutoLinkCanDlg::OnMenuClemsg()
{
	// TODO: 在此添加命令处理程序代码
	m_XmitList.DeleteAllItems();
	m_XmitArray.RemoveAll();
	UpdateData(FALSE);//更新数据
}



void CMFCAutoLinkCanDlg::OnMenuPreset()
{
	// TODO: 在此添加命令处理程序代码
	POSITION ps;
	INT nIndex;
	INT nSubItem = 3;
	ps = m_XmitList.GetFirstSelectedItemPosition();
	nIndex = m_XmitList.GetNextSelectedItem(ps);

	if (nIndex == -1) return;

	HWND hWnd1 = ::GetDlgItem(m_hWnd, IDC_LIST_XMIT);
	m_GetPre = TRUE;
	m_Item = nIndex;
	//Retrieve the text of the selected subItem 
	//from the list
	CString str = m_XmitList.GetItemText(nIndex, nSubItem);
	CRect rect;
	RECT rect1, rect2;
	// this macro is used to retrieve the Rectanle 
	// of the selected SubItem
	m_XmitList.GetSubItemRect(nIndex, nSubItem, LVIR_BOUNDS, rect);
	//Get the Rectange of the listControl
	::GetWindowRect(hWnd1, &rect1);
	//Get the Rectange of the Dialog
	::GetWindowRect(m_hWnd, &rect2);

	int x = rect1.left - rect2.left;
	int y = rect1.top - rect2.top;

	if (nIndex != -1)
		::SetWindowPos(::GetDlgItem(m_hWnd, IDC_EDIT_PREDATA),
			HWND_TOP, rect.left + x - 7, rect.top + 65,
			rect.right - rect.left,
			rect.bottom - rect.top, NULL);
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_PREDATA), SW_SHOW);
	::SetFocus(::GetDlgItem(m_hWnd, IDC_EDIT_PREDATA));
	//Draw a Rectangle around the SubItem
	//::Rectangle(::GetDC(pNMItemActivate->hdr.hwndFrom),
	//rect.left, rect.top - 1, rect.right, rect.bottom);
	//Set the listItem text in the EditBox
	::SetWindowText(::GetDlgItem(m_hWnd, IDC_EDIT_PREDATA), str);

	Invalidate();
}


void CMFCAutoLinkCanDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	INT nSubItem = 3;

	if (m_GetPre == FALSE) return;
	m_GetPre = FALSE;

	CString EditStr;
	//get the text from the EditBox
	m_EditPre.GetWindowText(EditStr);

	m_XmitList.SetItemText(m_Item, 3, EditStr);	         //Display the Can PreData In XmitList
	m_XmitList.ItemColorFlag[m_Item] = 1;

	CanMsg_t CanMsg;
	CanMsg = m_XmitArray.GetAt(m_Item);

	BYTE temp_buf[50];
	LONG temp_len;
	BYTE HexBuf[10];
	BYTE ttmp_buf[32];
	BYTE TmpChar;
	INT newflag = 1;
	INT DataNum = 0;
	temp_len = CanUtil::str2char(EditStr, temp_buf) - 1;
	for (INT i = 0; i<temp_len; i++)
	{
		TmpChar = temp_buf[i];
		if (TmpChar == ' ')
		{
			newflag = 1;
		}
		else
		{
			if (newflag == 1)
			{
				newflag = 0;
				ttmp_buf[0] = TmpChar;
				ttmp_buf[1] = 0;
				ttmp_buf[2] = 0;
			}
			else
			{
				newflag = 1;
				ttmp_buf[1] = TmpChar;
				ttmp_buf[2] = 0;
			}

			if (newflag == 1 || temp_buf[i + 1] == ' ')
			{
				CanUtil::str2HEX(ttmp_buf, HexBuf);
				CanMsg.PreData[DataNum++] = HexBuf[0];
				if (DataNum >= CanMsg.Dlc)
					break;
			}
		}
	}
	CanMsg.PreSet = TRUE;
	//Refresh the XmitArray
	m_XmitArray.SetAt(m_Item, CanMsg);

	m_EditPre.ShowWindow(SW_HIDE);

	Invalidate();
	//CDialogEx::OnOK();
}

void CMFCAutoLinkCanDlg::OnMenuApply()
{
	// TODO: 在此添加命令处理程序代码
	INT nItem;
	CanMsg_t CanMsg;
	INT ListSize = m_XmitList.GetItemCount();
	for (nItem = 0; nItem < ListSize; nItem++)
	{
		CanMsg = m_XmitArray.GetAt(nItem);
		if (CanMsg.PreSet == TRUE)
		{
			CanMsg.PreSet = FALSE;
			for (INT i = 0; i < CanMsg.Dlc; i++)
				CanMsg.Data[i] = CanMsg.PreData[i];

			//Refresh the XmitArray
			m_XmitArray.SetAt(nItem, CanMsg);

			m_XmitList.ItemColorFlag[nItem] = 0;
		}
	}
	Invalidate();
}


void CMFCAutoLinkCanDlg::OnMenuCancel()
{
	// TODO: 在此添加命令处理程序代码
	INT nItem;
	CanMsg_t CanMsg;
	CString str,str1;
	INT ListSize = m_XmitList.GetItemCount();
	for (nItem = 0; nItem < ListSize; nItem++)
	{
		CanMsg = m_XmitArray.GetAt(nItem);
		if (CanMsg.PreSet == TRUE)
		{
			CanMsg.PreSet = FALSE;

			str = _T("");
			for (INT i = 0; i < CanMsg.Dlc; i++)
			{
				str1.Format(_T("%02X"), CanMsg.Data[i]);
				str = (str + str1 + _T(" "));
			}
			m_XmitList.ItemColorFlag[nItem] = 0;
			m_XmitList.SetItemText(nItem, 3, str);	         //Display the Can PreData In XmitList
			//Refresh the XmitArray
			m_XmitArray.SetAt(nItem, CanMsg);
		}
	}
	Invalidate();
}

void CMFCAutoLinkCanDlg::OnNMDblclkListXmit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem == -1)
	{
		XmitMsgAdd();
		return;
	}

	INT nItem, nSubItem;
	//get the row number
	nItem = pNMItemActivate->iItem;
	//get the column number
	nSubItem = pNMItemActivate->iSubItem;

	EditMsg_t EditMsg;
	CString str;
	CEditMsgDlg Dlg;

	EditMsg.Cycle = m_XmitList.GetItemText(nItem, 0);
	EditMsg.Data = m_XmitList.GetItemText(nItem, 3);
	str = m_XmitList.GetItemText(nItem, 2);
	EditMsg.Dlc = 8;
	EditMsg.Id = m_XmitList.GetItemText(nItem, 1);
	EditMsg.FrameNum = m_XmitList.GetItemText(nItem, 5);

	Dlg.pEditMsg = &EditMsg;
	Dlg.DoModal();

	CanMsg_t CanMsg;
	CanMsg = MsgConvert(EditMsg);
	m_XmitArray.SetAt(nItem, CanMsg);

	m_XmitList.SetItemText(nItem, 0, EditMsg.Cycle);	//Cycle Time
	m_XmitList.SetItemText(nItem, 1, EditMsg.Id);	    //Can id
	str.Format(_T("%d"), EditMsg.Dlc);
	m_XmitList.SetItemText(nItem, 2, str);	            //Can Data Length
	m_XmitList.SetItemText(nItem, 3, EditMsg.Data);	    //Can Data
	m_XmitList.SetItemText(nItem, 5, EditMsg.FrameNum);	//Frame Num

	*pResult = 0;
}


void CMFCAutoLinkCanDlg::OnNMRClickListRecv(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	CRect rect;
	GetWindowRect(&rect);
	CPoint p;
	GetCursorPos(&p);//鼠标点的屏幕坐标  
	if (rect.PtInRect(p))
	{
		CMenu RMenu;
		RMenu.LoadMenu(IDR_MENU_RECV);//编辑好的菜单资源  
		CMenu *m_SubMenu = RMenu.GetSubMenu(0);

		m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);
	}

	*pResult = 0;
}

void CMFCAutoLinkCanDlg::OnMenuRecvclr()
{
	// TODO: 在此添加命令处理程序代码
	m_RecvList.DeleteAllItems();
	m_RecvArray.RemoveAll();
	UpdateData(FALSE);//更新数据
}

// Convert a EditMsg to CanMsg
CanMsg_t CMFCAutoLinkCanDlg::MsgConvert(EditMsg_t EditMsg)
{
	CanMsg_t CanMsg;

	BYTE temp_buf[50];
	LONG temp_len;
	BYTE HexBuf[10] = { 0 };
	BYTE DecBuf[10] = { 0 };

	temp_len = CanUtil::str2char(EditMsg.Id, temp_buf) - 1;
	CanUtil::str2HEX(temp_buf, HexBuf);
	CanMsg.Id = (UINT)HexBuf[0] << 8;
	CanMsg.Id |= (UINT)HexBuf[1] << 0;

	temp_len = CanUtil::str2char(EditMsg.Cycle, temp_buf) - 1;
	temp_len = CanUtil::str2DEC(temp_buf, DecBuf);
	CanMsg.Cycle = 0;
	for (int i = 0; i < temp_len; i++)
	{
		CanMsg.Cycle = CanMsg.Cycle * 10 + DecBuf[i];
	}

	temp_len = CanUtil::str2char(EditMsg.FrameNum, temp_buf) - 1;
	temp_len = CanUtil::str2DEC(temp_buf, DecBuf);
	CanMsg.FrameNum = 0;
	for (int i = 0; i < temp_len; i++)
	{
		CanMsg.FrameNum = CanMsg.FrameNum * 10 + DecBuf[i];
	}

	CanMsg.Dlc = EditMsg.Dlc;
	if (CanMsg.Dlc >= MSG_LEN)
		CanMsg.Dlc  = MSG_LEN;

	CanMsg.Count = 0;

	BYTE ttmp_buf[32];
	BYTE TmpChar;
	INT newflag = 1;
	INT DataNum = 0;
	temp_len = CanUtil::str2char(EditMsg.Data, temp_buf) - 1;
	for (INT i = 0; i<temp_len; i++)
	{
		TmpChar = temp_buf[i];
		if (TmpChar == ' ')
		{
			newflag = 1;
		}
		else
		{
			if (newflag == 1)
			{
				newflag = 0;
				ttmp_buf[0] = TmpChar;
				ttmp_buf[1] = 0;
				ttmp_buf[2] = 0;
			}
			else
			{
				newflag = 1;
				ttmp_buf[1] = TmpChar;
				ttmp_buf[2] = 0;
			}

			if (newflag == 1 || temp_buf[i + 1] == ' ')
			{
				CanUtil::str2HEX(ttmp_buf, HexBuf);
				CanMsg.Data[DataNum++] = HexBuf[0];
				if (DataNum >= CanMsg.Dlc)
					break;
			}
		}
	}

	return CanMsg;
}

#define TIME_MAX 50  //CNT
void CMFCAutoLinkCanDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//Can Msg Receive
	INT NumValue;
	INT num = 0;
	VCI_CAN_OBJ pCanObj[200];
	DWORD ReceivedID;
	CString str, str1;


	INT nItem;
	CanMsg_t CanMsg;
	BOOL IdExit;

	if (m_RecvEnable == TRUE)
	{
		//调用动态链接库接收函数
		NumValue = VCI_Receive(VCI_USBCAN2, CAN_DEVINDEX, CanConf.mCanChnl, pCanObj, 200, 0);

		for (num = 0; num < NumValue; num++)
		{

			if (m_RecvRow >= 59999)
			{
				m_RecvList.DeleteAllItems();
				m_RecvRow = 0;
			}

			INT_PTR ArraySize = m_RecvArray.GetSize();
			ReceivedID = pCanObj[num].ID;
			IdExit = FALSE;
			for (nItem = 0; nItem < ArraySize; nItem++)
			{
				CanMsg = m_RecvArray.GetAt(nItem);
				if (ReceivedID == CanMsg.Id)
				{
					IdExit = TRUE;
					break;
				}
			}

			if (IdExit == TRUE)
			{
				CanMsg.Id = ReceivedID;
				CanMsg.Dlc = pCanObj[num].DataLen;
				CanMsg.Count++;
				if (pCanObj[num].TimeFlag == 1)
				{
					CanMsg.TimeCnt++;
					if (CanMsg.LastTime == 0)
					{
						CanMsg.TimeCnt = 0;
						CanMsg.LastTime = pCanObj[num].TimeStamp;
					}

					if (CanMsg.TimeCnt >= TIME_MAX)
					{
						CanMsg.Cycle = (pCanObj[num].TimeStamp - CanMsg.LastTime) / (10 * TIME_MAX);
						CanMsg.TimeCnt = 0;
						CanMsg.LastTime = pCanObj[num].TimeStamp;
					}
				}
				else
				{
					CanMsg.Cycle = 0;
					CanMsg.TimeCnt = 0;;
					CanMsg.LastTime = 0;
				}
				//Refresh the RecvArray
				m_RecvArray.SetAt(nItem, CanMsg);
			}
			else
			{
				CanMsg.Id = ReceivedID;
				CanMsg.Dlc = pCanObj[num].DataLen;
				CanMsg.Count = 1;
				CanMsg.Cycle = 0;
				if (pCanObj[num].TimeFlag == 1)
				{
					CanMsg.TimeCnt = 0;
					CanMsg.LastTime = pCanObj[num].TimeStamp;
				}
				else
				{
					CanMsg.TimeCnt = 0;
					CanMsg.LastTime = 0;
				}

				nItem = m_RecvList.InsertItem(m_RecvRow, _T(""));
				m_RecvRow++;
				m_RecvArray.Add(CanMsg);

			}


			//Refresh the RecvList
			if (CanMsg.Cycle == 0)
				str.Format(_T("---"));
			else
				str.Format(_T("%d"), CanMsg.Cycle);
			m_RecvList.SetItemText(nItem, 0, str);	         //Cycle Time

			str.Format(_T("%04X"), CanMsg.Id);
			m_RecvList.SetItemText(nItem, 1, str);	         //Can id
			str.Format(_T("%d"), CanMsg.Dlc);
			m_RecvList.SetItemText(nItem, 2, str);	         //Can Data Length

			str = _T("");
			for (INT i = 0; i < CanMsg.Dlc; i++)
			{
				str1.Format(_T("%02X"), pCanObj[num].Data[i]);
				str = (str + str1 + _T(" "));
			}
			m_RecvList.SetItemText(nItem, 3, str);	         //Can Data

			str.Format(_T("%d"), CanMsg.Count);
			m_RecvList.SetItemText(nItem, 4, str);	         //Count

		}
	}
	/*---------------------------------------------------------------*
	 *---------------------------------------------------------------*/
	//处理发送消息
	if (m_XmitEnable == TRUE)
	{
		VCI_CAN_OBJ SendObj[1];

		if (m_XmitArray.GetSize() != m_XmitList.GetItemCount())
		{
			m_XmitList.DeleteAllItems();
			m_XmitArray.RemoveAll();
		}

		INT ListSize = m_XmitList.GetItemCount();
		for (nItem = 0; nItem < ListSize; nItem++)
		{
			if (m_XmitList.GetCheck(nItem))
			{
				if (CanMsg.Cycle == 0) continue;

				CanMsg = m_XmitArray.GetAt(nItem);
				CanMsg.TimeCnt += TIMOUT_CAN_MS;
				if (CanMsg.TimeCnt >= CanMsg.Cycle)
				{
					CanMsg.TimeCnt = 0;
					CanMsg.Count++;

					int FrameFormat, FrameType;
					FrameFormat = FRMFMT_STD;
					FrameType = FRMTYP_DAT;

					SendObj->ExternFlag = FrameType;
					SendObj->DataLen = CanMsg.Dlc;
					SendObj->RemoteFlag = FrameFormat;
					SendObj->ID = CanMsg.Id;

					for (INT i = 0; i < CanMsg.Dlc; i++)
						SendObj->Data[i] = CanMsg.Data[i];

					CMFCAutoLinkCanDlg::TransmitCanmsg(SendObj);


				}

				if (CanMsg.FrameNum > 0 && CanMsg.Count > CanMsg.FrameNum)
				{
					CanMsg.Count = 0;
					m_XmitList.SetCheck(nItem, FALSE);
				}
				//Refresh the RecvArray
				m_XmitArray.SetAt(nItem, CanMsg);
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


INT CMFCAutoLinkCanDlg::TransmitCanmsg(VCI_CAN_OBJ * SendObj)
{
	INT flag;
	//调用动态链接库发送函数
	flag = VCI_Transmit(VCI_USBCAN2, CAN_DEVINDEX, CanConf.mCanChnl, SendObj, 1);//CAN message send

	return flag;
}


void CMFCAutoLinkCanDlg::OnBnClickedCheckSameid()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}



void CMFCAutoLinkCanDlg::OnBnClickedCheckXmit()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}


void CMFCAutoLinkCanDlg::OnBnClickedCheckRecv()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}


BOOL CMFCAutoLinkCanDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_SPACE)
		{
			CanMsg_t CanMsg;
			VCI_CAN_OBJ SendObj[1];

			POSITION ps;
			INT nIndex;
			INT nSubItem = 3;
			ps = m_XmitList.GetFirstSelectedItemPosition();
			nIndex = m_XmitList.GetNextSelectedItem(ps);

			if (nIndex == -1) return TRUE;

			if (!m_XmitList.GetCheck(nIndex))
			{
				CanMsg = m_XmitArray.GetAt(nIndex);
				CanMsg.Count++;

				int FrameFormat, FrameType;
				FrameFormat = FRMFMT_STD;
				FrameType = FRMTYP_DAT;

				SendObj->ExternFlag = FrameType;
				SendObj->DataLen = CanMsg.Dlc;
				SendObj->RemoteFlag = FrameFormat;
				SendObj->ID = CanMsg.Id;

				for (INT i = 0; i < CanMsg.Dlc; i++)
					SendObj->Data[i] = CanMsg.Data[i];

				CMFCAutoLinkCanDlg::TransmitCanmsg(SendObj);

				//Refresh the RecvArray
				m_XmitArray.SetAt(nIndex, CanMsg);
			}

			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMFCAutoLinkCanDlg::XmitMsgAdd()
{
	EditMsg_t EditMsg;
	CEditMsgDlg Dlg;
	EditMsg.Dlc = 8;
	EditMsg.Cycle = _T("100");
	EditMsg.Data = _T("00 00 00 00 00 00 00 00");
	EditMsg.FrameNum = _T("0");
	Dlg.pEditMsg = &EditMsg;
	Dlg.DoModal();

	CanMsg_t CanMsg;
	CanMsg = MsgConvert(EditMsg);

	CanMsg_t nCanMsg;
	BOOL IdExit;
	UINT nItem;
	UINT nowItem;
	CString str;
	INT_PTR ListSize = m_XmitArray.GetSize();
	IdExit = FALSE;
	for (nItem = 0; nItem < ListSize; nItem++)
	{
		nCanMsg = m_XmitArray.GetAt(nItem);
		if (CanMsg.Id == nCanMsg.Id)
		{
			IdExit = TRUE;
			break;
		}
	}

	if (IdExit == FALSE || m_SameId == TRUE)
	{
		nowItem = m_XmitList.InsertItem(m_XmitRow, _T(""));
		m_XmitRow++;
		m_XmitArray.Add(CanMsg);
	}
	else
	{
		nowItem = nItem;
		m_XmitArray.SetAt(nItem, CanMsg);
	}

	m_XmitList.SetItemText(nowItem, 0, EditMsg.Cycle);	//Cycle Time
	m_XmitList.SetItemText(nowItem, 1, EditMsg.Id);	    //Can id
	str.Format(_T("%d"), EditMsg.Dlc);
	m_XmitList.SetItemText(nowItem, 2, str);	        //Can Data Length
	m_XmitList.SetItemText(nowItem, 3, EditMsg.Data);	    //Can Data
	m_XmitList.SetItemText(nowItem, 5, EditMsg.FrameNum);	////Frame Num
}