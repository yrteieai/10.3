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
        cout << "������ ��� ��� ����� �" << i + 1 << ":" << endl;

        cout << "�������: ";
        cin >> notes[i].surname;
        cin.get();
        cin.sync();

        cout << "��'�: ";
        cin >> notes[i].name;
        cin.get();
        cin.sync();

        do {
            cout << "����� ��������: ";
            cin >> notes[i].phone;
            cin.get();
            cin.sync();
            if (!isValidPhoneNumber(notes[i].phone)) {
                cout << "����������� ������ ������ ��������! ��������� �� ���." << endl;
            }
        } while (!isValidPhoneNumber(notes[i].phone));

        do {
            cout << "���� ���������� (����, �����, ��): ";
            cin >> notes[i].birthDate[0] >> notes[i].birthDate[1] >> notes[i].birthDate[2];
            cin.get();
            cin.sync();
            if (!isValidDate(notes[i].birthDate[0], notes[i].birthDate[1], notes[i].birthDate[2])) {
                cout << "���������� ���� ����������! ��������� �� ���." << endl;
            }
        } while (!isValidDate(notes[i].birthDate[0], notes[i].birthDate[1], notes[i].birthDate[2]));
    }
}

void printNotes(const Note* notes, int count) {
    cout << left << setw(15) << "�������" << setw(15) << "��'�" << setw(15) << "�������"
        << "���� ����������" << endl;
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
            cout << "�������� �����:" << endl;
            cout << "��'�: " << notes[i].name << ", ����� ��������: " << notes[i].phone
                << ", ���� ����������: " << notes[i].birthDate[0] << "/"
                << notes[i].birthDate[1] << "/" << notes[i].birthDate[2] << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "����� � �������� " << surname << " �� ��������." << endl;
    }
}

void saveToFile(Note* notes, const int count, const char* filename) {
    ofstream outFile(filename, ios::binary); 
    outFile.write((char*)&count, sizeof(count)); 
    for (int i = 0; i < count; i++) {
        outFile.write((char*)&notes[i], sizeof(Note)); 
    }
    outFile.close(); 
    cout << "��� �������� � ���� " << filename << endl;
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
    cout << "��� ������� � ����� " << filename << endl;
}


void menu() {
    int choice, count = 0;
    Note* notes = nullptr;
    string filename;

    do {
        cout << "\n����:\n";
        cout << "1. �������� �����\n";
        cout << "2. ��������� �����\n";
        cout << "3. ���������� �� ������� ��������\n";
        cout << "4. ����� �� ��������\n";
        cout << "5. ����� ����� � ����\n";
        cout << "6. ���������� ����� �� �����\n";
        cout << "0. �����\n";
        cout << "������� �����: ";

        while (!(cin >> choice) || choice < 0 || choice > 6) {
            cout << "������� ����. ��������� �� ���: ";
            cin.clear();
            cin.get();
            cin.sync();
        }

        switch (choice) {
        case 1:
            cout << "������ ������� ���: ";
            while (!(cin >> count) || count <= 0) {
                cout << "������ ��������. ������ ������ �����: ";
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
            cout << "��� ����������� �� ������� ��������." << endl;
            break;
        case 4: {
            string surname;
            cout << "������ ������� ��� ������: ";
            cin >> surname;
            cin.get();
            cin.sync();
            searchBySurname(notes, count, surname);
            break;
        }
        case 5: {
            cout << "������ ��'� ����� ��� ����������: ";
            cin >> filename;
            cin.get();
            cin.sync();
            saveToFile(notes, count, filename.c_str()); 
            break;
        }
        case 6: {
            cout << "������ ��'� ����� ��� ����������: ";
            cin >> filename;
            cin.get();
            cin.sync();
            loadFromFile(notes, count, filename.c_str()); 
            break;
        }

        case 0:
            cout << "���������� ��������." << endl;
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
