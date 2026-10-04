#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

const string FILE_NAME = "students.dat";

struct Student {
    int rollNo;
    string name;
    int age;
    string gender;
    string course;
    string phone;
    string email;
};

// Function declarations
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void showStudent(const Student& student);
bool rollExists(int rollNo);
void clearInput();

// -------------------- ADD STUDENT --------------------

void addStudent() {
    Student s;

    cout << "\n========== ADD STUDENT ==========\n";

    cout << "Enter Roll Number: ";
    cin >> s.rollNo;

    if (rollExists(s.rollNo)) {
        cout << "\nStudent with this Roll Number already exists!\n";
        return;
    }

    clearInput();

    cout << "Enter Name: ";
    getline(cin, s.name);

    cout << "Enter Age: ";
    cin >> s.age;
    clearInput();

    cout << "Enter Gender: ";
    getline(cin, s.gender);

    cout << "Enter Course: ";
    getline(cin, s.course);

    cout << "Enter Phone: ";
    getline(cin, s.phone);

    cout << "Enter Email: ";
    getline(cin, s.email);

    ofstream file(FILE_NAME, ios::binary | ios::app);

    if (!file) {
        cout << "\nError opening file!\n";
        return;
    }

    file.write(reinterpret_cast<char*>(&s), sizeof(Student));

    file.close();

    cout << "\nStudent added successfully!\n";
}

// -------------------- DISPLAY STUDENTS --------------------

void displayStudents() {
    Student s;

    ifstream file(FILE_NAME, ios::binary);

    if (!file) {
        cout << "\nNo student records found.\n";
        return;
    }

    cout << "\n==================== ALL STUDENTS ====================\n";

    cout << left
         << setw(10) << "Roll No"
         << setw(22) << "Name"
         << setw(8) << "Age"
         << setw(12) << "Gender"
         << setw(18) << "Course"
         << setw(16) << "Phone"
         << setw(25) << "Email"
         << endl;

    cout << string(111, '-') << endl;

    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        found = true;

        cout << left
             << setw(10) << s.rollNo
             << setw(22) << s.name
             << setw(8) << s.age
             << setw(12) << s.gender
             << setw(18) << s.course
             << setw(16) << s.phone
             << setw(25) << s.email
             << endl;
    }

    file.close();

    if (!found) {
        cout << "\nNo student records available.\n";
    }
}

// -------------------- SEARCH STUDENT --------------------

void searchStudent() {
    int rollNo;
    Student s;
    bool found = false;

    cout << "\n========== SEARCH STUDENT ==========\n";

    cout << "Enter Roll Number: ";
    cin >> rollNo;

    ifstream file(FILE_NAME, ios::binary);

    if (!file) {
        cout << "\nNo student records found.\n";
        return;
    }

    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {

        if (s.rollNo == rollNo) {
            cout << "\nStudent Found!\n";
            showStudent(s);
            found = true;
            break;
        }
    }

    file.close();

    if (!found) {
        cout << "\nStudent with Roll Number "
             << rollNo << " not found.\n";
    }
}

// -------------------- SHOW SINGLE STUDENT --------------------

void showStudent(const Student& s) {

    cout << "\n----------------------------------\n";
    cout << "Roll Number : " << s.rollNo << endl;
    cout << "Name        : " << s.name << endl;
    cout << "Age         : " << s.age << endl;
    cout << "Gender      : " << s.gender << endl;
    cout << "Course      : " << s.course << endl;
    cout << "Phone       : " << s.phone << endl;
    cout << "Email       : " << s.email << endl;
    cout << "----------------------------------\n";
}

// -------------------- UPDATE STUDENT --------------------

void updateStudent() {

    int rollNo;
    Student s;

    cout << "\n========== UPDATE STUDENT ==========\n";

    cout << "Enter Roll Number to update: ";
    cin >> rollNo;

    fstream file(FILE_NAME,
                 ios::binary |
                 ios::in |
                 ios::out);

    if (!file) {
        cout << "\nNo student records found.\n";
        return;
    }

    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {

        if (s.rollNo == rollNo) {

            found = true;

            cout << "\nCurrent Student Details:";
            showStudent(s);

            clearInput();

            cout << "\nEnter New Name: ";
            getline(cin, s.name);

            cout << "Enter New Age: ";
            cin >> s.age;
            clearInput();

            cout << "Enter New Gender: ";
            getline(cin, s.gender);

            cout << "Enter New Course: ";
            getline(cin, s.course);

            cout << "Enter New Phone: ";
            getline(cin, s.phone);

            cout << "Enter New Email: ";
            getline(cin, s.email);

            // Move pointer back to beginning of current record
            streampos position =
                file.tellg() - static_cast<streamoff>(sizeof(Student));

            file.seekp(position);

            file.write(reinterpret_cast<char*>(&s), sizeof(Student));

            file.close();

            cout << "\nStudent updated successfully!\n";
            break;
        }
    }

    if (!found) {
        file.close();
        cout << "\nStudent not found!\n";
    }
}

// -------------------- DELETE STUDENT --------------------

void deleteStudent() {

    int rollNo;
    Student s;

    cout << "\n========== DELETE STUDENT ==========\n";

    cout << "Enter Roll Number to delete: ";
    cin >> rollNo;

    ifstream inputFile(FILE_NAME, ios::binary);

    if (!inputFile) {
        cout << "\nNo student records found.\n";
        return;
    }

    ofstream tempFile("temp.dat", ios::binary);

    bool found = false;

    while (inputFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {

        if (s.rollNo == rollNo) {
            found = true;
        } else {
            tempFile.write(reinterpret_cast<char*>(&s), sizeof(Student));
        }
    }

    inputFile.close();
    tempFile.close();

    if (found) {
        remove(FILE_NAME.c_str());
        rename("temp.dat", FILE_NAME.c_str());

        cout << "\nStudent deleted successfully!\n";
    } else {
        remove("temp.dat");

        cout << "\nStudent not found!\n";
    }
}

// -------------------- CHECK ROLL NUMBER --------------------

bool rollExists(int rollNo) {

    Student s;

    ifstream file(FILE_NAME, ios::binary);

    if (!file) {
        return false;
    }

    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {

        if (s.rollNo == rollNo) {
            file.close();
            return true;
        }
    }

    file.close();

    return false;
}

// -------------------- CLEAR INPUT --------------------

void clearInput() {

    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );
}

// -------------------- MAIN MENU --------------------

int main() {

    int choice;

    do {

        cout << "\n\n";
        cout << "=============================================\n";
        cout << "        STUDENT MANAGEMENT SYSTEM\n";
        cout << "=============================================\n";

        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Exit\n";

        cout << "=============================================\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

            case 1:
                addStudent();
                break;

            case 2:
                displayStudents();
                break;

            case 3:
                searchStudent();
                break;

            case 4:
                updateStudent();
                break;

            case 5:
                deleteStudent();
                break;

            case 6:
                cout << "\nThank you for using Student Management System!\n";
                break;

            default:
                cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 6);

    return 0;
}