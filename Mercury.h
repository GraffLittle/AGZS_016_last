#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO::Ports;
using namespace System::Text; // for Encoding
using namespace System::Runtime::InteropServices; // for Marshal
using namespace System::Collections::Generic; // for LinkedList
using namespace System::Diagnostics; // for Debug::WriteLine("");
using namespace System::Threading;

#define MAXDATASTEPS 2

namespace Mercury {
	public ref struct TPOVersion
	{
		String^ sernum; // Серийный номер ККМ
		String^ name; // Имя
		String^ proiz; // Производитель
		String^ vers; // Версия ПО
	};
	//public ref struct TMerc {
	public ref class TMerc {
		public: TMerc (void) {
		}
		static TMerc^ obj = gcnew TMerc(); // for Application::OpenForms[0]->Invoke(gcnew Action(obj, &OutReceived));
		static RichTextBox^ OutTextBox = nullptr;
		static RichTextBox^ DebugBox = nullptr;
		static void MakeComPort(String^ pName);
	private: static System::Windows::Forms::Timer^ DataTimer = nullptr;
		   static System::Windows::Forms::Timer^ OpTimer = nullptr;
		   static System::IO::Ports::SerialPort^ ComPort = nullptr;
		   static bool OpenPort(String^ PName);
		   static System::Void Port_ErrorReceived(System::Object^ sender, System::IO::Ports::SerialErrorReceivedEventArgs^ e);
		   static System::Void Port_DataReceived(System::Object^ sender, System::IO::Ports::SerialDataReceivedEventArgs^ e);
		   static array<unsigned char>^ comBuf = nullptr;
		   static String^ comStr;
		   static void PortWrite(array<unsigned char>^ buff);
		   static void OutReceived(void); // static ???
		   static String^ ComandResult(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4);
		   static String^ StatusKKM(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4);
		   static void GetInfo5C(Byte n1, Byte n2, Byte n3);
		   // About BCC:
		   static int CalcBCC(array<unsigned char>^ buff, int strt, int end);
		   static String^ CalcSBCC(array<unsigned char>^ buff, int strt, int end);
		   static void AddBCC(array<unsigned char>^& Buf);
	private: static System::Void DataTimer_Tick(System::Object^ sender, System::EventArgs^ e);
		   static System::Void OpTimer_Tick(System::Object^ sender, System::EventArgs^ e);
	private: static void MakeDataTimer(int intrvl);
		   static void MakeOpTimer(int intrvl);
		   static void GetDataStep(int step);
		   static void Get5CStep(int step);
		   static int Stage = 0;
		   static int AquiredStage = -1;
		   // for Запрос «Считать параметры ККМ» (Код сообщения = 5CH)
		   static int par5C = -1;
		   static int parNeed5C = 0;
		   static int gotpar5C = -1;
		   // for making operations:
		   static int intpar;
		   static String^ strpar;

		   static System::Windows::Forms::Label^ dtLbl = nullptr; // Date&Time label
		   static System::Windows::Forms::Label^ vLbl = nullptr; // Version label
	public: static System::Windows::Forms::Label^ resLbl = nullptr; // Comand result label

	public:
		static void StartDataAquire( String^ PortName, RichTextBox^ Out); // TMerc::StartDataAquire()
		// for making operations:
		static void SetOpParams(int oppar, String^ Par) { intpar = oppar; strpar = Par; }
		static void MakeOperation(int opn, String^ PortName, RichTextBox^ Out);
		// To get Date&Time in Label dtl, to get Version in Label vl:
		static void GetMercDT(String^ PortName, System::Windows::Forms::Label^ dtl);
		static void GetMercVersion(String^ PortName, System::Windows::Forms::Label^ vl);
		// To use in AGZS:
		// 1. Set TMerc::resLbl for Label to out Comand Results. (may be TMerc::DebugBox)
		// 2. RegisterOperator() == open smena for operator opName. (here setting OutTextBox)
		static System::Void RegisterOperator( String^ PortName, String^ opName, RichTextBox^ Out);
		// 3. Print checks.
		static void PrintCheck( String^ PortName, double volume, double price, double accepted);
		// 4. PrintZ() == print Z-otchet == close smena.
		static void PrintZ(String^ PortName);

			static String^ ComRes; // TMerc::ComRes
			static array<Byte>^ aMercDate = gcnew array<Byte>(8); // TMerc::aMercDate
			static String^ sMercDate = ""; // TMerc::sMercDate
			static array<Byte>^ aMercTime = gcnew array<Byte>(4); // TMerc::aMercTime
			static String^ sMercTime = ""; // TMerc::sMercTime
			static String^ MercStatus = ""; // TMerc::MercStatus
			static TPOVersion^ VOP = gcnew TPOVersion(); // TMerc::VOP
			static Dictionary<int, String^>^ Kassir = gcnew Dictionary<int, String^>;
			static Dictionary<int, String^>^ Parameter5C = gcnew Dictionary<int, String^>;
	private:
		static int opnum = -1;
		static void GetDateTime(void);
		static void GetVersion(void);
		static void PrintCheck(String^ PortName, String^ Kassir, double volume, double price, double accepted,
			System::Windows::Forms::Label^ rl);
		static void PrintO(String^ PortName, Byte n, Label^ rl);
		static System::Void RegisterKassir(int num, String^ name);
		static array<Byte>^ MakeRekviz(int n, int ln);
		static array<Byte>^ MakeRekviz(int n, int ln, String^ str);
		static array<Byte>^ MakePrice11Rekviz(int n, int ln, double d1, double d2, String^ str);
		static array<Byte>^ MakePrice13Rekviz(int n, int ln, double d);
	};

}
