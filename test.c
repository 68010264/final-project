#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100    
#define LEN 100     

// เก็บข้อมูล
char requestID[MAX][LEN];
char requesterName[MAX][LEN];
char expenseItem[MAX][LEN];
int amount[MAX];
int count = 0;

// อ่านข้อมูลจากไฟล์ CSV
void loadCSV() {
    FILE *fp = fopen("data.csv", "r");
    if (!fp) return; // ถ้าไม่มีไฟล์ ให้ข้าม
    char line[256];
    fgets(line, sizeof(line), fp); // ข้าม header
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        if (!token) break;
        strcpy(requestID[count], token);

        token = strtok(NULL, ",");
        strcpy(requesterName[count], token);

        token = strtok(NULL, ",");
        strcpy(expenseItem[count], token);

        token = strtok(NULL, ",");
        amount[count] = atoi(token);

        count++;
    }
    fclose(fp);
}

// เขียนข้อมูลกลับลงไฟล์ CSV
void saveCSV() {
    FILE *fp = fopen("data.csv", "w");
    fprintf(fp, "RequestID,RequesterName,ExpenseItem,Amount\n");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%s,%s,%d\n", requestID[i], requesterName[i], expenseItem[i], amount[i]);
    }
    fclose(fp);
}

// แสดงข้อมูลเฉพาะผู้ขอ
void displayByRequester() {
    char name[LEN];
    printf("Enter requester name to display : ");
    scanf(" %[^\n]", name);

    int found = 0;
    printf("\n==== DATA for %s ====\n", name);
    for (int i = 0; i < count; i++) {
        if (strcmp(requesterName[i], name) == 0) {
            printf("%d) %s | %s | %s | %d\n", i+1, requestID[i], requesterName[i], expenseItem[i], amount[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("No records found for %s.\n", name);
    }
}

// เพิ่มข้อมูลใหม่
void addData() {
    if (count >= MAX) return;
    printf("RequestID: "); scanf(" %[^\n]", requestID[count]);
    printf("RequesterName: "); scanf(" %[^\n]", requesterName[count]);
    printf("ExpenseItem: "); scanf(" %[^\n]", expenseItem[count]);
    printf("Amount: "); scanf("%d", &amount[count]);
    count++;
}

// แก้ไขข้อมูล
void editData() {
    char name[LEN];
    printf("Enter requester name: ");
    scanf(" %[^\n]", name);

    // แสดงรายการของผู้ขอคนนั้น
    int foundCount = 0;
    int indices[MAX]; // เก็บ index ของรายการที่ตรงกับผู้ขอ
    printf("\n==== Records for %s ====\n", name);
    for (int i = 0; i < count; i++) {
        if (strcmp(requesterName[i], name) == 0) {
            foundCount++;
            indices[foundCount-1] = i; // เก็บ index
            printf("%d) %s | %s | %s | %d\n", foundCount, requestID[i], requesterName[i], expenseItem[i], amount[i]);
        }
    }

    if (foundCount == 0) {
        printf("No records found for %s.\n", name);
        return;
    }

    // เลือกรายการที่จะ edit
    int choice;
    printf("Enter record number to edit (0 to cancel): ");
    scanf("%d", &choice);
    if (choice == 0) return;

    if (choice < 1 || choice > foundCount) {
        printf("Invalid choice.\n");
        return;
    }

    int idx = indices[choice-1]; // index จริงใน array
    printf("You selected: %s | %s | %s | %d\n", requestID[idx], requesterName[idx], expenseItem[idx], amount[idx]);

    // ยืนยันก่อนแก้ไข
    char confirm;
    printf("Confirm edit? (y/n): ");
    scanf(" %c", &confirm);
    if (confirm != 'y' && confirm != 'Y') {
        printf("Edit canceled, returning to menu.\n");
        return;
    }

    // แก้ไข
    printf("Enter new ExpenseItem: ");
    scanf(" %[^\n]", expenseItem[idx]);
    printf("Enter new Amount: ");
    scanf("%d", &amount[idx]);

    printf("Record updated.\n");
}

// ลบข้อมูลเฉพาะรายการ 
void deleteData() {
    char name[LEN];
    printf("Enter requester name to delete: ");
    scanf(" %[^\n]", name);

    int foundCount = 0;
    int indices[MAX]; 
    printf("\n==== Records for %s ====\n", name);
    for (int i = 0; i < count; i++) {
        if (strcmp(requesterName[i], name) == 0) {
            foundCount++;
            indices[foundCount-1] = i;
            printf("%d) %s | %s | %s | %d\n", foundCount, requestID[i], requesterName[i], expenseItem[i], amount[i]);
        }
    }

    if (foundCount == 0) {
        printf("No records found for %s.\n", name);
        return;
    }

    int choice;
    char confirm;
    printf("Enter record number to delete (0 to cancel): ");
    scanf("%d", &choice);
    if (choice == 0) {
        printf("Cancel delete, back to menu.\n");
        return;
    }
    if (choice < 1 || choice > foundCount) {
        printf("Invalid choice!\n");
        return;
    }

    int idx = indices[choice-1];
    printf("You selected: %s | %s | %s | %d\n", requestID[idx], requesterName[idx], expenseItem[idx], amount[idx]);
    printf("Confirm delete? (y/n): ");
    scanf(" %c", &confirm);
    if (confirm == 'y' || confirm == 'Y') {
        for (int i = idx; i < count-1; i++) {
            strcpy(requestID[i], requestID[i+1]);
            strcpy(requesterName[i], requesterName[i+1]);
            strcpy(expenseItem[i], expenseItem[i+1]);
            amount[i] = amount[i+1];
        }
        count--;
        printf("Record deleted.\n");
    } else {
        printf("Delete canceled.\n");
    }
}

int main() {
    loadCSV();
    int menu;
    do {
        printf("\n1. Display by requester\n2. Add\n3. Delete\n4. Edit\n5. Save & Exit\nChoose: ");
        scanf("%d", &menu);
        getchar(); // ล้าง newline
        switch(menu) {
            case 1: displayByRequester(); break;
            case 2: addData(); break;
            case 3: deleteData(); break;
            case 4: editData(); break;
            case 5: saveCSV(); printf("Saved. Exit.\n"); break;
            default: printf("Invalid menu.\n");
        }
    } while(menu != 5);
    return 0;
}
