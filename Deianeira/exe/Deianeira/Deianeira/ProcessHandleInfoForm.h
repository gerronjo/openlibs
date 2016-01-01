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
	/// ProcessHandleInfoForm 摘要
	/// </summary>
	public ref class ProcessHandleInfoForm : public System::Windows::Forms::Form
	{
	public:
		ProcessHandleInfoForm(HANDLE hDriver,ULONG ulPID,String^ str_)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			this->hDriver = hDriver;
			this->ulPID = ulPID;
			this->str_ = str_;
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~ProcessHandleInfoForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListView^  lv_;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;
	private: System::Windows::Forms::ColumnHeader^  columnHeader4;
	private: System::Windows::Forms::ColumnHeader^  columnHeader5;
	private: System::Windows::Forms::ColumnHeader^  columnHeader6;
	private: System::Windows::Forms::ContextMenuStrip^  cms_;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_refresh;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_close;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_close2;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_export;

	private: System::ComponentModel::IContainer^  components;
	protected: 

	protected: 

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
			this->columnHeader5 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader6 = (gcnew System::Windows::Forms::ColumnHeader());
			this->cms_ = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->cms_refresh = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_close = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cms_close2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_export = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cms_->SuspendLayout();
			this->SuspendLayout();
			// 
			// lv_
			// 
			this->lv_->AllowColumnReorder = true;
			this->lv_->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(6) {this->columnHeader1, this->columnHeader2, 
				this->columnHeader3, this->columnHeader4, this->columnHeader5, this->columnHeader6});
			this->lv_->ContextMenuStrip = this->cms_;
			this->lv_->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lv_->FullRowSelect = true;
			this->lv_->Location = System::Drawing::Point(0, 0);
			this->lv_->MultiSelect = false;
			this->lv_->Name = L"lv_";
			this->lv_->ShowItemToolTips = true;
			this->lv_->Size = System::Drawing::Size(636, 315);
			this->lv_->TabIndex = 0;
			this->lv_->UseCompatibleStateImageBehavior = false;
			this->lv_->View = System::Windows::Forms::View::Details;
			this->lv_->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &ProcessHandleInfoForm::lv__ColumnClick);
			this->lv_->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &ProcessHandleInfoForm::lv__MouseUp);
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"句柄类型";
			this->columnHeader1->Width = 94;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"句柄名";
			this->columnHeader2->Width = 212;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"句柄";
			this->columnHeader3->Width = 80;
			// 
			// columnHeader4
			// 
			this->columnHeader4->Text = L"句柄对象";
			this->columnHeader4->Width = 79;
			// 
			// columnHeader5
			// 
			this->columnHeader5->Text = L"句柄类型索引";
			this->columnHeader5->Width = 90;
			// 
			// columnHeader6
			// 
			this->columnHeader6->Text = L"引用计数";
			this->columnHeader6->Width = 67;
			// 
			// cms_
			// 
			this->cms_->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->cms_refresh, this->toolStripSeparator1, 
				this->cms_close, this->cms_close2, this->toolStripSeparator2, this->cms_export});
			this->cms_->Name = L"cms_";
			this->cms_->Size = System::Drawing::Size(153, 126);
			// 
			// cms_refresh
			// 
			this->cms_refresh->Name = L"cms_refresh";
			this->cms_refresh->Size = System::Drawing::Size(152, 22);
			this->cms_refresh->Text = L"刷新";
			this->cms_refresh->Click += gcnew System::EventHandler(this, &ProcessHandleInfoForm::cms_refresh_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(149, 6);
			// 
			// cms_close
			// 
			this->cms_close->Name = L"cms_close";
			this->cms_close->Size = System::Drawing::Size(152, 22);
			this->cms_close->Text = L"关闭";
			this->cms_close->Click += gcnew System::EventHandler(this, &ProcessHandleInfoForm::cms_close_Click);
			// 
			// cms_close2
			// 
			this->cms_close2->Name = L"cms_close2";
			this->cms_close2->Size = System::Drawing::Size(152, 22);
			this->cms_close2->Text = L"强制关闭";
			this->cms_close2->Click += gcnew System::EventHandler(this, &ProcessHandleInfoForm::cms_close2_Click);
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
			this->cms_export->Click += gcnew System::EventHandler(this, &ProcessHandleInfoForm::cms_export_Click);
			// 
			// ProcessHandleInfoForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(636, 315);
			this->Controls->Add(this->lv_);
			this->MaximizeBox = false;
			this->Name = L"ProcessHandleInfoForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"ProcessHandleInfoForm";
			this->Load += gcnew System::EventHandler(this, &ProcessHandleInfoForm::ProcessHandleInfoForm_Load);
			this->cms_->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

		 HANDLE hDriver;
		 ULONG ulPID;
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
			 ULONG pid;
			 PPROCESS_HANDLE_INFO p;

			 pid = ulPID;

			 //获得返回的大小
			 bRet = DeviceIoControl(hDriver, IOCTL_PROCESS_GETHANDLEINFOSIZE, &pid, sizeof(ULONG), &ulSize, sizeof(ULONG), &dwRet, NULL);
			 if (!bRet)
			 {
				 MessageBox::Show("获取大小失败！");
				 return 0;
			 }

			 //分配内存
			 p = new PROCESS_HANDLE_INFO[ulSize];

			 //获得返回的数据
			 bRet = DeviceIoControl(hDriver, IOCTL_PROCESS_GETHANDLEINFO, NULL, 0, p, ulSize*sizeof(PROCESS_HANDLE_INFO), &dwRet, NULL);
			 if (!bRet)
			 {
				 MessageBox::Show("获取信息失败！");
				 return 0;
			 }

			 for (UINT32 i =0;i<ulSize;i++)
			 {
				 ListViewItem^ item_ = gcnew ListViewItem(gcnew String(p[i].wstrHandleTypeName));
				 item_->SubItems->AddRange(gcnew array<String^>{gcnew String(p[i].wstrHandleName),"0x" + p[i].ulHandle.ToString("X8"),
				 "0x" + p[i].ulObject.ToString("X8"),p[i].ulTypeIndex.ToString(),p[i].ulReferenceCount.ToString()});
				 lv_->Items->Add(item_);
			 }

			 return ulSize;
		 }

		 //窗体加载
private: System::Void ProcessHandleInfoForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 int ulCount = lv__show();
			 this->Text = "[" + str_ + "]进程句柄(" + ulCount + ")"; 
		 }

		 //控制右键菜单
private: System::Void lv__MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if(lv_->Items->Count == 0)
			 {
				 cms_close->Enabled = false;
				 cms_close2->Enabled = false;
			 }
			 else if(lv_->SelectedItems->Count == 0)
			 {
				 cms_close->Enabled = false;
				 cms_close2->Enabled = false;
			 }
			 else
			 {
				 cms_close->Enabled = true;
				 cms_close2->Enabled = true;
			 }
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

				 if(e->Column == 2 || e->Column == 3)
				 {
					 //16进制
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,16);
				 }
				 else if(e->Column == 4 || e->Column == 5)
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

				 if(e->Column == 2 || e->Column == 3)
				 {
					 //16进制
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,16);
				 }
				 else if(e->Column == 4 || e->Column == 5)
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

		 //刷新
private: System::Void cms_refresh_Click(System::Object^  sender, System::EventArgs^  e) {
			 lv_->Items->Clear();
			 ProcessHandleInfoForm_Load(sender,e);
		 }

		 //关闭
private: System::Void cms_close_Click(System::Object^  sender, System::EventArgs^  e) {

		 }

		 //强制关闭
private: System::Void cms_close2_Click(System::Object^  sender, System::EventArgs^  e) {

		 }

		 //导出
private: System::Void cms_export_Click(System::Object^  sender, System::EventArgs^  e) {
			 common_net::common_getListViewData(lv_);
		 }
};
}
