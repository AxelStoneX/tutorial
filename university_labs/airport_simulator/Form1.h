#pragma once


namespace UltimateAirportSimulator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
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
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btn_Simalate;
	protected: 
	private: System::Windows::Forms::Button^  btn_Save;
	private: System::Windows::Forms::TextBox^  tb_Filename_Reading;
	private: System::Windows::Forms::TextBox^  tb_String;
	private: System::Windows::Forms::Label^  lbl_String;
	private: System::Windows::Forms::Label^  lbl_Logo;
	private: System::Windows::Forms::Label^  lbl_Results;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::ComboBox^  cmb_Language;
	private: System::Windows::Forms::Label^  lbl_Lang;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label14;

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
			this->btn_Simalate = (gcnew System::Windows::Forms::Button());
			this->btn_Save = (gcnew System::Windows::Forms::Button());
			this->tb_Filename_Reading = (gcnew System::Windows::Forms::TextBox());
			this->tb_String = (gcnew System::Windows::Forms::TextBox());
			this->lbl_String = (gcnew System::Windows::Forms::Label());
			this->lbl_Logo = (gcnew System::Windows::Forms::Label());
			this->lbl_Results = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->cmb_Language = (gcnew System::Windows::Forms::ComboBox());
			this->lbl_Lang = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// btn_Simalate
			// 
			this->btn_Simalate->Location = System::Drawing::Point(12, 195);
			this->btn_Simalate->Name = L"btn_Simalate";
			this->btn_Simalate->Size = System::Drawing::Size(181, 50);
			this->btn_Simalate->TabIndex = 0;
			this->btn_Simalate->Text = L"Смоделировать работу аэропорта";
			this->btn_Simalate->UseVisualStyleBackColor = true;
			// 
			// btn_Save
			// 
			this->btn_Save->Location = System::Drawing::Point(577, 339);
			this->btn_Save->Name = L"btn_Save";
			this->btn_Save->Size = System::Drawing::Size(164, 50);
			this->btn_Save->TabIndex = 1;
			this->btn_Save->Text = L"Сохранить результат в текстовый файл";
			this->btn_Save->UseVisualStyleBackColor = true;
			// 
			// tb_Filename_Reading
			// 
			this->tb_Filename_Reading->Location = System::Drawing::Point(577, 395);
			this->tb_Filename_Reading->Name = L"tb_Filename_Reading";
			this->tb_Filename_Reading->Size = System::Drawing::Size(164, 20);
			this->tb_Filename_Reading->TabIndex = 2;
			// 
			// tb_String
			// 
			this->tb_String->Location = System::Drawing::Point(12, 164);
			this->tb_String->Name = L"tb_String";
			this->tb_String->Size = System::Drawing::Size(181, 20);
			this->tb_String->TabIndex = 3;
			// 
			// lbl_String
			// 
			this->lbl_String->AutoSize = true;
			this->lbl_String->Location = System::Drawing::Point(9, 138);
			this->lbl_String->Name = L"lbl_String";
			this->lbl_String->Size = System::Drawing::Size(184, 13);
			this->lbl_String->TabIndex = 4;
			this->lbl_String->Text = L"Введите длину посадочной полосы";
			this->lbl_String->Click += gcnew System::EventHandler(this, &Form1::lbl_String_Click);
			// 
			// lbl_Logo
			// 
			this->lbl_Logo->AutoSize = true;
			this->lbl_Logo->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->lbl_Logo->Location = System::Drawing::Point(88, 9);
			this->lbl_Logo->Name = L"lbl_Logo";
			this->lbl_Logo->Size = System::Drawing::Size(376, 31);
			this->lbl_Logo->TabIndex = 5;
			this->lbl_Logo->Text = L"The Ultimate Airport Simulator";
			// 
			// lbl_Results
			// 
			this->lbl_Results->AutoSize = true;
			this->lbl_Results->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->lbl_Results->Location = System::Drawing::Point(380, 129);
			this->lbl_Results->Name = L"lbl_Results";
			this->lbl_Results->Size = System::Drawing::Size(131, 25);
			this->lbl_Results->TabIndex = 6;
			this->lbl_Results->Text = L"Результаты:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(382, 154);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(135, 13);
			this->label1->TabIndex = 7;
			this->label1->Text = L"Общее число самолётов:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(382, 167);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(201, 13);
			this->label2->TabIndex = 8;
			this->label2->Text = L"Успешно приземлившиеся самолёты:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(382, 180);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(235, 13);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Отправленные в другой аэропорт самолёты:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(382, 193);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(260, 13);
			this->label4->TabIndex = 10;
			this->label4->Text = L"Среди них отправленные по закрытии аэропорта:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(382, 206);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(245, 13);
			this->label5->TabIndex = 11;
			this->label5->Text = L"Общее время ожидания в очереди на посадку:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(382, 219);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(183, 13);
			this->label6->TabIndex = 12;
			this->label6->Text = L"Побывавшие в очереди самолёты:";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(382, 232);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(141, 13);
			this->label7->TabIndex = 13;
			this->label7->Text = L"Среднее время ожидания:";
			// 
			// cmb_Language
			// 
			this->cmb_Language->FormattingEnabled = true;
			this->cmb_Language->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Русский", L"English"});
			this->cmb_Language->Location = System::Drawing::Point(542, 52);
			this->cmb_Language->Name = L"cmb_Language";
			this->cmb_Language->Size = System::Drawing::Size(121, 21);
			this->cmb_Language->TabIndex = 14;
			// 
			// lbl_Lang
			// 
			this->lbl_Lang->AutoSize = true;
			this->lbl_Lang->Location = System::Drawing::Point(539, 36);
			this->lbl_Lang->Name = L"lbl_Lang";
			this->lbl_Lang->Size = System::Drawing::Size(86, 13);
			this->lbl_Lang->TabIndex = 15;
			this->lbl_Lang->Text = L"Выберите язык";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(658, 154);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(35, 13);
			this->label8->TabIndex = 16;
			this->label8->Text = L"label8";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(658, 167);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(35, 13);
			this->label9->TabIndex = 17;
			this->label9->Text = L"label9";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(658, 180);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(41, 13);
			this->label10->TabIndex = 18;
			this->label10->Text = L"label10";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(658, 193);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(41, 13);
			this->label11->TabIndex = 19;
			this->label11->Text = L"label11";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(658, 206);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(41, 13);
			this->label12->TabIndex = 20;
			this->label12->Text = L"label12";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(658, 219);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(41, 13);
			this->label13->TabIndex = 21;
			this->label13->Text = L"label13";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(658, 232);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(41, 13);
			this->label14->TabIndex = 22;
			this->label14->Text = L"label14";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(753, 427);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->lbl_Lang);
			this->Controls->Add(this->cmb_Language);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->lbl_Results);
			this->Controls->Add(this->lbl_Logo);
			this->Controls->Add(this->lbl_String);
			this->Controls->Add(this->tb_String);
			this->Controls->Add(this->tb_Filename_Reading);
			this->Controls->Add(this->btn_Save);
			this->Controls->Add(this->btn_Simalate);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->Name = L"Form1";
			this->Text = L"The Ultimate Airport Sumulator v.1.1.1.1";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void lbl_String_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
};
}

