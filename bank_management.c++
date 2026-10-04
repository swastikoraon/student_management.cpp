#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

class Account {
private:
    int accountNumber;
    string name;
    string accountType;
    double balance;

public:
    Account() {
        accountNumber = 0;
        name = "";
        accountType = "";
        balance = 0.0;
    }

    Account(int accNo, string n, string type, double bal) {
        accountNumber = accNo;
        name = n;
        accountType = type;
        balance = bal;
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (amount <= 0 || amount > balance) {
            return false;
        }

        balance -= amount;
        return true;
    }

    void display() const {
        cout << "\n----------------------------------------\n";
        cout << "Account Number : " << accountNumber << endl;
        cout << "Account Holder : " << name << endl;
        cout << "Account Type   : " << accountType << endl;
        cout << fixed << setprecision(2);
        cout << "Balance        : Rs. " << balance << endl;
        cout << "----------------------------------------\n";
    }

    void save(ofstream &file) const {
        file << accountNumber << "|"
             << name << "|"
             << accountType << "|"
             << fixed << setprecision(2) << balance << "\n";
    }

    bool load(ifstream &file) {
        string accNo, bal;

        if (!getline(file, accNo, '|'))
            return false;

        if (!getline(file, name, '|'))
            return false;

        if (!getline(file, accountType, '|'))
            return false;

        if (!getline(file, bal))
            return false;

        try {
            accountNumber = stoi(accNo);
            balance = stod(bal);
        }
        catch (...) {
            return false;
        }

        return true;
    }
};

class Bank {
private:
    const string fileName = "accounts.txt";

    bool accountExists(int accountNumber) {
        ifstream file(fileName);

        Account acc;

        while (acc.load(file)) {
            if (acc.getAccountNumber() == accountNumber) {
                return true;
            }
        }

        return false;
    }

    bool findAccount(int accountNumber, Account &result) {
        ifstream file(fileName);

        Account acc;

        while (acc.load(file)) {
            if (acc.getAccountNumber() == accountNumber) {
                result = acc;
                return true;
            }
        }

        return false;
    }

    void saveAll(Account updatedAccount, int accountNumber) {
        ifstream input(fileName);
        ofstream temp("temp.txt");

        Account acc;

        while (acc.load(input)) {
            if (acc.getAccountNumber() == accountNumber) {
                updatedAccount.save(temp);
            } else {
                acc.save(temp);
            }
        }

        input.close();
        temp.close();

        remove(fileName.c_str());
        rename("temp.txt", fileName.c_str());
    }

public:

    void createAccount() {
        int accountNumber;
        string name;
        string accountType;
        double initialBalance;

        cout << "\n========== CREATE ACCOUNT ==========\n";

        cout << "Enter Account Number: ";
        cin >> accountNumber;

        if (accountExists(accountNumber)) {
            cout << "Account already exists!\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Account Holder Name: ";
        getline(cin, name);

        cout << "Enter Account Type (Savings/Current): ";
        getline(cin, accountType);

        cout << "Enter Initial Balance: Rs. ";
        cin >> initialBalance;

        if (initialBalance < 0) {
            cout << "Invalid balance!\n";
            return;
        }

        Account acc(accountNumber, name, accountType, initialBalance);

        ofstream file(fileName, ios::app);

        if (!file) {
            cout << "Error opening file!\n";
            return;
        }

        acc.save(file);
        file.close();

        cout << "\nAccount created successfully!\n";
    }

    void displayAllAccounts() {
        ifstream file(fileName);

        if (!file) {
            cout << "\nNo accounts found.\n";
            return;
        }

        Account acc;
        bool found = false;

        cout << "\n========== ALL ACCOUNTS ==========\n";

        while (acc.load(file)) {
            acc.display();
            found = true;
        }

        if (!found) {
            cout << "No accounts found.\n";
        }

        file.close();
    }

    void searchAccount() {
        int accountNumber;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;

        Account acc;

        if (findAccount(accountNumber, acc)) {
            cout << "\nAccount Found!";
            acc.display();
        } else {
            cout << "\nAccount not found!\n";
        }
    }

    void depositMoney() {
        int accountNumber;
        double amount;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;

        Account acc;

        if (!findAccount(accountNumber, acc)) {
            cout << "Account not found!\n";
            return;
        }

        cout << "Enter Deposit Amount: Rs. ";
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid amount!\n";
            return;
        }

        acc.deposit(amount);

        saveAll(acc, accountNumber);

        cout << "\nAmount deposited successfully!\n";
        cout << "New Balance: Rs. "
             << fixed << setprecision(2)
             << acc.getBalance() << endl;
    }

    void withdrawMoney() {
        int accountNumber;
        double amount;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;

        Account acc;

        if (!findAccount(accountNumber, acc)) {
            cout << "Account not found!\n";
            return;
        }

        cout << "Enter Withdrawal Amount: Rs. ";
        cin >> amount;

        if (!acc.withdraw(amount)) {
            cout << "\nInsufficient balance or invalid amount!\n";
            return;
        }

        saveAll(acc, accountNumber);

        cout << "\nAmount withdrawn successfully!\n";
        cout << "Remaining Balance: Rs. "
             << fixed << setprecision(2)
             << acc.getBalance() << endl;
    }

    void checkBalance() {
        int accountNumber;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;

        Account acc;

        if (findAccount(accountNumber, acc)) {
            cout << "\nCurrent Balance: Rs. "
                 << fixed << setprecision(2)
                 << acc.getBalance() << endl;
        } else {
            cout << "Account not found!\n";
        }
    }
};

int main() {

    Bank bank;
    int choice;

    do {
        cout << "\n\n====================================\n";
        cout << "       BANK MANAGEMENT SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Create Account\n";
        cout << "2. Display All Accounts\n";
        cout << "3. Search Account\n";
        cout << "4. Deposit Money\n";
        cout << "5. Withdraw Money\n";
        cout << "6. Check Balance\n";
        cout << "7. Exit\n";
        cout << "====================================\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bank.createAccount();
            break;

        case 2:
            bank.displayAllAccounts();
            break;

        case 3:
            bank.searchAccount();
            break;

        case 4:
            bank.depositMoney();
            break;

        case 5:
            bank.withdrawMoney();
            break;

        case 6:
            bank.checkBalance();
            break;

        case 7:
            cout << "\nThank you for using Bank Management System!\n";
            break;

        default:
            cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 7);

    return 0;
}