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
	/// RegUnlockViewForm 摘要
	/// </summary>
	public ref class RegUnlockViewForm : public System::Windows::Forms::Form
	{
	public:
		RegUnlockViewForm(HANDLE hDriver,String^ str_)
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
		~RegUnlockViewForm()
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
			this->lv_->TabIndex = 2;
			this->lv_->UseCompatibleStateImageBehavior = false;
			this->lv_->View = System::Windows::Forms::View::Details;
			this->lv_->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &RegUnlockViewForm::lv__ColumnClick);
			this->lv_->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &RegUnlockViewForm::lv__MouseUp);
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
			this->cms_refresh->Click += gcnew System::EventHandler(this, &RegUnlockViewForm::cms_refresh_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(115, 6);
			// 
			// cms_unlock
			// 
			this->cms_unlock->Name = L"cms_unlock";
			this->cms_unlock->Size = System::Drawing::Size(152, 22);
			this->cms_unlock->Text = L"解锁";
			this->cms_unlock->Click += gcnew System::EventHandler(this, &RegUnlockViewForm::cms_unlock_Click);
			// 
			// cms_unlock2
			// 
			this->cms_unlock2->Name = L"cms_unlock2";
			this->cms_unlock2->Size = System::Drawing::Size(152, 22);
			this->cms_unlock2->Text = L"解锁全部";
			this->cms_unlock2->Click += gcnew System::EventHandler(this, &RegUnlockViewForm::cms_unlock2_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(115, 6);
			// 
			// cms_export
			// 
			this->cms_export->Name = L"cms_export";
			this->cms_export->Size = System::Drawing::Size(152, 22);
			this->cms_export->Text = L"导出";
			this->cms_export->Click += gcnew System::EventHandler(this, &RegUnlockViewForm::cms_export_Click);
			// 
			// RegUnlockViewForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(636, 315);
			this->Controls->Add(this->lv_);
			this->MaximizeBox = false;
			this->Name = L"RegUnlockViewForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"RegUnlockViewForm";
			this->Load += gcnew System::EventHandler(this, &RegUnlockViewForm::RegUnlockViewForm_Load);
			this->cms_->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	
		 HANDLE hDriver;
		 String^ str_;

		 //
private: System::Void RegUnlockViewForm_Load(System::Object^  sender, System::EventArgs^  e) {
			
		 }

		 //排序
private: System::Void lv__ColumnClick(System::Object^  sender, System::Windows::Forms::ColumnClickEventArgs^  e) {
		 }

		 //控制右键菜单
private: System::Void lv__MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

		 }

		 //刷新
private: System::Void cms_refresh_Click(System::Object^  sender, System::EventArgs^  e) {

		 }

		 //解锁
private: System::Void cms_unlock_Click(System::Object^  sender, System::EventArgs^  e) {

		 }

		 //解锁全部
private: System::Void cms_unlock2_Click(System::Object^  sender, System::EventArgs^  e) {

		 }

		 //导出
private: System::Void cms_export_Click(System::Object^  sender, System::EventArgs^  e) {

		 }
};
}
