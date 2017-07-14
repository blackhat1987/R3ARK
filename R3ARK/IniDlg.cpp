// IniDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IniDlg.h"
#include "afxdialogex.h"
#include "resource.h"

// CIniDlg 对话框

IMPLEMENT_DYNAMIC(CIniDlg, CDialogEx)

CIniDlg::CIniDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_INI, pParent)
{

}

CIniDlg::~CIniDlg()
{
}

void CIniDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INI, m_listIni);
}


BEGIN_MESSAGE_MAP(CIniDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CIniDlg 消息处理程序
BOOL CIniDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_cfgList = mfc::listview(&m_listIni);

	m_cfgList.add_column(L"姓名");
	m_cfgList.add_column(L"年龄");
	m_cfgList.add_column(L"性别");
	m_cfgList.add_column(L"学历");

	for (auto i = 0; i < 100; i++)
	{
		m_cfgList.insert(L"1", L"1", L"1", L"1");
	}
	return TRUE;  
}



