#pragma once
#include "afxcmn.h"
#include "CommonMFC/mfc_listview.h"

// CIniDlg 对话框
class CIniDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIniDlg)

public:
	CIniDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIniDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listIni;
	mfc::listview m_cfgList;
	virtual BOOL OnInitDialog();
	
};
