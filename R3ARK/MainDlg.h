
// MainDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "IniDlg.h"
#include "ProcessDlg.h"
#include "CommonMFC/mfc_listview.h"
#include "CommonMFC/mfc_tab.h"


// CMainCDlg �Ի���
class CMainCDlg : public CDialogEx
{
// ����
public:
	CMainCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEMPMFC_DIALOG };
#endif

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
public:
	CTabCtrl m_tabMain;
	mfc::TabCtrl m_cfgTab;
	CIniDlg m_iniDlg;
	CProcessDlg m_processDlg;
	afx_msg void OnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
};
