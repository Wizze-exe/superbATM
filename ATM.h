#pragma once
#include "includes.h"

using namespace std;

class ATM;

const string centerUnit(45, ' ');
COORD coord = { 0 , 0 };
fstream userInfo;

void frame();							//Чертит границы в консоли
void moveConsoleCursor(int x, int y);				//Двигает курсор консоли
void eraseLine();						//Стирает линию в консоли

class ATM
{
private:
	int balance;			//Баланс
	string bankAccount;		//Банковский счёт
	string fullName;		//Имя человечка
	string passport;		//Серия и номер паспорта
	string pin;			//пинкод

public:
	void createAccount();
	void mainMenu();
	void transactionMenu();

	void cashDeposit(ATM);

	void cashWithdraw(ATM);
	void showBalance(ATM);

	ATM loginCheck();
};

void frame() {
	moveConsoleCursor(50, 1);
	cout << "Bank" << endl;
	moveConsoleCursor(1, 2);
	for (size_t i = 0; i < 100; i++)
	{
		cout << "_";
	}
	
	moveConsoleCursor(1, 24);
	for (size_t i = 0; i < 100; i++)
	{
		cout << "_";
	}
	moveConsoleCursor(50, 26);
	cout << "Bank" << endl;
}

void eraseLine() {
	printf("\x1b[1F");
	printf("\x1b[2K");
}

void moveConsoleCursor(int x, int y) {
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ATM::mainMenu() {
	char option;
	system("cls");
	system("COLOR 2F");


	frame();
	moveConsoleCursor(48, 4);
	cout << "Main menu";
	moveConsoleCursor(3, 12);
	cout << "1 : Create account\t2: Login to account\t3 : Exit";

	option = _getch();
	switch (option) {
	case '1':
		createAccount();
	case '2':
		transactionMenu();
	case '3':
		exit(0);
	default:
		mainMenu();
	}
}

void ATM::transactionMenu() {
	ATM atm;
	atm = loginCheck();
	char option;
	system("cls");
	system("COLOR 3F");

	frame();
	moveConsoleCursor(44, 4);
	cout << "Transaction menu";
	moveConsoleCursor(29, 12);
	cout << "1 : Deposit cash\t\t2 : Show balance";
	moveConsoleCursor(29, 15);
	cout << "3 : Withdraw cash\t\t4 : Exit";

	option = _getch();
	switch (option)
	{
	case '1':
		cashDeposit(atm);
	case '2':
		showBalance(atm);
	case '3':
		cashWithdraw(atm);
	case '4':
		exit(1);
	default:
		mainMenu();
	}
}

void ATM::cashDeposit(ATM atm) {
	int cash;
	system("cls");
	system("COLOR 3F");

	frame();
	while (true) {
		moveConsoleCursor(29, 13);
		cout << "Enter the amount of deposit: ";
		cin >> cash;
		if (cash % 50 != 0) {
			moveConsoleCursor(29, 15);
			cout << "Entered incorrect amount. Must be a multiple of 50.";
		}
		else {
			atm.balance += cash;
			break;
		}
	}
	

}

void ATM::cashWithdraw(ATM atm) {
	system("cls");
	system("COLOR 3F");
}

void ATM::showBalance(ATM atm) {
	system("cls");
	system("COLOR 3F");
}

void ATM::createAccount() {
	ATM atm;
	system("cls");
	system("COLOR 3F");

	frame();

	moveConsoleCursor(10, 23);
	cout << "Errors: " << endl;

	atm.balance = 0;

	while (true) {						//Ввод полного имени(имя + фамилия)
		moveConsoleCursor(10, 5);
		cout << "Enter your full name: ";
		getline(cin, atm.fullName);
		if (atm.fullName.find(" ") != string::npos) {
			//если в строке есть пробел, то выходим из цикла и идём дальше
			break;
		}
		else {
			//иначе стирает введённые данные и выводит ошибку
			eraseLine();
			moveConsoleCursor(18, 23);
			cout << "fullName must be 2 words with space between it.";
		}
	}

	while (true) {						//Ввод номера и серии паспорта
		moveConsoleCursor(10, 7);
		cout << "Enter passport number: ";
		getline(cin, atm.passport);
		if (atm.passport.size() == 10) {
			break;
		}
		else {
			eraseLine();
			moveConsoleCursor(11, 15);
			eraseLine();
			moveConsoleCursor(18, 23);
			cout << "Passport must contain 10 digits.";
		}
	}


	moveConsoleCursor(10, 9);
	cout << "Enter pin: ";
	int i = 0;
	while (i != 6) {
		char charDigit = _getch();
		if (isdigit(charDigit)) {
			atm.pin += charDigit;
			i++;
			cout << '*';
		}
		else {
			moveConsoleCursor(10, 11);
			cout << "ONLY DIGITS!";
			moveConsoleCursor(21 + i, 9);
		}
	}

	//Создаём банковский счёт, 16 цифр случайным образом
	srand(time(0));
	for (int i = 0; i < 16; i++) {
		atm.bankAccount += to_string((rand() % 10));
	}

	userInfo.open("info.txt", fstream::out | fstream::app);
	userInfo << atm.pin << ' ' << atm.balance << ' ' << atm.passport << ' ' << atm.bankAccount << ' ' << atm.fullName << endl;
	userInfo.close();


	moveConsoleCursor(33, 13);
	cout << "Your account successfully created!";
	_getch();
	atm.mainMenu();
}

ATM ATM::loginCheck() {
	system("cls");
	system("COLOR 2F");
	ATM atm;
	string pinCheck;
	int enteredPin = 0;
	char charDigit;

	frame();
	moveConsoleCursor(46, 4);
	cout << "Authorization" << endl;

	//Ввод пинкода
	while (true) {
		pinCheck = "";
		moveConsoleCursor(35, 12);
		cout << "Enter pin: ";

		int i = 0;
		while (i != 6) {
			charDigit = _getch();
			if (isdigit(charDigit)) {
				pinCheck += charDigit;
				i++;
				cout << '*';
			}
			else {
				moveConsoleCursor(35, 14);
				cout << "ONLY DIGITS!";
				moveConsoleCursor(46 + i, 12);
			}
		}

		//Ищем совпадение введённого пин кода в данных файлика
		userInfo.open("info.txt", fstream::in);
		while (!userInfo.eof()) {
			while (userInfo >> atm.pin >> atm.balance >> atm.passport >> atm.bankAccount >> atm.fullName) {
				if (atm.pin == pinCheck) {
					return atm;
				}
				else {
					if (enteredPin == 3) {
						exit(1);
					}
					break;
				}
			}
		}
		enteredPin++;
		userInfo.close();
	}
}
