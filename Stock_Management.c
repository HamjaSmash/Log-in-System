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
} Item;


Admin adminList[] = {
    {"Hamja", "Hamja123"},
    {"Sabbir", "Sabbir123"},
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


int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return columns;
}

void printCentered(const char *text) {
    int width = getConsoleWidth();
    int len = strlen(text);
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
        } else {
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

    itemCount++;
    printf("Item added successfully!\n");
}


void viewAllItems() {
    if (itemCount == 0) {
        printf("\nNo items to display.\n");
        return;
    }

    printf("\n----- List of Items -----\n");
    for (int i = 0; i < itemCount; i++) {
        printf("%d %s  $%.2f\n", i + 1, itemList[i].name, itemList[i].price);
    }
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
    printf("1. Add Item\n2. View All Items\n3. Logout\n\nChoose option: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            addItem();
            break;
        case 2:
            viewAllItems();
            break;
        case 3:
            adminLoggedIn = 0;
            printf("Logged out from admin.\n");
            break;
        default:
            printf("Invalid choice!\n");
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
    scanf("%s", employeeList[employeeCount].password);
    employeeCount++;
    printf("Employee registered successfully!\n");
}

void employeeLogin() {
    char username[30], password[30];
    printf("\nEmployee Login\nUsername: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(username, employeeList[i].username) == 0 &&
            strcmp(password, employeeList[i].password) == 0) {
            printf("Employee login successful!\n");
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
    scanf("%s", customerList[customerCount].password);
    customerCount++;
    printf("Customer registered successfully!\n");
}

void customerLogin() {
    char username[30], password[30];
    printf("\nCustomer Login\nUsername: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    for (int i = 0; i < customerCount; i++) {
        if (strcmp(username, customerList[i].username) == 0 &&
            strcmp(password, customerList[i].password) == 0) {
            printf("Customer login successful!\n");
            return;
        }
    }
    printf("Login failed!\n");
}

void customerMenu() {
    int choice;
    printf("\n------ Customer Menu ------\n1. Register\n2. Login\nChoose option: ");
    scanf("%d", &choice);

    if (choice == 1) customerRegister();
    else if (choice == 2) customerLogin();
    else printf("Invalid option!\n");
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
        printf("Enter your role(position): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: adminMenu(); break;
            case 2: employeeMenu(); break;
            case 3: customerMenu(); break;
            case 4: printf("Exiting From The System....\n"); return 0;
            default: printf("Invalid choice! Kindly Try again.\n");
        }
    }

    return 0;
}
