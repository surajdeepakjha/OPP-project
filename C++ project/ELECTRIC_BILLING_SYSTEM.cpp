#include <iostream>
#include <string>
using namespace std;

const double RATE_SLAB1 = 1.50;   
const double RATE_SLAB2 = 2.50;   
const double RATE_SLAB3 = 4.00;   
const double RATE_SLAB4 = 6.00;   
const double FIXED_CHARGE = 50.0; 
const double TAX_RATE     = 0.05; 
const int    MAX_CUSTOMERS = 100;

struct Customer {
    int    id;
    string name;
    string address;
    string phone;
    int    unitsConsumed;
    double billAmount;
    string month;
    bool   isPaid;
};

Customer customers[MAX_CUSTOMERS];
int customerCount = 0;
int nextID = 1001;

void   displayMenu();
void   addCustomer();
void   calculateBill();
void   displayAllRecords();
void   displaySingleRecord();
void   markAsPaid();
void   searchCustomer();
void   displaySummary();
double computeBill(int units);
int    findCustomerByID(int id);
void   printDivider(char ch, int len);
void   printBillReceipt(int index);

int main() {
    int choice;

    cout << "\n";
    printDivider('=', 55);
    cout << "      WELCOME TO ELECTRIC BILLING SYSTEM\n";
    printDivider('=', 55);

    do {
        displayMenu();
        cout << "  Enter your choice: ";
        cin  >> choice;

        switch (choice) {
            case 1: addCustomer();       break;
            case 2: calculateBill();     break;
            case 3: displayAllRecords(); break;
            case 4: displaySingleRecord();break;
            case 5: markAsPaid();        break;
            case 6: searchCustomer();    break;
            case 7: displaySummary();    break;
            case 0:
                printDivider('-', 55);
                cout << "  Thank you for using Electric Billing System!\n";
                printDivider('=', 55);
                break;
            default:
                cout << "  [!] Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}

void displayMenu() {
    cout << "\n";
    printDivider('-', 55);
    cout << "               MAIN MENU\n";
    printDivider('-', 55);
    cout << "  1. Add New Customer\n";
    cout << "  2. Calculate / Update Bill\n";
    cout << "  3. Display All Records\n";
    cout << "  4. Display Single Record\n";
    cout << "  5. Mark Bill as Paid\n";
    cout << "  6. Search Customer\n";
    cout << "  7. Summary Report\n";
    cout << "  0. Exit\n";
    printDivider('-', 55);
}


void addCustomer() {
    if (customerCount >= MAX_CUSTOMERS) {
        cout << "\n  [!] Customer storage full!\n";
        return;
    }

    Customer c;
    c.id       = nextID++;
    c.isPaid   = false;
    c.billAmount = 0.0;
    c.unitsConsumed = 0;

    cin.ignore();
    printDivider('-', 55);
    cout << "         ADD NEW CUSTOMER\n";
    printDivider('-', 55);

    cout << "  Name    : "; getline(cin, c.name);
    cout << "  Address : "; getline(cin, c.address);
    cout << "  Phone   : "; getline(cin, c.phone);
    cout << "  Month   : "; getline(cin, c.month);

    customers[customerCount++] = c;

    cout << "\n [OK] Customer added successfully!\n";
    cout << "  Assigned Customer ID: " << c.id << "\n";
}

void calculateBill() {
    if (customerCount == 0) {
        cout << "\n  [!] No customers found. Add a customer first.\n";
        return;
    }

    int id;
    cout << "\n  Enter Customer ID to calculate bill: ";
    cin  >> id;

    int idx = findCustomerByID(id);
    if (idx == -1) {
        cout << "  [!] Customer ID not found.\n";
        return;
    }

    cout << "  Enter units consumed this month: ";
    cin  >> customers[idx].unitsConsumed;

    customers[idx].billAmount = computeBill(customers[idx].unitsConsumed);
    customers[idx].isPaid     = false;

    cout << "\n  [OK] Bill calculated successfully!\n";
    printBillReceipt(idx);
}

double computeBill(int units) {
    double energy = 0.0;

    if (units <= 100) {
        energy = units * RATE_SLAB1;
    } else if (units <= 200) {
        energy = (100 * RATE_SLAB1) + ((units - 100) * RATE_SLAB2);
    } else if (units <= 300) {
        energy = (100 * RATE_SLAB1) + (100 * RATE_SLAB2) + ((units - 200) * RATE_SLAB3);
    } else {
        energy = (100 * RATE_SLAB1) + (100 * RATE_SLAB2) + (100 * RATE_SLAB3)
                 + ((units - 300) * RATE_SLAB4);
    }

    double subtotal = energy + FIXED_CHARGE;
    double tax      = subtotal * TAX_RATE;
    return subtotal + tax;
}

void printBillReceipt(int i) {
    Customer& c = customers[i];

    cout << "\n";
    printDivider('=', 55);
    cout << "           ELECTRICITY BILL RECEIPT\n";
    printDivider('=', 55);
    cout << "  Customer ID   : " << c.id       << "\n";
    cout << "  Name          : " << c.name     << "\n";
    cout << "  Address       : " << c.address  << "\n";
    cout << "  Phone         : " << c.phone    << "\n";
    cout << "  Billing Month : " << c.month    << "\n";
    printDivider('-', 55);
    cout << "  Units Consumed: " << c.unitsConsumed << " kWh\n\n";
    cout << "  Tariff Slabs Applied:\n";
    if (c.unitsConsumed > 0)
        cout << "    0-100 units @ Rs " << RATE_SLAB1 << "/unit\n";
    if (c.unitsConsumed > 100)
        cout << "    101-200 units @ Rs " << RATE_SLAB2 << "/unit\n";
    if (c.unitsConsumed > 200)
        cout << "    201-300 units @ Rs " << RATE_SLAB3 << "/unit\n";
    if (c.unitsConsumed > 300)
        cout << "    301+ units @ Rs "    << RATE_SLAB4 << "/unit\n";
    printDivider('-', 55);
    cout << "  Fixed Charge  : Rs " << FIXED_CHARGE << "\n";
    cout << "  Tax (5%)      : Rs ";
    // print tax
    double energy = c.billAmount / 1.05;
    cout << (energy * TAX_RATE) << "\n";
    cout << "  TOTAL AMOUNT  : Rs " << c.billAmount << "\n";
    printDivider('-', 55);
    cout << "  Payment Status: " << (c.isPaid ? "PAID" : "PENDING") << "\n";
    printDivider('=', 55);
}

void displayAllRecords() {
    if (customerCount == 0) {
        cout << "\n  [!] No records available.\n";
        return;
    }

    cout << "\n";
    printDivider('=', 70);
    cout << "  ID   | Name                | Month     | Units | Amount (Rs) | Status\n";
    printDivider('-', 70);

    for (int i = 0; i < customerCount; i++) {
        Customer& c = customers[i];
        cout << "  " << c.id << " | ";
        // Pad name to 20 chars
        string nm = c.name.length() > 19 ? c.name.substr(0,19) : c.name;
        cout << nm;
        for (int s = nm.length(); s < 20; s++) cout << ' ';
        cout << "| ";
        string mn = c.month.length() > 9 ? c.month.substr(0,9) : c.month;
        cout << mn;
        for (int s = mn.length(); s < 10; s++) cout << ' ';
        cout << "| " << c.unitsConsumed;
        for (int s = to_string(c.unitsConsumed).length(); s < 6; s++) cout << ' ';
        cout << "| " << c.billAmount << "       | ";
        cout << (c.isPaid ? "PAID" : "PENDING") << "\n";
    }
    printDivider('=', 70);
}

void displaySingleRecord() {
    int id;
    cout << "\n  Enter Customer ID: ";
    cin  >> id;

    int idx = findCustomerByID(id);
    if (idx == -1) {
        cout << "  [!] Customer not found.\n";
        return;
    }
    printBillReceipt(idx);
}

void markAsPaid() {
    int id;
    cout << "\n  Enter Customer ID to mark as paid: ";
    cin  >> id;

    int idx = findCustomerByID(id);
    if (idx == -1) {
        cout << "  [!] Customer not found.\n";
        return;
    }

    if (customers[idx].isPaid) {
        cout << "  [i] Bill is already marked as paid.\n";
    } else {
        customers[idx].isPaid = true;
        cout << "  [OK] Bill for " << customers[idx].name << " marked as PAID.\n";
    }
}

void searchCustomer() {
    string query;
    cin.ignore();
    cout << "\n  Enter name or phone to search: ";
    getline(cin, query);

    bool found = false;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].name.find(query)  != string::npos ||
            customers[i].phone.find(query) != string::npos) {
            printBillReceipt(i);
            found = true;
        }
    }
    if (!found) cout << "  [!] No matching customer found.\n";
}


void displaySummary() {
    if (customerCount == 0) {
        cout << "\n  [!] No records available.\n";
        return;
    }

    int    totalCustomers = customerCount;
    int    paidCount      = 0;
    int    pendingCount   = 0;
    double totalRevenue   = 0.0;
    double pendingAmount  = 0.0;
    int    totalUnits     = 0;

    for (int i = 0; i < customerCount; i++) {
        totalUnits += customers[i].unitsConsumed;
        if (customers[i].isPaid) {
            paidCount++;
            totalRevenue += customers[i].billAmount;
        } else {
            pendingCount++;
            pendingAmount += customers[i].billAmount;
        }
    }

    cout << "\n";
    printDivider('=', 55);
    cout << "            SUMMARY REPORT\n";
    printDivider('=', 55);
    cout << "  Total Customers    : " << totalCustomers << "\n";
    cout << "  Bills Paid         : " << paidCount      << "\n";
    cout << "  Bills Pending      : " << pendingCount   << "\n";
    cout << "  Total Units (kWh)  : " << totalUnits     << "\n";
    printDivider('-', 55);
    cout << "  Revenue Collected  : Rs " << totalRevenue  << "\n";
    cout << "  Pending Amount     : Rs " << pendingAmount << "\n";
    printDivider('=', 55);
}

int findCustomerByID(int id) {
    for (int i = 0; i < customerCount; i++)
        if (customers[i].id == id) return i;
    return -1;
}

void printDivider(char ch, int len) {
    cout << "  ";
    for (int i = 0; i < len; i++) cout << ch;
    cout << "\n";
}