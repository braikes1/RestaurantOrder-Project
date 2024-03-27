#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 10
#define MAX_ORDERS 100
#define FILE_NAME "orders.txt"
#define SUMMARY_FILE "summary.txt"

typedef struct {
    char itemName[50];
    float price;
} Item;

typedef struct {
    int orderID;
    char customerName[100];
    Item orderItems[MAX_ITEMS];
    int itemCount;
} Order;

Item menu[] = {
    {"Burger", 4.99},
    {"Sandwich", 4.49},
    {"Tenders", 3.99},
    {"Fries", 1.99},
    {"Cookies", 1.00},
    {"Soft Drink", 0.89}
};
int menuSize = sizeof(menu) / sizeof(menu[0]);
Order orders[MAX_ORDERS];
int orderCount = 0;

void displayMenu() {
    printf("Menu:\n");
    for (int i = 0; i < menuSize; i++) {
        printf("%d. %s - $%.2f\n", i + 1, menu[i].itemName, menu[i].price);
    }
}

void takeOrder() {
    Order order = {0, "", {{0}}, 0};
    order.orderID = orderCount + 1;

    printf("Enter customer name: ");
    scanf("%s", order.customerName);

    printf("Enter number of items: ");
    int numItems;
    scanf("%d", &numItems);

    for (int i = 0; i < numItems && i < MAX_ITEMS; i++) {
        int menuItemIndex;
        printf("Enter menu item number: ");
        scanf("%d", &menuItemIndex);
        menuItemIndex--; // Adjust for 0-indexed array

        if (menuItemIndex >= 0 && menuItemIndex < menuSize) {
            order.orderItems[i] = menu[menuItemIndex];
            order.itemCount++;
        }
    }

    orders[orderCount++] = order;
}

void saveOrderToFile(const Order *order) {
    FILE *file = fopen(FILE_NAME, "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "Order ID: %d, Customer: %s\n", order->orderID, order->customerName);
    for (int i = 0; i < order->itemCount; i++) {
        fprintf(file, "%s - $%.2f\n", order->orderItems[i].itemName, order->orderItems[i].price);
    }
    fprintf(file, "\n");
    fclose(file);
}

void calculateTotalAndTax() {
    for (int j = 0; j < orderCount; j++) {
        Order *order = &orders[j];
        float total = 0;
        for (int i = 0; i < order->itemCount; i++) {
            total += order->orderItems[i].price;
        }

        float tax = total * 0.07;
        float grandTotal = total + tax;

        printf("Order ID: %d, Customer: %s\n", order->orderID, order->customerName);
        for (int i = 0; i < order->itemCount; i++) {
            printf("%s - $%.2f\n", order->orderItems[i].itemName, order->orderItems[i].price);
        }
        printf("Total: $%.2f, Tax: $%.2f, Grand Total: $%.2f\n\n", total, tax, grandTotal);

        saveOrderToFile(order);
    }
}

void generateDailySummary() {
    FILE *file = fopen(SUMMARY_FILE, "w");
    if (file == NULL) {
        printf("Error opening summary file!\n");
        return;
    }

    float totalRevenue = 0;
    fprintf(file, "Daily Sales Summary:\n");
    for (int i = 0; i < orderCount; i++) {
        float orderTotal = 0;
        for (int j = 0; j < orders[i].itemCount; j++) {
            orderTotal += orders[i].orderItems[j].price;
        }
        totalRevenue += orderTotal;
        fprintf(file, "Order ID: %d - Total: $%.2f\n", orders[i].orderID, orderTotal);
    }
    fprintf(file, "Total Revenue: $%.2f\n", totalRevenue);
    fclose(file);

    printf("Daily summary saved to %s\n", SUMMARY_FILE);
}

int main() {
    int running = 1;
    while (running && orderCount < MAX_ORDERS) {
        displayMenu();
        takeOrder();
        printf("Continue taking orders? (1 for Yes, 0 for No): ");
        scanf("%d", &running);
    }

    calculateTotalAndTax();
    generateDailySummary();

    return 0;
}