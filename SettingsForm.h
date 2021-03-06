#pragma once

namespace AGZS {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO::Ports;

	/// <summary>
	/// ������ ��� SettingsForm
	/// </summary>
	public ref class SettingsForm : public System::Windows::Forms::Form
	{
	public:
		SettingsForm(void)
		{
			InitializeComponent();
			//
			//TODO: �������� ��� ������������
			//
			array<String^>^ ports = SerialPort::GetPortNames();
			ComBox->Items->Clear();
			ComBox->Items->AddRange(ports);
			ComBox->SelectedItem = ComBox->Items[ComBox->Items->Count - 1];
			ComBox->Text = ports[ports->Length - 1];

			setKolNumStr = "0";
			setKolNum = 0;
			comboBox1->SelectedIndex = 0; // Cast number
			comboBox2->SelectedIndex = 0;
			KolNumBox->SelectedIndex = 0;
		}
	private: System::Windows::Forms::GroupBox^ UstGroupBox;
	public:

	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::Label^ label5;
	//private: System::Windows::Forms::Label^ lblPL;
	public: System::Windows::Forms::Button^ UpdateBtn;
	//private: System::Windows::Forms::Label^ lblT;
	private: System::Windows::Forms::Label^ label7;
	//private: System::Windows::Forms::Label^ lblLM;
	private: System::Windows::Forms::Label^ label11;
	//private: System::Windows::Forms::Label^ lblIL;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::Label^ lblDT;
	public: System::Windows::Forms::Label^ lblPL;
	public: System::Windows::Forms::Label^ lblT;
	public: System::Windows::Forms::Label^ lblLM;
	public: System::Windows::Forms::Label^ lblIL;
	private: System::Windows::Forms::Label^ lbl1i;
	private: System::Windows::Forms::Label^ lbl2i;
	private: System::Windows::Forms::Label^ lbl3i;
	private: System::Windows::Forms::Label^ lbl4i;
	public: System::Windows::Forms::CheckBox^ check1i;
	public: System::Windows::Forms::CheckBox^ check4i;
	public: System::Windows::Forms::CheckBox^ check3i;
	public: System::Windows::Forms::CheckBox^ check2i;
	private: System::Windows::Forms::Timer^ InfTimer;
	public:
	private: System::IO::Ports::SerialPort^ SerialComPort;
		   String^ comStr;
		   String^ errStr;
		   String^ ReceivedPacket, ^ _Received;
		   String^ pStr, ^ tStr, ^ ilStr, ^ lmStr, ^ unStr; // - Info answers: $18.50,t36.6,I/L400,"l1.25 L/M5.5500"



	private: System::Windows::Forms::GroupBox^ groupBox3;
	private: System::Windows::Forms::ComboBox^ comboBox2;
	private: System::Windows::Forms::ComboBox^ comboBox1;
	private: System::Windows::Forms::Button^ CastBtn;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::GroupBox^ groupBox4;
	public: System::Windows::Forms::CheckBox^ checkStop;
	private:
	public: System::Windows::Forms::CheckBox^ checkLog;
	public: System::Windows::Forms::CheckBox^ checkPusk;
	private: System::Windows::Forms::Label^ label2;
	public:
	public: System::Windows::Forms::ComboBox^ ComBox;
	private:
	private: System::Windows::Forms::Label^ label1;
	public:
	public: System::Windows::Forms::TextBox^ CostBox;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::ComboBox^ KolNumBox;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::Button^ ComOpenButn;
	private: System::Windows::Forms::Button^ ResetBtn;
	private: System::Windows::Forms::ToolTip^ AllToolTip;
	private: System::Windows::Forms::Label^ ResetLbl;


	public:
	private:
	public:
	private:
		   String^ lastOuted;
	private: void OutReceived(void); // Analizing comStr
	private: void ErrReceived(void); // Analizing errStr
	private: int _InfoStage;
	public: System::Windows::Forms::Form^ MForm;

	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
		/// </summary>
		~SettingsForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:

	private:

	public:

	private: System::Windows::Forms::Button^ ApplyBtn;
	private: System::ComponentModel::IContainer^ components;
	public:
	private:

	private:
		/// <summary>
		/// ������������ ���������� ������������.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// ��������� ����� ��� ��������� ������������ � �� ��������� 
		/// ���������� ����� ������ � ������� ��������� ����.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->ApplyBtn = (gcnew System::Windows::Forms::Button());
			this->UstGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->check4i = (gcnew System::Windows::Forms::CheckBox());
			this->check3i = (gcnew System::Windows::Forms::CheckBox());
			this->check2i = (gcnew System::Windows::Forms::CheckBox());
			this->check1i = (gcnew System::Windows::Forms::CheckBox());
			this->lbl4i = (gcnew System::Windows::Forms::Label());
			this->lbl3i = (gcnew System::Windows::Forms::Label());
			this->lbl2i = (gcnew System::Windows::Forms::Label());
			this->lbl1i = (gcnew System::Windows::Forms::Label());
			this->lblDT = (gcnew System::Windows::Forms::Label());
			this->lblLM = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->lblIL = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->lblT = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->UpdateBtn = (gcnew System::Windows::Forms::Button());
			this->lblPL = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->InfTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->SerialComPort = (gcnew System::IO::Ports::SerialPort(this->components));
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->CastBtn = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->ComOpenButn = (gcnew System::Windows::Forms::Button());
			this->checkStop = (gcnew System::Windows::Forms::CheckBox());
			this->checkLog = (gcnew System::Windows::Forms::CheckBox());
			this->checkPusk = (gcnew System::Windows::Forms::CheckBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->ComBox = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->CostBox = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->KolNumBox = (gcnew System::Windows::Forms::ComboBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->ResetBtn = (gcnew System::Windows::Forms::Button());
			this->AllToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->ResetLbl = (gcnew System::Windows::Forms::Label());
			this->UstGroupBox->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->SuspendLayout();
			// 
			// ApplyBtn
			// 
			this->ApplyBtn->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->ApplyBtn->Location = System::Drawing::Point(107, 485);
			this->ApplyBtn->Name = L"ApplyBtn";
			this->ApplyBtn->Size = System::Drawing::Size(75, 23);
			this->ApplyBtn->TabIndex = 9;
			this->ApplyBtn->Text = L"�������";
			this->ApplyBtn->UseVisualStyleBackColor = true;
			// 
			// UstGroupBox
			// 
			this->UstGroupBox->Controls->Add(this->label4);
			this->UstGroupBox->Controls->Add(this->label3);
			this->UstGroupBox->Controls->Add(this->button2);
			this->UstGroupBox->Controls->Add(this->button1);
			this->UstGroupBox->Controls->Add(this->textBox2);
			this->UstGroupBox->Controls->Add(this->textBox1);
			this->UstGroupBox->Location = System::Drawing::Point(12, 163);
			this->UstGroupBox->Name = L"UstGroupBox";
			this->UstGroupBox->Size = System::Drawing::Size(292, 88);
			this->UstGroupBox->TabIndex = 10;
			this->UstGroupBox->TabStop = false;
			this->UstGroupBox->Text = L"���������";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(178, 49);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(22, 13);
			this->label4->TabIndex = 5;
			this->label4->Text = L"��";
			this->label4->Click += gcnew System::EventHandler(this, &SettingsForm::label4_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(178, 21);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(22, 13);
			this->label3->TabIndex = 4;
			this->label3->Text = L"��";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(211, 47);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 3;
			this->button2->Text = L"������";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &SettingsForm::button2_Click);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(211, 21);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 2;
			this->button1->Text = L"������";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &SettingsForm::button1_Click);
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(16, 49);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(154, 21);
			this->textBox2->TabIndex = 1;
			this->textBox2->TextChanged += gcnew System::EventHandler(this, &SettingsForm::textBox2_TextChanged);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(16, 21);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(154, 21);
			this->textBox1->TabIndex = 0;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &SettingsForm::textBox1_TextChanged);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->check4i);
			this->groupBox2->Controls->Add(this->check3i);
			this->groupBox2->Controls->Add(this->check2i);
			this->groupBox2->Controls->Add(this->check1i);
			this->groupBox2->Controls->Add(this->lbl4i);
			this->groupBox2->Controls->Add(this->lbl3i);
			this->groupBox2->Controls->Add(this->lbl2i);
			this->groupBox2->Controls->Add(this->lbl1i);
			this->groupBox2->Controls->Add(this->lblDT);
			this->groupBox2->Controls->Add(this->lblLM);
			this->groupBox2->Controls->Add(this->label11);
			this->groupBox2->Controls->Add(this->lblIL);
			this->groupBox2->Controls->Add(this->label9);
			this->groupBox2->Controls->Add(this->lblT);
			this->groupBox2->Controls->Add(this->label7);
			this->groupBox2->Controls->Add(this->UpdateBtn);
			this->groupBox2->Controls->Add(this->lblPL);
			this->groupBox2->Controls->Add(this->label5);
			this->groupBox2->Location = System::Drawing::Point(12, 257);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(292, 154);
			this->groupBox2->TabIndex = 11;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"����������";
			this->groupBox2->Enter += gcnew System::EventHandler(this, &SettingsForm::groupBox2_Enter);
			// 
			// check4i
			// 
			this->check4i->AutoSize = true;
			this->check4i->Location = System::Drawing::Point(218, 95);
			this->check4i->Name = L"check4i";
			this->check4i->Size = System::Drawing::Size(15, 14);
			this->check4i->TabIndex = 17;
			this->check4i->UseVisualStyleBackColor = true;
			// 
			// check3i
			// 
			this->check3i->AutoSize = true;
			this->check3i->Location = System::Drawing::Point(218, 69);
			this->check3i->Name = L"check3i";
			this->check3i->Size = System::Drawing::Size(15, 14);
			this->check3i->TabIndex = 16;
			this->check3i->UseVisualStyleBackColor = true;
			// 
			// check2i
			// 
			this->check2i->AutoSize = true;
			this->check2i->Location = System::Drawing::Point(218, 44);
			this->check2i->Name = L"check2i";
			this->check2i->Size = System::Drawing::Size(15, 14);
			this->check2i->TabIndex = 15;
			this->check2i->UseVisualStyleBackColor = true;
			// 
			// check1i
			// 
			this->check1i->AutoSize = true;
			this->check1i->Location = System::Drawing::Point(218, 20);
			this->check1i->Name = L"check1i";
			this->check1i->Size = System::Drawing::Size(15, 14);
			this->check1i->TabIndex = 14;
			this->check1i->UseVisualStyleBackColor = true;
			// 
			// lbl4i
			// 
			this->lbl4i->BackColor = System::Drawing::SystemColors::ControlDark;
			this->lbl4i->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->lbl4i->Location = System::Drawing::Point(239, 93);
			this->lbl4i->Name = L"lbl4i";
			this->lbl4i->Size = System::Drawing::Size(17, 17);
			this->lbl4i->TabIndex = 13;
			this->lbl4i->Text = L"4i";
			this->lbl4i->Click += gcnew System::EventHandler(this, &SettingsForm::lbl4i_Click);
			// 
			// lbl3i
			// 
			this->lbl3i->BackColor = System::Drawing::SystemColors::ControlDark;
			this->lbl3i->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->lbl3i->Location = System::Drawing::Point(239, 70);
			this->lbl3i->Name = L"lbl3i";
			this->lbl3i->Size = System::Drawing::Size(17, 17);
			this->lbl3i->TabIndex = 12;
			this->lbl3i->Text = L"3i";
			this->lbl3i->Click += gcnew System::EventHandler(this, &SettingsForm::lbl3i_Click);
			// 
			// lbl2i
			// 
			this->lbl2i->BackColor = System::Drawing::SystemColors::ControlDark;
			this->lbl2i->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->lbl2i->Location = System::Drawing::Point(239, 45);
			this->lbl2i->Name = L"lbl2i";
			this->lbl2i->Size = System::Drawing::Size(17, 17);
			this->lbl2i->TabIndex = 11;
			this->lbl2i->Text = L"2i";
			this->lbl2i->Click += gcnew System::EventHandler(this, &SettingsForm::lbl2i_Click);
			// 
			// lbl1i
			// 
			this->lbl1i->BackColor = System::Drawing::SystemColors::ControlDark;
			this->lbl1i->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->lbl1i->Location = System::Drawing::Point(239, 21);
			this->lbl1i->Name = L"lbl1i";
			this->lbl1i->Size = System::Drawing::Size(17, 17);
			this->lbl1i->TabIndex = 10;
			this->lbl1i->Text = L"1i";
			this->lbl1i->Click += gcnew System::EventHandler(this, &SettingsForm::lbl1i_Click);
			// 
			// lblDT
			// 
			this->lblDT->AutoSize = true;
			this->lblDT->Location = System::Drawing::Point(97, 125);
			this->lblDT->Name = L"lblDT";
			this->lblDT->Size = System::Drawing::Size(67, 13);
			this->lblDT->TabIndex = 9;
			this->lblDT->Text = L"���������:";
			// 
			// lblLM
			// 
			this->lblLM->AutoSize = true;
			this->lblLM->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->lblLM->Location = System::Drawing::Point(92, 95);
			this->lblLM->Name = L"lblLM";
			this->lblLM->Size = System::Drawing::Size(26, 15);
			this->lblLM->TabIndex = 8;
			this->lblLM->Text = L"L/M";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(7, 95);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(84, 13);
			this->label11->TabIndex = 7;
			this->label11->Text = L"���� � ������:";
			// 
			// lblIL
			// 
			this->lblIL->AutoSize = true;
			this->lblIL->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->lblIL->Location = System::Drawing::Point(92, 70);
			this->lblIL->Name = L"lblIL";
			this->lblIL->Size = System::Drawing::Size(22, 15);
			this->lblIL->TabIndex = 6;
			this->lblIL->Text = L"I/L";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(7, 70);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(76, 13);
			this->label9->TabIndex = 5;
			this->label9->Text = L"���. �� ����:";
			// 
			// lblT
			// 
			this->lblT->AutoSize = true;
			this->lblT->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->lblT->Location = System::Drawing::Point(92, 45);
			this->lblT->Name = L"lblT";
			this->lblT->Size = System::Drawing::Size(15, 15);
			this->lblT->TabIndex = 4;
			this->lblT->Text = L"T";
			this->lblT->Click += gcnew System::EventHandler(this, &SettingsForm::label6_Click);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(7, 45);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(77, 13);
			this->label7->TabIndex = 3;
			this->label7->Text = L"T����������:";
			// 
			// UpdateBtn
			// 
			this->UpdateBtn->Location = System::Drawing::Point(6, 120);
			this->UpdateBtn->Name = L"UpdateBtn";
			this->UpdateBtn->Size = System::Drawing::Size(85, 23);
			this->UpdateBtn->TabIndex = 2;
			this->UpdateBtn->Text = L"��������";
			this->UpdateBtn->UseVisualStyleBackColor = true;
			this->UpdateBtn->Click += gcnew System::EventHandler(this, &SettingsForm::button3_Click);
			// 
			// lblPL
			// 
			this->lblPL->AutoSize = true;
			this->lblPL->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->lblPL->Location = System::Drawing::Point(92, 21);
			this->lblPL->Name = L"lblPL";
			this->lblPL->Size = System::Drawing::Size(24, 15);
			this->lblPL->TabIndex = 1;
			this->lblPL->Text = L"P/L";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(7, 21);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(78, 13);
			this->label5->TabIndex = 0;
			this->label5->Text = L"���� �� ����:";
			// 
			// InfTimer
			// 
			this->InfTimer->Tick += gcnew System::EventHandler(this, &SettingsForm::InfTimer_Tick);
			// 
			// SerialComPort
			// 
			this->SerialComPort->ErrorReceived += gcnew System::IO::Ports::SerialErrorReceivedEventHandler(this, &SettingsForm::SerialComPort_ErrorReceived);
			this->SerialComPort->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &SettingsForm::SerialComPort_DataReceived);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->CastBtn);
			this->groupBox3->Controls->Add(this->label6);
			this->groupBox3->Controls->Add(this->comboBox2);
			this->groupBox3->Controls->Add(this->comboBox1);
			this->groupBox3->Location = System::Drawing::Point(12, 417);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(292, 58);
			this->groupBox3->TabIndex = 15;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"��������� ����� �������:";
			// 
			// CastBtn
			// 
			this->CastBtn->Location = System::Drawing::Point(211, 20);
			this->CastBtn->Name = L"CastBtn";
			this->CastBtn->Size = System::Drawing::Size(75, 23);
			this->CastBtn->TabIndex = 3;
			this->CastBtn->Text = L"���������";
			this->CastBtn->UseVisualStyleBackColor = true;
			this->CastBtn->Click += gcnew System::EventHandler(this, &SettingsForm::CastBtn_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(80, 25);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(23, 13);
			this->label6->TabIndex = 2;
			this->label6->Text = L"=>";
			// 
			// comboBox2
			// 
			this->comboBox2->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Items->AddRange(gcnew cli::array< System::Object^  >(10) {
				L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7",
					L"8", L"9"
			});
			this->comboBox2->Location = System::Drawing::Point(111, 20);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(59, 21);
			this->comboBox2->TabIndex = 1;
			// 
			// comboBox1
			// 
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(10) {
				L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7",
					L"8", L"9"
			});
			this->comboBox1->Location = System::Drawing::Point(6, 20);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(56, 21);
			this->comboBox1->TabIndex = 0;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->ComOpenButn);
			this->groupBox4->Controls->Add(this->checkStop);
			this->groupBox4->Controls->Add(this->checkLog);
			this->groupBox4->Controls->Add(this->checkPusk);
			this->groupBox4->Controls->Add(this->label2);
			this->groupBox4->Controls->Add(this->ComBox);
			this->groupBox4->Controls->Add(this->label1);
			this->groupBox4->Controls->Add(this->CostBox);
			this->groupBox4->Location = System::Drawing::Point(12, 12);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(292, 102);
			this->groupBox4->TabIndex = 16;
			this->groupBox4->TabStop = false;
			// 
			// ComOpenButn
			// 
			this->ComOpenButn->Location = System::Drawing::Point(170, 46);
			this->ComOpenButn->Name = L"ComOpenButn";
			this->ComOpenButn->Size = System::Drawing::Size(105, 23);
			this->ComOpenButn->TabIndex = 22;
			this->ComOpenButn->Text = L"������� ����";
			this->ComOpenButn->UseVisualStyleBackColor = true;
			this->ComOpenButn->Click += gcnew System::EventHandler(this, &SettingsForm::ComOpenButn_Click);
			// 
			// checkStop
			// 
			this->checkStop->AutoSize = true;
			this->checkStop->Location = System::Drawing::Point(77, 76);
			this->checkStop->Name = L"checkStop";
			this->checkStop->Size = System::Drawing::Size(62, 17);
			this->checkStop->TabIndex = 21;
			this->checkStop->Text = L"\"����\"";
			this->checkStop->UseVisualStyleBackColor = true;
			// 
			// checkLog
			// 
			this->checkLog->AutoSize = true;
			this->checkLog->Location = System::Drawing::Point(148, 76);
			this->checkLog->Name = L"checkLog";
			this->checkLog->Size = System::Drawing::Size(114, 17);
			this->checkLog->TabIndex = 20;
			this->checkLog->Text = L"����� ��� ������";
			this->checkLog->UseVisualStyleBackColor = true;
			// 
			// checkPusk
			// 
			this->checkPusk->AutoSize = true;
			this->checkPusk->Location = System::Drawing::Point(9, 76);
			this->checkPusk->Name = L"checkPusk";
			this->checkPusk->Size = System::Drawing::Size(62, 17);
			this->checkPusk->TabIndex = 19;
			this->checkPusk->Text = L"\"����\"";
			this->checkPusk->UseVisualStyleBackColor = true;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(17, 50);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(43, 13);
			this->label2->TabIndex = 18;
			this->label2->Text = L"COM�";
			// 
			// ComBox
			// 
			this->ComBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->ComBox->FormattingEnabled = true;
			this->ComBox->Location = System::Drawing::Point(66, 47);
			this->ComBox->Name = L"ComBox";
			this->ComBox->Size = System::Drawing::Size(98, 21);
			this->ComBox->TabIndex = 17;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(10, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(65, 13);
			this->label1->TabIndex = 16;
			this->label1->Text = L"����/����:";
			// 
			// CostBox
			// 
			this->CostBox->Location = System::Drawing::Point(77, 20);
			this->CostBox->Name = L"CostBox";
			this->CostBox->Size = System::Drawing::Size(121, 21);
			this->CostBox->TabIndex = 15;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(12, 132);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(144, 13);
			this->label8->TabIndex = 17;
			this->label8->Text = L"��������� ��� ������� �";
			// 
			// KolNumBox
			// 
			this->KolNumBox->FormattingEnabled = true;
			this->KolNumBox->Items->AddRange(gcnew cli::array< System::Object^  >(10) {
				L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7",
					L"8", L"9"
			});
			this->KolNumBox->Location = System::Drawing::Point(157, 129);
			this->KolNumBox->Name = L"KolNumBox";
			this->KolNumBox->Size = System::Drawing::Size(39, 21);
			this->KolNumBox->TabIndex = 18;
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(198, 128);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(93, 23);
			this->button3->TabIndex = 19;
			this->button3->Text = L"������� �����";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &SettingsForm::button3_Click_1);
			// 
			// ResetBtn
			// 
			this->ResetBtn->Location = System::Drawing::Point(294, 128);
			this->ResetBtn->Name = L"ResetBtn";
			this->ResetBtn->Size = System::Drawing::Size(18, 23);
			this->ResetBtn->TabIndex = 20;
			this->ResetBtn->Text = L"&C";
			this->ResetBtn->UseVisualStyleBackColor = true;
			this->ResetBtn->Visible = false;
			this->ResetBtn->Click += gcnew System::EventHandler(this, &SettingsForm::ResetBtn_Click);
			this->ResetBtn->MouseHover += gcnew System::EventHandler(this, &SettingsForm::ResetBtn_MouseHover);
			// 
			// ResetLbl
			// 
			this->ResetLbl->AutoSize = true;
			this->ResetLbl->Location = System::Drawing::Point(196, 501);
			this->ResetLbl->Name = L"ResetLbl";
			this->ResetLbl->Size = System::Drawing::Size(14, 13);
			this->ResetLbl->TabIndex = 21;
			this->ResetLbl->Text = L"&";
			this->ResetLbl->UseMnemonic = false;
			this->ResetLbl->Visible = false;
			// 
			// SettingsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(316, 520);
			this->Controls->Add(this->ResetLbl);
			this->Controls->Add(this->ResetBtn);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->KolNumBox);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->UstGroupBox);
			this->Controls->Add(this->ApplyBtn);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->KeyPreview = true;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"SettingsForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"���������";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &SettingsForm::SettingsForm_FormClosed);
			this->Load += gcnew System::EventHandler(this, &SettingsForm::SettingsForm_Load);
			this->Shown += gcnew System::EventHandler(this, &SettingsForm::SettingsForm_Shown);
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &SettingsForm::SettingsForm_KeyPress);
			this->UstGroupBox->ResumeLayout(false);
			this->UstGroupBox->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void SettingsForm_Load(System::Object^ sender, System::EventArgs^ e);
private: System::Void SettingsForm_Shown(System::Object^ sender, System::EventArgs^ e);

private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e); // ������
private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e); // ������
	   bool kSended;
private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {}
private: System::Void textBox2_TextChanged(System::Object^ sender, System::EventArgs^ e) {}
private: System::Void label4_Click(System::Object^ sender, System::EventArgs^ e) {}
private: System::Void groupBox2_Enter(System::Object^ sender, System::EventArgs^ e) {}
private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e); // UpdateBtn
private: System::Void label6_Click(System::Object^ sender, System::EventArgs^ e) {}
private: System::Void lbl1i_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void lbl2i_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void lbl3i_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void lbl4i_Click(System::Object^ sender, System::EventArgs^ e);
	public: System::Void StartInfo(void);
	public: System::Void StopInfo(void);
private: System::Void SettingsForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
private: System::Void SerialComPort_DataReceived(System::Object^ sender, System::IO::Ports::SerialDataReceivedEventArgs^ e);
private: System::Void SerialComPort_ErrorReceived(System::Object^ sender, System::IO::Ports::SerialErrorReceivedEventArgs^ e);
private: System::Void InfTimer_Tick(System::Object^ sender, System::EventArgs^ e);
	   System::Void PortWrite(String^ s);
	   System::Void PortWriteLikeis(String^ s);
	   System::Void LogReceived(String^ s);
private: System::Void CastBtn_Click(System::Object^ sender, System::EventArgs^ e);
	   String^ setKolNumStr;
	   int setKolNum;
private: System::Void button3_Click_1(System::Object^ sender, System::EventArgs^ e) { // KolNumBox "������� �����"
	setKolNumStr = KolNumBox->Text;
	setKolNum = Convert::ToInt32(setKolNumStr);
	String^ t = "��������� ����� ��� ������� �" + setKolNumStr + "."; // +setKolNum.ToString();
	MessageBox::Show(t, "���������");
}
private: System::Void ComOpenButn_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void ResetBtn_MouseHover(System::Object^ sender, System::EventArgs^ e) {
	AllToolTip->SetToolTip(ResetBtn, "RESET �������");
}
private: System::Void ResetBtn_Click(System::Object^ sender, System::EventArgs^ e);//toWrite="n"+KolNumBox->Text+"&";
private: System::Void SettingsForm_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
	int key = e->KeyChar;
	//ResetLbl->Visible = true;
	//ResetLbl->Text = key.ToString();
	//UstGroupBox->Text = "���������: " + key;
	if (e->KeyChar == 5) SendE(setKolNum); // Ctrl-E == 5
	// Ctrl-S == 19
	if (e->KeyChar == 18) ResetBtn_Click( sender, e); // Ctrl-R == 18
}
public: System::Void SendE(int n); //Command to start fuel
public: System::Void SendS(int n); //Command to stop fuel
public: System::Void SendReset(int n); // '&'
};
}
