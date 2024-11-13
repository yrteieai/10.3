#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <Windows.h>

using namespace std;

struct Note {
    string surname;
    string name;
    string phone;
    int birthDate[3];
};

bool isValidPhoneNumber(const string& phone) {
    for (char ch : phone) {
        if (!isdigit(ch)) return false;
    }
    return phone.length() >= 10;
}

bool isValidDate(int day, int month, int year) {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > 28 + isLeap) return false;
    }
    return true;
}

void inputNotes(Note* notes, int count) {
    for (int i = 0; i < count; i++) {
        cout << "Введіть дані для особи №" << i + 1 << ":" << endl;

        cout << "Прізвище: ";
        cin >> notes[i].surname;
        cin.get();
        cin.sync();

        cout << "Ім'я: ";
        cin >> notes[i].name;
        cin.get();
        cin.sync();

        do {
            cout << "Номер телефону: ";
            cin >> notes[i].phone;
            cin.get();
            cin.sync();
            if (!isValidPhoneNumber(notes[i].phone)) {
                cout << "Некоректний формат номера телефону! Спробуйте ще раз." << endl;
            }
        } while (!isValidPhoneNumber(notes[i].phone));

        do {
            cout << "Дата народження (день, місяць, рік): ";
            cin >> notes[i].birthDate[0] >> notes[i].birthDate[1] >> notes[i].birthDate[2];
            cin.get();
            cin.sync();
            if (!isValidDate(notes[i].birthDate[0], notes[i].birthDate[1], notes[i].birthDate[2])) {
                cout << "Некоректна дата народження! Спробуйте ще раз." << endl;
            }
        } while (!isValidDate(notes[i].birthDate[0], notes[i].birthDate[1], notes[i].birthDate[2]));
    }
}

void printNotes(const Note* notes, int count) {
    cout << left << setw(15) << "Прізвище" << setw(15) << "Ім'я" << setw(15) << "Телефон"
        << "Дата народження" << endl;
    for (int i = 0; i < count; i++) {
        cout << setw(15) << notes[i].surname << setw(15) << notes[i].name
            << setw(15) << notes[i].phone
            << notes[i].birthDate[0] << "/" << notes[i].birthDate[1] << "/"
            << notes[i].birthDate[2] << endl;
    }
}

void sortNotes(Note* notes, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (notes[j].phone > notes[j + 1].phone) {
                Note temp = notes[j];
                notes[j] = notes[j + 1];
                notes[j + 1] = temp;
            }
        }
    }
}

void searchBySurname(const Note* notes, int count, const string& surname) {
    bool found = false;
    for (int i = 0; i < count; i++) {
        if (notes[i].surname == surname) {
            cout << "Знайдено особу:" << endl;
            cout << "Ім'я: " << notes[i].name << ", Номер телефону: " << notes[i].phone
                << ", Дата народження: " << notes[i].birthDate[0] << "/"
                << notes[i].birthDate[1] << "/" << notes[i].birthDate[2] << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Особа з прізвищем " << surname << " не знайдена." << endl;
    }
}

void saveToFile(Note* notes, const int count, const char* filename) {
    ofstream outFile(filename, ios::binary); 
    outFile.write((char*)&count, sizeof(count)); 
    for (int i = 0; i < count; i++) {
        outFile.write((char*)&notes[i], sizeof(Note)); 
    }
    outFile.close(); 
    cout << "Дані записано у файл " << filename << endl;
}

void loadFromFile(Note*& notes, int& count, const char* filename) {
    delete[] notes;
    ifstream inFile(filename, ios::binary); 
    inFile.read((char*)&count, sizeof(count)); 
    notes = new Note[count]; 
    for (int i = 0; i < count; i++) {
        inFile.read((char*)&notes[i], sizeof(Note));
    }
    inFile.close();
    cout << "Дані зчитано з файлу " << filename << endl;
}


void menu() {
    int choice, count = 0;
    Note* notes = nullptr;
    string filename;

    do {
        cout << "\nМеню:\n";
        cout << "1. Введення даних\n";
        cout << "2. Виведення даних\n";
        cout << "3. Сортування за номером телефону\n";
        cout << "4. Пошук за прізвищем\n";
        cout << "5. Запис даних у файл\n";
        cout << "6. Зчитування даних із файлу\n";
        cout << "0. Вихід\n";
        cout << "Виберіть опцію: ";

        while (!(cin >> choice) || choice < 0 || choice > 6) {
            cout << "Невірний вибір. Спробуйте ще раз: ";
            cin.clear();
            cin.get();
            cin.sync();
        }

        switch (choice) {
        case 1:
            cout << "Введіть кількість осіб: ";
            while (!(cin >> count) || count <= 0) {
                cout << "Невірне значення. Введіть додатнє число: ";
                cin.clear();
                cin.get();
                cin.sync();
            }
            delete[] notes;
            notes = new Note[count];
            inputNotes(notes, count);
            break;
        case 2:
            printNotes(notes, count);
            break;
        case 3:
            sortNotes(notes, count);
            cout << "Дані відсортовано за номером телефону." << endl;
            break;
        case 4: {
            string surname;
            cout << "Введіть прізвище для пошуку: ";
            cin >> surname;
            cin.get();
            cin.sync();
            searchBySurname(notes, count, surname);
            break;
        }
        case 5: {
            cout << "Введіть ім'я файлу для збереження: ";
            cin >> filename;
            cin.get();
            cin.sync();
            saveToFile(notes, count, filename.c_str()); 
            break;
        }
        case 6: {
            cout << "Введіть ім'я файлу для зчитування: ";
            cin >> filename;
            cin.get();
            cin.sync();
            loadFromFile(notes, count, filename.c_str()); 
            break;
        }

        case 0:
            cout << "Завершення програми." << endl;
            break;
        }
    } while (choice != 0);

    delete[] notes;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    menu();
    return 0;
}
