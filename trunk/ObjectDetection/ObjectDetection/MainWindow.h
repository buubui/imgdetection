#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
#include <msclr\marshal_cppstd.h>
#include "detection.h"

namespace ObjectDetection {

	/// <summary>
	/// Summary for MainWindow
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  imgBox1;
	protected: 

	protected: 






	private: System::Windows::Forms::Button^  btnOpen;
	private: System::Windows::Forms::Button^  btnDetect;
	private: System::Windows::Forms::Button^  btnRefresh;


	private: System::Windows::Forms::PictureBox^  imgBox2;


	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::ComboBox^  cbBox1;
	private: System::Windows::Forms::ComboBox^  cbBox2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txtScaleStep;


	private: System::Windows::Forms::Label^  label2;

	private: System::Windows::Forms::Label^  label3;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  txtMinB;
	private: System::Windows::Forms::TextBox^  txtRadius;
	private: System::Windows::Forms::TextBox^  txtMinCSize;






	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->imgBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->btnOpen = (gcnew System::Windows::Forms::Button());
			this->btnDetect = (gcnew System::Windows::Forms::Button());
			this->btnRefresh = (gcnew System::Windows::Forms::Button());
			this->imgBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->cbBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->cbBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txtScaleStep = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->txtMinB = (gcnew System::Windows::Forms::TextBox());
			this->txtRadius = (gcnew System::Windows::Forms::TextBox());
			this->txtMinCSize = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imgBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imgBox2))->BeginInit();
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// imgBox1
			// 
			this->imgBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->imgBox1->Cursor = System::Windows::Forms::Cursors::Cross;
			this->imgBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->imgBox1->Location = System::Drawing::Point(3, 29);
			this->imgBox1->Name = L"imgBox1";
			this->imgBox1->Size = System::Drawing::Size(311, 294);
			this->imgBox1->TabIndex = 0;
			this->imgBox1->TabStop = false;
			// 
			// btnOpen
			// 
			this->btnOpen->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->btnOpen->Location = System::Drawing::Point(200, 368);
			this->btnOpen->Name = L"btnOpen";
			this->btnOpen->Size = System::Drawing::Size(75, 23);
			this->btnOpen->TabIndex = 2;
			this->btnOpen->Text = L"Open";
			this->btnOpen->UseVisualStyleBackColor = true;
			this->btnOpen->Click += gcnew System::EventHandler(this, &MainWindow::btnOpen_Click);
			// 
			// btnDetect
			// 
			this->btnDetect->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->btnDetect->Location = System::Drawing::Point(281, 368);
			this->btnDetect->Name = L"btnDetect";
			this->btnDetect->Size = System::Drawing::Size(75, 23);
			this->btnDetect->TabIndex = 3;
			this->btnDetect->Text = L"Detect";
			this->btnDetect->UseVisualStyleBackColor = true;
			this->btnDetect->Click += gcnew System::EventHandler(this, &MainWindow::btnDetect_Click);
			// 
			// btnRefresh
			// 
			this->btnRefresh->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->btnRefresh->Location = System::Drawing::Point(362, 368);
			this->btnRefresh->Name = L"btnRefresh";
			this->btnRefresh->Size = System::Drawing::Size(75, 23);
			this->btnRefresh->TabIndex = 4;
			this->btnRefresh->Text = L"Refresh";
			this->btnRefresh->UseVisualStyleBackColor = true;
			this->btnRefresh->Click += gcnew System::EventHandler(this, &MainWindow::btnRefresh_Click);
			// 
			// imgBox2
			// 
			this->imgBox2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->imgBox2->Cursor = System::Windows::Forms::Cursors::Cross;
			this->imgBox2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->imgBox2->Location = System::Drawing::Point(320, 29);
			this->imgBox2->Name = L"imgBox2";
			this->imgBox2->Size = System::Drawing::Size(311, 294);
			this->imgBox2->TabIndex = 0;
			this->imgBox2->TabStop = false;
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				50)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				50)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				20)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				20)));
			this->tableLayoutPanel1->Controls->Add(this->imgBox2, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->imgBox1, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->cbBox1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->cbBox2, 1, 0);
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 36);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 2;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.050847F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 91.94915F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(634, 326);
			this->tableLayoutPanel1->TabIndex = 6;
			// 
			// cbBox1
			// 
			this->cbBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->cbBox1->FormattingEnabled = true;
			this->cbBox1->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Origin Image", L"Detected Image", L"Detected & Mean-shifted image"});
			this->cbBox1->Location = System::Drawing::Point(3, 3);
			this->cbBox1->Name = L"cbBox1";
			this->cbBox1->Size = System::Drawing::Size(311, 21);
			this->cbBox1->TabIndex = 1;
			this->cbBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MainWindow::cbBox1_SelectedIndexChanged);
			// 
			// cbBox2
			// 
			this->cbBox2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->cbBox2->FormattingEnabled = true;
			this->cbBox2->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Origin Image", L"Detected Image", L"Detected & Mean-shifted image"});
			this->cbBox2->Location = System::Drawing::Point(320, 3);
			this->cbBox2->Name = L"cbBox2";
			this->cbBox2->Size = System::Drawing::Size(311, 21);
			this->cbBox2->TabIndex = 2;
			this->cbBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &MainWindow::cbBox2_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(60, 13);
			this->label1->TabIndex = 7;
			this->label1->Text = L"Scale step:";
			// 
			// txtScaleStep
			// 
			this->txtScaleStep->Location = System::Drawing::Point(79, 10);
			this->txtScaleStep->Name = L"txtScaleStep";
			this->txtScaleStep->Size = System::Drawing::Size(48, 20);
			this->txtScaleStep->TabIndex = 8;
			this->txtScaleStep->Text = L"1.2";
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(521, 15);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(59, 13);
			this->label2->TabIndex = 9;
			this->label2->Text = L"min C Size:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(193, 13);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(55, 13);
			this->label3->TabIndex = 11;
			this->label3->Text = L"min value:";
			// 
			// label4
			// 
			this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(328, 13);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(102, 13);
			this->label4->TabIndex = 13;
			this->label4->Text = L"Mean-shift\'s Radius:";
			// 
			// txtMinB
			// 
			this->txtMinB->Location = System::Drawing::Point(257, 10);
			this->txtMinB->Name = L"txtMinB";
			this->txtMinB->Size = System::Drawing::Size(42, 20);
			this->txtMinB->TabIndex = 14;
			this->txtMinB->Text = L"0.0";
			// 
			// txtRadius
			// 
			this->txtRadius->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->txtRadius->Location = System::Drawing::Point(436, 10);
			this->txtRadius->Name = L"txtRadius";
			this->txtRadius->Size = System::Drawing::Size(42, 20);
			this->txtRadius->TabIndex = 15;
			this->txtRadius->Text = L"50";
			// 
			// txtMinCSize
			// 
			this->txtMinCSize->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->txtMinCSize->Location = System::Drawing::Point(586, 10);
			this->txtMinCSize->Name = L"txtMinCSize";
			this->txtMinCSize->Size = System::Drawing::Size(48, 20);
			this->txtMinCSize->TabIndex = 16;
			this->txtMinCSize->Text = L"2";
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(646, 403);
			this->Controls->Add(this->txtMinCSize);
			this->Controls->Add(this->txtRadius);
			this->Controls->Add(this->txtMinB);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->txtScaleStep);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Controls->Add(this->btnRefresh);
			this->Controls->Add(this->btnDetect);
			this->Controls->Add(this->btnOpen);
			this->MinimumSize = System::Drawing::Size(654, 433);
			this->Name = L"MainWindow";
			this->Text = L"MainWindow";
			this->Load += gcnew System::EventHandler(this, &MainWindow::MainWindow_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imgBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imgBox2))->EndInit();
			this->tableLayoutPanel1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		System::String^ fname;
		System::String^ fext;
		System::String^ fpath;
		System::String^ ffullpath;
	private: System::Void btnOpen_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenFileDialog^ openFileDialog1=gcnew OpenFileDialog();
			//	 DialogResult res = openFileDialog1.ShowDialog();
				 if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 /*System::IO::StreamReader^ sr = gcnew
						 System::IO::StreamReader(openFileDialog1->FileName);
					 MessageBox::Show(sr->ReadToEnd());
					 sr->Close();*/
					 ffullpath = openFileDialog1->FileName;
					 fname = System::IO::Path::GetFileNameWithoutExtension(ffullpath); 
					 fext =System::IO::Path::GetExtension(ffullpath); 
					 fpath = System::IO::Path::GetDirectoryName(ffullpath);
					 /*if (fpath->get_Chars(fpath->Length-1)!='//')
					 {
						 fpath=fpath+"//";
					 }*/
					 
					 //msclr::interop::marshal_as<string>(
					// MessageBox::Show(gcnew System::String(fname.c_str()));
				//	 MessageBox::Show(gcnew System::String(path.c_str()));
				//	 MessageBox::Show(gcnew System::String(ext.c_str()));
				//	detection(path,fname,ext);	
					 cbBox1->SelectedIndex=0;
					 cbBox1_SelectedIndexChanged(sender,e);
					 cbBox2->SelectedIndex=0;
					 cbBox2_SelectedIndexChanged(sender,e);
					 this->Text=ffullpath;
				//	 System::String^ imgpath="output/"+fname+"_meanshift.png";
				//	this->imgBox1->Image =Image::FromFile(ffullpath);
					

				 }

			 }
private: System::Void cbBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 int selected=cbBox1->SelectedIndex;
			 System::String^ imgpath;
			 switch(selected){
				 case 0:imgpath=ffullpath;break;
				 case 1:imgpath="output/"+fname+"_multiscale.png";break;
				 case 2:imgpath="output/"+fname+"_meanshift.png";break;
			 }
			 System::IO::FileStream^ fs = gcnew System::IO::FileStream(imgpath, 
				 IO::FileMode::Open, IO::FileAccess::Read);
				 imgBox1->Image = System::Drawing::Image::FromStream(fs);
				 fs->Close();

			// this->imgBox1->Image =Image::FromFile(imgpath);
			 Image^ img = imgBox1->Image;
			 if((double)img->Height*img->Width> (double)imgBox1->Width*imgBox1->Height)
			 {
				this->imgBox1->SizeMode=  System::Windows::Forms::PictureBoxSizeMode::Zoom;
			 }else
				 this->imgBox1->SizeMode=  System::Windows::Forms::PictureBoxSizeMode::CenterImage;
		 }
private: System::Void MainWindow_Load(System::Object^  sender, System::EventArgs^  e) {
			 this->imgBox1->SizeMode=  System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			 this->imgBox2->SizeMode=  System::Windows::Forms::PictureBoxSizeMode::CenterImage;
		 }
private: System::Void cbBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 int selected=cbBox2->SelectedIndex;
			 System::String^ imgpath;
			 switch(selected){
				 case 0:imgpath=ffullpath;break;
				 case 1:imgpath="output/"+fname+"_multiscale.png";break;
				 case 2:imgpath="output/"+fname+"_meanshift.png";break;
			 }
			 System::IO::FileStream^ fs = gcnew System::IO::FileStream(imgpath, 
				 IO::FileMode::Open, IO::FileAccess::Read);
				 imgBox2->Image = System::Drawing::Image::FromStream(fs);
				 fs->Close();

			// this->imgBox2->Image =Image::FromFile(imgpath);
			 Image^ img = imgBox2->Image;
			 if((double)img->Height*img->Width> (double)imgBox2->Width*imgBox2->Height)
			 {
				 this->imgBox2->SizeMode=  System::Windows::Forms::PictureBoxSizeMode::Zoom;
			 }else
				 this->imgBox2->SizeMode=  System::Windows::Forms::PictureBoxSizeMode::CenterImage;
		 }
private: System::Void btnDetect_Click(System::Object^  sender, System::EventArgs^  e) {
			 std::string path=msclr::interop::marshal_as<std::string>(fpath->ToString());
			 std::string name=msclr::interop::marshal_as<std::string>(fname->ToString());
			 std::string ext=msclr::interop::marshal_as<std::string>(fext->ToString());
			 float scaleStep = atof(msclr::interop::marshal_as<std::string>(txtScaleStep->Text).c_str());
			 float minV = atof(msclr::interop::marshal_as<std::string>(txtMinB->Text).c_str());
			 float radius= atof(msclr::interop::marshal_as<std::string>(txtRadius->Text).c_str());
			 int minCsize= atoi(msclr::interop::marshal_as<std::string>(txtMinCSize->Text).c_str());
			 detection(path,name,ext,scaleStep,minV,radius,minCsize,false);	
			 cbBox1->SelectedIndex=1;
			 cbBox2->SelectedIndex=2;
			cbBox1_SelectedIndexChanged(sender,e);
			 cbBox2_SelectedIndexChanged(sender,e);
			// MessageBox::Show("Detection is finished!");

		 }
private: System::Void btnRefresh_Click(System::Object^  sender, System::EventArgs^  e) {
			 /*if(cbBox1->SelectedIndex==2){
				 
				 cbBox1->SelectedIndex=0;
				 cbBox1_SelectedIndexChanged(sender,e);
			 }
			 if(cbBox2->SelectedIndex==2){
				 cbBox2->SelectedIndex=0;
				 cbBox2_SelectedIndexChanged(sender,e);
			 }*/
			 std::string path=msclr::interop::marshal_as<std::string>(fpath->ToString());
			 std::string name=msclr::interop::marshal_as<std::string>(fname->ToString());
			 std::string ext=msclr::interop::marshal_as<std::string>(fext->ToString());
			 float scaleStep = atof(msclr::interop::marshal_as<std::string>(txtScaleStep->Text).c_str());
			 float minV = atof(msclr::interop::marshal_as<std::string>(txtMinB->Text).c_str());
			 float radius= atof(msclr::interop::marshal_as<std::string>(txtRadius->Text).c_str());
			 int minCsize= atoi(msclr::interop::marshal_as<std::string>(txtMinCSize->Text).c_str());
			 printf("%f %f %f %d",scaleStep,minV,radius,minCsize);
			 detection(path,name,ext,scaleStep,minV,radius,minCsize,true);	
			 cbBox1->SelectedIndex=1;
			 cbBox2->SelectedIndex=2;
			 cbBox1_SelectedIndexChanged(sender,e);
			 cbBox2_SelectedIndexChanged(sender,e);
		 }
};
}
