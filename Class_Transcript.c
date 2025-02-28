#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLASSES 50
#define MAX_NAME_LENGTH 50
#define MAX_SEMESTER_LENGTH 15

typedef struct {
    char className[MAX_NAME_LENGTH];
    char classNumber[MAX_NAME_LENGTH];
    int credits;
    char grade[3];
    char semester[MAX_SEMESTER_LENGTH];
} CourseRecord;

CourseRecord courses[MAX_CLASSES];
int courseCount = 0;

void flushStdIn();
void executeAction(char);
void save(char* fileName);
void load(char* fileName);
double calculateGPA(int numCourses, int credits[], char grades[]);

int addCourse(char* className, char* classNumber, int credits, char* grade, char* semester) {
    if (courseCount >= MAX_CLASSES) return 2;
    
    strcpy(courses[courseCount].className, className);
    strcpy(courses[courseCount].classNumber, classNumber);
    courses[courseCount].credits = credits;
    strcpy(courses[courseCount].grade, grade);
    strcpy(courses[courseCount].semester, semester);
    
    courseCount++;
    return 1;
}

void display() {
    if (courseCount == 0) {
        printf("\nNo courses available.\n");
        return;
    }

    printf("\n%-10s %-10s %-10s\n", "Course", "Credits", "Grade");
    printf("-----------------------------\n");
    
    int credits[MAX_CLASSES];
    char grades[MAX_CLASSES];

    for (int i = 0; i < courseCount; i++) {
        credits[i] = courses[i].credits;
        grades[i] = courses[i].grade[0];  // Take only the first letter of grade
        printf("%-10d %-10d %-10c\n", i + 1, credits[i], grades[i]);
    }

    // Call GPA calculation inside display
    double gpa = calculateGPA(courseCount, credits, grades);
    printf("\nYour GPA is: %.2f\n", gpa);
}

void save(char* fileName) {
    FILE* file = fopen(fileName, "wb");
    if (!file) {
        printf("Error opening file for saving.\n");
        return;
    }
    fwrite(&courseCount, sizeof(courseCount), 1, file);
    fwrite(courses, sizeof(CourseRecord), courseCount, file);
    fclose(file);
}

void load(char* fileName) {
    FILE* file = fopen(fileName, "rb");
    if (!file) return;
    fread(&courseCount, sizeof(courseCount), 1, file);
    fread(courses, sizeof(CourseRecord), courseCount, file);
    fclose(file);
}

void flushStdIn() {
    char c;
    do c = getchar();
    while (c != '\n' && c != EOF);
}
void deleteCourse() {
    if (courseCount == 0) {
        printf("\nNo courses available to delete.\n");
        return;
    }

    int index;
    printf("\nEnter the course number to delete (1-%d): ", courseCount);
    scanf("%d", &index);
    flushStdIn();

    if (index < 1 || index > courseCount) {
        printf("\nInvalid course number!\n");
        return;
    }

    index--; // Convert to 0-based index

    // Shift courses down to remove the selected course
    for (int i = index; i < courseCount - 1; i++) {
        courses[i] = courses[i + 1];
    }

    courseCount--; // Reduce course count

    printf("\nCourse successfully deleted!\n");
}

void executeAction(char c) {
    char className[MAX_NAME_LENGTH], classNumber[MAX_NAME_LENGTH], grade[3], semester[MAX_SEMESTER_LENGTH];
    int credits, add_result;
    switch (c) {
        case 'a':
            printf("\nEnter class name: ");
            fgets(className, sizeof(className), stdin);
            className[strcspn(className, "\n")] = '\0';
            printf("Enter class number: ");
            fgets(classNumber, sizeof(classNumber), stdin);
            classNumber[strcspn(classNumber, "\n")] = '\0';
            printf("Enter credits: ");
            scanf("%d", &credits);
            flushStdIn();
            printf("Enter grade (e.g., A, B+): ");
            fgets(grade, sizeof(grade), stdin);
            grade[strcspn(grade, "\n")] = '\0';
            printf("Enter semester (e.g., Fall 2024): ");
            fgets(semester, sizeof(semester), stdin);
            semester[strcspn(semester, "\n")] = '\0';
            
            add_result = addCourse(className, classNumber, credits, grade, semester);
            if (add_result == 1)
                printf("\nCourse successfully added!\n\n");
            else
                printf("\nUnable to add. Course list is full!\n\n");
            break;
        case 'd': 
            display();
            break;
        case 'r': 
            deleteCourse();
            break;
        case 'q': 
            break;
        default: 
            printf("%c is invalid input!\n", c);
    }
}


double calculateGPA(int numCourses, int credits[], char grades[]) {
    double totalPoints = 0.0;
    int totalCredits = 0;

    for (int i = 0; i < numCourses; i++) {
        double gradePoint;

        // Convert letter grade to GPA points
        switch (grades[i]) {
            case 'A': gradePoint = 4.0; break;
            case 'B': gradePoint = 3.0; break;
            case 'C': gradePoint = 2.0; break;
            case 'D': gradePoint = 1.0; break;
            case 'F': gradePoint = 0.0; break;
            default: gradePoint = 0.0; break; // Invalid grade
        }

        totalPoints += (credits[i] * gradePoint);
        totalCredits += credits[i];
    }

    if (totalCredits == 0) return 0.0; // Avoid division by zero

    return totalPoints / totalCredits; // Final GPA
}

int main() {
    char* fileName = "Student_Courses.txt";
    load(fileName);
    char choice = 'i';
    do {
        printf("\nEnter your selection:\n");
        printf("\t a: add a new course\n");
        printf("\t d: display course list\n");
        printf("\t r: remove a course\n");
        printf("\t q: quit\n");
        choice = getchar();
        flushStdIn();
        executeAction(choice);
    } while (choice != 'q');
    save(fileName);
    return 0;
}
