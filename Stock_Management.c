#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define MAX_USERS 10
#define MAX_ITEMS 50


typedef struct {
    char username[30];
    char password[30];
} Admin;

typedef struct {
    char username[30];
    char password[30];
} Employee;

typedef struct {
    char username[30];
    char password[30];
} Customer;


typedef struct {
    char name[50];
    float price;
    char category[30];
    int   quantity;
} Item;


Admin adminList[] = {
    {"Hamja",   "Hamja123"},
    {"Sabbir",  "Sabbir123"},
    {"Nafisha", "Nafisha123"}
};
int adminCount = 3;

Employee employeeList[MAX_USERS];
int employeeCount = 0;

Customer customerList[MAX_USERS];
int customerCount = 0;

Item itemList[MAX_ITEMS];
int itemCount = 0;

int adminLoggedIn = 0;
int customerLoggedIn = 0;
char currentCustomer[30] = "";

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return columns;
}

void printCentered(const char *text) {
    int width = getConsoleWidth();
    int len = (int)strlen(text);
    int padding = (width - len) / 2;
    if (padding < 0) padding = 0;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", text);
}

void getHiddenPassword(char *password) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13) { 
            password[i] = '\0';
            printf("\n");
            break;
        } else if (ch == 8) { 
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < 29 && ch >= 32 && ch <= 126) { 
            password[i++] = ch;
            printf("*");
        }
    }
}

void displayFullAsciiLogo() {
    const char *logo[] = {
        "................................................................................",
        "................................................................................",
        ".................................:-=+*#####+=-:.................................",
        ".............................=#%%%%%%%%%%%%%%%%%%#=.....::......................",
        "..........................=%%%%%%*:..........:+%%%%%%+.*%%......................",
        ".......................:*%%%%+:...................=%%%%%%%*.....................",
        "......................*%%%#:.....:::::..............#%%%%%%.....................",
        "....................:%%%#.......:#####=...........:%%%%%%%%*....................",
        "...................=%%%-........:#####=................:-=++....................",
        "..................=%%%:.........:#####=.........:#%%-...........................",
        ".................-%%%:...+****+.:#####=......-%%%%##%%%+:.......................",
        ".................%%%:....*#####.:#####=...+%%%#*******#%%%#-....................",
        "................=%%#.....*#####.:###*::#%%%#**************%%%%=.................",
        "................#%%:.....*#####.:#:-#%%%#************=..*****#%%%+:.............",
        "................%%%......*#####.:*.%%%#************+..:*******#%%%*.............",
        "................%%%####=.*#####.:*.%%#%%%#***=..+*:..******#%%%%#%*.............",
        "................%%%####=.*#####.:*.%%***#%%%#**:...-****#%%%#***#%*.............",
        "................#%%####=.*#####.:*.%%******#%%%%*-***#%%%#******#%*.............",
        "................=%%%###=.*#####.:*.%%**********%%%%%%%#*********#%*.............",
        ".................%%%###=.*#####.:*.%%************#%%************#%*.............",
        ".................-%%%##=.*#####.:*.%%************#%%************#%*.............",
        "..................=%%%*-.=+++++.:+.%%%***********#%%***********#%%+.............",
        "...................+%%%-............:*%%%#*******#%%********#%%%=...............",
        "....................-%%%*..............:*%%%*****#%%*****#%%%-..................",
        "......................#%%%=...............:*%%%#*#%%**#%%#-.....................",
        ".......................:%%%%#:...............:+%%%%%%%#-........................",
        "..........................%%%%%%=:............:-.=%#:...........................",
        "............................-*%%%%%%%%%%%%%%%%%%%%:.............................",
        "................................:=#%%%%%%%%%%*=:................................",
        "................................................................................",
        "................................................................................",
        "..........+%%%%:%%%%%@:+%%%%#..-%%%%%:%%.+%+.....=%%:..+%*..*%:+%%%%#:..........",
        "..........*%=:....%%..*%:...##:%#.....%%%%-.....:%+*%..+%#%.*%:+%...#%..........",
        "...........:=#%-..%%..*%:...#%:%*.....%%-%#.....#%+*%+.+%.*%#%:+%...#%..........",
        ".........:%%%%%-..%%...#%%%%%:.-%%%%%:%%..%%...=%-..=%:+%..-%%:+%%%%%:..........",
        "................................................................................",
        ".........:+=.+=..:+-=+:..:+--++++=.+-..:+:=+++++:.:+*+:..=+++-.=*:..+*:.........",
        ".........:%*:%%#.-%+.%*..%%:+%:....%%*.:%-..=%..:%%:.=%*.*%..%%.*%.*%-..........",
        ".........:%*:%**%-%+.=%--%:.+%%%%:.%+#%:%-..=%..+%-...+%-*%*#%*..#%%-...........",
        ".........:%*:%*.+%%+..##%#..+%:....%+.+%%-..=%..:%*...%#.*%.#%:..:%+............",
        ".........:%+:%+..-%+..:%%...+%%%%*.%=..-%-..=%....+%%%-..*#..#%:.:%+............",
        "................................................................................",
        "................................................................................",
    };

    int lines = sizeof(logo) / sizeof(logo[0]);

    printf("\033[1;36m");
    for (int i = 0; i < lines; i++) {
        printCentered(logo[i]);
    }
    printf("\033[0m");
}


const char *categories[] = {
    "Phone", "Laptop", "Monitor", "Electronics",
    "Fruit", "Vegetable", "Meat", "Drinks"
};
#define CATEGORY_COUNT (sizeof(categories) / sizeof(categories[0]))


void viewAllItems() {
    if (itemCount == 0) {
        printf("\nNo items to display.\n");
        return;
    }
    printf("\n----- List of Items -----\n");
    for (int i = 0; i < itemCount; i++) {
        printf("%d. %s | Category: %s | $%.2f | Stock: %d\n",
               i + 1,
               itemList[i].name,
               itemList[i].category,
               itemList[i].price,
               itemList[i].quantity);
    }
}

void addItem() {
    if (itemCount >= MAX_ITEMS) {
        printf("Item storage is full!\n");
        return;
    }

    printf("\nAdd New Item\n");
    printf("Enter item name: ");
    scanf(" %[^\n]", itemList[itemCount].name);

    printf("Enter item price: ");
    scanf("%f", &itemList[itemCount].price);

    printf("\nSelect a category:\n");
    for (int i = 0; i < CATEGORY_COUNT; i++) {
        printf("%d. %s\n", i + 1, categories[i]);
    }

    int categoryChoice;
    printf("Enter category number: ");
    scanf("%d", &categoryChoice);

    if (categoryChoice < 1 || categoryChoice > (int)CATEGORY_COUNT) {
        printf("Invalid category! Setting to 'Uncategorized'\n");
        strcpy(itemList[itemCount].category, "Uncategorized");
    } else {
        strcpy(itemList[itemCount].category, categories[categoryChoice - 1]);
    }

    printf("Enter quantity: ");
    scanf("%d", &itemList[itemCount].quantity);
    if (itemList[itemCount].quantity < 0) itemList[itemCount].quantity = 0;

    itemCount++;
    printf("Item added successfully!\n");
}

void removeItem() {
    if (itemCount == 0) {
        printf("No items to remove.\n");
        return;
    }

    printf("\n----- Remove Item -----\n");
    viewAllItems();

    int index;
    printf("Enter the item number to remove: ");
    scanf("%d", &index);

    if (index < 1 || index > itemCount) {
        printf("Invalid item number.\n");
        return;
    }

    index--;

    for (int i = index; i < itemCount - 1; i++) {
        itemList[i] = itemList[i + 1];
    }

    itemCount--;
    printf("Item removed successfully.\n");
}

void updateItem() {
    if (itemCount == 0) {
        printf("No items to update.\n");
        return;
    }

    printf("\n--- Update Item ---\n");
    viewAllItems();

    int index;
    printf("Enter item number to update: ");
    scanf("%d", &index);

    if (index < 1 || index > itemCount) {
        printf("Invalid item number.\n");
        return;
    }

    index--;

    printf("Updating item: %s ($%.2f, %s, stock %d)\n",
           itemList[index].name,
           itemList[index].price,
           itemList[index].category,
           itemList[index].quantity);

    printf("Enter new name (or . to keep): ");
    char newName[50];
    scanf(" %[^\n]", newName);
    if (strcmp(newName, ".") != 0) {
        strcpy(itemList[index].name, newName);
    }

    printf("Enter new price (or -1 to keep): ");
    float newPrice;
    scanf("%f", &newPrice);
    if (newPrice >= 0) {
        itemList[index].price = newPrice;
    }

    printf("\nSelect new category (or 0 to keep):\n");
    for (int i = 0; i < CATEGORY_COUNT; i++) {
        printf("%d. %s\n", i + 1, categories[i]);
    }
    int categoryChoice;
    printf("Enter category number: ");
    scanf("%d", &categoryChoice);
    if (categoryChoice >= 1 && categoryChoice <= (int)CATEGORY_COUNT) {
        strcpy(itemList[index].category, categories[categoryChoice - 1]);
    }

    printf("Enter new quantity (or -1 to keep): ");
    int newQty;
    scanf("%d", &newQty);
    if (newQty >= 0) {
        itemList[index].quantity = newQty;
    }

    printf("Item updated successfully!\n");
}

void viewItemsByCategory() {
    if (itemCount == 0) {
        printf("\nNo items in the inventory.\n");
        return;
    }

    printf("\nSelect category to view:\n");
    for (int i = 0; i < CATEGORY_COUNT; i++) {
        printf("%d. %s\n", i + 1, categories[i]);
    }

    int choice;
    printf("Enter category number: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > (int)CATEGORY_COUNT) {
        printf("Invalid category choice!\n");
        return;
    }

    printf("\n--- Items in Category: %s ---\n", categories[choice - 1]);
    int found = 0;
    for (int i = 0; i < itemCount; i++) {
        if (strcmp(itemList[i].category, categories[choice - 1]) == 0) {
            printf("%d. %s | $%.2f | Stock: %d\n",
                   i + 1, itemList[i].name, itemList[i].price, itemList[i].quantity);
            found = 1;
        }
    }

    if (!found) {
        printf("No items found in this category.\n");
    }
}

void purchaseItem() {
    if (!customerLoggedIn) {
        printf("Please login as customer to purchase.\n");
        return;
    }
    if (itemCount == 0) {
        printf("No items available.\n");
        return;
    }

    viewAllItems();
    int choice, qty;
    printf("Enter item number to buy: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > itemCount) {
        printf("Invalid item number!\n");
        return;
    }

    Item *it = &itemList[choice - 1];

    printf("Enter quantity to buy: ");
    scanf("%d", &qty);

    if (qty <= 0 || qty > it->quantity) {
        printf("Invalid quantity! Only %d in stock.\n", it->quantity);
        return;
    }

    it->quantity -= qty;
    float total = qty * it->price;
    printf("Purchase successful, %s! Total: $%.2f | Remaining stock: %d\n",
           currentCustomer, total, it->quantity);
}


void adminLogin() {
    char username[30], password[30];
    printf("\nAdmin Login\nUsername: ");
    scanf("%s", username);
    printf("Password: ");
    getHiddenPassword(password);

    for (int i = 0; i < adminCount; i++) {
        if (strcmp(username, adminList[i].username) == 0 &&
            strcmp(password, adminList[i].password) == 0) {
            printf("Admin login successful! Welcome, %s\n", username);
            adminLoggedIn = 1;
            return;
        }
    }
    printf("Invalid username or password!\n");
    adminLoggedIn = 0;
}

void adminMenu() {
    int choice;

    if (!adminLoggedIn) {
        printf("\n------ Admin Menu ------\n1. Login\nChoose option: ");
        scanf("%d", &choice);
        if (choice == 1) {
            adminLogin();
        } else {
            printf("Invalid option!\n");
            return;
        }
    }

    while (adminLoggedIn) {
        printf("\n------ Admin Actions ------\n");
        printf("1. Add Item\n");
        printf("2. View All Items\n");
        printf("3. View Items by Category\n");
        printf("4. Update Item\n");
        printf("5. Remove Item\n");
        printf("6. Logout\n\nChoose option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addItem(); break;
            case 2: viewAllItems(); break;
            case 3: viewItemsByCategory(); break;
            case 4: updateItem(); break;
            case 5: removeItem(); break;
            case 6: adminLoggedIn = 0; printf("Logged out from admin.\n"); break;
            default: printf("Invalid choice!\n");
        }
    }
}


void employeeRegister() {
    if (employeeCount >= MAX_USERS) {
        printf("Highest Employee limit reached!\n");
        return;
    }
    printf("\nEmployee Register\nUsername: ");
    scanf("%s", employeeList[employeeCount].username);
    printf("Password: ");
    getHiddenPassword(employeeList[employeeCount].password);
    employeeCount++;
    printf("Employee registered successfully!\n");
}

void employeeLogin() {
    char username[30], password[30];
    printf("\nEmployee Login\nUsername: ");
    scanf("%s", username);
    printf("Password: ");
    getHiddenPassword(password);

    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(username, employeeList[i].username) == 0 &&
            strcmp(password, employeeList[i].password) == 0) {
            printf("Employee login successful! Welcome, %s\n", username);
            return;
        }
    }
    printf("Login failed!\n");
}

void employeeMenu() {
    int choice;
    printf("\n------ Employee Menu ------\n1. Register\n2. Login\nChoose option: ");
    scanf("%d", &choice);

    if (choice == 1) employeeRegister();
    else if (choice == 2) employeeLogin();
    else printf("Invalid option!\n");
}


void customerRegister() {
    if (customerCount >= MAX_USERS) {
        printf("Highest Customer limit reached!\n");
        return;
    }
    printf("\nCustomer Register\nUsername: ");
    scanf("%s", customerList[customerCount].username);
    printf("Password: ");
    getHiddenPassword(customerList[customerCount].password);
    customerCount++;
    printf("Customer registered successfully!\n");
}

void customerLogin() {
    char username[30], password[30];
    printf("\nCustomer Login\nUsername: ");
    scanf("%s", username);
    printf("Password: ");
    getHiddenPassword(password);

    for (int i = 0; i < customerCount; i++) {
        if (strcmp(username, customerList[i].username) == 0 &&
            strcmp(password, customerList[i].password) == 0) {
            printf("Customer login successful! Welcome, %s\n", username);
            customerLoggedIn = 1;
            strcpy(currentCustomer, username);
            return;
        }
    }
    printf("Login failed!\n");
    customerLoggedIn = 0;
    currentCustomer[0] = '\0';
}

void customerActionsMenu() {
    if (!customerLoggedIn) {
        printf("Please login first.\n");
        return;
    }

    int choice;
    while (customerLoggedIn) {
        printf("\n------ Customer Actions (%s) ------\n", currentCustomer);
        printf("1. View All Items\n");
        printf("2. View Items by Category\n");
        printf("3. Purchase Item\n");
        printf("4. Logout\nChoose option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewAllItems(); break;
            case 2: viewItemsByCategory(); break;
            case 3: purchaseItem(); break;
            case 4: customerLoggedIn = 0; currentCustomer[0] = '\0'; printf("Logged out.\n"); break;
            default: printf("Invalid choice!\n");
        }
    }
}

void customerMenu() {
    int choice;
    printf("\n------ Customer Menu ------\n1. Register\n2. Login\n3. Back\nChoose option: ");
    scanf("%d", &choice);

    if (choice == 1) {
        customerRegister();
    } else if (choice == 2) {
        customerLogin();
        if (customerLoggedIn) customerActionsMenu();
    } else if (choice == 3) {
        return;
    } else {
        printf("Invalid option!\n");
    }
}


int main() {
    int choice;

    displayFullAsciiLogo();

    while (1) {
        printf("\n==========================================\n");
        printCentered("System Menu:");
        printCentered("1. Admin");
        printCentered("2. Employee");
        printCentered("3. Customer");
        printCentered("4. Exit");
        printf("===========================================\n");
        printf("Enter your role (position): ");
        if (scanf("%d", &choice) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input. Try again.\n");
            continue;
        }

        switch (choice) {
            case 1: adminMenu(); break;
            case 2: employeeMenu(); break;
            case 3: customerMenu(); break;
            case 4: printf("Exiting From The System....\n"); return 0;
            default: printf("Invalid choice! Kindly try again.\n");
        }
    }

    return 0;
}
