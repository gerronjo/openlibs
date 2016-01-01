#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Deianeira {

	/// <summary>
	/// CatchForm 摘要
	///
	/// 警告: 如果更改此类的名称，则需要更改
	///          与此类所依赖的所有 .resx 文件关联的托管资源编译器工具的
	///          “资源文件名”属性。否则，
	///          设计器将不能与此窗体的关联
	///          本地化资源正确交互。
	/// </summary>
	public ref class CatchForm : public System::Windows::Forms::Form
	{
	public:
		CatchForm(void)
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
		~CatchForm()
		{
			if (components)
			{
				delete components;
			}
		}

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
			this->SuspendLayout();
			// 
			// CatchForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 266);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = L"CatchForm";
			this->ShowInTaskbar = false;
			this->Text = L"CatchForm";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &CatchForm::CatchForm_Load);
			this->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &CatchForm::CatchForm_MouseClick);
			this->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &CatchForm::CatchForm_MouseDoubleClick);
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &CatchForm::CatchForm_MouseDown);
			this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &CatchForm::CatchForm_MouseMove);
			this->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &CatchForm::CatchForm_MouseUp);
			this->ResumeLayout(false);

		}
#pragma endregion

		//记录鼠标按下坐标，用来确定绘图起点
		static Point DownPoint = Point::Empty;
		//用来表示是否截图完成
		static bool CatchFinished = false;    
		//表示截图是否开始
		static bool CatchStart = false;
		//用来保存原始图像
		static Bitmap^ originBmp;             
		//用来保存截图的矩形
		static System::Drawing::Rectangle CatchRect;          
		//保存被截取的图片，主要是传到主窗体
	    public: Bitmap^ CatchedBmp;

			//窗体加载事件
private: System::Void CatchForm_Load(System::Object^  sender, System::EventArgs^  e) {


				 //BackgroundImage为全屏图片，我们另用变量来保存全屏图片
				 //BackgroundImage在主窗体调用的时候，已经为其指定了
				 originBmp = gcnew Bitmap(this->BackgroundImage);
			 }

			 //单击
private: System::Void CatchForm_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 //如果你按下的事鼠标右键
				 if (e->Button == System::Windows::Forms::MouseButtons::Right)
				 {
					 //返回DialogResult
					 this->DialogResult = System::Windows::Forms::DialogResult::None;
					 //关闭此窗体
					 this->Close();
				 }
			 }

			 //单击事件
private: System::Void CatchForm_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 //如果你按下的事鼠标左键
				 if (e->Button == System::Windows::Forms::MouseButtons::Left)
				 {
					 //如果捕捉没有开始
					 if (!CatchStart)
					 {					
						 CatchStart = true;
						 //保存鼠标按下坐标
						 DownPoint = Point(e->X, e->Y);
					 }
				 }
			 }

			 //鼠标移动
private: System::Void CatchForm_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 //如果捕捉开始
				 if (CatchStart)
				 {
					 //新建一个图片对象，并让它与原始图片相同
					 Bitmap^ destBmp = (Bitmap^)originBmp->Clone();
					 //获取鼠标的坐标
					 Point newPoint = Point(DownPoint.X, DownPoint.Y);

					 Graphics^ g = Graphics::FromImage(destBmp);
					 //在刚才新建的图片上新建一个画板
					 Pen^ p = gcnew Pen(Color::Blue, 1);
					 //获取矩形的长和宽
					 int width = Math::Abs(e->X - DownPoint.X);
					 int height = Math::Abs(e->Y - DownPoint.Y);
					 if (e->X < DownPoint.X)
					 {
						 newPoint.X = e->X;
					 }
					 if (e->Y < DownPoint.Y)
					 {
						 newPoint.Y = e->Y;
					 }
					 //保存矩形
					 CatchRect = System::Drawing::Rectangle(newPoint,System::Drawing::Size(width, height));
					 //将矩形画在这个画板上
					 g->DrawRectangle(p, CatchRect);
					 //重新新建一个Graphics类
					 Graphics^ g1 = this->CreateGraphics();
					 ////在整个全屏窗体上新建画板
					 //g1 = this->CreateGraphics();
					 //将刚才所画的图片画到这个窗体上
					 g1->DrawImage(destBmp,Point(0, 0));
				 }
			 }

		 //鼠标弹上
private: System::Void CatchForm_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 if (e->Button == System::Windows::Forms::MouseButtons::Left)
				 {
					 if (CatchStart)
					 {
						 CatchStart = false;
						 CatchFinished = true;
					 }
				 }
			 }

         //鼠标双击
private: System::Void CatchForm_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 if (e->Button == System::Windows::Forms::MouseButtons::Left && CatchFinished)
				 {
					 if (CatchRect.Contains(Point(e->X, e->Y)))
					 {
						 //新建一个于矩形等大的空白图片
						 CatchedBmp = gcnew Bitmap(CatchRect.Width, CatchRect.Height);
						 Graphics^ g = Graphics::FromImage(CatchedBmp);
						 //从originBmp复制图片到CatchedBmp
						 g->DrawImage(originBmp, System::Drawing::Rectangle(0, 0, CatchRect.Width, CatchRect.Height), CatchRect, GraphicsUnit::Pixel);
						 this->DialogResult = System::Windows::Forms::DialogResult::OK;
						 this->Close();
					 }
				 }
			 }
	};
}
