#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* ������ ��� ������ ������� */
#define SHW_ADR(ID, I) (printf("ID %s \t is at virtual address: %8p\n", ID, (void*)&I))

char* cptr = "This message is output by the function showit()\n"; /* ����������� ���������� */
char buffer1[25];

/* �������� ������� */
int showit(char* p);

int main() {
    int i = 0; /* �������������� ���������� */

    /* ����� ���������� � ������ �������� */
    printf("\nAddress of main: %8p\n", (void*)main);
    printf("Address of showit: %8p\n", (void*)showit);
    printf("Address of cptr: %8p\n", (void*)cptr);
    printf("Address of buffer1: %8p\n", (void*)buffer1);
    printf("Address of i: %8p\n", (void*)&i);

    strcpy(buffer1, "A demonstration\n");   /* ����������� ������ */
    write(1, buffer1, strlen(buffer1) + 1); /* ��������� ����� */
    showit(cptr);

    return 0;
}

/* ������� ��� ������������ ������ � ������� */
int showit(char* p) {
    char* buffer2;
    if ((buffer2 = (char*)malloc((unsigned)(strlen(p) + 1))) != NULL) {
        printf("Allocated memory at %p\n", (void*)buffer2);
        strcpy(buffer2, p);    /* ����������� ������ */
        printf("%s", buffer2); /* ����� ������ */
        free(buffer2);         /* ������������ ������ */
    }
    else {
        printf("Allocation error\n");
        exit(1);
    }
    return 0;
}