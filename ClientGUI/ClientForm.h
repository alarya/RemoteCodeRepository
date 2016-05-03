#pragma once
/////////////////////////////////////////////////////////////////////////////
// ClientForm.h - Code Behind file for the GUI App                         //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio Enterprise 2015                  //
// Platform:    ThinkPad L440, Core i7-4712MQ                              //
// Author:      Alok Arya                                                  //
//              (315) 728-0088, alarya@syr.edu                             //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* Defines the GUI elements that appear
* Wires up the event handlers for the GUI elements
*
*
* Required Files:
* ===============
* ClientForm.cpp, ClientForm.resx
* Channel.h, Channel.cpp
* HttpMessage.h, HttpMessage.cpp
* XMLResponseBodyGenerator.h, XMLResponseBodyGenerator.cpp
* 
*
* Build Command:
* ==============
* Build Command: devenv Project4.sln /rebuild debug /project ClientGUI/ClientGUI.vcxproj
*
* Maintenance History:
* ====================
*
* ver 1.0 : 2 May 2016
* - first release
*/

#include "../Channel/Channel.h"
#include "../HttpMessage/HttpMessage.h"
#include "../XMLResponseBodyGenerator/XMLResponseBodyGenerator.h"
#include <msclr\marshal_cppstd.h>
#include <vector>
#include <functional>
#include <iostream>

namespace ClientGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace msclr::interop;

	/// <summary>
	/// Summary for ClientForm
	/// </summary>
	public ref class ClientForm : public System::Windows::Forms::Form
	{

		ISender* pSender;
		IReceiver* pReceiver;
		IChannel* pChannel;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::ListBox^  listBox2;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::ListBox^  listBox3;
	private: System::Windows::Forms::Label^  label11;
			 

	public:
		ClientForm(void)
		{
			InitializeComponent();

			//set up channel
			ObjectFactory* pObjectFactory = new ObjectFactory();
			pSender = pObjectFactory->createSender();
			pReceiver = pObjectFactory->createReceiver();
			pChannel = pObjectFactory->createChannel(pSender, pReceiver);
			pChannel->start();
			delete pObjectFactory;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ClientForm()
		{
			if (components)
			{
				delete components;
			}
			delete pSender;
			delete pReceiver;
			delete pChannel;
		}
	
	private: System::Windows::Forms::TabControl^  tabControl1;
	
	protected:
	
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Label^  label4;

	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label2;
	private: System::IO::FileSystemWatcher^  fileSystemWatcher1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  textBox2;

	private: System::Windows::Forms::OpenFileDialog^  openFileDialog2;

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
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->listBox2 = (gcnew System::Windows::Forms::ListBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->listBox3 = (gcnew System::Windows::Forms::ListBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->fileSystemWatcher1 = (gcnew System::IO::FileSystemWatcher());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openFileDialog2 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->panel1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->panel2->SuspendLayout();
			this->tabPage3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->fileSystemWatcher1))->BeginInit();
			this->SuspendLayout();
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Location = System::Drawing::Point(12, 12);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(678, 426);
			this->tabControl1->TabIndex = 0;
			this->tabControl1->SelectedIndexChanged += gcnew System::EventHandler(this, &ClientForm::tabControl1_SelectedIndexChanged);
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->panel1);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(670, 400);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"tabPage1";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->label7);
			this->panel1->Controls->Add(this->button2);
			this->panel1->Controls->Add(this->label6);
			this->panel1->Controls->Add(this->button4);
			this->panel1->Controls->Add(this->button3);
			this->panel1->Controls->Add(this->button1);
			this->panel1->Controls->Add(this->textBox3);
			this->panel1->Controls->Add(this->label5);
			this->panel1->Controls->Add(this->textBox2);
			this->panel1->Controls->Add(this->label4);
			this->panel1->Controls->Add(this->textBox1);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->listBox1);
			this->panel1->Location = System::Drawing::Point(6, 6);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(658, 388);
			this->panel1->TabIndex = 0;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(16, 364);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(445, 13);
			this->label3->TabIndex = 15;
			this->label3->Text = L"                                                                                 "
				L"                                                                 ";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(22, 339);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(207, 13);
			this->label7->TabIndex = 14;
			this->label7->Text = L"Select Depedencies of Check-In Package";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(111, 53);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(118, 23);
			this->button2->TabIndex = 13;
			this->button2->Text = L"Get Packages";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &ClientForm::button2_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(298, 25);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(99, 24);
			this->label6->TabIndex = 12;
			this->label6->Text = L"CHECK-IN";
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(562, 203);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(75, 23);
			this->button4->TabIndex = 11;
			this->button4->Text = L"Open";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &ClientForm::button4_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(562, 147);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 10;
			this->button3->Text = L"Open";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &ClientForm::button3_Click);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(481, 329);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 9;
			this->button1->Text = L"Check-In";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ClientForm::button1_Click);
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(347, 203);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(209, 20);
			this->textBox3->TabIndex = 8;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(255, 213);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(51, 13);
			this->label5->TabIndex = 7;
			this->label5->Text = L"hFilePath";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(347, 150);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(209, 20);
			this->textBox2->TabIndex = 6;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(255, 157);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(64, 13);
			this->label4->TabIndex = 5;
			this->label4->Text = L"CppFilePath";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(347, 90);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(120, 20);
			this->textBox1->TabIndex = 3;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(255, 97);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(81, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Package Name";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(16, 58);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(89, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Server Packages";
			this->label1->Click += gcnew System::EventHandler(this, &ClientForm::label1_Click);
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(19, 90);
			this->listBox1->Name = L"listBox1";
			this->listBox1->SelectionMode = System::Windows::Forms::SelectionMode::MultiSimple;
			this->listBox1->Size = System::Drawing::Size(210, 238);
			this->listBox1->TabIndex = 0;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->panel2);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(670, 400);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"tabPage2";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->label10);
			this->panel2->Controls->Add(this->button6);
			this->panel2->Controls->Add(this->button5);
			this->panel2->Controls->Add(this->label9);
			this->panel2->Controls->Add(this->listBox2);
			this->panel2->Controls->Add(this->label8);
			this->panel2->Location = System::Drawing::Point(6, 6);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(658, 388);
			this->panel2->TabIndex = 0;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(78, 355);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(328, 13);
			this->label10->TabIndex = 6;
			this->label10->Text = L"                                                                                 "
				L"                          ";
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(379, 179);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(125, 66);
			this->button6->TabIndex = 5;
			this->button6->Text = L"Close Check-In";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &ClientForm::button6_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(168, 85);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(86, 23);
			this->button5->TabIndex = 4;
			this->button5->Text = L"Get Packages";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &ClientForm::button5_Click);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(78, 90);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(84, 13);
			this->label9->TabIndex = 3;
			this->label9->Text = L"Open Packages";
			// 
			// listBox2
			// 
			this->listBox2->FormattingEnabled = true;
			this->listBox2->Location = System::Drawing::Point(81, 118);
			this->listBox2->Name = L"listBox2";
			this->listBox2->Size = System::Drawing::Size(173, 212);
			this->listBox2->TabIndex = 2;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label8->Location = System::Drawing::Point(229, 26);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(191, 24);
			this->label8->TabIndex = 1;
			this->label8->Text = L"Close Open Check-In";
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->label13);
			this->tabPage3->Controls->Add(this->button8);
			this->tabPage3->Controls->Add(this->checkBox1);
			this->tabPage3->Controls->Add(this->button7);
			this->tabPage3->Controls->Add(this->label12);
			this->tabPage3->Controls->Add(this->listBox3);
			this->tabPage3->Controls->Add(this->label11);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(670, 400);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"tabPage3";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(289, 309);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(346, 13);
			this->label13->TabIndex = 6;
			this->label13->Text = L"                                                                                 "
				L"                                ";
			// 
			// button8
			// 
			this->button8->Location = System::Drawing::Point(368, 200);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(139, 59);
			this->button8->TabIndex = 5;
			this->button8->Text = L"Check-Out";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &ClientForm::button8_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(368, 142);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(139, 17);
			this->checkBox1->TabIndex = 4;
			this->checkBox1->Text = L"include dependencies \?";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// button7
			// 
			this->button7->Location = System::Drawing::Point(142, 78);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(96, 23);
			this->button7->TabIndex = 3;
			this->button7->Text = L"Get Packages";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &ClientForm::button7_Click);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(41, 83);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(89, 13);
			this->label12->TabIndex = 2;
			this->label12->Text = L"Server Packages";
			// 
			// listBox3
			// 
			this->listBox3->FormattingEnabled = true;
			this->listBox3->Location = System::Drawing::Point(44, 110);
			this->listBox3->Name = L"listBox3";
			this->listBox3->Size = System::Drawing::Size(194, 212);
			this->listBox3->TabIndex = 1;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label11->Location = System::Drawing::Point(279, 38);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(100, 24);
			this->label11->TabIndex = 0;
			this->label11->Text = L"Check-Out";
			// 
			// fileSystemWatcher1
			// 
			this->fileSystemWatcher1->EnableRaisingEvents = true;
			this->fileSystemWatcher1->SynchronizingObject = this;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &ClientForm::openFileDialog1_FileOk);
			// 
			// openFileDialog2
			// 
			this->openFileDialog2->FileName = L"openFileDialog2";
			// 
			// ClientForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(702, 450);
			this->Controls->Add(this->tabControl1);
			this->Name = L"ClientForm";
			this->Text = L"ClientForm";
			this->Load += gcnew System::EventHandler(this, &ClientForm::ClientForm_Load);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->tabPage3->ResumeLayout(false);
			this->tabPage3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->fileSystemWatcher1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void ClientForm_Load(System::Object^  sender, System::EventArgs^  e) 
	{
	
	}

	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) 
	{
	
	}

	private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
	{

	}

	//-------open cpp file------------------------------------------------------------//
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		openFileDialog1->Filter = "Cpp files|*.cpp";
		openFileDialog1->Title = "Select a CPP file";
		
		System::String^ cppFilePath ;

		if (openFileDialog1->ShowDialog().ToString() == "OK")
		{
			cppFilePath = openFileDialog1->FileName->ToString();
			textBox2->Text = cppFilePath;
		}
	}

	//------open h file-------------------------------------------------------------//
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e)
	{
		openFileDialog2->Filter = "h files|*.h";
		openFileDialog2->Title = "Select a H files";

		System::String^ hFilePath;

		if (openFileDialog2->ShowDialog().ToString() == "OK")
		{
			hFilePath = openFileDialog2->FileName->ToString();
			textBox3->Text = hFilePath;
		}
	}

	 //-----check-In button--------------------------------------------------------//
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {	
		if (textBox1->Text == ""){		
			label3->Text = "Please enter a package Name";
			return;
		}
		else if (textBox2->Text == ""){
			label3->Text = "Please select a Implementation file for the package";
			return;
		}
		else if (textBox3->Text == ""){
			label3->Text = "Please select a Header file for the package";
			return;
		}
		vector<Package> dependencies;
		//get all selected packages for dependenencies
		for each (auto item in listBox1->SelectedItems){		
			System::String^ itemString = item->ToString();
			System::String^ depName = itemString->Split(' ')[0];
			System::String^ depVersion = itemString->Split(' ')[2];
			Package dep;
			dep.name = marshal_as<std::string>(depName);
			dep.version = marshal_as<std::string>(depVersion);
			dependencies.push_back(dep);
		}
		HttpMessage msg;
		HttpMessage::Attribute attr;
		attr.first = "Command"; attr.second = "Check-In";
		msg.addAttribute(attr);
		HttpMessage::Attribute cppFilePath;
		cppFilePath.first = "cppFilePath" ; cppFilePath.second = marshal_as<std::string>(textBox2->Text);
		msg.addAttribute(cppFilePath);
		HttpMessage::Attribute hFilePath;
		hFilePath.first = "hFilePath"; hFilePath.second = marshal_as<std::string>(textBox3->Text);
		msg.addAttribute(hFilePath);

		Package checkInPackage;
		checkInPackage.name = marshal_as<std::string>(textBox1->Text);

		XMLResponseBodyGenerator xml;
		msg.setBody(xml.getRequestBodyForCheckIn(checkInPackage, dependencies));
		pSender->postMessage(msg);
		//currently blocking on the response
		HttpMessage response = pReceiver->getMessage();
		label3->Text = gcnew String(response.getBody().c_str());
	}

	private: System::Void tabControl1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
	{
		//if (tabControl1->SelectedTab == tabControl1->TabPages["tabPage1"])
		//{
		//	
		//}		
	}

	private: System::Void tabPage1_Load(System::Object^ sender, System::EventArgs^ e)
	{

	}
	
	//------------get Server packages (Check-In tab)----------------------------------//
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 	
	{
		listBox1->Items->Clear();

		HttpMessage msg;
		HttpMessage::Attribute attr;
		attr.first = "Command"; attr.second = "GetFiles";
		msg.addAttribute(attr);

		pSender->postMessage(msg);

		//currently blocking on the response
		HttpMessage response = pReceiver->getMessage();

		XMLResponseBodyGenerator xmlMgr;
		std::vector<Package> packages = xmlMgr.parseResponseBodyForGetFiles(response.getBody());

		for (auto package : packages)
		{
			if (package.status != "open")
			{
				std::string packagNameVer = package.name + " Version " + package.version;
				System::String^ pack = gcnew String(packagNameVer.c_str());

				listBox1->Items->Add(pack);
			}			
		}
	}
	
    //------------------Get open packages list----------------------------------------//
	private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		listBox2->Items->Clear();

		HttpMessage msg;
		HttpMessage::Attribute attr;
		attr.first = "Command"; attr.second = "GetOpenCheck-In";
		msg.addAttribute(attr);

		pSender->postMessage(msg);

		//currently blocking on the response
		HttpMessage response = pReceiver->getMessage();

		XMLResponseBodyGenerator xmlMgr;
		std::vector<Package> packages = xmlMgr.parseResponseBodyForGetFiles(response.getBody());

		for (auto package : packages)
		{
			std::string packagNameVer = package.name + " Version " + package.version;
			System::String^ pack = gcnew String(packagNameVer.c_str());

			listBox2->Items->Add(pack);
		}
	}

	//-------------Close Check-In Package--------------------------------------------//
	private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (listBox2->SelectedItems->Count == 0)
		{
			label10->Text = "Please select a Package";
			return;
		}

		HttpMessage msg;
		HttpMessage::Attribute attr;
		attr.first = "Command"; attr.second = "CloseOpenCheck-In";
		msg.addAttribute(attr);

		System::String^ item = listBox2->SelectedItem->ToString();
		System::String^ packageName = item->Split(' ')[0];
		System::String^ packageVersion = item->Split(' ')[2];

		Package closeCheckInPackage;
		closeCheckInPackage.name = marshal_as<std::string>(packageName);
		closeCheckInPackage.version = marshal_as<std::string>(packageVersion);

		XMLResponseBodyGenerator xml;
		msg.setBody(xml.getRequestBodyForCloseCheckIn(closeCheckInPackage));

		pSender->postMessage(msg);

		//currently blocking on the response
		HttpMessage response = pReceiver->getMessage();

		label10->Text = gcnew String(response.getBody().c_str());

	}

    //----------------Get Server Packages : Check-Out Tab ---------------------------//
	private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e)
	{
		listBox3->Items->Clear();

		HttpMessage msg;
		HttpMessage::Attribute attr;
		attr.first = "Command"; attr.second = "GetFiles";
		msg.addAttribute(attr);

		pSender->postMessage(msg);

		//currently blocking on the response
		HttpMessage response = pReceiver->getMessage();

		XMLResponseBodyGenerator xmlMgr;
		std::vector<Package> packages = xmlMgr.parseResponseBodyForGetFiles(response.getBody());

		for (auto package : packages)
		{
				std::string packagNameVer = package.name + " Version " + package.version;
				System::String^ pack = gcnew String(packagNameVer.c_str());

				listBox3->Items->Add(pack);
		}
	}

	//----------Check-Out package----------------------------------------------------//
	private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		if (listBox3->SelectedItems->Count == 0)
		{
			label13->Text = "Please select a Package";
			return;
		}

		HttpMessage msg;
		HttpMessage::Attribute attr;
		attr.first = "Command"; attr.second = "Check-Out";
		msg.addAttribute(attr);

		System::Boolean includeDep = checkBox1->Checked;

		if (includeDep == true)
		{
			HttpMessage::Attribute includeDependencies;
			includeDependencies.first = "includeDependencies"; includeDependencies.second = "true";
			msg.addAttribute(includeDependencies);
		}

		System::String^ item = listBox3->SelectedItem->ToString();
		System::String^ packageName = item->Split(' ')[0];
		System::String^ packageVersion = item->Split(' ')[2];

		Package checkOutPackage;
		checkOutPackage.name = marshal_as<std::string>(packageName);
		checkOutPackage.version = marshal_as<std::string>(packageVersion);

		vector<Package> dependencies;  //will send empty dependency in body, server fills up if required

		XMLResponseBodyGenerator xml;
		msg.setBody(xml.getRequestBodyforCheckOut(checkOutPackage, dependencies));

		pSender->postMessage(msg);

		//currently blocking on the response
		HttpMessage response = pReceiver->getMessage();

		label13->Text = gcnew String("Packages have been downloaded");
	}
};

} //end of namespace
