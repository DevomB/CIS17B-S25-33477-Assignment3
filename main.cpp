#include <iostream>
#include <memory>
#include <stdexcept>

// Define custom exception classes
class NegativeDepositException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: Cannot deposit a negative amount!";
    }
};

class OverdrawException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: Insufficient funds!";
    }
};

class InvalidAccountOperationException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: Account is closed for transactions!";
    }
};

// BankAccount class definition
class BankAccount {
private:
    std::string accountNumber;
    double balance;
    bool isActive;

public:
    // Constructor to initialize account number, balance, and active status
    BankAccount(std::string accNum, double initialBalance) : accountNumber(accNum), balance(initialBalance), isActive(true) {
        std::cout << "Bank Account Created: #" << accountNumber << std::endl;
    }

    // Deposit method
    void deposit(double amount) {
        if (amount < 0) {
            throw NegativeDepositException();
        }
        if (!isActive) {
            throw InvalidAccountOperationException();
        }
        balance += amount;
        std::cout << "Depositing $" << amount << "..." << std::endl;
    }

    // Withdraw method
    void withdraw(double amount) {
        if (!isActive) {
            throw InvalidAccountOperationException();
        }
        if (amount > balance) {
            throw OverdrawException();
        }
        balance -= amount;
        std::cout << "Withdrawing $" << amount << "..." << std::endl;
    }

    // Method to get current balance
    double getBalance() const {
        return balance;
    }

    // Method to close the account
    void closeAccount() {
        isActive = false;
        std::cout << "Closing Account..." << std::endl;
    }
};

int main() {
    try {
        // Create a BankAccount object using smart pointers
        auto account = std::make_unique<BankAccount>("123456", 1000.0);

        // Perform valid transactions
        account->deposit(1000);
        account->withdraw(500);
        std::cout << "Current Balance: $" << account->getBalance() << std::endl;

        // Try withdrawing more than the available balance
        std::cout << "Attempting to withdraw $600..." << std::endl;
        account->withdraw(600); // This should trigger an exception

    } 
    catch (const std::exception& e) {
        // Catch and handle exceptions properly
        std::cerr << e.what() << std::endl;
    }

    try {
        // Attempt to deposit a negative amount
        auto account = std::make_unique<BankAccount>("7891011", 500.0);
        account->deposit(-50);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    try {
        // Close the account and try a transaction
        auto account = std::make_unique<BankAccount>("111213", 1000.0);
        account->closeAccount();
        account->deposit(100);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
