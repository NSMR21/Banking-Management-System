#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>

// Account structure
typedef struct {
    int accountNumber;
    char name[50];
    char pin[5]; // Simple PIN storage (not secure for real applications)
    float balance;
    int isActive;
} Account;

// Transaction structure
typedef struct {
    int accountNumber;
    char type[20]; // "Deposit", "Withdrawal", "Transfer"
    float amount;
    time_t timestamp;
} Transaction;

// Global variables
Account accounts[1000];
Transaction transactions[10000];
int accountCount = 0;
int transactionCount = 0;
int currentAccountIndex = -1;

// Function prototypes
void initializeSystem();
void mainMenu();
void adminMenu();
void customerMenu();
void createAccount();
void deleteAccount();
void viewAllAccounts();
void deposit();
void withdraw();
void transfer();
void checkBalance();
void changePIN();
void viewTransactionHistory();
int findAccount(int accountNumber);
void saveData();
void loadData();
void printAccountDetails(int index);
void printTransaction(int index);
void simulateATM();
void clearInputBuffer();

// Main function
int main() {
    initializeSystem();
    loadData();
    mainMenu();
    saveData();
    return 0;
}

// Initialize the system
void initializeSystem() {
    srand(time(0));
    if (accountCount == 0) {
        accounts[0].accountNumber = 1000;
        strcpy(accounts[0].name, "Admin");
        strcpy(accounts[0].pin, "1234"); // Default admin PIN
        accounts[0].balance = 0;
        accounts[0].isActive = 1;
        accountCount = 1;
    }
}

// Save data to text files
void saveData() {
    FILE *accFile = fopen("accounts.txt", "w");
    FILE *transFile = fopen("transactions.txt", "w");
    
    if (accFile) {
        for (int i = 0; i < accountCount; i++) {
            fprintf(accFile, "%d,%s,%s,%.2f,%d\n", 
                   accounts[i].accountNumber,
                   accounts[i].name,
                   accounts[i].pin,
                   accounts[i].balance,
                   accounts[i].isActive);
        }
        fclose(accFile);
    } else {
        printf("\n\n\t\tError saving accounts data!");
    }
    
    if (transFile) {
        for (int i = 0; i < transactionCount; i++) {
            fprintf(transFile, "%d,%s,%.2f,%ld\n", 
                   transactions[i].accountNumber,
                   transactions[i].type,
                   transactions[i].amount,
                   transactions[i].timestamp);
        }
        fclose(transFile);
    } else {
        printf("\n\n\t\tError saving transactions data!");
    }
}

// Load data from text files
void loadData() {
    FILE *accFile = fopen("accounts.txt", "r");
    if (accFile) {
        accountCount = 0;
        while (fscanf(accFile, "%d,%49[^,],%4[^,],%f,%d\n", 
                     &accounts[accountCount].accountNumber,
                     accounts[accountCount].name,
                     accounts[accountCount].pin,
                     &accounts[accountCount].balance,
                     &accounts[accountCount].isActive) == 5) {
            accountCount++;
            if (accountCount >= 1000) break;
        }
        fclose(accFile);
    }
    
    FILE *transFile = fopen("transactions.txt", "r");
    if (transFile) {
        transactionCount = 0;
        while (fscanf(transFile, "%d,%19[^,],%f,%ld\n", 
                     &transactions[transactionCount].accountNumber,
                     transactions[transactionCount].type,
                     &transactions[transactionCount].amount,
                     &transactions[transactionCount].timestamp) == 4) {
            transactionCount++;
            if (transactionCount >= 10000) break;
        }
        fclose(transFile);
    }
}

// Main menu
void mainMenu() {
    int choice;
    do {
        system("cls");
        printf("\n\n\t\tBANKING MANAGEMENT SYSTEM");
        printf("\n\n\t\t1. ADMIN LOGIN");
        printf("\n\t\t2. CUSTOMER LOGIN");
        printf("\n\t\t3. EXIT");
        printf("\n\n\t\tENTER YOUR CHOICE: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: {
                system("cls");
                printf("\n\n\t\tADMIN LOGIN");
                int accountNumber;
                char pin[5];
                
                printf("\n\n\t\tEnter Account Number: ");
                scanf("%d", &accountNumber);
                clearInputBuffer();
                
                printf("\t\tEnter PIN: ");
                int i = 0;
                char ch;
                while (i < 4) {
                    ch = getch();
                    if (ch == 13) break;
                    if (ch == 8 && i > 0) {
                        printf("\b \b");
                        i--;
                        continue;
                    }
                    if (isdigit(ch)) {
                        pin[i] = ch;
                        printf("*");
                        i++;
                    }
                }
                pin[i] = '\0';
                
                if (accountNumber == 1000 && strcmp(pin, "1234") == 0) {
                    adminMenu();
                } else {
                    printf("\n\n\t\tInvalid credentials! Access denied.");
                    getch();
                }
                break;
            }
            case 2: {
                system("cls");
                printf("\n\n\t\tCUSTOMER LOGIN");
                int accountNumber;
                char pin[5];
                
                printf("\n\n\t\tEnter Account Number: ");
                scanf("%d", &accountNumber);
                clearInputBuffer();
                
                int accountIndex = findAccount(accountNumber);
                if (accountIndex == -1) {
                    printf("\n\n\t\tAccount not found!");
                    getch();
                    break;
                }
                
                printf("\t\tEnter PIN: ");
                int i = 0;
                char ch;
                while (i < 4) {
                    ch = getch();
                    if (ch == 13) break;
                    if (ch == 8 && i > 0) {
                        printf("\b \b");
                        i--;
                        continue;
                    }
                    if (isdigit(ch)) {
                        pin[i] = ch;
                        printf("*");
                        i++;
                    }
                }
                pin[i] = '\0';
                
                if (strcmp(accounts[accountIndex].pin, pin) == 0 && accounts[accountIndex].isActive) {
                    currentAccountIndex = accountIndex;
                    customerMenu();
                } else {
                    printf("\n\n\t\tInvalid credentials or account inactive! Access denied.");
                    getch();
                }
                break;
            }
            case 3:
                printf("\n\n\t\tThank you for using our banking system!");
                break;
            default:
                printf("\n\n\t\tInvalid choice! Please try again.");
                getch();
        }
    } while(choice != 3);
}

// Admin menu
void adminMenu() {
    int choice;
    do {
        system("cls");
        printf("\n\n\t\tADMIN MENU");
        printf("\n\n\t\t1. CREATE NEW ACCOUNT");
        printf("\n\t\t2. DELETE ACCOUNT");
        printf("\n\t\t3. VIEW ALL ACCOUNTS");
        printf("\n\t\t4. VIEW TRANSACTION HISTORY");
        printf("\n\t\t5. BACK TO MAIN MENU");
        printf("\n\n\t\tENTER YOUR CHOICE: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: createAccount(); break;
            case 2: deleteAccount(); break;
            case 3: viewAllAccounts(); break;
            case 4: viewTransactionHistory(); break;
            case 5: break;
            default: printf("\n\n\t\tInvalid choice! Please try again."); getch();
        }
    } while(choice != 5);
}

// Customer menu
void customerMenu() {
    int choice;
    do {
        system("cls");
        printf("\n\n\t\tCUSTOMER MENU");
        printf("\n\n\t\tAccount: %d", accounts[currentAccountIndex].accountNumber);
        printf("\n\t\tWelcome, %s", accounts[currentAccountIndex].name);
        printf("\n\n\t\t1. DEPOSIT");
        printf("\n\t\t2. WITHDRAW");
        printf("\n\t\t3. TRANSFER");
        printf("\n\t\t4. CHECK BALANCE");
        printf("\n\t\t5. CHANGE PIN");
        printf("\n\t\t6. VIEW TRANSACTION HISTORY");
        printf("\n\t\t7. ATM SIMULATION");
        printf("\n\t\t8. BACK TO MAIN MENU");
        printf("\n\n\t\tENTER YOUR CHOICE: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: deposit(); break;
            case 2: withdraw(); break;
            case 3: transfer(); break;
            case 4: checkBalance(); break;
            case 5: changePIN(); break;
            case 6: viewTransactionHistory(); break;
            case 7: simulateATM(); break;
            case 8: currentAccountIndex = -1; break;
            default: printf("\n\n\t\tInvalid choice! Please try again."); getch();
        }
    } while(choice != 8);
}

// Create a new account
void createAccount() {
    system("cls");
    printf("\n\n\t\tCREATE NEW ACCOUNT");
    
    if (accountCount >= 1000) {
        printf("\n\n\t\tMaximum account limit reached!");
        getch();
        return;
    }
    
    // Generate random account number (1001-9999)
    int accountNumber;
    do {
        accountNumber = 1001 + rand() % 8999;
    } while (findAccount(accountNumber) != -1);
    
    accounts[accountCount].accountNumber = accountNumber;
    accounts[accountCount].isActive = 1;
    accounts[accountCount].balance = 0;
    
    printf("\n\n\t\tEnter Customer Name: ");
    scanf(" %[^\n]", accounts[accountCount].name);
    clearInputBuffer();
    
    printf("\t\tSet 4-digit PIN: ");
    int i = 0;
    char ch;
    while (i < 4) {
        ch = getch();
        if (ch == 13) break;
        if (ch == 8 && i > 0) {
            printf("\b \b");
            i--;
            continue;
        }
        if (isdigit(ch)) {
            accounts[accountCount].pin[i] = ch;
            printf("*");
            i++;
        }
    }
    accounts[accountCount].pin[i] = '\0';
    
    printf("\n\n\t\tAccount created successfully!");
    printf("\n\t\tAccount Number: %d", accountNumber);
    printf("\n\t\tCustomer Name: %s", accounts[accountCount].name);
    
    accountCount++;
    getch();
}

// Delete an account
void deleteAccount() {
    system("cls");
    printf("\n\n\t\tDELETE ACCOUNT");
    
    int accountNumber;
    printf("\n\n\t\tEnter Account Number to delete: ");
    scanf("%d", &accountNumber);
    clearInputBuffer();
    
    int index = findAccount(accountNumber);
    if (index == -1) {
        printf("\n\n\t\tAccount not found!");
    } else if (accounts[index].balance > 0) {
        printf("\n\n\t\tCannot delete account with remaining balance!");
    } else {
        accounts[index].isActive = 0;
        printf("\n\n\t\tAccount deactivated successfully!");
    }
    getch();
}

// View all accounts
void viewAllAccounts() {
    system("cls");
    printf("\n\n\t\tALL ACCOUNTS\n");
    printf("\n\t%-15s %-20s %-10s %-10s", "Account No.", "Name", "Balance", "Status");
    printf("\n\t------------------------------------------------");
    
    for (int i = 0; i < accountCount; i++) {
        printf("\n\t%-15d %-20s %-10.2f %-10s", 
               accounts[i].accountNumber, 
               accounts[i].name, 
               accounts[i].balance, 
               accounts[i].isActive ? "Active" : "Inactive");
    }
    
    getch();
}

// Deposit money
void deposit() {
    system("cls");
    printf("\n\n\t\tDEPOSIT MONEY");
    
    if (currentAccountIndex == -1) {
        printf("\n\n\t\tNo account selected!");
        getch();
        return;
    }
    
    float amount;
    printf("\n\n\t\tEnter amount to deposit: ");
    scanf("%f", &amount);
    clearInputBuffer();
    
    if (amount <= 0) {
        printf("\n\n\t\tInvalid amount!");
    } else {
        accounts[currentAccountIndex].balance += amount;
        
        // Record transaction
        transactions[transactionCount].accountNumber = accounts[currentAccountIndex].accountNumber;
        strcpy(transactions[transactionCount].type, "Deposit");
        transactions[transactionCount].amount = amount;
        transactions[transactionCount].timestamp = time(NULL);
        transactionCount++;
        
        printf("\n\n\t\tDeposit successful!");
        printf("\n\t\tNew balance: %.2f", accounts[currentAccountIndex].balance);
    }
    getch();
}

// Withdraw money
void withdraw() {
    system("cls");
    printf("\n\n\t\tWITHDRAW MONEY");
    
    if (currentAccountIndex == -1) {
        printf("\n\n\t\tNo account selected!");
        getch();
        return;
    }
    
    float amount;
    printf("\n\n\t\tEnter amount to withdraw: ");
    scanf("%f", &amount);
    clearInputBuffer();
    
    if (amount <= 0) {
        printf("\n\n\t\tInvalid amount!");
    } else if (amount > accounts[currentAccountIndex].balance) {
        printf("\n\n\t\tInsufficient balance!");
    } else {
        accounts[currentAccountIndex].balance -= amount;
        
        // Record transaction
        transactions[transactionCount].accountNumber = accounts[currentAccountIndex].accountNumber;
        strcpy(transactions[transactionCount].type, "Withdrawal");
        transactions[transactionCount].amount = amount;
        transactions[transactionCount].timestamp = time(NULL);
        transactionCount++;
        
        printf("\n\n\t\tWithdrawal successful!");
        printf("\n\t\tNew balance: %.2f", accounts[currentAccountIndex].balance);
    }
    getch();
}

// Transfer money
void transfer() {
    system("cls");
    printf("\n\n\t\tTRANSFER MONEY");
    
    if (currentAccountIndex == -1) {
        printf("\n\n\t\tNo account selected!");
        getch();
        return;
    }
    
    int targetAccountNumber;
    printf("\n\n\t\tEnter recipient's account number: ");
    scanf("%d", &targetAccountNumber);
    clearInputBuffer();
    
    int targetIndex = findAccount(targetAccountNumber);
    if (targetIndex == -1) {
        printf("\n\n\t\tRecipient account not found!");
        getch();
        return;
    }
    
    float amount;
    printf("\n\t\tEnter amount to transfer: ");
    scanf("%f", &amount);
    clearInputBuffer();
    
    if (amount <= 0) {
        printf("\n\n\t\tInvalid amount!");
    } else if (amount > accounts[currentAccountIndex].balance) {
        printf("\n\n\t\tInsufficient balance!");
    } else {
        accounts[currentAccountIndex].balance -= amount;
        accounts[targetIndex].balance += amount;
        
        // Record transactions for both accounts
        // Sender's transaction
        transactions[transactionCount].accountNumber = accounts[currentAccountIndex].accountNumber;
        strcpy(transactions[transactionCount].type, "Transfer Out");
        transactions[transactionCount].amount = amount;
        transactions[transactionCount].timestamp = time(NULL);
        transactionCount++;
        
        // Recipient's transaction
        transactions[transactionCount].accountNumber = accounts[targetIndex].accountNumber;
        strcpy(transactions[transactionCount].type, "Transfer In");
        transactions[transactionCount].amount = amount;
        transactions[transactionCount].timestamp = time(NULL);
        transactionCount++;
        
        printf("\n\n\t\tTransfer successful!");
        printf("\n\t\tNew balance: %.2f", accounts[currentAccountIndex].balance);
    }
    getch();
}

// Check balance
void checkBalance() {
    system("cls");
    printf("\n\n\t\tACCOUNT BALANCE");
    
    if (currentAccountIndex == -1) {
        printf("\n\n\t\tNo account selected!");
    } else {
        printf("\n\n\t\tAccount Number: %d", accounts[currentAccountIndex].accountNumber);
        printf("\n\t\tCustomer Name: %s", accounts[currentAccountIndex].name);
        printf("\n\t\tCurrent Balance: %.2f", accounts[currentAccountIndex].balance);
    }
    getch();
}

// Change PIN
void changePIN() {
    system("cls");
    printf("\n\n\t\tCHANGE PIN");
    
    if (currentAccountIndex == -1) {
        printf("\n\n\t\tNo account selected!");
        getch();
        return;
    }
    
    char oldPIN[5], newPIN[5];
    printf("\n\n\t\tEnter current PIN: ");
    
    int i = 0;
    char ch;
    while (i < 4) {
        ch = getch();
        if (ch == 13) break;
        if (ch == 8 && i > 0) {
            printf("\b \b");
            i--;
            continue;
        }
        if (isdigit(ch)) {
            oldPIN[i] = ch;
            printf("*");
            i++;
        }
    }
    oldPIN[i] = '\0';
    
    if (strcmp(oldPIN, accounts[currentAccountIndex].pin) != 0) {
        printf("\n\n\t\tIncorrect current PIN!");
        getch();
        return;
    }
    
    printf("\n\t\tEnter new 4-digit PIN: ");
    i = 0;
    while (i < 4) {
        ch = getch();
        if (ch == 13) break;
        if (ch == 8 && i > 0) {
            printf("\b \b");
            i--;
            continue;
        }
        if (isdigit(ch)) {
            newPIN[i] = ch;
            printf("*");
            i++;
        }
    }
    newPIN[i] = '\0';
    
    printf("\n\t\tConfirm new 4-digit PIN: ");
    char confirmPIN[5];
    i = 0;
    while (i < 4) {
        ch = getch();
        if (ch == 13) break;
        if (ch == 8 && i > 0) {
            printf("\b \b");
            i--;
            continue;
        }
        if (isdigit(ch)) {
            confirmPIN[i] = ch;
            printf("*");
            i++;
        }
    }
    confirmPIN[i] = '\0';
    
    if (strcmp(newPIN, confirmPIN) != 0) {
        printf("\n\n\t\tPINs do not match!");
    } else {
        strcpy(accounts[currentAccountIndex].pin, newPIN);
        printf("\n\n\t\tPIN changed successfully!");
    }
    getch();
}

// View transaction history
void viewTransactionHistory() {
    system("cls");
    printf("\n\n\t\tTRANSACTION HISTORY");
    
    int accountNumber;
    if (currentAccountIndex == -1) {
        printf("\n\n\t\tEnter account number: ");
        scanf("%d", &accountNumber);
        clearInputBuffer();
    } else {
        accountNumber = accounts[currentAccountIndex].accountNumber;
    }
    
    printf("\n\n\t\tTransactions for Account: %d", accountNumber);
    printf("\n\t%-20s %-15s %-20s", "Type", "Amount", "Date/Time");
    printf("\n\t------------------------------------------------------------");
    
    int found = 0;
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].accountNumber == accountNumber) {
            found = 1;
            printf("\n\t%-20s %-15.2f %-20s", 
                   transactions[i].type, 
                   transactions[i].amount, 
                   ctime(&transactions[i].timestamp));
        }
    }
    
    if (!found) {
        printf("\n\n\t\tNo transactions found for this account.");
    }
    getch();
}

// Find account by account number
int findAccount(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1;
}

// ATM simulation
void simulateATM() {
    system("cls");
    printf("\n\n\t\tATM SIMULATION");
    
    if (currentAccountIndex == -1) {
        printf("\n\n\t\tNo account selected!");
        getch();
        return;
    }
    
    int choice;
    float amount;
    
    do {
        system("cls");
        printf("\n\n\t\tATM MENU");
        printf("\n\n\t\t1. WITHDRAW CASH");
        printf("\n\t\t2. CHECK BALANCE");
        printf("\n\t\t3. EXIT");
        printf("\n\n\t\tENTER YOUR CHOICE: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1:
                printf("\n\n\t\tEnter amount to withdraw: ");
                scanf("%f", &amount);
                clearInputBuffer();
                
                if (amount <= 0) {
                    printf("\n\n\t\tInvalid amount!");
                } else if (amount > accounts[currentAccountIndex].balance) {
                    printf("\n\n\t\tInsufficient balance!");
                } else if (amount > 10000) {
                    printf("\n\n\t\tMaximum withdrawal limit is 10,000 per transaction!");
                } else {
                    accounts[currentAccountIndex].balance -= amount;
                    
                    // Record transaction
                    transactions[transactionCount].accountNumber = accounts[currentAccountIndex].accountNumber;
                    strcpy(transactions[transactionCount].type, "ATM Withdrawal");
                    transactions[transactionCount].amount = amount;
                    transactions[transactionCount].timestamp = time(NULL);
                    transactionCount++;
                    
                    printf("\n\n\t\tPlease take your cash!");
                    printf("\n\t\tNew balance: %.2f", accounts[currentAccountIndex].balance);
                }
                getch();
                break;
                
            case 2:
                printf("\n\n\t\tAccount Balance: %.2f", accounts[currentAccountIndex].balance);
                getch();
                break;
                
            case 3:
                break;
                
            default:
                printf("\n\n\t\tInvalid choice!");
                getch();
        }
    } while(choice != 3);
}

// Print account details
void printAccountDetails(int index) {
    printf("\n\tAccount Number: %d", accounts[index].accountNumber);
    printf("\n\tCustomer Name: %s", accounts[index].name);
    printf("\n\tBalance: %.2f", accounts[index].balance);
    printf("\n\tStatus: %s", accounts[index].isActive ? "Active" : "Inactive");
}

// Print transaction details
void printTransaction(int index) {
    printf("\n\tType: %s", transactions[index].type);
    printf("\n\tAmount: %.2f", transactions[index].amount);
    printf("\n\tDate/Time: %s", ctime(&transactions[index].timestamp));
}

// Clear input buffer
void clearInputBuffer() {
    while (getchar() != '\n');
}