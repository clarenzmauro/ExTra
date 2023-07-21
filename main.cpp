#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

struct Expense {
    int id;
    std::string name;
    double amount;
    std::string category;
    std::string date;

    Expense(int _id, const std::string& _name, double _amount, const std::string& _category, const std::string& _date)
        : id(_id), name(_name), amount(_amount), category(_category), date(_date) {}
};

void BorderDesign() {
    // Prints a decorative border design to enhance the appearance of the program's output.
    std::cout << "======================================================================= ExTra ====================================================================" << std::endl;
    std::cout << "==================================================================================================================================================\n\n\n\n";
}

struct ExpenseTracker {
    double budget;
    std::vector<Expense> expenses;

    ExpenseTracker(double _budget) : budget(_budget) {}

    void addExpense(const Expense& expense) {
        // Adds a new expense to the expenses vector, saves the changes, and displays a success message.
        expenses.push_back(expense);
        saveExpenses();
        system("cls");
        BorderDesign();
        std::cout << "\t\t\t\t\t\t\t    Expense added successfully!" << std::endl;
    }

    void removeExpense(int expenseId) {
        // Removes an expense from the expenses vector based on its ID, saves the changes, and displays a success or error message.
        auto it = std::find_if(expenses.begin(), expenses.end(), [expenseId](const Expense& expense) {
            return expense.id == expenseId;
        });

        if (it != expenses.end()) {
            expenses.erase(it);
            saveExpenses();
            system("cls");
            BorderDesign();
            std::cout << "\t\t\t\t\t\t\t  Expense removed successfully!" << std::endl;
        } else {
            system("cls");
            BorderDesign();
            std::cout << "\t\t\t\t\t\t\t   Expense with ID " << expenseId << " not found." << std::endl;
        }
    }

    void findExpense(int expenseId) {
        // Searches for an expense in the expenses vector based on its ID and displays it if found, or displays an error message.
        auto it = std::find_if(expenses.begin(), expenses.end(), [expenseId](const Expense& expense) {
            return expense.id == expenseId;
        });

        if (it != expenses.end()) {
            system("cls");
            BorderDesign();
            std::cout << "\t\t\t\t\t\t\t\t Expense found: \n" << std::endl;
            displayExpense(*it);
        } else {
            system("cls");
            BorderDesign();
            std::cout << "\t\t\t\t\t\t\t   Expense with ID " << expenseId << " not found." << std::endl;
        }
    }

    void updateExpense(int expenseId, const Expense& newExpense) {
        // Updates an expense in the expenses vector with new information, saves the changes, and displays a success or error message.
        auto it = std::find_if(expenses.begin(), expenses.end(), [expenseId](const Expense& expense) {
            return expense.id == expenseId;
        });

        if (it != expenses.end()) {
            *it = newExpense;
            saveExpenses();
            system("cls");
            BorderDesign();
            std::cout << "\t\t\t\t\t\t\t\t Expense updated successfully!" << std::endl;
        } else {
            system("cls");
            BorderDesign();
            std::cout << "\t\t\t\t\t\t\t\t Expense with ID " << expenseId << " not found." << std::endl;
        }
    }

    void viewExpenses() {
        // Displays all logged expenses.
        std::cout << "\t\t\t\t\t\t\t\t ALL LOGGED EXPENSES: \n\n" << std::endl;
        for (const auto& expense : expenses) {
            displayExpense(expense);
        }
    }

    void displayExpense(const Expense& expense) {
        // Displays the details of a single expense.
        std::cout << "\t\t\t\t\t\t\t\t ID: " << expense.id << std::endl;
        std::cout << "\t\t\t\t\t\t\t\t Name: " << expense.name << std::endl;
        std::cout << "\t\t\t\t\t\t\t\t Amount: " << expense.amount << std::endl;
        std::cout << "\t\t\t\t\t\t\t\t Category: " << expense.category << std::endl;
        std::cout << "\t\t\t\t\t\t\t\t Date: " << expense.date << std::endl;
        std::cout << "\t\t\t\t\t\t\t\t ------------------------" << std::endl;
    }

    void generateReports() {
        // Generates and displays financial reports based on the logged expenses.
        double totalExpense = 0.0;
        double thisMonthExpense = 0.0;
        double thisWeekExpense = 0.0;
        double todayExpense = 0.0;
        std::time_t currentTime = std::time(nullptr);
        std::tm* currentLocalTime = std::localtime(&currentTime);
        int currentMonth = currentLocalTime->tm_mon + 1;
        int currentWeek = currentLocalTime->tm_yday / 7;

        for (const auto& expense : expenses) {
            totalExpense += expense.amount;

            std::string expenseDate = expense.date;
            std::tm expenseTime = {};
            std::istringstream iss(expenseDate);
            iss >> std::get_time(&expenseTime, "%Y-%m-%d");

            int expenseMonth = expenseTime.tm_mon + 1;
            int expenseWeek = expenseTime.tm_yday / 7;

            if (expenseMonth == currentMonth)
                thisMonthExpense += expense.amount;
            if (expenseWeek == currentWeek)
                thisWeekExpense += expense.amount;
            if (expenseDate == std::to_string(currentLocalTime->tm_year + 1900) + "-" +
                               std::to_string(currentLocalTime->tm_mon + 1) + "-" +
                               std::to_string(currentLocalTime->tm_mday))
                todayExpense += expense.amount;
        }

        std::cout << "\t\t\t\t\t\t\t\t Financial Report:" << std::endl;
        std::cout << "\t\t\t\t\t\t\t\t Total Expense: " << totalExpense << std::endl;
        std::cout << "\t\t\t\t\t\t\t\t Expense This Month: " << thisMonthExpense << std::endl;
        std::cout << "\t\t\t\t\t\t\t\t Expense This Week: " << thisWeekExpense << std::endl;
        std::cout << "\t\t\t\t\t\t\t\t Expense Today: " << todayExpense << std::endl;

        if (totalExpense > budget) {
            std::cout << "\n\t\t\t\t\t\t\tWarning: You have exceeded the budget!" << std::endl;
        } else {
            std::cout << "\n\t\t\t\t\t\t\t\tYou are within the budget." << std::endl;
        }
    }

    void saveExpenses() {
        // Saves the expenses to a file named "ExpenseTracker.txt".
        std::ofstream file("ExpenseTracker.txt");
        if (file.is_open()) {
            for (const auto& expense : expenses) {
                file << expense.id << "," << expense.name << "," << expense.amount << "," << expense.category << "," << expense.date << "\n";
            }
            file.close();
        } else {
            std::cout << "Failed to open the file for saving expenses." << std::endl;
        }
    }

    void loadExpenses() {
        // Loads the expenses from the "ExpenseTracker.txt" file.
        expenses.clear();
        std::ifstream file("ExpenseTracker.txt");
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                int id;
                std::string name, category, date;
                double amount;
                if (iss >> id >> std::quoted(name) >> amount >> std::quoted(category) >> std::quoted(date)) {
                    expenses.emplace_back(id, name, amount, category, date);
                }
            }
            file.close();
        } else {
            std::cout << "\t\t\t\t\t    Failed to open the file for loading expenses." << std::endl;
        }
    }
};

int getValidIntegerInput() {
    // Reads an integer input from the user, validates it, and handles invalid inputs gracefully.
    int value;
    while (true) {
        std::cin >> value;
        if (std::cin.fail()) {
            system("cls");
            BorderDesign();
            std::cout << "\t\t\t\t\t\t\t\t   Invalid input!" << std::endl;
            system("pause");
            system("cls");
            BorderDesign();
            std::cout << "\t\t\t\t\t\t\t   Please enter a valid number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

void displayMenu(ExpenseTracker& tracker) {
    // Prints the main menu options of the expense tracker program.
    system("cls");
    BorderDesign();
    std::cout << "\t\t\t\t\t\t\t\t     _MAIN MENU_\n\n\n" << std::endl;
    std::cout << "\t\t\t\t\t\t\t\t  = Set Budget[1] =\n" << std::endl;
    std::cout << "\t\t\t\t\t\t\t\t= Log an Expense[2] =\n" << std::endl;
    std::cout << "\t\t\t\t\t\t\t   = View All Logged Expenses[3] =\n" << std::endl;
    std::cout << "\t\t\t\t\t\t\t = View a Specific Logged Expense[4] =\n" << std::endl;
    std::cout << "\t\t\t\t\t\t\t     = Edit a Logged Expense[5] =\n" << std::endl;
    std::cout << "\t\t\t\t\t\t\t    = Delete a Logged Expense[6] =\n" << std::endl;
    std::cout << "\t\t\t\t\t\t\t      = Get Financial Report[7] =\n\n" << std::endl;
    std::cout << "\t\t\t\t\t\t\t\t     = Quit[8] =" << std::endl;
}

int main() {
    ExpenseTracker tracker(0.0);
    tracker.loadExpenses();

    bool exitMenu = false;

    while (!exitMenu) {
        system("cls");
        displayMenu(tracker);
        std::cout << "\nENTER YOUR CHOICE: ";
        int choice = getValidIntegerInput();

        switch (choice) {
            case 1: {
                // Sets the budget.
                system("cls");
                BorderDesign();
                std::cout << "\t\t\t\t\t\t\t\t ENTER YOUR BUDGET: ";
                double budget = 0.0;
                std::cin >> budget;
                tracker = ExpenseTracker(budget);
                tracker.saveExpenses();
                break;
            }
            case 2: {
                // Logs a new expense.
                system("cls");
                BorderDesign();
                std::cout << "\t\t\t\t\t\t\t\tEnter expense ID: ";
                int id = getValidIntegerInput();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "\t\t\t\t\t\t\t\tEnter expense name: ";
                std::string name;
                std::getline(std::cin, name);

                std::cout << "\t\t\t\t\t\t\t\tEnter expense amount: ";
                double amount = 0.0;
                std::cin >> amount;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "\t\t\t\t\t\t\t\tEnter expense category: ";
                std::string category;
                std::getline(std::cin, category);

                std::cout << "\t\t\t\t\t\t\t\tEnter expense date (YYYY-MM-DD): ";
                std::string date;
                std::getline(std::cin, date);

                tracker.addExpense(Expense(id, name, amount, category, date));
                break;
            }
            case 3:
                // Views all logged expenses.
                system("cls");
                BorderDesign();
                tracker.viewExpenses();
                break;
            case 4: {
                // Views a specific logged expense.
                system("cls");
                BorderDesign();
                std::cout << "\t\t\t\t\t\t\t\tEnter expense ID to view: ";
                int id = getValidIntegerInput();
                tracker.findExpense(id);
                break;
            }
            case 5: {
                // Edits a logged expense.
                system("cls");
                BorderDesign();
                std::cout << "\t\t\t\t\t\t\t  Enter expense ID to edit: ";
                int id = getValidIntegerInput();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "\t\t\t\t\t\t\t  Enter new expense name: ";
                std::string name;
                std::getline(std::cin, name);

                std::cout << "\t\t\t\t\t\t\t  Enter new expense amount: ";
                double amount = 0.0;
                std::cin >> amount;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "\t\t\t\t\t\t\t  Enter new expense category: ";
                std::string category;
                std::getline(std::cin, category);

                std::cout << "\t\t\t\t\t\t\t  Enter new expense date (YYYY-MM-DD): ";
                std::string date;
                std::getline(std::cin, date);

                tracker.updateExpense(id, Expense(id, name, amount, category, date));
                break;
            }
            case 6: {
                // Deletes a logged expense.
                system("cls");
                BorderDesign();
                std::cout << "\t\t\t\t\t\t\t\tEnter expense ID to delete: ";
                int id = getValidIntegerInput();
                tracker.removeExpense(id);
                break;
            }
            case 7:
                // Generates and displays financial reports.
                system("cls");
                BorderDesign();
                tracker.generateReports();
                break;
            case 8:
                exitMenu = true;
                break;
            default:
                system("cls");
                BorderDesign();
                std::cout << "\n\t\t\t\t\t\t\t  Invalid choice. Please try again." << std::endl;
                break;
        }

        if (!exitMenu) {
            system("pause");
        }
    }

    return 0;
}
