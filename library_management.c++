#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// ================= BOOK CLASS =================

class Book {
public:
    int bookId;
    string title;
    string author;
    bool issued;

    Book() {
        bookId = 0;
        title = "";
        author = "";
        issued = false;
    }

    Book(int id, string t, string a) {
        bookId = id;
        title = t;
        author = a;
        issued = false;
    }

    void save(ofstream &file) {
        file << bookId << "|"
             << title << "|"
             << author << "|"
             << issued << "\n";
    }

    bool load(ifstream &file) {
        string id, status;

        if (!getline(file, id, '|'))
            return false;

        if (!getline(file, title, '|'))
            return false;

        if (!getline(file, author, '|'))
            return false;

        if (!getline(file, status))
            return false;

        try {
            bookId = stoi(id);
            issued = (stoi(status) == 1);
        }
        catch (...) {
            return false;
        }

        return true;
    }

    void display() const {
        cout << "\n-------------------------------\n";
        cout << "Book ID : " << bookId << endl;
        cout << "Title   : " << title << endl;
        cout << "Author  : " << author << endl;
        cout << "Status  : " << (issued ? "Issued" : "Available") << endl;
        cout << "-------------------------------\n";
    }
};


// ================= MEMBER CLASS =================

class Member {
public:
    int memberId;
    string name;
    string phone;

    Member() {
        memberId = 0;
        name = "";
        phone = "";
    }

    Member(int id, string n, string p) {
        memberId = id;
        name = n;
        phone = p;
    }

    void save(ofstream &file) {
        file << memberId << "|"
             << name << "|"
             << phone << "\n";
    }

    bool load(ifstream &file) {
        string id;

        if (!getline(file, id, '|'))
            return false;

        if (!getline(file, name, '|'))
            return false;

        if (!getline(file, phone))
            return false;

        try {
            memberId = stoi(id);
        }
        catch (...) {
            return false;
        }

        return true;
    }

    void display() const {
        cout << "\n-------------------------------\n";
        cout << "Member ID : " << memberId << endl;
        cout << "Name      : " << name << endl;
        cout << "Phone     : " << phone << endl;
        cout << "-------------------------------\n";
    }
};


// ================= LIBRARY CLASS =================

class Library {
private:

    const string bookFile = "books.txt";
    const string memberFile = "members.txt";

    bool bookExists(int id) {
        ifstream file(bookFile);
        Book book;

        while (book.load(file)) {
            if (book.bookId == id)
                return true;
        }

        return false;
    }

    bool memberExists(int id) {
        ifstream file(memberFile);
        Member member;

        while (member.load(file)) {
            if (member.memberId == id)
                return true;
        }

        return false;
    }

    bool getBook(int id, Book &result) {
        ifstream file(bookFile);
        Book book;

        while (book.load(file)) {
            if (book.bookId == id) {
                result = book;
                return true;
            }
        }

        return false;
    }

    void updateBook(Book updatedBook) {

        ifstream input(bookFile);
        ofstream temp("temp_books.txt");

        Book book;

        while (book.load(input)) {

            if (book.bookId == updatedBook.bookId)
                updatedBook.save(temp);
            else
                book.save(temp);
        }

        input.close();
        temp.close();

        remove(bookFile.c_str());
        rename("temp_books.txt", bookFile.c_str());
    }


public:

    // ================= ADD BOOK =================

    void addBook() {

        int id;
        string title;
        string author;

        cout << "\n========== ADD BOOK ==========\n";

        cout << "Enter Book ID: ";
        cin >> id;

        if (bookExists(id)) {
            cout << "Book already exists!\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Book Title: ";
        getline(cin, title);

        cout << "Enter Author Name: ";
        getline(cin, author);

        Book book(id, title, author);

        ofstream file(bookFile, ios::app);

        if (!file) {
            cout << "Error opening file!\n";
            return;
        }

        book.save(file);
        file.close();

        cout << "\nBook added successfully!\n";
    }


    // ================= DISPLAY BOOKS =================

    void displayBooks() {

        ifstream file(bookFile);

        if (!file) {
            cout << "\nNo books found.\n";
            return;
        }

        Book book;
        bool found = false;

        cout << "\n========== ALL BOOKS ==========\n";

        while (book.load(file)) {
            book.display();
            found = true;
        }

        if (!found)
            cout << "No books found.\n";

        file.close();
    }


    // ================= SEARCH BOOK =================

    void searchBook() {

        int id;

        cout << "\nEnter Book ID: ";
        cin >> id;

        Book book;

        if (getBook(id, book)) {
            cout << "\nBook Found!";
            book.display();
        }
        else {
            cout << "\nBook not found!\n";
        }
    }


    // ================= ADD MEMBER =================

    void addMember() {

        int id;
        string name;
        string phone;

        cout << "\n========== ADD MEMBER ==========\n";

        cout << "Enter Member ID: ";
        cin >> id;

        if (memberExists(id)) {
            cout << "Member already exists!\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Member Name: ";
        getline(cin, name);

        cout << "Enter Phone Number: ";
        getline(cin, phone);

        Member member(id, name, phone);

        ofstream file(memberFile, ios::app);

        if (!file) {
            cout << "Error opening file!\n";
            return;
        }

        member.save(file);
        file.close();

        cout << "\nMember added successfully!\n";
    }


    // ================= DISPLAY MEMBERS =================

    void displayMembers() {

        ifstream file(memberFile);

        if (!file) {
            cout << "\nNo members found.\n";
            return;
        }

        Member member;
        bool found = false;

        cout << "\n========== ALL MEMBERS ==========\n";

        while (member.load(file)) {
            member.display();
            found = true;
        }

        if (!found)
            cout << "No members found.\n";

        file.close();
    }


    // ================= ISSUE BOOK =================

    void issueBook() {

        int bookId;
        int memberId;

        cout << "\n========== ISSUE BOOK ==========\n";

        cout << "Enter Book ID: ";
        cin >> bookId;

        Book book;

        if (!getBook(bookId, book)) {
            cout << "Book not found!\n";
            return;
        }

        if (book.issued) {
            cout << "Book is already issued!\n";
            return;
        }

        cout << "Enter Member ID: ";
        cin >> memberId;

        if (!memberExists(memberId)) {
            cout << "Member not found!\n";
            return;
        }

        book.issued = true;

        updateBook(book);

        cout << "\nBook issued successfully!\n";
    }


    // ================= RETURN BOOK =================

    void returnBook() {

        int bookId;

        cout << "\n========== RETURN BOOK ==========\n";

        cout << "Enter Book ID: ";
        cin >> bookId;

        Book book;

        if (!getBook(bookId, book)) {
            cout << "Book not found!\n";
            return;
        }

        if (!book.issued) {
            cout << "This book is not currently issued.\n";
            return;
        }

        book.issued = false;

        updateBook(book);

        cout << "\nBook returned successfully!\n";
    }
};


// ================= MAIN FUNCTION =================

int main() {

    Library library;

    int choice;

    do {

        cout << "\n\n========================================\n";
        cout << "       LIBRARY MANAGEMENT SYSTEM\n";
        cout << "========================================\n";

        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Search Book\n";
        cout << "4. Add Member\n";
        cout << "5. Display All Members\n";
        cout << "6. Issue Book\n";
        cout << "7. Return Book\n";
        cout << "8. Exit\n";

        cout << "========================================\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            library.addBook();
            break;

        case 2:
            library.displayBooks();
            break;

        case 3:
            library.searchBook();
            break;

        case 4:
            library.addMember();
            break;

        case 5:
            library.displayMembers();
            break;

        case 6:
            library.issueBook();
            break;

        case 7:
            library.returnBook();
            break;

        case 8:
            cout << "\nThank you for using Library Management System!\n";
            break;

        default:
            cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 8);

    return 0;
}