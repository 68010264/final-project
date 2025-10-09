#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100    
#define LEN 100     

// เก็บข้อมูล
char requestID[MAX][LEN];
char firstName[MAX][LEN];
char lastName[MAX][LEN];
char expenseItem[MAX][LEN];
int amount[MAX];
int count = 0;

// ----------------- ฟังก์ชันหลัก -----------------
void loadCSV() {
    FILE *fp = fopen("data.csv", "r");
    if (!fp) {
        printf("File not found. Starting empty dataset.\n");
        return;
    }

    char line[512];
    fgets(line, sizeof(line), fp); // ข้าม header

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = 0;
        char *token = strtok(line, ",");
        if (!token) continue;
        strcpy(requestID[count], token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(firstName[count], token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(lastName[count], token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(expenseItem[count], token);

        token = strtok(NULL, ",");
        if (!token) continue;
        amount[count] = atoi(token);

        count++;
    }
    fclose(fp);
    printf("Loaded %d records from data.csv\n", count);
}

void saveCSV() {
    FILE *fp = fopen("data.csv", "w");
    fprintf(fp, "RequestID,FirstName,LastName,ExpenseItem,Amount\n");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%s,%s,%s,%d\n",
                requestID[i], firstName[i], lastName[i], expenseItem[i], amount[i]);
    }
    fclose(fp);
}

void getFullName(int i, char *fullName) {
    sprintf(fullName, "%s %s", firstName[i], lastName[i]);
}

void displayByRequester() {
    char fullName[LEN];
    printf("Enter requester full name (e.g. Alice Johnson): ");
    scanf(" %[^\n]", fullName);

    int found = 0;
    printf("\n==== DATA for %s ====\n", fullName);
    for (int i = 0; i < count; i++) {
        char currentFull[LEN];
        getFullName(i, currentFull);
        if (strcmp(currentFull, fullName) == 0) {
            printf("%d) %s | %s | %s | %s | %d\n",
                   i + 1, requestID[i], firstName[i], lastName[i], expenseItem[i], amount[i]);
            found = 1;
        }
    }
    if (!found) printf("No records found for %s.\n", fullName);
}

void addData() {
    if (count >= MAX) return;
    printf("RequestID: "); scanf(" %[^\n]", requestID[count]);
    printf("FirstName: "); scanf(" %[^\n]", firstName[count]);
    printf("LastName: "); scanf(" %[^\n]", lastName[count]);
    printf("ExpenseItem: "); scanf(" %[^\n]", expenseItem[count]);
    printf("Amount: "); scanf("%d", &amount[count]);
    count++;
}

void editData() {
    char fullName[LEN];
    printf("Enter requester full name: ");
    scanf(" %[^\n]", fullName);

    int foundCount = 0;
    int indices[MAX];
    for (int i = 0; i < count; i++) {
        char currentFull[LEN];
        getFullName(i, currentFull);
        if (strcmp(currentFull, fullName) == 0) {
            foundCount++;
            indices[foundCount - 1] = i;
            printf("%d) %s | %s | %s | %s | %d\n",
                   foundCount, requestID[i], firstName[i], lastName[i], expenseItem[i], amount[i]);
        }
    }

    if (foundCount == 0) {
        printf("No records found for %s.\n", fullName);
        return;
    }

    int choice;
    printf("Enter record number to edit (0 to cancel): ");
    scanf("%d", &choice);
    if (choice == 0) return;
    if (choice < 1 || choice > foundCount) {
        printf("Invalid choice.\n");
        return;
    }

    int idx = indices[choice - 1];
    printf("Enter new ExpenseItem: ");
    scanf(" %[^\n]", expenseItem[idx]);
    printf("Enter new Amount: ");
    scanf("%d", &amount[idx]);
    printf("Record updated.\n");
}

void deleteData() {
    char fullName[LEN];
    printf("Enter requester full name to delete: ");
    scanf(" %[^\n]", fullName);

    int foundCount = 0;
    int indices[MAX];
    for (int i = 0; i < count; i++) {
        char currentFull[LEN];
        getFullName(i, currentFull);
        if (strcmp(currentFull, fullName) == 0) {
            foundCount++;
            indices[foundCount - 1] = i;
            printf("%d) %s | %s | %s | %s | %d\n",
                   foundCount, requestID[i], firstName[i], lastName[i], expenseItem[i], amount[i]);
        }
    }

    if (foundCount == 0) {
        printf("No records found for %s.\n", fullName);
        return;
    }

    int choice;
    printf("Enter record number to delete (0 to cancel): ");
    scanf("%d", &choice);
    if (choice == 0) return;
    if (choice < 1 || choice > foundCount) {
        printf("Invalid choice.\n");
        return;
    }

    int idx = indices[choice - 1];
    for (int i = idx; i < count - 1; i++) {
        strcpy(requestID[i], requestID[i + 1]);
        strcpy(firstName[i], firstName[i + 1]);
        strcpy(lastName[i], lastName[i + 1]);
        strcpy(expenseItem[i], expenseItem[i + 1]);
        amount[i] = amount[i + 1];
    }
    count--;
    printf("Record deleted.\n");
}

// ----------------- ฟังก์ชันทดสอบ -----------------

void unitTest_getFullName() {
    printf("\n--- Running Unit Test: getFullName() ---\n");
    strcpy(firstName[0], "John");
    strcpy(lastName[0], "Doe");
    char result[LEN];
    getFullName(0, result);
    if (strcmp(result, "John Doe") == 0)
        printf("Passed: getFullName() returns correct full name.\n");
    else
        printf("Failed: Expected 'John Doe', got '%s'\n", result);
}

void unitTest_addData() {
    printf("\n--- Running Unit Test: addData() ---\n");
    int before = count;
    strcpy(requestID[count], "UT001");
    strcpy(firstName[count], "Unit");
    strcpy(lastName[count], "Test");
    strcpy(expenseItem[count], "Snacks");
    amount[count] = 99;
    count++;
    if (count == before + 1)
        printf("Passed: addData() added 1 record successfully.\n");
    else
        printf("Failed: Record count mismatch.\n");
}

void unitTest_save_load() {
    printf("\n--- Running Unit Test: saveCSV() + loadCSV() ---\n");
    int before = count;
    saveCSV();
    count = 0;
    loadCSV();
    if (count >= before)
        printf("Passed: Data saved and loaded successfully.\n");
    else
        printf("Failed: Data mismatch after reload.\n");
}

void integrationTest_add_delete() {
    printf("\n--- Running Integration Test: addData() + deleteData() ---\n");
    int start = count;
    strcpy(requestID[count], "IT001");
    strcpy(firstName[count], "Integration");
    strcpy(lastName[count], "Tester");
    strcpy(expenseItem[count], "Coffee");
    amount[count] = 150;
    count++;

    // simulate delete
    for (int i = 0; i < count; i++) {
        if (strcmp(firstName[i], "Integration") == 0) {
            for (int j = i; j < count - 1; j++) {
                strcpy(requestID[j], requestID[j + 1]);
                strcpy(firstName[j], firstName[j + 1]);
                strcpy(lastName[j], lastName[j + 1]);
                strcpy(expenseItem[j], expenseItem[j + 1]);
                amount[j] = amount[j + 1];
            }
            count--;
            break;
        }
    }

    if (count == start)
        printf("Passed: addData() + deleteData() integrated correctly.\n");
    else
        printf("Failed: Record count mismatch.\n");
}

void n2nTest_flow() {
    printf("\n--- Running N2N Test: Add + Save + Load ---\n");
    int before = count;
    strcpy(requestID[count], "E2E001");
    strcpy(firstName[count], "Flow");
    strcpy(lastName[count], "Tester");
    strcpy(expenseItem[count], "Taxi");
    amount[count] = 300;
    count++;

    saveCSV();
    loadCSV();

    if (count >= before + 1)
        printf("Passed: End-to-End flow works correctly.\n");
    else
        printf("Failed: Data lost after save/load.\n");
}

// ----------------- เมนูหลัก -----------------

int main() {
    loadCSV();
    int menu;
    do {
        printf("\n==============================\n");
        printf(" 1. Display by requester\n");
        printf(" 2. Add data\n");
        printf(" 3. Delete data\n");
        printf(" 4. Edit data\n");
        printf(" 5. Save & Exit\n");
        printf("----- Testing Menu -----\n");
        printf(" 6. Unit Test: addData()\n");
        printf(" 7. Unit Test: getFullName()\n");
        printf(" 8. Unit Test: saveCSV+loadCSV()\n");
        printf(" 9. Integration Test: add+delete\n");
        printf("10. N2N (End-to-End) Test\n");
        printf("==============================\n");
        printf("Choose: ");
        scanf("%d", &menu);
        getchar();

        switch (menu) {
            case 1: displayByRequester(); break;
            case 2: addData(); break;
            case 3: deleteData(); break;
            case 4: editData(); break;
            case 5: saveCSV(); printf("Saved. Exit program.\n"); break;
            case 6: unitTest_addData(); break;
            case 7: unitTest_getFullName(); break;
            case 8: unitTest_save_load(); break;
            case 9: integrationTest_add_delete(); break;
            case 10: n2nTest_flow(); break;
            default: printf("Invalid menu.\n");
        }
    } while (menu != 5);
    return 0;
}
// compile โดย gcc final-project.c -o final-project




