#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class Student {
    int rollNo;
    char name[50];
    int marks[5]; // marks of 5 subjects
    float percentage;
    char grade;

public:
    void input();
    void show() const;
    int getRollNo() const;
    void calculate();
};

void Student::input() {
    cout << "\nEnter Roll No: ";
    cin >> rollNo;
    cin.ignore();
    cout << "Enter Name: ";
    cin.getline(name, 50);
    cout << "Enter marks of 5 subjects:\n";
    for (int i = 0; i < 5; i++) {
        cout << "Subject " << i+1 << ": ";
        cin >> marks[i];
    }
    calculate();
}

void Student::calculate() {
    int total = 0;
    for (int i = 0; i < 5; i++)
        total += marks[i];
    percentage = total / 5.0;

    if (percentage >= 90)
        grade = 'A';
    else if (percentage >= 80)
        grade = 'B';
    else if (percentage >= 70)
        grade = 'C';
    else if (percentage >= 60)
        grade = 'D';
    else
        grade = 'F';
}

void Student::show() const {
    cout << "\nRoll No: " << rollNo
         << "\nName: " << name
         << "\nMarks: ";
    for (int i = 0; i < 5; i++)
        cout << marks[i] << " ";
    cout << "\nPercentage: " << percentage
         << "\nGrade: " << grade << "\n";
}

int Student::getRollNo() const {
    return rollNo;
}

// Function to write student to file
void writeStudent() {
    Student s;
    ofstream outFile("student.dat", ios::binary | ios::app);
    s.input();
    outFile.write(reinterpret_cast<char*>(&s), sizeof(Student));
    outFile.close();
    cout << "\nStudent record added successfully!";
}

// Function to display all students
void displayAll() {
    Student s;
    ifstream inFile("student.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        s.show();
        cout << "----------------------\n";
    }
    inFile.close();
}

// Function to search student by roll number
void searchStudent(int roll) {
    Student s;
    bool found = false;
    ifstream inFile("student.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.getRollNo() == roll) {
            s.show();
            found = true;
            break;
        }
    }
    inFile.close();
    if (!found)
        cout << "\nStudent not found!";
}

// Function to delete student
void deleteStudent(int roll) {
    Student s;
    ifstream inFile("student.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.getRollNo() != roll)
            outFile.write(reinterpret_cast<char*>(&s), sizeof(Student));
        else
            found = true;
    }
    inFile.close();
    outFile.close();

    remove("student.dat");
    rename("temp.dat", "student.dat");

    if (found)
        cout << "\nStudent record deleted.";
    else
        cout << "\nStudent not found.";
}

// Function to modify student
void modifyStudent(int roll) {
    Student s;
    fstream file("student.dat", ios::binary | ios::in | ios::out);
    bool found = false;
    while (!file.eof() && !found) {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char*>(&s), sizeof(Student));
        if (s.getRollNo() == roll) {
            cout << "\nCurrent details:\n";
            s.show();
            cout << "\nEnter new details:\n";
            s.input();
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&s), sizeof(Student));
            cout << "\nRecord updated successfully!";
            found = true;
        }
    }
    file.close();
    if (!found)
        cout << "\nStudent not found.";
}

void menu() {
    int choice, roll;
    do {
        cout << "\n===== Student Result Management =====";
        cout << "\n1. Add Student";
        cout << "\n2. Display All Students";
        cout << "\n3. Search Student by Roll No";
        cout << "\n4. Delete Student";
        cout << "\n5. Modify Student";
        cout << "\n6. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: writeStudent(); break;
            case 2: displayAll(); break;
            case 3:
                cout << "\nEnter Roll No: ";
                cin >> roll;
                searchStudent(roll);
                break;
            case 4:
                cout << "\nEnter Roll No to delete: ";
                cin >> roll;
                deleteStudent(roll);
                break;
            case 5:
                cout << "\nEnter Roll No to modify: ";
                cin >> roll;
                modifyStudent(roll);
                break;
            case 6:
                cout << "\nExiting...";
                break;
            default:
                cout << "\nInvalid choice. Try again!";
        }

    } while (choice != 6);
}

// Main Function
int main() {
    menu();
    return 0;
}