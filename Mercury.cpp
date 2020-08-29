#include "Mercury.h"

namespace Mercury {

	// Utils:
	String^ N3(Byte b1, Byte b2, Byte b3) {
		array<unsigned char>^ n3 = { b1,b2,b3 };
		String^ np = System::Text::Encoding::GetEncoding(866)->GetString(n3);
		return np;
	}
	String^ HexString(array<unsigned char>^ arr) {
		String^ res = Convert::ToString(arr[0], 16)->ToUpper();
		for (int i = 1; i < arr->Length; i++) { res += " " + Convert::ToString(arr[i], 16)->ToUpper(); }
		return res;
	}
	String^ StringOf(array<unsigned char>^ in) {
		String^ out = "";
		for (int i = 0; i < in->Length; i++) {
			if (in[i] > 10) out += wchar_t(in[i]);
			else out += "_";
		}
		String^ t866 = System::Text::Encoding::GetEncoding(866)->GetString(in);
		String^ t1251 = System::Text::Encoding::GetEncoding(1251)->GetString(in);
		String^ tDef = System::Text::Encoding::Default->GetString(in);
		out += "|866:" + t866 + "|1251:" + t1251 + "|Default:" + tDef + "|";
		return out;
	}

// public ref class TMerc --------------------------------------------------------------------------------------

void TMerc::PortWrite(array<unsigned char>^ buff) {
	if (ComPort == nullptr) //MakeComPort("COM2");
		if (DebugBox != nullptr) { DebugBox->Text += "\r\n:(ComPort == nullptr)!"; return; }
	if (!ComPort->IsOpen) {
		if (!OpenPort(ComPort->PortName)) {
			if (DebugBox != nullptr) DebugBox->Text += "\r\n:Port " + ComPort->PortName + " NOT opened!";
			return;
		}
		if (DebugBox != nullptr) DebugBox->Text += "\r\n:Port " + ComPort->PortName + " opened.";
	}
	if (ComPort->IsOpen) {
		ComPort->Write(buff, 0, buff->Length);
		if (DebugBox != nullptr)
			DebugBox->Text += "\r\nComPort->Write({" + HexString(buff) + "}, 0, " + buff->Length + ").\r\n";
	}
}

void TMerc::GetInfo5C(Byte n1, Byte n2, Byte n3) {
	// n1,n2,n3 == 0x30+num
	array<unsigned char>^ b = { 0x02, 0x5C, 0x30, 0x30, 0x30, 0x30, 0x00, 0x30 + n1, 0x30 + n2, 0x30 + n3, 0x00, 0x0B, 0xCC, 0x03 };
	AddBCC(b);
	PortWrite(b);
	//if (OutTextBox != nullptr) OutTextBox->Text += "\r\nGetInfo5C(" + n1 + "," + n2 + "," + n3 + ")";
}
void TMerc::GetDateTime(void) {
	array<unsigned char>^ b = { 0x02, 0x48, 0x30, 0x30, 0x30, 0x30, 0x00, 0x0B, 0xCC, 0x03 };
	AddBCC(b);
	PortWrite(b);
	//if (OutTextBox != nullptr) OutTextBox->Text += "\r\nTMerc::GetDateTime():" + ComandResult();
}
void TMerc::GetVersion(void) {
	array<unsigned char>^ b = { 0x02, 0x45, 0x30, 0x30, 0x30, 0x30, 0x00, 0x30, 0x35, 0x03 };
	PortWrite(b);
}

System::Void TMerc::OpTimer_Tick(System::Object^ sender, System::EventArgs^ e) {
	String^ s = nullptr;
	switch (opnum) {
	case 0:
		if (DebugBox != nullptr)
			DebugBox->Text += "\r\nOpTimer_Tick():opnum=0,sMercDate=" + sMercDate + ",sMercTime=" + sMercTime;
		if ((sMercDate == "") || (sMercTime == "")) {
			GetDateTime(); // &TMerc::sMercDate=05.08.2020,TMerc::sMercTime=21:04
		}
		else {
			OpTimer->Stop();
			if (OutTextBox != nullptr)
				OutTextBox->Text += "\r\nDate&Time=" + sMercDate + " " + sMercTime;
		}
		break;
	case 1:
		Kassir->TryGetValue(intpar, s);
		if (DebugBox != nullptr) {
			DebugBox->Text += "\r\nMakeOperation(1):intpar=" + intpar + ",strpar=" + strpar;
			if (s != nullptr) DebugBox->Text += ", was Kassir[" + intpar + "]='" + s + "'";
			else DebugBox->Text += ",Kassir[" + intpar + "] not exists yet.";
			if (s == nullptr) DebugBox->Text += "\r\n(s == nullptr)"; // (s == nullptr)
			if (s == "") DebugBox->Text += "\r\n(s == '')";
		}
		if (s != strpar) RegisterKassir(intpar, strpar);
		else {
			OpTimer->Stop();
			//OutTextBox->Text += "\r\nDate&Time=" + sMercDate + " " + sMercTime;
		}
		break;
	case 2:
		if (OutTextBox != nullptr) OutTextBox->Text += "\r\nTMerc::OpTimer_Tick(opnum=2)";
		if (VOP->vers == nullptr) GetVersion();
		else {
			OpTimer->Stop();
			if (vLbl != nullptr)
				vLbl->Text = "ПО ККМ:" + VOP->sernum + "," + VOP->name + "," + VOP->proiz + "," + VOP->vers;
			if (OutTextBox != nullptr)
				OutTextBox->Text += "\r\nПО ККМ:" + VOP->sernum + "," + VOP->name + "," + VOP->proiz + "," + VOP->vers;
		}
		break;
	case 3:
		if (OutTextBox != nullptr) OutTextBox->Text += "\r\nTMerc::OpTimer_Tick(opnum=3)";
		OpTimer->Stop();
		break;
	default:
		OutTextBox->Text += "\r\nMakeOperation(unknown opnum!):" + opnum;
		OpTimer->Stop();
		break;
	}
}

System::Void TMerc::DataTimer_Tick(System::Object^ sender, System::EventArgs^ e) {
	if (gotpar5C == par5C) Get5CStep(par5C = parNeed5C);
	if (gotpar5C < 0) {
		if (Kassir->Count > 0) {
			OutTextBox->Text += "\r\nКассирoв:" + Kassir->Count.ToString();
			for each (KeyValuePair<int, String^> k in Kassir)
				OutTextBox->Text += "\r\nКассир №" + k.Key + "=" + k.Value;
		}
		if (Parameter5C->Count > 0) {
			/*
			OutTextBox->Text += "\r\nParameters:" + Parameter5C->Count.ToString();
			for each (KeyValuePair<int, String^> k in Parameter5C)
				OutTextBox->Text += "\r\nParameter5C[" + k.Key + "]=" + k.Value;
			*/
			OutTextBox->Text += "\r\nЧПМ:" + Parameter5C[307];
			OutTextBox->Text += "\r\nЗаголовок 1:" + Parameter5C[300];
			OutTextBox->Text += "\r\nЗаголовок 2:" + Parameter5C[301];
			OutTextBox->Text += "\r\nЗаголовок 3:" + Parameter5C[302];
			OutTextBox->Text += "\r\nЗаголовок 4:" + Parameter5C[303];
			OutTextBox->Text += "\r\nИНН:" + Parameter5C[304];
		}
		gotpar5C = 0; // Showing one time.
	}
}

System::Void TMerc::Port_ErrorReceived(System::Object^ sender, System::IO::Ports::SerialErrorReceivedEventArgs^ e) {
	if (OutTextBox != nullptr) OutTextBox->Text += "\r\nTMerc::Port_ErrorReceived():" + e->ToString();
}
System::Void TMerc::Port_DataReceived(System::Object^ sender, System::IO::Ports::SerialDataReceivedEventArgs^ e) {
	int btr = ComPort->BytesToRead;
	comBuf = gcnew array<unsigned char>(btr);
	ComPort->Read(comBuf, 0, btr);
	comStr = System::Text::Encoding::GetEncoding(866)->GetString(comBuf);
	Application::OpenForms[0]->Invoke(gcnew Action(&OutReceived));
}
void TMerc::OutReceived(void) {
	unsigned char curcom = 0;
	try { if ((comBuf != nullptr) && (comBuf->Length > 1) && (comBuf[0] == 2)) curcom = comBuf[1]; else return; }
	catch (Exception^ E) {
		if (DebugBox != nullptr) DebugBox->Text += "\r\nTMerc::OutReceived(curcom=comBuf[1])-Exception: " + E->Message;
	}
	if ((curcom != 0x5C) && (curcom != 0x48) && (curcom != 0x45)) // Excluded binary dump for this comands (0x53 чек?)
	for (int i = 0; i < comBuf->Length; i++) {
		String^ sH = Convert::ToString(comBuf[i], 16);
		if (DebugBox != nullptr) DebugBox->Text += "," + comBuf[i] + "[" + sH->ToUpper() + "]";
		if (Char::IsLetterOrDigit(comBuf[i])) {
			if (DebugBox != nullptr) DebugBox->Text += "(" + wchar_t(comBuf[i]) + ")";
		}
	}
	try {
		List<Byte>^ rec = gcnew List<Byte>(comBuf);
		String^ res = Encoding::GetEncoding(866)->GetString(rec->GetRange(8, 4)->ToArray());
		ComRes = ComandResult(comBuf[8], comBuf[9], comBuf[10], comBuf[11]);
		if (resLbl != nullptr) resLbl->Text = ComRes;
		if (res != "0000") { // for Номер параметра=107,308,400
			if ((res != "000d") && (res != "0002"))
				if (DebugBox != nullptr) DebugBox->Text += "\r\nTMerc::ComRes=" + ComRes + "par=" + par5C + ",res=" + res;
			if (DataTimer != nullptr) // About aquiring data: here gotpar5C, par5C, parNeed5C
			if (par5C < 400) {
				parNeed5C = -1; gotpar5C = -1; // To stop it!
				return;
			}
			else if (res == "000d") { // TMerc::ComRes=000D:Кассир не зарегистрирован.par=400,res=000d
				if (par5C < 405) { gotpar5C = par5C; parNeed5C = par5C + 1; }
				else { parNeed5C = -1; gotpar5C = -1; }
				return;
			}
		}
		if (curcom == 0x53) { // Button 'ЧЕК!' proceed it
			String^ RecStr = StringOf(rec->ToArray());
			if (DebugBox != nullptr) DebugBox->Text += "\r\n Результат выполнения команды: " + TMerc::ComRes;
			// Результат выполнения команды: 0002:Не закрыта смена.
			String^ stat = StatusKKM(rec[3], rec[4], rec[5], rec[6]);
			// Текущий статус принтера B 2: 4e
			String^ KasNum = RecStr->Substring(16, 2);
			String^ DocNum = RecStr->Substring(19, 5);
			String^ Date = RecStr->Substring(25, 8);
			String^ Time = RecStr->Substring(34, 4);
			String^ Type = RecStr->Substring(39, 2);
			String^ Summ = Encoding::GetEncoding(866)->GetString(rec->GetRange(42, 15)->ToArray());
			String^ ChNum = RecStr->Substring(58, 5);
			if (DebugBox != nullptr) {
				DebugBox->Text += "\r\n " + stat; // Текущий статус ККМ...
				DebugBox->Text += "\r\n Номер кассира: " + KasNum; // Номер кассира: 02=Kass. Павел Владимирович
				DebugBox->Text += "=" + Kassir[Convert::ToInt32(KasNum)];
				DebugBox->Text += "\r\n Номер документа: " + DocNum;
				DebugBox->Text += "\r\n Дата: " + Date;
				DebugBox->Text += "\r\n Время: " + Time;
				DebugBox->Text += "\r\n Тип документа: " + Type;
				DebugBox->Text += "\r\n Сумма: " + Summ; // Сумма: 12.49
				DebugBox->Text += "\r\n Номер чека: " + ChNum;
			}
		}
		else if (curcom == 0x31) { // Register Kassir:
			if (DebugBox != nullptr) DebugBox->Text += "\r\n(curcom == 0x31):" + ComRes;
			if (OpTimer != nullptr) {
				OpTimer->Stop();
				if (DebugBox != nullptr) DebugBox->Text += "\r\n:intpar=" + intpar + ",strpar=" + strpar; // :intpar=3,strpar=Kas-sir$
				if (res == "0000") {
					MakeOperation(2,ComPort->PortName,OutTextBox);
				}
			}
		}
		else if (curcom == 0x45) {
			VOP->sernum = Encoding::GetEncoding(866)->GetString(rec->GetRange(16, 7)->ToArray())->TrimEnd({ '\0' });
			VOP->name = Encoding::GetEncoding(866)->GetString(rec->GetRange(24, 30)->ToArray())->TrimEnd({ '\0' });
			VOP->proiz = Encoding::GetEncoding(866)->GetString(rec->GetRange(55, 30)->ToArray())->TrimEnd({ '\0' });
			VOP->vers = Encoding::GetEncoding(866)->GetString(rec->GetRange(86, 30)->ToArray())->TrimEnd({ '\0' });

			if (vLbl != nullptr)
				vLbl->Text = "ПО ККМ:" + VOP->sernum + "," + VOP->name + "," + VOP->proiz + "," + VOP->vers;
			if (OutTextBox != nullptr) {
				OutTextBox->Text += "\r\n«Информация о версии ПО ККМ»";
				OutTextBox->Text += "\r\nСерийный номер ККМ: " + TMerc::VOP->sernum; // Серийный номер ККМ: 0681961
				OutTextBox->Text += "\r\nИмя: " + TMerc::VOP->name; // Имя: Меркурий MS-K
				OutTextBox->Text += "\r\nПроизводитель: " + TMerc::VOP->proiz; // Производитель: (C) 2004 "ИНКОТЕКС"
				OutTextBox->Text += "\r\nВерсия ПО: " + TMerc::VOP->vers; // Версия ПО: 1.0 MSK B5.6.6
			}
		}
		else if (curcom == 0x48) { // Mercury date&time
			aMercDate = rec->GetRange(16, 8)->ToArray();
			aMercTime = rec->GetRange(25, 4)->ToArray();
			sMercDate = Encoding::GetEncoding(866)->GetString(TMerc::aMercDate)->Insert(4, ".")->Insert(2, ".");
			sMercTime = Encoding::GetEncoding(866)->GetString(TMerc::aMercTime)->Insert(2, ":");

			if (dtLbl != nullptr) dtLbl->Text = sMercDate + "," + sMercTime;
			AquiredStage = 0;
		}
		else if (curcom == 0x5C) {
			String^ _statfr = Encoding::GetEncoding(866)->GetString(rec->GetRange(3, 4)->ToArray()); // Текущий статус ФР
			String^ _result = Encoding::GetEncoding(866)->GetString(rec->GetRange(8, 4)->ToArray()); // Результат выполнения команды
			String^ _statpr = Encoding::GetEncoding(866)->GetString(rec->GetRange(13, 2)->ToArray()); // Текущий статус принтера
			String^ _pnum = Encoding::GetEncoding(866)->GetString(rec->GetRange(16, 3)->ToArray()); // Номер параметра
			String^ _prmtr = Encoding::GetEncoding(866)->GetString(rec->GetRange(20, 40)->ToArray()); // Параметр
			int _pn = Convert::ToInt32(_pnum);
			if (Parameter5C->ContainsKey(_pn)) Parameter5C[_pn] = _prmtr;
			else Parameter5C->Add(_pn,_prmtr);
			if (DataTimer != nullptr) { // About aquiring data: here gotpar5C, par5C, parNeed5C
				gotpar5C = _pn;
				if (gotpar5C < 7) parNeed5C = gotpar5C + 1;
				else if (gotpar5C == 7) parNeed5C = 100;
				else if (gotpar5C < 106) parNeed5C = gotpar5C + 1;
				else if (gotpar5C == 106) parNeed5C = 200;
				else if (gotpar5C < 203) parNeed5C = gotpar5C + 1;
				else if (gotpar5C == 203) parNeed5C = 300;
				else if (gotpar5C < 307) parNeed5C = gotpar5C + 1;
				else if (gotpar5C == 307) parNeed5C = 400;
				else if (gotpar5C < 402) parNeed5C = gotpar5C + 1;
				else { parNeed5C = -1; gotpar5C = -1; } // To stop it!
			}
			MercStatus = StatusKKM(comBuf[3], comBuf[4], comBuf[5], comBuf[6]);
			array<unsigned char>^ n3 = { comBuf[16],comBuf[17],comBuf[18] }; // Номер параметра
			if (comBuf[16] == 0x34) { // 400 - 499 - Фамилия кассира (по номеру кассира 00 - 99)
				String^ n3 = N3(comBuf[16], comBuf[17], comBuf[18]); // Номер параметра N 3:402
				int kn = Convert::ToInt32(n3) - 400; // номер кассира
				String^ kName = Encoding::GetEncoding(866)->GetString(rec->GetRange(20, 40)->ToArray());
				Kassir[kn] = kName;
			}
		} // (curcom == 0x5C)
	}
	catch (Exception^ E) {
		if (OutTextBox != nullptr) OutTextBox->Text += "\r\n TMerc::OutReceived() - Exception: " + E->Message;
	}
} // TMerc::OutReceived()

String^ TMerc::StatusKKM(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4) { // Текущий статус ККМ. B 4
	String^ res = "Текущий статус ККМ:";
	String^ s1 = Convert::ToString(b1, 16)->ToUpper();
	String^ s2 = Convert::ToString(b2, 16)->ToUpper();
	String^ s3 = Convert::ToString(b3, 16)->ToUpper();
	String^ s4 = Convert::ToString(b4, 16)->ToUpper();

	array<Byte>^ buf = { b1,b2,b3,b4 };
	String^ merc = Encoding::GetEncoding(866)->GetString(buf); // fc13

	String^ bits1 = Convert::ToString(b1, 2);
	String^ bits2 = Convert::ToString(b2, 2);
	String^ bits3 = Convert::ToString(b3, 2);
	String^ bits4 = Convert::ToString(b4, 2);

	if (b4 & 0b00000001) res += "\r\n!Смена Открыта!";
	else res += "\r\n!Смена Закрыта!";
	if (b4 & 0b00000010) res += "\r\nШирина ленты 57 мм.";
	else res += "\r\nШирина ленты 80 мм.";
	if (b4 & 0b00000100) res += "\r\nБуфер копии документа близок к концу(осталось менее 5%) Да.";
	else res += "\r\nБуфер копии документа близок к концу(осталось менее 5%) Нет.";
	if (b4 & 0b00001000) res += "\r\nравно 1.";
	else res += "\r\nНе используется (равно 0).";
	if (b4 & 0b00010000) res += "\r\nАппарат фискализирован Да.";
	else res += "\r\nАппарат фискализирован Нет.";
	if (b4 & 0b00100000) res += "\r\nФискальная память близка к концу(осталось менее 30 записей) Да.";
	else res += "\r\nФискальная память близка к концу(осталось менее 30 записей) Нет.";
	if (b4 & 0b01000000) res += "\r\nФискальная память исчерпана Да.";
	else res += "\r\nФискальная память исчерпана Нет.";
	if (b4 & 0b10000000) res += "\r\nПротокол XON/XOFF.";
	else res += "\r\nПротокол BS.";

	while (bits3->Length < 8) bits3 = "0" + bits3; // must be 8 symbols.
	String^ doc = bits3->Substring(6, 2);
	String^ oper = bits3->Substring(1, 5);

	if (doc == "00") res += "\r\nДокумент закрыт.";
	else if (doc == "01") res += "\r\nДокумент Документ открыт (возможно проведение финансовых операций).";
	else if (doc == "10") res += "\r\nИтог (проведение финансовых операций запрещено, документ не может быть закрыт т.к. напечатаны не все обязательные реквизиты).";
	else if (doc == "11") res += "\r\nЗавершение документа (проведение финансовых операций запрещено, документ может быть закрыт).";
	if (oper == "00000") res += "\r\nПроводимая операция: Продажа.";
	else if (oper == "00001") res += "\r\nПроводимая операция: Возврат продажи за наличные.";
	else if (oper == "00010") res += "\r\nПроводимая операция: Внесение суммы (подкрепление).";
	else if (oper == "00011") res += "\r\nПроводимая операция: Выплата суммы (инкассация).";
	else if (oper == "10010") res += "\r\nПроводимая операция: Возврат продажи за безналичные.";
	else if (oper == "11111") res += "\r\nПроводимая операция: Завершена.";
	else res += "\r\nПроводимая операция: unknown.";

	if (b3 & 0b10000000) res += "\r\nНаличие копии последнего документа Да.";
	else res += "\r\nНаличие копии последнего документа Нет.";

	return res;
}

String^ TMerc::ComandResult(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4) {
	array<unsigned char>^ bytes = {b1,b2,b3,b4};
	String^ B4 = Encoding::Default->GetString(bytes)->ToUpper();

	String^ res = "Unknown error.";
	if (B4 == "0000") res = "Oшибок нет.";
	else if (B4 == "0001") res = "Ошибка в фискальных данных, аппарат блокирован.";
	else if (B4 == "0002") res = "Не закрыта смена.";
	else if (B4 == "0003") res = "Исчерпан ресурс сменных записей в фискальную память.";
	else if (B4 == "0004") res = "Превышена длина поля команды.";
	else if (B4 == "0005") res = "Неверный формат поля команды.";
	else if (B4 == "0006") res = "Ошибка чтения таймера.";
	else if (B4 == "0007") res = "Неверная дата.";
	else if (B4 == "0008") res = "Неверное время.";
	else if (B4 == "0009") res = "Дата меньше последней даты, зарегистрированной в фискальной памяти.";
	else if (B4 == "000A") res = "Операция прервана пользователем. Документ аннулирован.";
	else if (B4 == "000B") res = "Запрещенная команда ПУ (см п. 3.3).";
	else if (B4 == "000C") res = "Не открыта смена.";
	else if (B4 == "000D") res = "Кассир не зарегистрирован.";
	else if (B4 == "000E") res = "Переполнение приёмного буфера.";
	else if (B4 == "000F") res = "Ошибка записи в фискальную память.";
	else if (B4 == "0010") res = "Ошибка установки таймера.";
	else if (B4 == "0011") res = "Неверный пароль налогового инспектора.";
	else if (B4 == "0012") res = "Неверный пароль на связь.";
	else if (B4 == "0013") res = "Исчерпан ресурс перерегистраций.";
	else if (B4 == "0014") res = "Аппарат не фискализирован.";
	else if (B4 == "0015") res = "Значение поля команды вне диапазона.";
	else if (B4 == "0016") res = "Ошибка чтения фискальной памяти.";
	else if (B4 == "0017") res = "Переполнение или отрицательный результат счётчика.";
	else if (B4 == "0018") res = "Обязательное поле команды имеет нулевую длину.";
	else if (B4 == "0019") res = "Неверный формат команды. 001A Дата или время последнего документа в смене меньше предыдущего.";
	else if (B4 == "001B") res = "Не используется.";
	else if (B4 == "001C") res = "Ошибка в расположении реквизитов (пересечение или выход за область печати).";
	else if (B4 == "001D") res = "Нет такой команды.";
	else if (B4 == "001E") res = "Неверная контрольная сумма (BCC)";
	else if (B4 == "001F") res = "Нет фискальных записей.";
	else if (B4 == "0021") res = "Оформление документа прервано по окончанию времени ожидания готовности принтера.";
	else if (B4 == "0024") res = "Буфер ответа пуст.";
	else if (B4 == "0025") res = "Услуга не введена";
	else if (B4 == "0029") res = "Дублирование обязательных реквизитов документа.";
	else if (B4 == "002A") res = "Текущее состояние ККМ не позволяет выполнить операцию.";
	else if (B4 == "002B") res = "Ошибка в данных энергонезависимой памяти. Аппарат блокирован.";
	else if (B4 == "002C") res = "Невозможно выполнить инициализацию ФП. ФП уже инициализирована.";
	else if (B4 == "002D") res = "Вывод прерван по окончанию времени ожидания готовности дисплея.";
	else if (B4 == "002E") res = "Ошибка записи FLASH памяти.";
	else if (B4 == "002F") res = "Ошибка. Нет записей.";
	else if (B4 == "0030") res = "Ошибка связи с ЭКЛЗ";
	else if (B4 == "0031") res = "Некорректный формат или параметр команды ЭКЛЗ";
	else if (B4 == "0032") res = "Некорректное состояние ЭКЛЗ";
	else if (B4 == "0033") res = "Авария ЭКЛЗ";
	else if (B4 == "0034") res = "Авария криптографического процессора ЭКЛЗ";
	else if (B4 == "0035") res = "Исчерпан временной ресурс использования ЭКЛЗ";
	else if (B4 == "0036") res = "ЭКЛЗ переполнена";
	else if (B4 == "0037") res = "Неверные дата или время в ЭКЛЗ";
	else if (B4 == "0038") res = "Нет запрошенных данных в ЭКЛЗ";
	else if (B4 == "0039") res = "Переполнение счётчиков ЭКЛЗ";
	else if (B4 == "0046") res = "Ошибка протокола обмена ЭКЛЗ";
	else if (B4 == "0047") res = "Переполнение приёмного буфера ЭКЛЗ";
	else if (B4 == "0048") res = "Неверная контрольная сумма ЭКЛЗ";
	else if (B4 == "0049") res = "ЭКЛЗ активизирована в составе другой ККМ. Аппарат блокирован.";
	else if (B4 == "004A") res = "ЭКЛЗ не активизирована.";
	else if (B4 == "004B") res = "Неисправимая ошибка ЭКЛЗ";
	else if (B4 == "004C") res = "Исчерпан ресурс активизаций ЭКЛЗ.";
	else if (B4 == "004D") res = "ЭКЛЗ уже активизирована.";
	else if (B4 == "004E") res = "Превышено количество секций в документе. (16 секций -ограничение ЭКЛЗ)";
	else if (B4 == "004F") res = "Архив ЭКЛЗ закрыт или переполнение архива";
	else if (B4 == "0050") res = "Ошибка. Данные фискальной памяти и ЭКЛЗ различаются.";
	else if (B4 == "0070") res = "Переполнение счётчика количества в чеке, при сложении.";
	else if (B4 == "0071") res = "Переполнение счётчика количества в чеке, при вычитании.";
	else if (B4 == "0072") res = "Переполнение счётчика суммы аннулирований в чеке.";
	else if (B4 == "0073") res = "Переполнение сменного счётчика суммы аннулирований.";
	else if (B4 == "0074") res = "Переполнение сменного счётчика суммы продаж.";
	else if (B4 == "0075") res = "Переполнение сменного счётчика суммы наличных.";
	else if (B4 == "0076") res = "Переполнение сменного счётчика суммы оплаты в кредит.";
	else if (B4 == "0077") res = "Переполнение сменного счётчика суммы оплаты картой.";
	else if (B4 == "0078") res = "Переполнение сменного счётчика суммы наличных при вычитании.";
	else if (B4 == "0079") res = "Переполнение сменного счётчика суммы возвратов при оплате в кредит.";
	else if (B4 == "007A") res = "Переполнение счётчика итоговой суммы в чеке при добавлении.";
	else if (B4 == "007B") res = "Переполнение счётчика итоговой суммы в чеке при вычитании.";
	else if (B4 == "007C") res = "Переполнение счётчика стоимости в чеке, при умножении количества на стоимость.";
	else if (B4 == "007D") res = "Переполнение счётчика итоговой суммы в чеке при вычислении скидки.";
	else if (B4 == "007E") res = "Переполнение счётчика итоговой суммы по секции в чеке при вычислении скидки.";
	else if (B4 == "007F") res = "Переполнение счётчика итоговой суммы учёта тары в чеке при вычислении скидки.";
	else if (B4 == "0080") res = "Переполнение счётчика скидок в чеке.";
	else if (B4 == "0081") res = "Переполнение счётчика итоговой суммы налоговой группы в чеке при вычислении скидки.";
	else if (B4 == "0082") res = "Переполнение счётчика итоговой суммы безналоговой группы в чеке при вычислении скидки.";
	else if (B4 == "0083") res = "Переполнение счётчика итоговой суммы в чеке при вычислении надбавки.";
	else if (B4 == "0084") res = "Переполнение счётчика итоговой суммы по секции в чеке при вычислении надбавки.";
	else if (B4 == "0085") res = "Переполнение счётчика итоговой суммы учёта тары в чеке при вычислении надбавки.";
	else if (B4 == "0086") res = "Переполнение счётчика надбавок в чеке.";
	else if (B4 == "0087") res = "Переполнение счётчика итоговой суммы налоговой группы в чеке при вычислении надбавки.";
	else if (B4 == "0088") res = "Переполнение счётчика итоговой суммы безналоговой группы в чеке при вычислении надбавки.";
	else if (B4 == "0089") res = "Переполнение счётчика итоговой суммы по секции в чеке.";
	else if (B4 == "008A") res = "Переполнение счётчика итоговой суммы учёта тары в чеке.";
	else if (B4 == "008B") res = "Переполнение счётчика итоговой суммы налоговой группы в чеке.";
	else if (B4 == "008C") res = "Переполнение счётчика итоговой суммы безналоговой группы в чеке.";
	else if (B4 == "008D") res = "Переполнение счётчика итоговой суммы в чеке при вычислении аннулирования скидки.";
	else if (B4 == "008E") res = "Переполнение счётчика итоговой суммы по секции в чеке при вычислении аннулирования скидки.";
	else if (B4 == "008F") res = "Переполнение счётчика итоговой суммы учёта тары в чеке при вычислении аннулирования скидки.";
	else if (B4 == "0090") res = "Переполнение счётчика скидок в чеке при вычислении аннулирования скидки.";
	else if (B4 == "0091") res = "Переполнение счётчика итоговой суммы налоговой группы в чеке при вычислении аннулирования скидки.";
	else if (B4 == "0092") res = "Переполнение счётчика итоговой суммы безналоговой группы в чеке при вычислении аннулирования скидки.";
	else if (B4 == "0093") res = "Переполнение счётчика итоговой суммы в чеке при вычислении аннулирования надбавки.";
	else if (B4 == "0094") res = "Переполнение счётчика итоговой суммы по секции в чеке при вычислении аннулирования надбавки.";
	else if (B4 == "0095") res = "Переполнение счётчика итоговой суммы учёта тары в чеке при вычислении аннулирования надбавки.";
	else if (B4 == "0096") res = "Переполнение счётчика надбавок в чеке при вычислении аннулирования надбавки.";
	else if (B4 == "0097") res = "Переполнение счётчика итоговой суммы налоговой группы в чеке при вычислении аннулирования надбавки.";
	else if (B4 == "0098") res = "Переполнение счётчика итоговой суммы безналоговой группы в чеке при вычислении аннулирования надбавки.";
	else if (B4 == "0099") res = "Сумма оплаты меньше суммы чека.";
	else if (B4 == "009A") res = "Переполнение счётчика итоговой суммы оплаты при комбинированной оплате.";
	else if (B4 == "009B") res = "Сумма безналичной оплаты больше суммы чека.";
	else if (B4 == "009C") res = " Блокировка выполнения команды. Расхождение текущей даты и даты последней записи в ФП больше запрограммированного значения.";
	return B4 + ":" + res;
}

void TMerc::MakeDataTimer(int intrvl) {
	DataTimer = gcnew System::Windows::Forms::Timer();
	DataTimer->Interval = intrvl;
	DataTimer->Tick += gcnew System::EventHandler(&TMerc::DataTimer_Tick);
}
void TMerc::MakeOpTimer(int intrvl) {
	OpTimer = gcnew System::Windows::Forms::Timer();
	OpTimer->Interval = intrvl;
	OpTimer->Tick += gcnew System::EventHandler(&TMerc::OpTimer_Tick);
}
void TMerc::MakeComPort(String^ pName) {
	if (ComPort != nullptr) return;
	ComPort = (gcnew System::IO::Ports::SerialPort());
	ComPort->ErrorReceived += gcnew System::IO::Ports::SerialErrorReceivedEventHandler(&Port_ErrorReceived);
	ComPort->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(&Port_DataReceived);
	OpenPort(pName);
}
void TMerc::MakeOperation(int opn, String^ PortName, RichTextBox^ Out) {
	OutTextBox = Out;
	MakeComPort(PortName);
	if (OpTimer == nullptr) MakeOpTimer(333);
	opnum = opn;
	switch (opn) {
	case 0:
		sMercDate = ""; sMercTime = "";
		break;
	case 1: // RegisterKassir(int num, String^ name);
		break;
	case 2: // PO version
		break;
	case 3:
		if (OutTextBox != nullptr) OutTextBox->Text += "\r\nMakeOperation(" + opnum + ") not inplemented.";
		break;
	default:
		if (OutTextBox != nullptr) OutTextBox->Text += "\r\nMakeOperation(unknown opnum!):" + opnum;
		break;
	}
	OpTimer->Start();
}
void TMerc::GetMercDT(String^ PortName, System::Windows::Forms::Label^ dtl) {
	dtLbl = dtl;
	MakeOperation( 0, PortName, OutTextBox);
}
void TMerc::GetMercVersion(String^ PortName, System::Windows::Forms::Label^ vl) {
	vLbl = vl;
	MakeOperation(2, PortName, OutTextBox);
}
void TMerc::StartDataAquire(String^ PortName, RichTextBox^ Out) {
	if (DataTimer != nullptr) {
		if (OutTextBox != nullptr)
			OutTextBox->Text += "\r\n(DataTimer != nullptr) => stoping TMerc::StartDataAquire( " + PortName + ", Out)";
		return;
	}
	OutTextBox = Out;
	MakeComPort(PortName);
	MakeDataTimer(333);
	Stage = -1;
	par5C = -1;
	DataTimer->Start();
}
bool TMerc::OpenPort(String^ PName)
{
	if (ComPort->IsOpen) ComPort->Close();
	ComPort->PortName = PName;
	ComPort->StopBits = System::IO::Ports::StopBits::One;
	ComPort->DataBits = 8;
	ComPort->BaudRate = 115200;
	ComPort->Parity = System::IO::Ports::Parity::None;
	ComPort->Handshake = System::IO::Ports::Handshake::None;
	//this->ComPort->WriteTimeout; //this->ComPort->ReadTimeout; - need ???
	try {
		ComPort->Open();
	}
	catch (Exception^ E) {
		if (OutTextBox != nullptr) OutTextBox->Text += "\r\n TMerc::OpenPort('" + PName + 
			"') - ComPort->Open() - Exception: " + E->Message;
		return ComPort->IsOpen;
	}
	if (OutTextBox != nullptr) {
		OutTextBox->Text += "\r\nSerialPort PortName=" + ComPort->PortName;
		OutTextBox->Text += "\r\nSerialPort Parity=" + ComPort->Parity.ToString();
		OutTextBox->Text += "\r\nSerialPort BaudRate=" + ComPort->BaudRate.ToString();
		OutTextBox->Text += "\r\nSerialPort DataBits=" + ComPort->DataBits.ToString();
		OutTextBox->Text += "\r\nSerialPort StopBits=" + ComPort->StopBits.ToString();
		OutTextBox->Text += "\r\nSerialPort Handshake=" + ComPort->Handshake.ToString();
		OutTextBox->Text += "\r\nSerialPort IsOpen=" + ComPort->IsOpen.ToString();
	}
	return ComPort->IsOpen;
}

void TMerc::Get5CStep(int step) { // step == parameter number
	int i1 = step % 10;
	int i2 = (step / 10) % 10;
	int i3 = step / 100;
	GetInfo5C(i3, i2, i1);
}

void TMerc::GetDataStep(int step) {
	if (OutTextBox != nullptr) OutTextBox->Text += "\r\n" + DateTime::Now.ToString() + ">TMerc::GetDataStep(" + step + ")";
	switch (step) {
	case 0:
		GetDateTime(); // &TMerc::sMercDate=05.08.2020,TMerc::sMercTime=21:04
		break;
	case 1:
		if (true) GetInfo5C( 3, 0, 0);
		else for (int i = 300; i < 317; i++) {
			int i1 = i % 10;
			int i2 = (i / 10) % 10;
			int i3 = i / 100;
			if (OutTextBox != nullptr) OutTextBox->Text += "\r\n" + i + ":" + i3 + "," + i2 + "," + i1 + ".";
			GetInfo5C(i3, i2, i1);
			Thread::Sleep(100);
		}
		AquiredStage = step;
		break;
	case 2:
		if (OutTextBox != nullptr) OutTextBox->Text += "\r\nMercGetDataStep(" + step + ") not inplemented.";
		AquiredStage = step;
		break;
	default:
		OutTextBox->Text += "\r\nTMerc::GetDataStep(unknown step!):" + step;
		break;
	}
}

System::Void TMerc::RegisterKassir(int num, String^ name) { // Номер кассира N 2 «00...99»
	if (DebugBox != nullptr) DebugBox->Text += "\r\n=>RegisterKassir( " + num + ", '" + name + "'):";
	int i2 = num % 10;
	int i1 = (num / 10) % 10;
	array<unsigned char>^ buff = { 02, 0x31, 0x30,0x30,0x30,0x30,00, 0x30 + i1, 0x30 + i2,00,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, // S 40
	0, 0,0, 3 };
	array<wchar_t>^ wc = name->ToCharArray();
	array<unsigned char>^ nam = gcnew array<unsigned char>(wc->Length);
	AddBCC(buff);

	array<unsigned char>^ merc866 = Encoding::GetEncoding(866)->GetBytes(name);
	String^ _merc = Encoding::GetEncoding(866)->GetString(merc866);
	merc866->CopyTo(buff, 10);
	AddBCC(buff);
	PortWrite(buff);
}

void TMerc::PrintCheck(String^ PortName, double volume, double price, double accepted) { // for registered kassir N2
	MakeComPort(PortName);
	List<Byte>^ Check = gcnew List<Byte>(0);
	int RecCnt = 14;
	array<unsigned char>^ buf = { 2, 0x53, 0x30, 0x30, 0x30, 0x30, 0, 0x30, 0, 0x31, 0x34, 0, 0x31, 0x37, 0, 0 };//11
	{
		int ost = RecCnt % 10;
		int dec = (RecCnt - ost) / 10;
		buf[12] = 0x30 + dec;
		buf[13] = 0x30 + ost;
	}
	Check->AddRange(buf);
	Check->AddRange(MakeRekviz(1, 0));
	Check->AddRange(MakeRekviz(2, 1));
	Check->AddRange(MakeRekviz(3, 2));
	Check->AddRange(MakeRekviz(4, 3));
	Check->AddRange(MakeRekviz(0, 4));
	Check->AddRange(MakeRekviz(10, 5));
	Check->AddRange(MakeRekviz(5, 6));
	Check->AddRange(MakeRekviz(7, 7));
	Check->AddRange(MakeRekviz(8, 8));
	Check->AddRange(MakeRekviz(6, 9));
	Check->AddRange(MakePrice11Rekviz(11, 10, volume, price, "литр"));
	Check->AddRange(MakeRekviz(12, 11));
	Check->AddRange(MakePrice13Rekviz(13, 12, accepted));
	Check->AddRange(MakeRekviz(14, 13));
	array<unsigned char>^ end = { 0x0B, 0xCC, 3 };
	Check->AddRange(end);
	array<unsigned char>^ Buff = Check->ToArray();
	String^ ss = Encoding::GetEncoding(866)->GetString(Buff);
	if (DebugBox != nullptr) DebugBox->Text += "\r\n > Check End=" + ss->Replace('\0', '_')->Replace("\3", "\r\n");
	AddBCC(Buff);
	PortWrite(Buff);
}
void TMerc::PrintCheck(String^ PortName, String^ Kassir, double volume, double price, double accepted,
	System::Windows::Forms::Label^ rl) {
	resLbl = rl;
	MakeComPort(PortName);
	RegisterKassir(2, Kassir);
	List<Byte>^ Check = gcnew List<Byte>(0);
	int RecCnt = 16;
	array<unsigned char>^ buf = { 2, 0x53, 0x30, 0x30, 0x30, 0x30, 0, 0x30, 0, 0x31, 0x34, 0, 0x31, 0x37, 0, 0 };//11
	{
		int ost = RecCnt % 10;
		int dec = (RecCnt - ost) / 10;
		buf[12] = 0x30 + dec;
		buf[13] = 0x30 + ost;
	}
	Check->AddRange(buf);
	Check->AddRange(MakeRekviz(1, 0));
	Check->AddRange(MakeRekviz(2, 1));
	Check->AddRange(MakeRekviz(3, 2));
	Check->AddRange(MakeRekviz(4, 3));
	Check->AddRange(MakeRekviz(99, 4, "==================="));
	Check->AddRange(MakeRekviz(0, 5));
	Check->AddRange(MakeRekviz(10, 6));
	Check->AddRange(MakeRekviz(5, 7));
	Check->AddRange(MakeRekviz(7, 8));
	Check->AddRange(MakeRekviz(8, 9));
	Check->AddRange(MakeRekviz(6, 10));
	Check->AddRange(MakeRekviz(99, 11, "Литры"));
	Check->AddRange(MakePrice11Rekviz(11, 12, volume, price, "литр"));
	Check->AddRange(MakeRekviz(12, 13));
	Check->AddRange(MakePrice13Rekviz(13, 14, accepted));
	Check->AddRange(MakeRekviz(14, 15));
	array<unsigned char>^ end = { 0x0B, 0xCC, 3 };
	Check->AddRange(end);
	array<unsigned char>^ Buff = Check->ToArray();
	String^ ss = Encoding::GetEncoding(866)->GetString(Buff);
	if (DebugBox != nullptr) DebugBox->Text += "\r\n > Check End=" + ss->Replace('\0', '_')->Replace("\3", "\r\n");
	AddBCC(Buff);
	PortWrite(Buff);
}

array<Byte>^ TMerc::MakeRekviz(int n, int ln) { // int n - номер рквизита, int ln - номер строки
	array<int> ^ not = { 5,6,7,8,12 };
	array<unsigned char>^ buf = { 0x30, 0x30 + n, 0, 0x38, 0x30, 0x32, 0x30, 0, 0x30, 0, 0, 0x30 + ln, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	if (Array::IndexOf(not, n) >= 0) buf[3] = 0x30;
	if (n > 9) {
		int ost = n % 10;
		int dec = (n - ost) / 10;
		buf[0] = 0x30 + dec;
		buf[1] = 0x30 + ost;
	}
	if (ln > 9) {
		int ost = ln % 10;
		int dec = (ln - ost) / 10;
		buf[11] = 0x30 + dec;
		buf[12] = 0x30 + ost;
	}
	List<Byte>^ Check = gcnew List<Byte>(0);
	Check->AddRange(buf);
	String^ merc = Encoding::GetEncoding(866)->GetString(Check->ToArray());
	if (DebugBox != nullptr) DebugBox->Text += "\r\nMakeRekviz(" + n + "):" + merc->Replace('\0', '_');
	return buf;
}
array<Byte>^ TMerc::MakeRekviz(int n, int ln, String^ str) { // str == Собственно реквизит -строка S 40 20H..FFH
	array<unsigned char>^ buf = { 0x39, 0x39, 0, 0x38, 0x30, 0x32, 0x30, 0, 0x30, 0,0, 0x30 + ln, 0,0,0,0,0,0,0,0,0,0,0 };
	if (ln > 9) {
		int ost = ln % 10;
		int dec = (ln - ost) / 10;
		buf[11] = 0x30 + dec;
		buf[12] = 0x30 + ost;
	}
	List<Byte>^ Check = gcnew List<Byte>(100);
	Check->AddRange(buf);
	if (str->Length > 40) str = str->Substring(0, 40);
	array<unsigned char>^ str866 = Encoding::GetEncoding(866)->GetBytes(str);
	Check->AddRange(str866);
	while (Check->Count < 61) Check->Add(0);
	return Check->ToArray();
}
array<Byte>^ TMerc::MakePrice11Rekviz(int n, int ln, double d1, double d2, String^ str) {
	if (DebugBox != nullptr) DebugBox->Text += "\r\n MakePrice11Rekviz(" + n + "," + ln + "," + d1 + "," + d2 + "," + str + "):";
	array<unsigned char>^ buf11 = {
0x31, 0x31, 0, 0x30, 0x30, 0x32, 0x32, 0, 0x30, 0, 0, 0x30 + ln, 0, 0, 0, 0x31, 0, 0, 0x30, 0,0,0,0,0,0,0,0,0,0,0,0,
0x30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x56, 0, 0, 0, 0, 0
	};
	if (ln > 9) {
		int ost = ln % 10;
		int dec = (ln - ost) / 10;
		buf11[11] = 0x30 + dec;
		buf11[12] = 0x30 + ost;
	}
	String^ d1str = String::Format("{0:0.00}", d1);
	String^ d2str = String::Format("{0:0.00}", d2);
	array<unsigned char>^ merc1 = Encoding::GetEncoding(866)->GetBytes(d1str);
	array<unsigned char>^ merc2 = Encoding::GetEncoding(866)->GetBytes(d2str);
	merc1->CopyTo(buf11, 31); // buf11[31] - begining of 'double d1'
	merc2->CopyTo(buf11, 43); // buf11[43] - begining of 'double d2'
	if (str->Length > 5) str = str->Substring(0, 5);
	array<unsigned char>^ mercS = Encoding::GetEncoding(866)->GetBytes(str);
	mercS->CopyTo(buf11, 55); // buf11[55] - begining of str

	List<Byte>^ Check = gcnew List<Byte>(70);
	Check->AddRange(buf11);

	array<unsigned char>^ merc866 = Encoding::GetEncoding(866)->GetBytes(str);
	while (Check->Count < 61) Check->Add(0);

	return Check->ToArray();
}
array<Byte>^ TMerc::MakePrice13Rekviz(int n, int ln, double d) {
	if (DebugBox != nullptr) DebugBox->Text += "\r\n MakePrice13Rekviz(" + n + "," + ln + "," + d + "):";
	array<unsigned char>^ buf13 = {
0x31, 0x33, 0, 0x30, 0x30, 0x32, 0x30, 0, 0x30, 0, 0, 0x30 + ln, 0, 0, 0, 0, 0, 0, 0, 0,
0x30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	if (ln > 9) {
		int ost = ln % 10;
		int dec = (ln - ost) / 10;
		buf13[11] = 0x30 + dec;
		buf13[12] = 0x30 + ost;
	}
	String^ dstr = String::Format("{0:0.00}", d); // d = 1234.56789; => 1234.57
	array<unsigned char>^ merc866 = Encoding::GetEncoding(866)->GetBytes(dstr);
	merc866->CopyTo(buf13, 20); // buf13[20] - begining of 'double d'

	List<Byte>^ Check = gcnew List<Byte>(70);
	Check->AddRange(buf13);
	while (Check->Count < 61) Check->Add(0);

	return Check->ToArray();
}

void TMerc::PrintO(String^ PortName, Byte n, Label^ rl) { // Common print otchet (Z,X)
	if (DebugBox != nullptr) DebugBox->Text += "\r\nTMerc::PrintZ(" + PortName + "," + n + ")";
	if ((n < 0) || (n > 3)) return;
	resLbl = rl;
	MakeComPort(PortName);

	array<unsigned char>^ comm = { 2, 0x5F, 0x30,0x30,0x30,0x30,0,
		//0x32,0, // X-Отчёт N 1 по кассирам
		0x30+n,0, // Тип отчёта В 1 30H-Z-Отчёт, 31H-X-Отчёт сводный, 32H-X-Отчёт по кассирам, 33H-X-Отчёт по секциям
		0x31,0x34,0, // Флаги B 2 см. Таб. 3.5.2.
		0x30,0x30,0,
		//0,0,0, // Номер кассира или секции N 2 00-99
		0x0B,0xCC, 3 };
	AddBCC(comm);
	if (DebugBox != nullptr) DebugBox->Text += "\r\n BCC Bytes: " + HexString(comm);
	PortWrite(comm);
}
void TMerc::PrintZ(String^ PortName) {
	if (DebugBox != nullptr) DebugBox->Text += "\r\nTMerc::PrintZ(" + PortName + ")";
	MakeComPort(PortName);

	array<unsigned char>^ comm = { 2, 0x5F, 0x30,0x30,0x30,0x30,0,
		0x30,0, // Тип отчёта В 1 30H-Z-Отчёт, 31H-X-Отчёт сводный, 32H-X-Отчёт по кассирам, 33H-X-Отчёт по секциям
		0x31,0x34,0, // Флаги B 2 см. Таб. 3.5.2.
		0,0,0, // Номер кассира или секции N 2 00-99
		0x0B,0xCC, 3 };
	AddBCC(comm);
	if (DebugBox != nullptr) DebugBox->Text += "\r\n BCC Bytes: " + HexString(comm);
	PortWrite(comm);
}

System::Void TMerc::RegisterOperator( String^ PortName, String^ opName, RichTextBox^ Out) {
	TMerc::SetOpParams( 2, opName);
	TMerc::MakeOperation( 1, PortName, Out);
}

// About BCC:
int TMerc::CalcBCC(array<unsigned char>^ buff, int strt, int end) { // BCC - B 2 - Тип Размер(байт)
	int c = 0;
	for (int i = strt; i < end; i++) { c += buff[i]; }
	return c;
}
String^ TMerc::CalcSBCC(array<unsigned char>^ buff, int strt, int end) { // String of BCC
	int bcc = CalcBCC(buff, strt, end);
	String^ s = Convert::ToString(bcc, 16);
	return s->Substring(s->Length - 2, 2)->ToUpper();
}
void TMerc::AddBCC(array<unsigned char>^& Buf) {
	// Buf format: { 0x02, <comand code>, 0x30, 0x30, 0x30, 0x30, 0x00, <data>, 0x00, 0x00, 0x03 }
	String^ _sbcc = CalcSBCC(Buf, 1, Buf->Length - 4);
	unsigned char _sbcc0 = _sbcc[0];
	unsigned char _sbcc1 = _sbcc[1];
	Buf[Buf->Length - 3] = _sbcc0;
	Buf[Buf->Length - 2] = _sbcc1;
}

} // namespace Mercury
