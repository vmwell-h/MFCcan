
// MFCAutoLinkCan.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCAutoLinkCanApp: 
// �йش����ʵ�֣������ MFCAutoLinkCan.cpp
//

class CMFCAutoLinkCanApp : public CWinApp
{
public:
	CMFCAutoLinkCanApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCAutoLinkCanApp theApp;