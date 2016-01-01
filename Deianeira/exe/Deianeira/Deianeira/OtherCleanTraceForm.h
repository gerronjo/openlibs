#pragma once
#include "..\common\common.h"
#include "..\common\common_net.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// OtherCleanTraceForm 摘要
	/// </summary>
	public ref class OtherCleanTraceForm : public System::Windows::Forms::Form
	{
	public:
		OtherCleanTraceForm(String^ str_,List<String^>^ lst_)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			this->str_ = str_;
			this->lst_ = lst_;
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~OtherCleanTraceForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListView^  lv_;
	protected: 

	protected: 
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ContextMenuStrip^  cms_;



	private: System::Windows::Forms::ToolStripMenuItem^  cms_delete;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_export;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_find;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_explorer;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_showProperty;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;

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
			this->cms_ = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->cms_find = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_explorer = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cms_showProperty = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_delete = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_export = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cms_->SuspendLayout();
			this->SuspendLayout();
			// 
			// lv_
			// 
			this->lv_->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(1) {this->columnHeader1});
			this->lv_->ContextMenuStrip = this->cms_;
			this->lv_->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lv_->FullRowSelect = true;
			this->lv_->Location = System::Drawing::Point(0, 0);
			this->lv_->MultiSelect = false;
			this->lv_->Name = L"lv_";
			this->lv_->ShowItemToolTips = true;
			this->lv_->Size = System::Drawing::Size(611, 388);
			this->lv_->TabIndex = 0;
			this->lv_->UseCompatibleStateImageBehavior = false;
			this->lv_->View = System::Windows::Forms::View::Details;
			this->lv_->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &OtherCleanTraceForm::lv__ColumnClick);
			this->lv_->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &OtherCleanTraceForm::lv__MouseUp);
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"文件位置";
			this->columnHeader1->Width = 592;
			// 
			// cms_
			// 
			this->cms_->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {this->cms_find, this->toolStripSeparator3, 
				this->cms_explorer, this->cms_showProperty, this->toolStripSeparator1, this->cms_delete, this->toolStripSeparator2, this->cms_export});
			this->cms_->Name = L"cms_";
			this->cms_->Size = System::Drawing::Size(167, 154);
			// 
			// cms_find
			// 
			this->cms_find->Name = L"cms_find";
			this->cms_find->Size = System::Drawing::Size(166, 22);
			this->cms_find->Text = L"查找";
			this->cms_find->Click += gcnew System::EventHandler(this, &OtherCleanTraceForm::cms_find_Click);
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(163, 6);
			// 
			// cms_explorer
			// 
			this->cms_explorer->Name = L"cms_explorer";
			this->cms_explorer->Size = System::Drawing::Size(166, 22);
			this->cms_explorer->Text = L"定位";
			this->cms_explorer->Click += gcnew System::EventHandler(this, &OtherCleanTraceForm::cms_explorer_Click);
			// 
			// cms_showProperty
			// 
			this->cms_showProperty->Name = L"cms_showProperty";
			this->cms_showProperty->Size = System::Drawing::Size(166, 22);
			this->cms_showProperty->Text = L"查看文件属性";
			this->cms_showProperty->Click += gcnew System::EventHandler(this, &OtherCleanTraceForm::cms_showProperty_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(163, 6);
			// 
			// cms_delete
			// 
			this->cms_delete->Name = L"cms_delete";
			this->cms_delete->Size = System::Drawing::Size(166, 22);
			this->cms_delete->Text = L"删除时排除此记录";
			this->cms_delete->Click += gcnew System::EventHandler(this, &OtherCleanTraceForm::cms_delete_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(163, 6);
			// 
			// cms_export
			// 
			this->cms_export->Name = L"cms_export";
			this->cms_export->Size = System::Drawing::Size(166, 22);
			this->cms_export->Text = L"导出";
			this->cms_export->Click += gcnew System::EventHandler(this, &OtherCleanTraceForm::cms_export_Click);
			// 
			// OtherCleanTraceForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(611, 388);
			this->Controls->Add(this->lv_);
			this->MaximizeBox = false;
			this->Name = L"OtherCleanTraceForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"OtherCleanTraceForm";
			this->Load += gcnew System::EventHandler(this, &OtherCleanTraceForm::OtherCleanTraceForm_Load);
			this->cms_->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

		 String^ str_;
		 List<String^>^ lst_;

		 //ListViewItem排序所需，记录当前列
		 static int lv_item_curColumn = -1; 
		 //表示降序还是升序
		 static bool lv_item_bAsc = false; 

		 //
private: System::Void OtherCleanTraceForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 //text
			 this->Text = "查看详细--->>>[" + str_ + "]";

			 for each(String^ str_ in lst_)
			 {
				 ListViewItem^ item_ = gcnew ListViewItem(str_);
				 lv_->Items->Add(item_);
			 }
		 }

		 //控制右键菜单的显示
private: System::Void lv__MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if(lv_->Items->Count == 0)
			 {
				 cms_delete->Enabled = false;
			 }
			 else if(lv_->SelectedItems->Count == 0)
			 {
				 cms_delete->Enabled = false;
			 }
			 else
			 {
				 cms_delete->Enabled = true;
			 }
		 }

		 //
private: System::Void cms_delete_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 if(lst_->Remove(item_->SubItems[0]->Text))
			 {
				 item_->Remove();
			 }
		 }

		 //
private: System::Void cms_export_Click(System::Object^  sender, System::EventArgs^  e) {
			 common_net::common_getListViewData(lv_);
		 }

		 //排序
private: System::Void lv__ColumnClick(System::Object^  sender, System::Windows::Forms::ColumnClickEventArgs^  e) {
			 //
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

				 //字符串排序
				 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,1,0);			 			 
			 }
			 else
			 {		
				 lv_item_bAsc = false;

				 //字符串排序
				 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,1,0);	

				 //记录当前
				 lv_item_curColumn = e->Column;
			 }
		 }

		 //查找
private: System::Void cms_find_Click(System::Object^  sender, System::EventArgs^  e) {

		 }

		 //定位
private: System::Void cms_explorer_Click(System::Object^  sender, System::EventArgs^  e) {

		 }

		 //查看文件属性
private: System::Void cms_showProperty_Click(System::Object^  sender, System::EventArgs^  e) {

		 }
};
}
