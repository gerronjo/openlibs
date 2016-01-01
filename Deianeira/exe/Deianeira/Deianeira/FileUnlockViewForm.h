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
	/// FileUnlockViewForm 摘要
	/// </summary>
	public ref class FileUnlockViewForm : public System::Windows::Forms::Form
	{
	public:
		FileUnlockViewForm(HANDLE hDriver,String^ str_)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			this->hDriver = hDriver;
			this->str_ = str_;
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~FileUnlockViewForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListView^  lv_;
	protected: 
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;
	private: System::Windows::Forms::ColumnHeader^  columnHeader4;
	private: System::Windows::Forms::ContextMenuStrip^  cms_;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_refresh;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_unlock;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_unlock2;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_export;

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->lv_ = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
			this->cms_ = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->cms_refresh = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_unlock = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cms_unlock2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_export = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cms_->SuspendLayout();
			this->SuspendLayout();
			// 
			// lv_
			// 
			this->lv_->AllowColumnReorder = true;
			this->lv_->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {this->columnHeader1, this->columnHeader2, 
				this->columnHeader3, this->columnHeader4});
			this->lv_->ContextMenuStrip = this->cms_;
			this->lv_->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lv_->FullRowSelect = true;
			this->lv_->Location = System::Drawing::Point(0, 0);
			this->lv_->MultiSelect = false;
			this->lv_->Name = L"lv_";
			this->lv_->ShowItemToolTips = true;
			this->lv_->Size = System::Drawing::Size(636, 315);
			this->lv_->TabIndex = 1;
			this->lv_->UseCompatibleStateImageBehavior = false;
			this->lv_->View = System::Windows::Forms::View::Details;
			this->lv_->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &FileUnlockViewForm::lv__ColumnClick);
			this->lv_->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &FileUnlockViewForm::lv__MouseUp);
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"映像名称";
			this->columnHeader1->Width = 121;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"进程ID";
			this->columnHeader2->Width = 73;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"句柄";
			this->columnHeader3->Width = 101;
			// 
			// columnHeader4
			// 
			this->columnHeader4->Text = L"文件路径";
			this->columnHeader4->Width = 319;
			// 
			// cms_
			// 
			this->cms_->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->cms_refresh, this->toolStripSeparator1, 
				this->cms_unlock, this->cms_unlock2, this->toolStripSeparator2, this->cms_export});
			this->cms_->Name = L"cms_";
			this->cms_->Size = System::Drawing::Size(153, 126);
			// 
			// cms_refresh
			// 
			this->cms_refresh->Name = L"cms_refresh";
			this->cms_refresh->Size = System::Drawing::Size(152, 22);
			this->cms_refresh->Text = L"刷新";
			this->cms_refresh->Click += gcnew System::EventHandler(this, &FileUnlockViewForm::cms_refresh_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(149, 6);
			// 
			// cms_unlock
			// 
			this->cms_unlock->Name = L"cms_unlock";
			this->cms_unlock->Size = System::Drawing::Size(152, 22);
			this->cms_unlock->Text = L"解锁";
			this->cms_unlock->Click += gcnew System::EventHandler(this, &FileUnlockViewForm::cms_unlock_Click);
			// 
			// cms_unlock2
			// 
			this->cms_unlock2->Name = L"cms_unlock2";
			this->cms_unlock2->Size = System::Drawing::Size(152, 22);
			this->cms_unlock2->Text = L"解锁全部";
			this->cms_unlock2->Click += gcnew System::EventHandler(this, &FileUnlockViewForm::cms_unlock2_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(149, 6);
			// 
			// cms_export
			// 
			this->cms_export->Name = L"cms_export";
			this->cms_export->Size = System::Drawing::Size(152, 22);
			this->cms_export->Text = L"导出";
			this->cms_export->Click += gcnew System::EventHandler(this, &FileUnlockViewForm::cms_export_Click);
			// 
			// FileUnlockViewForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(636, 315);
			this->Controls->Add(this->lv_);
			this->MaximizeBox = false;
			this->Name = L"FileUnlockViewForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"FileUnlockViewForm";
			this->Load += gcnew System::EventHandler(this, &FileUnlockViewForm::FileUnlockViewForm_Load);
			this->cms_->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	
		HANDLE hDriver;
		String^ str_;

		//ListViewItem排序所需，记录当前列
		static int lv_item_curColumn = -1; 
		//表示降序还是升序
		static bool lv_item_bAsc = false;

		//
private: int lv__show() 
		 { 
			 BOOL bRet=FALSE;
			 DWORD dwRet=0;
			 ULONG ulSize=0;
			 PFILE_HANDLE_INFO p;

			 LPTSTR lpFilePath = common_net::common_stringToHGlobalUni(str_);

			 //获得返回的大小
			 bRet = DeviceIoControl(hDriver, IOCTL_FILE_GETHANDLEINFOSIZE, lpFilePath, wcslen(lpFilePath)*sizeof(WCHAR), &ulSize, sizeof(ULONG), &dwRet, NULL);
			 if (!bRet)
			 {
				 MessageBox::Show("获取大小失败！");
				 return 0;
			 }

			 //分配内存
			 p = new FILE_HANDLE_INFO[ulSize];

			 //获得返回的数据
			 bRet = DeviceIoControl(hDriver, IOCTL_FILE_GETHANDLEINFO, NULL, 0, p, ulSize*sizeof(FILE_HANDLE_INFO), &dwRet, NULL);
			 if (!bRet)
			 {
				 MessageBox::Show("获取信息失败！");
				 return 0;
			 }

			 for (UINT32 i =0;i<ulSize;i++)
			 {
				 ListViewItem^ item_ = gcnew ListViewItem(common_net::common_getProcessName(p[i].ulPID));
				 item_->SubItems->AddRange(gcnew array<String^>{ p[i].ulPID.ToString(),"0x" + p[i].ulHandle.ToString("X8"),gcnew String(p[i].wstrHandleName)});
				 lv_->Items->Add(item_);
			 }

			 return ulSize;
		 }

		 //窗体加载
private: System::Void FileUnlockViewForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 int ulCount = lv__show();
			 this->Text = "文件句柄(" + ulCount + ")" + "[" + str_ + "]";
		 }

		 //排序
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

				 if(e->Column == 2)
				 {
					 //16进制
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,16);
				 }
				 else if(e->Column == 1)
				 {
					 //数子排序
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,10);
				 }
				 else
				 {
					 //字符串排序
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,1,0);
				 }
			 }
			 else
			 {		
				 lv_item_bAsc = false;

				 if(e->Column == 2)
				 {
					 //16进制
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,16);
				 }
				 else if(e->Column == 1)
				 {
					 //数子排序
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,10);
				 }
				 else
				 {
					 //字符串排序
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,1,0);
				 }

				 //记录当前
				 lv_item_curColumn = e->Column;
			 }
		 }

		 //控制右键菜单
private: System::Void lv__MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if(lv_->Items->Count == 0)
			 {
				 cms_unlock->Enabled = false;
				 cms_unlock2->Enabled = false;
			 }
			 else if(lv_->SelectedItems->Count == 0)
			 {
				 cms_unlock->Enabled = false;
				 cms_unlock2->Enabled = false;
			 }
			 else
			 {
				 cms_unlock->Enabled = true;
				 cms_unlock2->Enabled = true;
			 }
		 }

		 //刷新
private: System::Void cms_refresh_Click(System::Object^  sender, System::EventArgs^  e) {
			 lv_->Items->Clear();
			 FileUnlockViewForm_Load(sender,e);
		 }

		 //解锁
private: System::Void cms_unlock_Click(System::Object^  sender, System::EventArgs^  e) {

		 }

		 //解锁全部
private: System::Void cms_unlock2_Click(System::Object^  sender, System::EventArgs^  e) {

		 }

		 //导出
private: System::Void cms_export_Click(System::Object^  sender, System::EventArgs^  e) {
			 common_net::common_getListViewData(lv_);
		 }
};
}
