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
	/// OtherCleanSetForm 摘要
	/// </summary>
	public ref class OtherCleanSetForm : public System::Windows::Forms::Form
	{
	public:
		OtherCleanSetForm(void)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~OtherCleanSetForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	protected: 
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::SplitContainer^  splitContainer3;
	private: System::Windows::Forms::ListView^  lv_;

	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::Button^  btnQuit;

	private: System::Windows::Forms::Button^  btnOK;

	private: System::Windows::Forms::Label^  lbl_recommended;

	private: System::Windows::Forms::Label^  lbl_noall;

	private: System::Windows::Forms::Label^  lbl_all;
	private: System::Windows::Forms::Button^  btnBrowser;
	private: System::Windows::Forms::TextBox^  txt_;
	private: System::Windows::Forms::FolderBrowserDialog^  FBD;
	private: System::Windows::Forms::ContextMenuStrip^  cms_;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_delete;

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
			System::Windows::Forms::ListViewItem^  listViewItem1 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.\?\?$", 
				L"*.\?\?$ 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem2 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.\?\?~", 
				L"*.\?\?~ 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem3 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.\?~\?", 
				L"*.\?~\? 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem4 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.~*", 
				L"*.~* 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem5 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.^*", 
				L"*.^* 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem6 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.temp", 
				L"*.temp 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem7 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.tmp", 
				L"*.tmp 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem8 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.~mp", 
				L"*.~mp 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem9 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*._mp", 
				L"*._mp 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem10 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.$*", 
				L"*.$* 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem11 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.___", 
				L"*.___ 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem12 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.$$$", 
				L"*.$$$ 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem13 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.@@@", 
				L"*.@@@ 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem14 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.---", 
				L"*.--- 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem15 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.ftg", 
				L"*.ftg 全文本搜索组文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem16 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.%%%", 
				L"*.%%% 临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem17 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"mscreate.dir", 
				L"mscreate.dir 微软安装临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem18 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"imagedb.aid", 
				L"imagedb.aid 微软安装临时文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem19 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.fts", 
				L"*.fts 全文本搜索引文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem20 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.gid", 
				L"*.gid 临时帮助文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem21 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"errorlog.txt", 
				L"errorlog.txt Windows安装日志文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem22 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.syd", 
				L"*.syd 程序运行时产生的备份文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem23 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"comsetup.txt", 
				L"comsetup.txt Windows安装日志文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem24 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"directx.txt", 
				L"directx.txt Windows安装日志文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem25 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"mpcsetup.txt", 
				L"mpcsetup.txt Windows安装日志文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem26 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"netsetup.txt", 
				L"netsetup.txt Windows安装日志文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem27 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.prv", 
				L"*.prv 错误日志文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem28 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.fnd", 
				L"*.fnd 保存搜索结果的"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem29 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.err", 
				L"*.err 编译错误文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem30 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.wbk", 
				L"*.wbk Word备份文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem31 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.xlk", 
				L"*.xlk Excel备份文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem32 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.cdr_", 
				L"*.CoreDraw备份文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem33 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.diz", 
				L"*.diz 共享软件描述文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem34 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.chk", 
				L"*.chk 磁盘检查数据文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem35 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.mch", 
				L"*.mch Macromedia Xtra 缓存文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem36 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"Thumbs.db", 
				L"Thumbs.db 图片预览缩略图文件"}, -1));
			System::Windows::Forms::ListViewItem^  listViewItem37 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(2) {L"*.dmp", 
				L"*.dmp 系统错误内存转储文件"}, -1));
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->splitContainer3 = (gcnew System::Windows::Forms::SplitContainer());
			this->lv_ = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->cms_ = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->cms_delete = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->btnBrowser = (gcnew System::Windows::Forms::Button());
			this->txt_ = (gcnew System::Windows::Forms::TextBox());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->lbl_recommended = (gcnew System::Windows::Forms::Label());
			this->lbl_noall = (gcnew System::Windows::Forms::Label());
			this->lbl_all = (gcnew System::Windows::Forms::Label());
			this->FBD = (gcnew System::Windows::Forms::FolderBrowserDialog());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer2))->BeginInit();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer3))->BeginInit();
			this->splitContainer3->Panel1->SuspendLayout();
			this->splitContainer3->Panel2->SuspendLayout();
			this->splitContainer3->SuspendLayout();
			this->cms_->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->splitContainer2);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->btnQuit);
			this->splitContainer1->Panel2->Controls->Add(this->btnOK);
			this->splitContainer1->Panel2->Controls->Add(this->lbl_recommended);
			this->splitContainer1->Panel2->Controls->Add(this->lbl_noall);
			this->splitContainer1->Panel2->Controls->Add(this->lbl_all);
			this->splitContainer1->Size = System::Drawing::Size(377, 420);
			this->splitContainer1->SplitterDistance = 379;
			this->splitContainer1->TabIndex = 0;
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Name = L"splitContainer2";
			this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->label1);
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->splitContainer3);
			this->splitContainer2->Size = System::Drawing::Size(377, 379);
			this->splitContainer2->SplitterDistance = 42;
			this->splitContainer2->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(30, 14);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(245, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"未被选中或未自定义的文件类型将不会扫描。";
			// 
			// splitContainer3
			// 
			this->splitContainer3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer3->Location = System::Drawing::Point(0, 0);
			this->splitContainer3->Name = L"splitContainer3";
			this->splitContainer3->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer3.Panel1
			// 
			this->splitContainer3->Panel1->Controls->Add(this->lv_);
			// 
			// splitContainer3.Panel2
			// 
			this->splitContainer3->Panel2->Controls->Add(this->btnBrowser);
			this->splitContainer3->Panel2->Controls->Add(this->txt_);
			this->splitContainer3->Size = System::Drawing::Size(377, 333);
			this->splitContainer3->SplitterDistance = 302;
			this->splitContainer3->TabIndex = 0;
			// 
			// lv_
			// 
			this->lv_->CheckBoxes = true;
			this->lv_->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->columnHeader1, this->columnHeader2});
			this->lv_->ContextMenuStrip = this->cms_;
			this->lv_->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lv_->FullRowSelect = true;
			listViewItem1->StateImageIndex = 0;
			listViewItem2->StateImageIndex = 0;
			listViewItem3->StateImageIndex = 0;
			listViewItem4->StateImageIndex = 0;
			listViewItem5->StateImageIndex = 0;
			listViewItem6->StateImageIndex = 0;
			listViewItem7->StateImageIndex = 0;
			listViewItem8->StateImageIndex = 0;
			listViewItem9->StateImageIndex = 0;
			listViewItem10->StateImageIndex = 0;
			listViewItem11->StateImageIndex = 0;
			listViewItem12->StateImageIndex = 0;
			listViewItem13->StateImageIndex = 0;
			listViewItem14->StateImageIndex = 0;
			listViewItem15->StateImageIndex = 0;
			listViewItem16->StateImageIndex = 0;
			listViewItem17->StateImageIndex = 0;
			listViewItem18->StateImageIndex = 0;
			listViewItem19->StateImageIndex = 0;
			listViewItem20->StateImageIndex = 0;
			listViewItem21->StateImageIndex = 0;
			listViewItem22->StateImageIndex = 0;
			listViewItem23->StateImageIndex = 0;
			listViewItem24->StateImageIndex = 0;
			listViewItem25->StateImageIndex = 0;
			listViewItem26->StateImageIndex = 0;
			listViewItem27->StateImageIndex = 0;
			listViewItem28->StateImageIndex = 0;
			listViewItem29->StateImageIndex = 0;
			listViewItem30->StateImageIndex = 0;
			listViewItem31->StateImageIndex = 0;
			listViewItem32->StateImageIndex = 0;
			listViewItem33->StateImageIndex = 0;
			listViewItem34->StateImageIndex = 0;
			listViewItem35->StateImageIndex = 0;
			listViewItem36->StateImageIndex = 0;
			listViewItem37->StateImageIndex = 0;
			this->lv_->Items->AddRange(gcnew cli::array< System::Windows::Forms::ListViewItem^  >(37) {listViewItem1, listViewItem2, listViewItem3, 
				listViewItem4, listViewItem5, listViewItem6, listViewItem7, listViewItem8, listViewItem9, listViewItem10, listViewItem11, listViewItem12, 
				listViewItem13, listViewItem14, listViewItem15, listViewItem16, listViewItem17, listViewItem18, listViewItem19, listViewItem20, 
				listViewItem21, listViewItem22, listViewItem23, listViewItem24, listViewItem25, listViewItem26, listViewItem27, listViewItem28, 
				listViewItem29, listViewItem30, listViewItem31, listViewItem32, listViewItem33, listViewItem34, listViewItem35, listViewItem36, 
				listViewItem37});
			this->lv_->Location = System::Drawing::Point(0, 0);
			this->lv_->MultiSelect = false;
			this->lv_->Name = L"lv_";
			this->lv_->Size = System::Drawing::Size(377, 302);
			this->lv_->TabIndex = 0;
			this->lv_->UseCompatibleStateImageBehavior = false;
			this->lv_->View = System::Windows::Forms::View::Details;
			this->lv_->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &OtherCleanSetForm::lv__ColumnClick);
			this->lv_->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &OtherCleanSetForm::lv__MouseUp);
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"类型";
			this->columnHeader1->Width = 100;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"说明";
			this->columnHeader2->Width = 278;
			// 
			// cms_
			// 
			this->cms_->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->cms_delete});
			this->cms_->Name = L"cms_";
			this->cms_->Size = System::Drawing::Size(95, 26);
			// 
			// cms_delete
			// 
			this->cms_delete->Name = L"cms_delete";
			this->cms_delete->Size = System::Drawing::Size(94, 22);
			this->cms_delete->Text = L"删除";
			this->cms_delete->Click += gcnew System::EventHandler(this, &OtherCleanSetForm::cms_delete_Click);
			// 
			// btnBrowser
			// 
			this->btnBrowser->Location = System::Drawing::Point(330, 1);
			this->btnBrowser->Name = L"btnBrowser";
			this->btnBrowser->Size = System::Drawing::Size(43, 23);
			this->btnBrowser->TabIndex = 3;
			this->btnBrowser->Text = L"...";
			this->btnBrowser->UseVisualStyleBackColor = true;
			this->btnBrowser->Click += gcnew System::EventHandler(this, &OtherCleanSetForm::btnBrowser_Click);
			// 
			// txt_
			// 
			this->txt_->Location = System::Drawing::Point(3, 2);
			this->txt_->Name = L"txt_";
			this->txt_->Size = System::Drawing::Size(321, 21);
			this->txt_->TabIndex = 0;
			this->txt_->Text = L"目标文件夹(回车可添加自定义类型(格式:类型:说明))";
			this->txt_->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &OtherCleanSetForm::txt__KeyPress);
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(285, 6);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(67, 23);
			this->btnQuit->TabIndex = 2;
			this->btnQuit->Text = L"取消";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &OtherCleanSetForm::btnQuit_Click);
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(194, 6);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(67, 23);
			this->btnOK->TabIndex = 2;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &OtherCleanSetForm::btnOK_Click);
			// 
			// lbl_recommended
			// 
			this->lbl_recommended->AutoSize = true;
			this->lbl_recommended->Location = System::Drawing::Point(108, 11);
			this->lbl_recommended->Name = L"lbl_recommended";
			this->lbl_recommended->Size = System::Drawing::Size(53, 12);
			this->lbl_recommended->TabIndex = 1;
			this->lbl_recommended->Text = L"推荐选项";
			this->lbl_recommended->Click += gcnew System::EventHandler(this, &OtherCleanSetForm::lbl_recommended_Click);
			// 
			// lbl_noall
			// 
			this->lbl_noall->AutoSize = true;
			this->lbl_noall->Location = System::Drawing::Point(52, 11);
			this->lbl_noall->Name = L"lbl_noall";
			this->lbl_noall->Size = System::Drawing::Size(41, 12);
			this->lbl_noall->TabIndex = 1;
			this->lbl_noall->Text = L"全不选";
			this->lbl_noall->Click += gcnew System::EventHandler(this, &OtherCleanSetForm::lbl_noall_Click);
			// 
			// lbl_all
			// 
			this->lbl_all->AutoSize = true;
			this->lbl_all->Location = System::Drawing::Point(8, 11);
			this->lbl_all->Name = L"lbl_all";
			this->lbl_all->Size = System::Drawing::Size(29, 12);
			this->lbl_all->TabIndex = 0;
			this->lbl_all->Text = L"全选";
			this->lbl_all->Click += gcnew System::EventHandler(this, &OtherCleanSetForm::lbl_all_Click);
			// 
			// OtherCleanSetForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(377, 420);
			this->Controls->Add(this->splitContainer1);
			this->MaximizeBox = false;
			this->Name = L"OtherCleanSetForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"清理垃圾设置";
			this->Load += gcnew System::EventHandler(this, &OtherCleanSetForm::OtherCleanSetForm_Load);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->Panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel1->PerformLayout();
			this->splitContainer2->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer2))->EndInit();
			this->splitContainer2->ResumeLayout(false);
			this->splitContainer3->Panel1->ResumeLayout(false);
			this->splitContainer3->Panel2->ResumeLayout(false);
			this->splitContainer3->Panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer3))->EndInit();
			this->splitContainer3->ResumeLayout(false);
			this->cms_->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

		 
		 //ListViewItem排序所需，记录当前列
		 static int lv_item_curColumn = -1; 
		 //表示降序还是升序
		 static bool lv_item_bAsc = false; 

private: System::Void save()
		 {
			 String^ str_ = String::Empty;

			 for each(ListViewItem^ item_ in lv_->Items)
			 {
				 if (item_->Checked)
				 {
					 str_ = str_ + item_->Text + ";";
				 }
			 }

			 //保存信息到配置文件
			 common_net::config_writePrivateProfileString("main","tv_other_clean_3",str_,Application::StartupPath+"\\Deianeira.config");

			 //记录目标文件夹信息
			 if(Directory::Exists(txt_->Text))
			 {
				 //保存信息到配置文件
				 common_net::config_writePrivateProfileString("main","tv_other_clean_2",txt_->Text,Application::StartupPath+"\\Deianeira.config");
			 }
		 }

		//加载
private: System::Void OtherCleanSetForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 //加载自定义的扫描类型信息
			 String^ str_tv_other_clean_1 = common_net::config_getPrivateProfileString("main","tv_other_clean_1",Application::StartupPath+"\\Deianeira.config");
			 array<String^>^ strs_ = str_tv_other_clean_1->Split(gcnew array<String^>{";"},StringSplitOptions::RemoveEmptyEntries);
			 for (int i = 0;i<strs_->Length;i++)
			 {
				 ListViewItem^ item_ = gcnew ListViewItem(strs_[i]->Split(':')[0]);
				 item_->SubItems->Add(strs_[i]->Split(':')[1]);
				 lv_->Items->Add(item_);
			 }	

			 //得到上次扫描类型信息
			 String^ str_tv_other_clean_3 = common_net::config_getPrivateProfileString("main","tv_other_clean_3",Application::StartupPath+"\\Deianeira.config");

			 //如果为空
			 if (String::IsNullOrEmpty(str_tv_other_clean_3->ToString()) || String::IsNullOrWhiteSpace(str_tv_other_clean_3->ToString()))
			 {
				 //就选中推荐选项
				 lbl_recommended_Click(sender,e);
			 } 
			 else
			 {
				 array<String^>^ strs_ = str_tv_other_clean_3->ToString()->Split(gcnew array<String^>{";"},StringSplitOptions::RemoveEmptyEntries);
				 for (int i = 0;i<strs_->Length;i++)
				 {
					 for each(ListViewItem^ item_ in lv_->Items)
					 {
						 if (item_->Text == strs_[i])
						 {
							 item_->Checked = true;
							 break;
						 }		 
					 }
				 }
			 }
		 }

		 //全选
private: System::Void lbl_all_Click(System::Object^  sender, System::EventArgs^  e) {
			for each(ListViewItem^ item_ in lv_->Items)
			{
				item_->Checked = true;
			}
		 }

		 //全不选
private: System::Void lbl_noall_Click(System::Object^  sender, System::EventArgs^  e) {
			 for each(ListViewItem^ item_ in lv_->Items)
			 {
				 item_->Checked = false;
			 }
		 }

		 //推荐选项
private: System::Void lbl_recommended_Click(System::Object^  sender, System::EventArgs^  e) {
			 bool isChecked = true;

			 String^ str_tv_other_clean_1 = common_net::config_getPrivateProfileString("main","tv_other_clean_1",Application::StartupPath+"\\Deianeira.config");
			 array<String^>^ strs_ = str_tv_other_clean_1->Split(gcnew array<String^>{";"},StringSplitOptions::RemoveEmptyEntries);

			 for each(ListViewItem^ item_ in lv_->Items)
			 {
				 isChecked = true;

				 for each(String^ str_ in gcnew array<String^>{"*.syd","*.wbk","*.xlk","*.cdr_","*.chk","*.dmp"})
				 {
					 if (item_->Text == str_)
					 {
						 isChecked = false;
						 break;
					 }
				 }

				 if (isChecked)
				 {
					 for each(String^ str_ in strs_)
					 {
						 if (item_->Text == str_->Split(':')[0])
						 {
							 isChecked = false;
							 break;
						 }
					 }
				 }	 

				 if (isChecked)
				 {
					 item_->Checked = true;
				 }
				 else
				 {
					 item_->Checked = false;
				 }
			 }
		 }

		 //确定
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 //保留本次扫描类型信息
			 save();

			 this->Close();
		 }

		 //取消
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

		 //浏览
private: System::Void btnBrowser_Click(System::Object^  sender, System::EventArgs^  e) {
			 FBD->ShowNewFolderButton = true;
			 if (FBD->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			 {
				 txt_->Text = FBD->SelectedPath;
			 }
		 }

		 //
private: System::Void txt__KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
			 if (e->KeyChar == (Char)13)
			 {
				 if (String::IsNullOrEmpty(txt_->Text))
				 {
					 MessageBox::Show("不能为空！");
					 txt_->Focus();
					 return;
				 }
				 //这里用正则判定目标字符串格式
				 if (!Regex::IsMatch(txt_->Text,".*[^ ].*:.*[^ ].*"))
				 {
					 MessageBox::Show("格式不匹配！");
					 txt_->Focus();
					 return;
				 }

				 for each(ListViewItem^ item_ in lv_->Items)
				 {
					 if (item_->Text == txt_->Text->Split(':')[0])
					 {
						 MessageBox::Show("类型重复！");
						 txt_->Focus();
						 return;
					 }
				 }

				 ListViewItem^ item_ = gcnew ListViewItem(txt_->Text->Split(':')[0]);
				 item_->SubItems->Add(txt_->Text->Split(':')[1]);
				 lv_->Items->Add(item_);
				 lv_->Focus();
				 item_->Selected = true;
				 item_->EnsureVisible();

				 String^ str_tv_other_clean_1 = common_net::config_getPrivateProfileString("main","tv_other_clean_1",Application::StartupPath+"\\Deianeira.config");
				 str_tv_other_clean_1 = str_tv_other_clean_1 + txt_->Text + ";";

				 //保存信息到配置文件
				 common_net::config_writePrivateProfileString("main","tv_other_clean_1",str_tv_other_clean_1,Application::StartupPath+"\\Deianeira.config");
			 }	 
		 }

		 //删除
private: System::Void cms_delete_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];

			 String^ str_tv_other_clean_1 = common_net::config_getPrivateProfileString("main","tv_other_clean_1",Application::StartupPath+"\\Deianeira.config");
			 array<String^>^ strs_ = str_tv_other_clean_1->Split(gcnew array<String^>{";"},StringSplitOptions::RemoveEmptyEntries);

			 //清空
			 str_tv_other_clean_1 = String::Empty;

			 for each(String^ str_ in strs_)
			 {
				 if (item_->Text == str_->Split(':')[0])
				 {
					 continue;
				 }

				 str_tv_other_clean_1 = str_tv_other_clean_1 + str_;
			 }

			 //重新写入配置文件
			 common_net::config_writePrivateProfileString("main","tv_other_clean_1",str_tv_other_clean_1,Application::StartupPath+"\\Deianeira.config");

			 item_->Remove();
		 }

		 //控制右键菜单的显示
private: System::Void lv__MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (lv_->Items->Count == 0)
			 {
				 cms_delete->Enabled = false;
			 }
			 else if (lv_->SelectedItems->Count == 0)
			 {
				 cms_delete->Enabled = false;
			 }
			 else if (lv_->SelectedItems[0]->Index < 37)
			 {
				 cms_delete->Enabled = false;
			 }
			 else
			 {
				 cms_delete->Enabled = true;
			 }
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

};
}
