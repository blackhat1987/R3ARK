#pragma once
#include "afxcmn.h"
#include "CommonMFC/mfc_listview.h"

// CIniDlg �Ի���
class CIniDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIniDlg)

public:
	CIniDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIniDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listIni;
	mfc::listview m_cfgList;
	virtual BOOL OnInitDialog();
	
};
