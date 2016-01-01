#pragma once
#include "..\common\common.h"
#include "..\common\common_net.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// other_table_export_form 摘要
	/// </summary>
	public ref class other_table_export_form : public System::Windows::Forms::Form
	{
	public:
		other_table_export_form(String^ str_)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			this->str_ = str_;
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~other_table_export_form()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  tc_;
	protected: 

	protected: 
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::ListView^  lv_;

	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;
	private: System::Windows::Forms::ColumnHeader^  columnHeader4;
	private: System::Windows::Forms::TextBox^  txtAddrOfNameOrd;


	private: System::Windows::Forms::TextBox^  txtDllName;
	private: System::Windows::Forms::TextBox^  txtAddrOfNames;


	private: System::Windows::Forms::TextBox^  txtName;
	private: System::Windows::Forms::TextBox^  txtAddrOfFuncs;


	private: System::Windows::Forms::TextBox^  txtBase;


	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  txtNumOfNames;

	private: System::Windows::Forms::TextBox^  txtTimeDateStamp;

	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  txtNumOfFuncs;


	private: System::Windows::Forms::TextBox^  txtCharac;

	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label1;

	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->tc_ = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->txtAddrOfNameOrd = (gcnew System::Windows::Forms::TextBox());
			this->txtDllName = (gcnew System::Windows::Forms::TextBox());
			this->txtAddrOfNames = (gcnew System::Windows::Forms::TextBox());
			this->txtName = (gcnew System::Windows::Forms::TextBox());
			this->txtAddrOfFuncs = (gcnew System::Windows::Forms::TextBox());
			this->txtBase = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->txtNumOfNames = (gcnew System::Windows::Forms::TextBox());
			this->txtTimeDateStamp = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txtNumOfFuncs = (gcnew System::Windows::Forms::TextBox());
			this->txtCharac = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->lv_ = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
			this->tc_->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->SuspendLayout();
			// 
			// tc_
			// 
			this->tc_->Controls->Add(this->tabPage1);
			this->tc_->Controls->Add(this->tabPage2);
			this->tc_->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tc_->Location = System::Drawing::Point(0, 0);
			this->tc_->Name = L"tc_";
			this->tc_->SelectedIndex = 0;
			this->tc_->Size = System::Drawing::Size(597, 285);
			this->tc_->TabIndex = 0;
			this->tc_->SelectedIndexChanged += gcnew System::EventHandler(this, &other_table_export_form::tc__SelectedIndexChanged);
			// 
			// tabPage1
			// 
			this->tabPage1->BackColor = System::Drawing::SystemColors::Window;
			this->tabPage1->Controls->Add(this->txtAddrOfNameOrd);
			this->tabPage1->Controls->Add(this->txtDllName);
			this->tabPage1->Controls->Add(this->txtAddrOfNames);
			this->tabPage1->Controls->Add(this->txtName);
			this->tabPage1->Controls->Add(this->txtAddrOfFuncs);
			this->tabPage1->Controls->Add(this->txtBase);
			this->tabPage1->Controls->Add(this->label11);
			this->tabPage1->Controls->Add(this->label5);
			this->tabPage1->Controls->Add(this->label10);
			this->tabPage1->Controls->Add(this->label4);
			this->tabPage1->Controls->Add(this->label9);
			this->tabPage1->Controls->Add(this->label3);
			this->tabPage1->Controls->Add(this->txtNumOfNames);
			this->tabPage1->Controls->Add(this->txtTimeDateStamp);
			this->tabPage1->Controls->Add(this->label8);
			this->tabPage1->Controls->Add(this->label2);
			this->tabPage1->Controls->Add(this->txtNumOfFuncs);
			this->tabPage1->Controls->Add(this->txtCharac);
			this->tabPage1->Controls->Add(this->label7);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Location = System::Drawing::Point(4, 21);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(589, 260);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Basic Information";
			// 
			// txtAddrOfNameOrd
			// 
			this->txtAddrOfNameOrd->Location = System::Drawing::Point(431, 183);
			this->txtAddrOfNameOrd->Name = L"txtAddrOfNameOrd";
			this->txtAddrOfNameOrd->Size = System::Drawing::Size(129, 21);
			this->txtAddrOfNameOrd->TabIndex = 22;
			// 
			// txtDllName
			// 
			this->txtDllName->Location = System::Drawing::Point(136, 183);
			this->txtDllName->Name = L"txtDllName";
			this->txtDllName->Size = System::Drawing::Size(129, 21);
			this->txtDllName->TabIndex = 21;
			// 
			// txtAddrOfNames
			// 
			this->txtAddrOfNames->Location = System::Drawing::Point(431, 147);
			this->txtAddrOfNames->Name = L"txtAddrOfNames";
			this->txtAddrOfNames->Size = System::Drawing::Size(129, 21);
			this->txtAddrOfNames->TabIndex = 24;
			// 
			// txtName
			// 
			this->txtName->Location = System::Drawing::Point(136, 147);
			this->txtName->Name = L"txtName";
			this->txtName->Size = System::Drawing::Size(129, 21);
			this->txtName->TabIndex = 27;
			// 
			// txtAddrOfFuncs
			// 
			this->txtAddrOfFuncs->Location = System::Drawing::Point(431, 111);
			this->txtAddrOfFuncs->Name = L"txtAddrOfFuncs";
			this->txtAddrOfFuncs->Size = System::Drawing::Size(129, 21);
			this->txtAddrOfFuncs->TabIndex = 26;
			// 
			// txtBase
			// 
			this->txtBase->Location = System::Drawing::Point(136, 111);
			this->txtBase->Name = L"txtBase";
			this->txtBase->Size = System::Drawing::Size(129, 21);
			this->txtBase->TabIndex = 25;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(288, 187);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(137, 12);
			this->label11->TabIndex = 17;
			this->label11->Text = L"AddressOfNameOrdinals:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(29, 187);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(59, 12);
			this->label5->TabIndex = 20;
			this->label5->Text = L"Dll Name:";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(288, 151);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(95, 12);
			this->label10->TabIndex = 19;
			this->label10->Text = L"AddressOfNames:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(29, 151);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(35, 12);
			this->label4->TabIndex = 14;
			this->label4->Text = L"Name:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(288, 115);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(119, 12);
			this->label9->TabIndex = 15;
			this->label9->Text = L"AddressOfFunctions:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(29, 115);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(35, 12);
			this->label3->TabIndex = 16;
			this->label3->Text = L"Base:";
			// 
			// txtNumOfNames
			// 
			this->txtNumOfNames->Location = System::Drawing::Point(431, 76);
			this->txtNumOfNames->Name = L"txtNumOfNames";
			this->txtNumOfNames->Size = System::Drawing::Size(129, 21);
			this->txtNumOfNames->TabIndex = 12;
			// 
			// txtTimeDateStamp
			// 
			this->txtTimeDateStamp->Location = System::Drawing::Point(136, 76);
			this->txtTimeDateStamp->Name = L"txtTimeDateStamp";
			this->txtTimeDateStamp->Size = System::Drawing::Size(129, 21);
			this->txtTimeDateStamp->TabIndex = 13;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(288, 80);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(89, 12);
			this->label8->TabIndex = 10;
			this->label8->Text = L"NumberOfNames:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(29, 80);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(89, 12);
			this->label2->TabIndex = 11;
			this->label2->Text = L"TimeDateStamp:";
			// 
			// txtNumOfFuncs
			// 
			this->txtNumOfFuncs->Location = System::Drawing::Point(431, 44);
			this->txtNumOfFuncs->Name = L"txtNumOfFuncs";
			this->txtNumOfFuncs->Size = System::Drawing::Size(129, 21);
			this->txtNumOfFuncs->TabIndex = 8;
			// 
			// txtCharac
			// 
			this->txtCharac->Location = System::Drawing::Point(136, 44);
			this->txtCharac->Name = L"txtCharac";
			this->txtCharac->Size = System::Drawing::Size(129, 21);
			this->txtCharac->TabIndex = 9;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(288, 48);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(113, 12);
			this->label7->TabIndex = 6;
			this->label7->Text = L"NumberOfFunctions:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(29, 48);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(101, 12);
			this->label1->TabIndex = 7;
			this->label1->Text = L"Characteristics:";
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->lv_);
			this->tabPage2->Location = System::Drawing::Point(4, 21);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(589, 260);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Function Name";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// lv_
			// 
			this->lv_->AllowColumnReorder = true;
			this->lv_->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {this->columnHeader1, this->columnHeader2, 
				this->columnHeader3, this->columnHeader4});
			this->lv_->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lv_->FullRowSelect = true;
			this->lv_->Location = System::Drawing::Point(3, 3);
			this->lv_->MultiSelect = false;
			this->lv_->Name = L"lv_";
			this->lv_->ShowItemToolTips = true;
			this->lv_->Size = System::Drawing::Size(583, 254);
			this->lv_->TabIndex = 0;
			this->lv_->UseCompatibleStateImageBehavior = false;
			this->lv_->View = System::Windows::Forms::View::Details;
			this->lv_->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &other_table_export_form::lv__ColumnClick);
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"Ordinal";
			this->columnHeader1->Width = 84;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"Relative Virtual Address";
			this->columnHeader2->Width = 176;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"Virtual Address";
			this->columnHeader3->Width = 130;
			// 
			// columnHeader4
			// 
			this->columnHeader4->Text = L"Function Name";
			this->columnHeader4->Width = 155;
			// 
			// other_table_export_form
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(597, 285);
			this->Controls->Add(this->tc_);
			this->MaximizeBox = false;
			this->Name = L"other_table_export_form";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"导出表信息";
			this->Load += gcnew System::EventHandler(this, &other_table_export_form::other_table_export_form_Load);
			this->tc_->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	
		 //ListViewItem排序所需，记录当前列
		 static int lv_item_curColumn = -1; 

		 //表示降序还是升序
		 static bool lv_item_bAsc = false; 

		 //
		 static String^ str_ = String::Empty;

private: System::Void GetExportDirInfo(LPTSTR lpFilePath)
		 {
			 //打开目标文件
			 HANDLE hFile = CreateFile(lpFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,3,0,NULL);

			 //打开失败
			 if (!hFile)
			 {
				 return;
			 }

			 //文件映射
			 HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);

			 //映射失败
			 if (!hMap)
			 {
				 CloseHandle(hFile);
				 return;
			 }

			 //文件到内存的映射
			 PVOID lpBase = MapViewOfFile(hMap,FILE_MAP_READ,0,0,0);

			 //失败
			 if (!lpBase)
			 {
				 CloseHandle(hMap);
				 CloseHandle(hFile);
				 return;
			 }

			 //获得dos头部
			 PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)lpBase;
			 //获得nt头部
			 PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)lpBase + pImage_dos_header->e_lfanew);
			 //获得导出表
			 PIMAGE_EXPORT_DIRECTORY pImage_export_directory = (PIMAGE_EXPORT_DIRECTORY)ImageRvaToVa(pImage_nt_header,lpBase,pImage_nt_header->OptionalHeader.DataDirectory[0].VirtualAddress,NULL);

			 if (!pImage_export_directory)
			 {
				 CloseHandle(hMap);
				 CloseHandle(hFile);
				 UnmapViewOfFile(lpBase);
				 return;
			 }

			 txtCharac->Text = "0x" + (pImage_export_directory->Characteristics).ToString("X8");
			 txtTimeDateStamp->Text = "0x" + (pImage_export_directory->TimeDateStamp).ToString("X8");
			 txtBase->Text = "0x" + (pImage_export_directory->Base).ToString("X8");
			 txtName->Text = "0x" + (pImage_export_directory->Name).ToString("X8");
			 txtNumOfFuncs->Text = "0x" + (pImage_export_directory->NumberOfFunctions).ToString("X8");
			 txtNumOfNames->Text = "0x" + (pImage_export_directory->NumberOfNames).ToString("X8");
			 txtAddrOfFuncs->Text = "0x" + (pImage_export_directory->AddressOfFunctions).ToString("X8");
			 txtAddrOfNames->Text = "0x" + (pImage_export_directory->AddressOfNames).ToString("X8");
			 txtAddrOfNameOrd->Text = "0x" + (pImage_export_directory->AddressOfNameOrdinals).ToString("X8");
			 txtDllName->Text = gcnew String((LPSTR)ImageRvaToVa(pImage_nt_header,lpBase,pImage_export_directory->Name,NULL));

			 CloseHandle(hMap);
			 CloseHandle(hFile);
			 UnmapViewOfFile(lpBase);
		 }

		 //
private: System::Void GetExportTableInfo(LPTSTR lpFilePath)
		 {
			 //打开目标文件
			 HANDLE hFile = CreateFile(lpFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,3,0,NULL);

			 //打开失败
			 if (!hFile)
			 {
				 return;
			 }

			 //文件映射
			 HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);

			 //映射失败
			 if (!hMap)
			 {
				 CloseHandle(hFile);
				 return;
			 }

			 //文件到内存的映射
			 PVOID lpBase = MapViewOfFile(hMap,FILE_MAP_READ,0,0,0);

			 //失败
			 if (!lpBase)
			 {
				 CloseHandle(hMap);
				 CloseHandle(hFile);
				 return;
			 }

			 //获得dos头部
			 PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)lpBase;
			 //获得nt头部
			 PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)lpBase + pImage_dos_header->e_lfanew);
			 //获得导出表
			 PIMAGE_EXPORT_DIRECTORY pImage_export_directory = (PIMAGE_EXPORT_DIRECTORY)ImageRvaToVa(pImage_nt_header,lpBase,pImage_nt_header->OptionalHeader.DataDirectory[0].VirtualAddress,NULL);

			 if (!pImage_export_directory)
			 {
				 CloseHandle(hMap);
				 CloseHandle(hFile);
				 UnmapViewOfFile(lpBase);
				 return;
			 }

			 //
			 PWORD pwOrdinals = (PWORD)ImageRvaToVa(pImage_nt_header, lpBase, pImage_export_directory->AddressOfNameOrdinals,NULL);
			 //
			 PDWORD pdwRvas = (PDWORD)ImageRvaToVa(pImage_nt_header, lpBase, pImage_export_directory->AddressOfFunctions,NULL);
			 //
			 PDWORD pdwNames = (PDWORD)ImageRvaToVa(pImage_nt_header, lpBase, pImage_export_directory->AddressOfNames,NULL);

			 //
			 if(!pdwRvas)
			 {
				 return;
			 }

			 for (int i = 0;i < pImage_export_directory->NumberOfFunctions;i ++)
			 {
				 //
				 ListViewItem^ item_ = gcnew ListViewItem((i).ToString("X4"));

				 //
				 item_->SubItems->AddRange(gcnew array<String^>{
					 "0x" + (*pdwRvas).ToString("X8"),
						 "0x" + ((DWORD)ImageRvaToVa(pImage_nt_header, lpBase, pdwNames[i],NULL)).ToString("X8"),	 
						 gcnew String((LPSTR)ImageRvaToVa(pImage_nt_header,lpBase,pdwNames[i],NULL))
				 });

				 //
				 lv_->Items->Add(item_);					 

				 pdwRvas ++;
			 }

			 CloseHandle(hMap);
			 CloseHandle(hFile);
			 UnmapViewOfFile(lpBase);
		 }

		 //
private: System::Void other_table_export_form_Load(System::Object^  sender, System::EventArgs^  e) {
			 //
			 GetExportDirInfo(common_net::common_stringToHGlobalUni(str_));
		 }

		 //sort
private: System::Void lv__ColumnClick(System::Object^  sender, System::Windows::Forms::ColumnClickEventArgs^  e) {
			 if(e->Column == lv_item_curColumn)
			 {		 
				 if (lv_item_bAsc)
				 {
					 lv_item_bAsc = false;
				 }
				 else
				 {
					 lv_item_bAsc = true;
				 }

				 if(e->Column == 3)
				 {
					 //字符串排序
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,1,0);
				 }
				 else
				 {
					 //16进制
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,16);
				 }
			 }
			 else
			 {		
				 lv_item_bAsc = false;

				 if(e->Column == 3)
				 {
					 //字符串排序
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,1,0);
				 }
				 else
				 {
					 //16进制
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,16);
				 }

				 //记录当前
				 lv_item_curColumn = e->Column;
			 }
		 }

		 //
private: System::Void tc__SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (tc_->SelectedIndex == 0)
			 {
				 //
				 GetExportDirInfo(common_net::common_stringToHGlobalUni(str_));
			 }
			 else
			 {
				 //
				 GetExportTableInfo(common_net::common_stringToHGlobalUni(str_));
			 }
		 }

};
}
