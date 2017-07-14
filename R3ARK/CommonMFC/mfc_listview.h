#pragma once
#include "stdafx.h"
#include "afxcmn.h"

namespace mfc
{
	using _listview_item = std::map<_tstring, _tstring>;
	using _listview_list = std::vector<_listview_item>;
	class listview
	{
	public:
		listview() :_listctrl(nullptr), _nColumnCount(0)
		{};
		~listview() {};
		listview(CListCtrl*_list) :_listctrl(_list) {
			_nColumnCount = 0;
			init_list();
		};
	public:
		//����XX,�Զ��������ȸ����ı���С*100
		void add_column(_tstring _columnName, UINT _width = 0)
		{
			_MapColumnToString[_nColumnCount] = _columnName;
			_MapStringToColumn[_columnName] = _nColumnCount;
			auto _min_width = _columnName.size() * 40;
			_listctrl->InsertColumn(_nColumnCount, _columnName.c_str(), LVCFMT_LEFT, max(_width, _min_width));
			_nColumnCount++;
		}
	public:
		static int CALLBACK list_compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
		{
			CListCtrl* pListCtrl = (CListCtrl*)lParamSort;
			CString strItem1 = pListCtrl->GetItemText(lParam1, 0);
			CString strItem2 = pListCtrl->GetItemText(lParam2, 0);


			LPCTSTR s1 = (LPCTSTR)strItem1;
			LPCTSTR s2 = (LPCTSTR)strItem2;
			
			auto i= _tcscmp(s2, s1);
			if (i>0)
			{
				return -1;
			}
			if (i<0)
			{
				return 1;
			}
			return 0;
		}
		//����item
		template<typename ...ARGS>
		void insert(ARGS &&...args)
		{
			auto i = 0;
			auto _item_index = 0;
			_tstring xitem[] = { std::forward<ARGS>(args)... };
			for (auto _p : xitem)
			{
				if (i == 0)
				{
					_item_index = _listctrl->InsertItem(0, xitem[i].c_str());
				}
				else
				{
					_listctrl->SetItemText(_item_index, i, xitem[i].c_str());
				}
				i++;
			}
			sort();
		}
	private:
		void sort()
		{
			_listctrl->SortItemsEx(list_compare, (DWORD_PTR)_listctrl);
		}
		int get_subitemIndex(_tstring item_name)
		{
			return _MapStringToColumn[item_name];
		}
	public:
		//��ȡѡ��
		void getSelected(_listview_list &_selected)
		{
			for (int i = 0; i < _listctrl->GetItemCount(); i++)
			{
				if (_listctrl->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED
					|| _listctrl->GetCheck(i))
				{
					_listview_item _item = {};
					for (auto subItem = 0; subItem < _nColumnCount; subItem++)
					{
						TCHAR szSubItem[MAX_PATH] = {};
						_listctrl->GetItemText(i, subItem, szSubItem, MAX_PATH);
						_item[_MapColumnToString[subItem]] = _tstring(szSubItem);
					}
					_selected.push_back(_item);
				}
			}
		}
		//ɾ��ѡ��
		void delSelected()
		{
			auto bfind = false;
			do {
				bfind = false;
				for (int i = 0; i < _listctrl->GetItemCount(); i++)
				{
					if (_listctrl->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED
						|| _listctrl->GetCheck(i))
					{
						_listctrl->DeleteItem(i);
						bfind = true;
						break;
					}
				}
			} while (bfind);
		}
		//����������һ��_listview_list
		void save(_listview_list &_saveList)
		{
			for (int i = 0; i < _listctrl->GetItemCount(); i++)
			{
				_listview_item _item = {};
				for (auto subItem = 0; subItem < _nColumnCount; subItem++)
				{
					TCHAR szSubItem[MAX_PATH] = {};
					_listctrl->GetItemText(i, subItem, szSubItem, MAX_PATH);
					_item[_MapColumnToString[subItem]] = _tstring(szSubItem);
				}
				_saveList.push_back(_item);
			}
		}
		//����_listview_list
		void load(_listview_list _listdata)
		{
			for (auto _item : _listdata)
			{
				auto Index = _listctrl->InsertItem(0, _T(""));
				for (auto _xitem : _item)
				{
					auto subItemName = _xitem.first;
					auto subItemText = _xitem.second;
					auto subIndex = get_subitemIndex(subItemName);
					_listctrl->SetItemText(Index, subIndex, subItemText.c_str());
				}
			}
			sort();
		}
		void get_column(std::vector<_tstring> &_list)
		{
			_list.resize(_nColumnCount);
			for (auto i =0;i<_nColumnCount;i++)
			{
				_list[i] = _MapColumnToString[i];
			}
		}
	public:
		//����˸
		void supend_redraw()
		{
			_listctrl->SetRedraw(FALSE);
		}
		void resume_redraw()
		{
			_listctrl->SetRedraw(TRUE);
			_listctrl->Invalidate();
			_listctrl->UpdateWindow();
		}
	public:
		void clear()
		{
			//ɾ������listview
			_listctrl->DeleteAllItems();
			while (_listctrl->DeleteColumn(0));
			_nColumnCount = 0;
			_MapColumnToString.clear();
			_MapStringToColumn.clear();
		}
	public:
		listview & operator =(const listview &_ll)
		{
			this->_listctrl = _ll._listctrl;
			this->_MapColumnToString = _ll._MapColumnToString;
			this->_MapStringToColumn = _ll._MapStringToColumn;
			this->_nColumnCount = _ll._nColumnCount;
			return *this;
		}
	private:
		CListCtrl *_listctrl;
		std::map<int, _tstring> _MapColumnToString;
		std::map<_tstring, int> _MapStringToColumn;
	private:
		void init_list()
		{
			if (_listctrl == nullptr)
			{
				RaiseException(0x80000003L, 0, 0, nullptr);
				return;
			}
			_nColumnCount = 0;
			_MapStringToColumn.clear();
			_MapColumnToString.clear();

			LONG lStyle;
			lStyle = GetWindowLong(_listctrl->m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
			lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
			lStyle |= LVS_REPORT; //����style
			lStyle |= LVS_SINGLESEL;//��ѡ
			SetWindowLong(_listctrl->m_hWnd, GWL_STYLE, lStyle);//����style
			
			DWORD dwStyle = _listctrl->GetExtendedStyle();
			dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
			dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
			dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
			_listctrl->SetExtendedStyle(dwStyle); //������չ���
		}
	private:
		int _nColumnCount;
	};
}
