# class ATM
Консольный симулятор банкомата.
____

## Переменные класса ATM
<img src="https://user-images.githubusercontent.com/56793645/199517700-3b1d4527-1d51-42ad-bb26-9d6ed73ca134.png" alt="drawing" width="500"/>

**balance** - баланс пользователя банкомата.

**bankAccount** - банковский счёт.

**fullName** - имя и фамилия владельца счёта.

**passport** - серия и номер паспорта.

**pin** - пинкод для входа в банкомат.

## Вспомогательные методы
<img src="https://user-images.githubusercontent.com/56793645/199526105-4a2a518e-e27a-4a34-b5b1-81549f6acb24.png" alt="drawing" width="500"/>
Перед тем, как рассмотреть основные методы класса, изучим вспомогательные.

### upperFrame и downFrame
Два метода, которые в будущем стоит объединить в один. Чертят визуальные границы окна банкомата.
```cpp
string centerUnit(45, ' ');
void upperFrame() {
  cout << "\n" << centerUnit + "bank" + centerUnit << endl;
  for (int i = 0; i < 100; i++) {
    cout << "_";
  }
}
void downFrame() {
  cout << endl << endl;
  for (int i = 0; i < 100; i++) {
    cout << "_";
  }
  cout << "\n" + centerUnit + "bank" + centerUnit << endl;
}
```

### moveConsoleCursor
Метод перещает курсор консоли по координатам x и y. Для реализации необходимо `#include <Windows.h>`
```cpp
COORD coord = { 0, 0 };
void moveConsoleCursor(int x, int y) {
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
```

### eraseLine
Метод сначала перемещает курсор консоли в начало строки и затем стирает всю строчку. Делается с помощью ANSI escape code.
```cpp
void eraseLine() {
  printf("\x1b[1F");
  printf("\x1b[2K");
}
```

## Методы класса ATM

<img src="https://user-images.githubusercontent.com/56793645/199522142-2b15ece9-d12e-4efb-baea-c9c0fee30261.png" alt="drawing" width="500"/>
Перед выполнением каждого из методов, чистится консоль и меняется цвет фона. Рамка рисуется вспомогательными методами, то что находится между рамками в каждом методе отличается, сниппет начала следующего метода как пример:

```cpp
system("cls");
system("COLOR 2F");
upperFrame();
cout << "\n" + centerUnit + "Main menu" + "\n\n";
cout << "1 : Create account\t2 : Login to account\t3 : Exit";
cout << "\n\n" << endl;
downFrame();
```


### mainMenu()
Это главное меню с которого запускается программа. В нём есть три опции: перейти к регистрации аккаунта, залогиниться в аккаунт и закрыть программу.

```cpp
char option;
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
```

### createAccount()
Суть метода в создании и заполнении объекта класса информацией о пользователе. Заполниться должны все переменные класса ATM. Метод разделён на логические модули, каждый из которых отвечает за заполнение одной из переменных. В самом начале метода создаём объект класса: `ATM atm;`

#### fullName
Запрашиваем у пользователя фамилию и имя. Если в полученной строке есть пробел(функция `find`), то выходим из цикла, иначе стираем линию и печатаем на экран ошибку.
```cpp
while (true) {
  moveConsoleCursor(10, 5);
  cout << "Enter your full name: ";
  getline(cin, atm.fullName);
  if (atm.fullName.find(" ") != string::npos) {
    break;
  }
  else {
    eraseLine();
    moveConsoleCursor(18, 15);
    cout << "fullName must be 2 words with space between it.";
  }
}
```

#### passport
Принцип такой же как и у `fullName`, но в условии смотрим на размер строки функцией `size`. Если он равен 10, то выходим из цикла.
```cpp
while (true) {
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
```

#### pin
Чтобы получить пинкод, добавляем счётчик i. Пока этот счётчик не равен 6(количеству символов в пинкоде), то запрашиваем у пользователя по одному символу функцией `_getch`. Полученный символ должен быть цифрой, проверяем функцией `isdigit`. Если это цифра то добавляем её к строке pin и инкрементируем счётчик, иначе выводим ошибку на экран.
```cpp
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
```

#### bankAccount
С банковским счётом всё просто. Генерируем случайные цифры от 1 до 9 и добавляем их к строке.
```cpp
srand(time(0));
for (int i = 0; i < 16; i++) {
  atm.bankAccount += to_string((rand() % 10));
}
```

#### fstream
Полученные данные добавляем в файл `info.txt` с помощью файлового потока `fstream`. Открываем поток с модами `fstream::out | fstream::app`. Out позволяет записывать данные в файл, а app осуществляет все операции записи в конце файла, добавляя их к уже существующим записям. Не забываем закрывать потоки. `userInfo` - название файлового потока.
```cpp
userInfo.open("info.txt", fstream::out | fstream::app);
userInfo << atm.pin << ' ' << atm.balance << ' ' << atm.passport << ' ' << atm.bankAccount << ' ' << atm.fullName << endl;
userInfo.close();
```

### ATM loginCheck()
Этот метод проверяет введённый пинкод `pinCheck` с пинкодами, содержащимися в файле. Если найдено совпадение, то записываем все данные этого пользователя в объект класса и возвращаем его.

Открываем поток на чтение, мод `fstream::in`. Данные считываются через пробел и записываются в переменные объекта класса. Есть проверка на количество введённых пинкодов(если больше 3, то программа закрывается).
Метод требует доработки.

```cpp
userInfo.open("info.txt", fstream::in);
while (userInfo >> atm.pin >> atm.balance >> atm.passport >> atm.bankAccount >> atm.fullName) {
  if (atm.pin == pinCheck) {
    return atm;
  }
  else {
    if (enteredPin == 3) {
      exit(1);
    }
    else {
      eraseLine();
      moveConsoleCursor(35, 8);
      cout << "Enter pin: ";
    }
    break;
  }
}
enteredPin++;
userInfo.close();
```
