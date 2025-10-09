#include "final-project.c"
void n2nTest_flow() {
    printf("\n--- Running N2N Test: Add + Save + Load (Production File) ---\n");

    if (count >= MAX) {
        printf("Cannot add new record, MAX reached.\n");
        return;
    }

    int before = count;
    strcpy(requestID[count], "E2E001");
    strcpy(firstName[count], "Flow");
    strcpy(lastName[count], "Tester");
    strcpy(expenseItem[count], "Taxi");
    amount[count] = 300;
    count++;

    // ใช้ไฟล์ production จริง
    const char *prodFile = "data.csv";
    saveCSV_test(prodFile);
    count = 0; // ลบข้อมูลใน memory ก่อน load
    loadCSV_test(prodFile);

    // ตรวจสอบว่ามี record ใหม่อยู่ใน array
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(requestID[i], "E2E001") == 0 &&
            strcmp(firstName[i], "Flow") == 0 &&
            strcmp(lastName[i], "Tester") == 0 &&
            strcmp(expenseItem[i], "Taxi") == 0 &&
            amount[i] == 300) {
            found = 1;
            break;
        }
    }

    if (found)
        printf("Passed: End-to-End flow works correctly.\n");
    else
        printf("Failed: Data lost after save/load.\n");
}
// gcc final-project.c n2n.c -o test_program
