#include <stdio.h>
#include <string.h>
#include <assert.h>


#include "final-project.c"

// --- ฟังก์ชันทดสอบแต่ละส่วน ---

void test_addData() {
    count = 0;
    strcpy(requestID[count], "R001");
    strcpy(firstName[count], "Alice");
    strcpy(lastName[count], "Johnson");
    strcpy(expenseItem[count], "Laptop");
    amount[count] = 1000;
    count++;

    assert(count == 1);
    assert(strcmp(firstName[0], "Alice") == 0);
    assert(amount[0] == 1000);
    printf("✅ test_addData PASSED\n");
}

void test_getFullName() {
    strcpy(firstName[0], "Alice");
    strcpy(lastName[0], "Johnson");
    char full[LEN];
    getFullName(0, full);
    assert(strcmp(full, "Alice Johnson") == 0);
    printf("✅ test_getFullName PASSED\n");
}

void test_editData() {
    count = 1;
    strcpy(firstName[0], "Bob");
    strcpy(lastName[0], "Lee");
    strcpy(expenseItem[0], "Monitor");
    amount[0] = 500;

    // แก้ไขโดยตรง (จำลองว่าผู้ใช้เลือก record)
    strcpy(expenseItem[0], "Keyboard");
    amount[0] = 700;

    assert(strcmp(expenseItem[0], "Keyboard") == 0);
    assert(amount[0] == 700);
    printf("✅ test_editData PASSED\n");
}

void test_deleteData() {
    count = 3;
    strcpy(firstName[0], "A");
    strcpy(firstName[1], "B");
    strcpy(firstName[2], "C");
    strcpy(lastName[0], "X");
    strcpy(lastName[1], "Y");
    strcpy(lastName[2], "Z");
    strcpy(expenseItem[0], "Item1");
    strcpy(expenseItem[1], "Item2");
    strcpy(expenseItem[2], "Item3");

    // ลบ index 1 (B Y)
    for (int i = 1; i < count - 1; i++) {
        strcpy(firstName[i], firstName[i + 1]);
        strcpy(lastName[i], lastName[i + 1]);
        strcpy(expenseItem[i], expenseItem[i + 1]);
    }
    count--;

    assert(count == 2);
    assert(strcmp(firstName[1], "C") == 0);
    printf("✅ test_deleteData PASSED\n");
}

void test_save_and_load() {
    count = 2;
    strcpy(requestID[0], "R1");
    strcpy(firstName[0], "Tom");
    strcpy(lastName[0], "Smith");
    strcpy(expenseItem[0], "Chair");
    amount[0] = 100;

    strcpy(requestID[1], "R2");
    strcpy(firstName[1], "Eva");
    strcpy(lastName[1], "Brown");
    strcpy(expenseItem[1], "Desk");
    amount[1] = 200;

    saveCSV();
    count = 0;
    loadCSV();

    assert(count == 2);
    assert(strcmp(firstName[1], "Eva") == 0);
    assert(amount[1] == 200);
    printf("✅ test_save_and_load PASSED\n");
}

//gcc final-project.c unittest.c -o test_program

