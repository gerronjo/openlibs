#ifndef _LISTVIEWSORTER_
#define _LISTVIEWSORTER_
#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Text;

namespace Plugin {

	/// <summary>
	/// config 摘要
	/// </summary>
	public ref class ListViewItemComparer : public System::Collections::IComparer
	{
	private: 

		int Column;

		//排序类型
		int Type;

		//表示升序或者降序
		bool bAsc;

		int param1;

	public: 

		//默认构造函数
		ListViewItemComparer(void)
		{
			//
			//TODO: 在此处添加构造函数代码
			//
		}

		//Column字段
		//bAsc表示升序还是降序
		//Type类型，0表示数字排序，1表示字符串排序
		//param1数字比较时要用的，代表进制
		ListViewItemComparer(int Column,bool bAsc, int Type,int param1)
		{
			this->Column = Column;
			this->Type = Type;
			this->bAsc = bAsc;
			this->param1 = param1;
		}

		//重新实现
		virtual int Compare(Object^ x, Object^ y)
		{
			//数字排序
			switch(Type)
			{
			case 0:
				{
					int item1;
					int item2;

					try
					{
						if (bAsc)
						{
							switch(param1)
							{
								//十进制比较
							case 10:
								item1 = Convert::ToInt32((dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text);
								item2 = Convert::ToInt32((dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text);
								break;

								//16进制比较
							case 16:
								item1 = Convert::ToInt32((dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text,16);
								item2 = Convert::ToInt32((dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text,16);
								break;
							}			
						}
						else
						{
							switch(param1)
							{
								//十进制比较
							case 10:
								item1 = Convert::ToInt32((dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text);
								item2 = Convert::ToInt32((dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text);
								break;

								//16进制比较
							case 16:
								item1 = Convert::ToInt32((dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text,16);
								item2 = Convert::ToInt32((dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text,16);
								break;
							}
						}
					}
					catch(Exception^ ex)
					{
						//转换出错
						return 0;
					}

					if(item1 >= item2)
					{
						return 0;
					}
					else
					{			
						return 1;
					}
				}
				break;
			case 1:
				{
					if (!bAsc)
					{
						try
						{
							return String::Compare( 
								(dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text,
								(dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text 
								);
						}
						catch(Exception^ ex)
						{
							return 0;
						}

					}
					else
					{
						try
						{
							return String::Compare( 
								(dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text,
								(dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text 
								);
						}
						catch(Exception^ ex)
						{
							return 1;
						}			
					}	
				}
				break;
			case 2:
				{
					if (bAsc)
					{
						try
						{
							return DateTime::Compare( 
								DateTime::Parse((dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text),
								DateTime::Parse((dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text) 
								);
						}
						catch(Exception^ ex)
						{
							return 0;
						}

					}
					else
					{
						try
						{
							return DateTime::Compare( 
								DateTime::Parse((dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text),
								DateTime::Parse((dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text) 
								);
						}
						catch(Exception^ ex)
						{
							return 1;
						}			
					}
				}
				break;
			}

			//返回失败
			return 0;
		}

	};
}

#endif //_LISTVIEWSORTER_