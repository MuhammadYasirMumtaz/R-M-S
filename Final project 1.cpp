#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

// ANSI escape codes for color
const string RESET = "\033[0m";
const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";

// Structure for a menu item
struct MenuItem {
    string name;
    double price;
};

// Structure for an order
struct Order {
    string customerName;
    vector<MenuItem> orderedItems;
    double totalAmount = 0;
};

// Class to handle restaurant operations
class RestaurantManagementSystem {
private:
    vector<MenuItem> menu;
    vector<Order> orders;
    vector<Order> pendingOrders;
    map<string, int> itemOrderCount;  // Map to track how often an item is ordered

public:
    // Function to add menu item
    void addMenuItem(const string& name, double price) {
        MenuItem item;
        item.name = name;
        item.price = price;
        menu.push_back(item);
    }

    // Function to list all menu items
    void displayMenu() {
        cout << CYAN << "\n---- Menu ----\n" << RESET;
        if (menu.empty()) {
            cout << RED << "No menu items available.\n" << RESET;
            return;
        }

        for (int i = 0; i < menu.size(); i++) {
            cout << YELLOW << i + 1 << ". " << menu[i].name << " - $" << menu[i].price << RESET << endl;
        }
    }

    // Function to take order from the customer
    void takeOrder() {
        string customerName;
        vector<MenuItem> orderItems;
        double totalAmount = 0;

        cout << CYAN << "\nEnter customer name: " << RESET;
        cin.ignore();
        getline(cin, customerName);

        displayMenu();

        int choice;
        cout << CYAN << "\nEnter item numbers to order (enter 0 to finish):\n" << RESET;
        while (true) {
            cin >> choice;
            if (choice == 0) break;
            if (choice > 0 && choice <= menu.size()) {
                orderItems.push_back(menu[choice - 1]);
                totalAmount += menu[choice - 1].price;
                // Track the order count for hot selling items
                itemOrderCount[menu[choice - 1].name]++;
            }
            else {
                cout << RED << "Invalid choice. Try again.\n" << RESET;
            }
        }

        Order newOrder;
        newOrder.customerName = customerName;
        newOrder.orderedItems = orderItems;
        newOrder.totalAmount = totalAmount;

        pendingOrders.push_back(newOrder);  // Order is pending until processed
        cout << GREEN << "\nOrder placed successfully! It's in pending state.\n" << RESET;
    }

    // Function to display all orders
    void displayOrders() {
        cout << CYAN << "\n---- All Orders ----\n" << RESET;
        if (orders.empty()) {
            cout << RED << "No processed orders available.\n" << RESET;
            return;
        }

        for (int i = 0; i < orders.size(); i++) {
            cout << YELLOW << "\nOrder #" << i + 1 << " (Customer: " << orders[i].customerName << ")\n" << RESET;
            for (auto& item : orders[i].orderedItems) {
                cout << item.name << " - $" << item.price << endl;
            }
            cout << GREEN << "Total Bill: $" << orders[i].totalAmount << RESET << endl;
        }
    }

    // Function to generate a bill for a specific order
    void generateBill() {
        int orderId;
        cout << CYAN << "\nEnter Order ID to generate the bill: " << RESET;
        cin >> orderId;

        if (orderId > 0 && orderId <= orders.size()) {
            Order order = orders[orderId - 1];
            cout << CYAN << "\n---- Bill for Order #" << orderId << " ----\n" << RESET;
            cout << "Customer: " << order.customerName << endl;
            for (auto& item : order.orderedItems) {
                cout << item.name << " - $" << item.price << endl;
            }
            cout << GREEN << "Total: $" << order.totalAmount << RESET << endl;
        }
        else {
            cout << RED << "Invalid Order ID.\n" << RESET;
        }
    }

    // Function to remove a menu item (by index)
    void removeMenuItem() {
        int choice;
        cout << CYAN << "\nEnter menu item number to remove: " << RESET;
        cin >> choice;

        if (choice > 0 && choice <= menu.size()) {
            menu.erase(menu.begin() + choice - 1);
            cout << GREEN << "Menu item removed successfully.\n" << RESET;
        }
        else {
            cout << RED << "Invalid item number.\n" << RESET;
        }
    }

    // Function to process pending orders (mark them as processed)
    void processOrder() {
        if (pendingOrders.empty()) {
            cout << RED << "No pending orders.\n" << RESET;
            return;
        }

        int orderId;
        cout << CYAN << "\nEnter Order ID to process: " << RESET;
        cin >> orderId;

        if (orderId > 0 && orderId <= pendingOrders.size()) {
            orders.push_back(pendingOrders[orderId - 1]);
            pendingOrders.erase(pendingOrders.begin() + orderId - 1);
            cout << GREEN << "Order processed successfully!\n" << RESET;
        }
        else {
            cout << RED << "Invalid Order ID.\n" << RESET;
        }
    }

    // Function to cancel an order
    void cancelOrder() {
        if (pendingOrders.empty()) {
            cout << RED << "No pending orders to cancel.\n" << RESET;
            return;
        }

        int orderId;
        cout << CYAN << "\nEnter Order ID to cancel: " << RESET;
        cin >> orderId;

        if (orderId > 0 && orderId <= pendingOrders.size()) {
            pendingOrders.erase(pendingOrders.begin() + orderId - 1);
            cout << GREEN << "Order cancelled successfully.\n" << RESET;
        }
        else {
            cout << RED << "Invalid Order ID.\n" << RESET;
        }
    }

    // Function to display hot selling items
    void displayHotSellingItems() {
        cout << CYAN << "\n---- Hot Selling Items ----\n" << RESET;

        if (itemOrderCount.empty()) {
            cout << RED << "No orders placed yet.\n" << RESET;
            return;
        }

        vector<pair<string, int>> itemCounts(itemOrderCount.begin(), itemOrderCount.end());
        sort(itemCounts.begin(), itemCounts.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second > b.second;  // Sort by order count (descending)
            });

        for (auto& item : itemCounts) {
            cout << YELLOW << item.first << " - " << item.second << " orders\n" << RESET;
        }
    }
};

int main() {
    RestaurantManagementSystem rms;

    int option;

    do {
        cout << CYAN << "\n---- Restaurant Management System ----\n" << RESET;
        cout << "1. Add Menu Item\n";
        cout << "2. Display Menu\n";
        cout << "3. Take Order\n";
        cout << "4. Process Order\n";  // Moved Process Order after Take Order
        cout << "5. Display Orders\n";
        cout << "6. Generate Bill\n";
        cout << "7. Remove Menu Item\n";
        cout << "8. Cancel Order\n";
        cout << "9. Display Hot Selling Items\n";
        cout << "10. Exit\n";
        cout << CYAN << "Enter option: " << RESET;
        cin >> option;

        switch (option) {
        case 1: {
            string name;
            double price;
            cout << CYAN << "Enter menu item name: " << RESET;
            cin.ignore();
            getline(cin, name);
            cout << CYAN << "Enter menu item price: " << RESET;
            cin >> price;
            rms.addMenuItem(name, price);
            break;
        }
        case 2:
            rms.displayMenu();
            break;
        case 3:
            rms.takeOrder();
            break;
        case 4:
            rms.processOrder();
            break;
        case 5:
            rms.displayOrders();
            break;
        case 6:
            rms.generateBill();
            break;
        case 7:
            rms.removeMenuItem();
            break;
        case 8:
            rms.cancelOrder();
            break;
        case 9:
            rms.displayHotSellingItems();
            break;
        case 10:
            cout << GREEN << "Exiting the system...\n" << RESET;
            break;
        default:
            cout << RED << "Invalid option, try again.\n" << RESET;
        }

    } while (option != 10);

    return 0;
}
