// IniDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IniDlg.h"
#include "afxdialogex.h"
#include "resource.h"

// CIniDlg �Ի���

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


// CIniDlg ��Ϣ�������
BOOL CIniDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_cfgList = mfc::listview(&m_listIni);

	m_cfgList.add_column(L"����");
	m_cfgList.add_column(L"����");
	m_cfgList.add_column(L"�Ա�");
	m_cfgList.add_column(L"ѧ��");

	for (auto i = 0; i < 100; i++)
	{
		m_cfgList.insert(L"1", L"1", L"1", L"1");
	}
	return TRUE;  
}



