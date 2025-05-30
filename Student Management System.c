#include <stdio.h>
#define MAX_STUDENTS 100
typedef struct {
    int id;
    char name[50];
    float grade;
} Student;
int main() {
    Student students[MAX_STUDENTS];
    int count = 0;
    int choice;
    do {
        printf("\n1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            if (count < MAX_STUDENTS) {
                printf("Enter student ID: ");
                scanf("%d", &students[count].id);
                printf("Enter student name: ");
                scanf("%s", students[count].name);
                printf("Enter student grade: ");
                scanf("%f", &students[count].grade);
                count++;
            } else {
                printf("Student list is full.\n");
            }
        } else if (choice == 2) {
            for (int i = 0; i < count; i++) {
                printf("ID: %d, Name: %s, Grade: %.2f\n", students[i].id, students[i].name, students[i].grade);
            }
        }
    } while (choice != 3);
    return 0;
}
