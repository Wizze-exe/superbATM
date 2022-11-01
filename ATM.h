#pragma once
#include "includes.h"

using namespace std;

class ATM;

const string centerUnit(45, ' ');
COORD coord = { 0 , 0 };
fstream userInfo;

void upperFrame();						//Рисует верхнюю рамочку
void downFrame();						//Рисует нижнюю рамочку
void moveConsoleCursor(int x, int y);	//Двигает курсор консоли
void eraseLine();						//Стирает линию в консоли

class ATM
{
private:
	int balance;			//Баланс
	string bankAccount;		//Банковский счёт
	string fullName;		//Имя человечка
	string passport;		//Серия и номер паспорта
	string pin;				//пинкод

public:
	void createAccount();
	void mainMenu();
	void transactionMenu();

	void cashDeposit(ATM);

	void cashWithdraw(ATM);
	void showBalance(ATM);

	ATM loginCheck();
};

void upperFrame() {
	cout << "\n" << centerUnit + "Sberbank" + centerUnit << endl;
	for (int i = 0; i < 100; i++) {
		cout << "_";
	}
}

void downFrame() {
	cout << endl << endl;
	for (int i = 0; i < 100; i++) {
		cout << "_";
	}
	cout << "\n" + centerUnit + "Sberbank" + centerUnit;
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

	upperFrame();
	cout << "\n" + centerUnit + "Main menu" + "\n\n";
	cout << "1 : Create account\t2 : Login to account\t3 : Exit";
	cout << "\n\n" << endl;
	downFrame();

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
	system("cls");
	system("COLOR 3F");


	upperFrame();
	cout << "\n\n\n\n\n\n\n\n\n\n\n" << endl;
	downFrame();
	getchar();
}

void ATM::createAccount() {
	ATM atm;
	system("cls");
	system("COLOR 3F");

	upperFrame();
	cout << "\n\n\n\n\n\n\n\n\n\n\n" << endl;
	downFrame();

	moveConsoleCursor(10, 15);
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
			moveConsoleCursor(18, 15);
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
			moveConsoleCursor(18, 15);
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
	string userInfoValue;

	upperFrame();
	cout << "\n\n" + centerUnit << "\b\bAUTHORIZATION" << endl;
	cout << "\n\n\n\n\n\n\n\n\n\n";
	downFrame();

	//Ввод пинкода
	while (true) {
		moveConsoleCursor(35, 8);
		cout << "Enter pin: ";

		int i = 0;
		while (i != 6) {
			char charDigit = _getch();
			if (isdigit(charDigit)) {
				pinCheck += charDigit;
				i++;
				cout << '*';
			}
			else {
				moveConsoleCursor(35, 10);
				cout << "ONLY DIGITS!";
				moveConsoleCursor(46 + i, 8);
			}
		}

		//Ищем совпадение введённого пин кода в данных файлика
		userInfo.open("info.txt", fstream::in);
		while (userInfo >> userInfoValue) {
			if (userInfoValue == pinCheck) {
				return atm;
			}
			else {
				if (enteredPin == 3) {
					exit(1);
				}
				//Здесь чёто не работает:
				/*else {
					eraseLine();
					moveConsoleCursor(35, 8);
					cout << "Enter pin: ";
				}*/
				break;
			}
		}
		enteredPin++;
		userInfo.close();
	}
}