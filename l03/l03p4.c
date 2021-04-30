/**
The program manages, in memory, an array of students on which the user can apply the following operations using the keyboard:
    add - add a new student;
        After the command, you need to input, in the folowing order: first name, surname, age, height.
        The first name and the surname cannot contain spaces.
    del - delete a student by the given position
        After the command you need to input the position.
    list - print all students;
    exit - exit the application;
At the beginnig of the program, the array is empty.
*/

/**
Programul gestioneaza, in memorie, un vector de studenti, pe care utilizatorul poate aplica operatiile, de la tastatura:
    add - adaugare student nou;
        Dupa comanda se introduce, in ordine: prenume, nume, varsta, inaltimea.
        Numele și prenumele nu pot conține spații.
    del - stergerea unui student dupa pozitie;
        Dupa comanda se introduce pozitia.
    list - afisarea tuturor studentilor;
    exit - iesirea din aplicatie;
La pornirea programului, vectorul este gol.
*/

#include <stdlib.h>
#include <stdio.h>

#define STUDENT_VECTOR_CAPACITY     2
#define STUDENT_MAX_NAME            30
#define MAX_COMMAND                 10

#define TRUE    1
#define FALSE   0

struct student 
{
    char *name;     /* no blanks */
    char *surname;  /* no blanks */
    int age; 
    float height; 
};

struct student_vector
{
    struct student **students;
    int count;
    int capacity;
};

struct student_vector *student_vector_create()
{
    struct student_vector *student_vector;

    student_vector = (struct student_vector*)malloc(sizeof(struct student_vector));
    if(NULL == student_vector) {
        return NULL;
    }
    
    student_vector->students = (struct student**)malloc(STUDENT_VECTOR_CAPACITY * sizeof(struct student*));
    if(NULL == student_vector->students) {
        return NULL;
    }
    student_vector->capacity = STUDENT_VECTOR_CAPACITY;
    student_vector->count = 0;
    
    return student_vector;
}

int student_vector_add(struct student_vector *student_vector, struct student *student)
{
    if(student_vector->capacity == student_vector->count) {
        struct student **new_students;
        int i;
        
        new_students = (struct student**)malloc((2 * student_vector->capacity) * sizeof(struct student*));
        if(NULL == new_students) {
            return FALSE;
        }
        for(i = 0; i < student_vector->count; i += 1) {
            new_students[i] = student_vector->students[i];
        }
        student_vector->capacity *= 2;
        student_vector->students = new_students;
    }
    student_vector->students[student_vector->count] = student;
    student_vector->count += 1;
    return TRUE;
}

int student_vector_remove(struct student_vector *student_vector, int index)
{
    int i;
    if(index < 0 || index >= student_vector->count) {
        return FALSE;
    }
    student_vector->count -= 1;
    for(i = index; i < student_vector->count; i += 1) {
        student_vector->students[i] = student_vector->students[i + 1];
    }
    return TRUE;
}

int main()
{
    char *command;
    int number_of_fields_read, i;
    struct student_vector *student_vector;
    int success;
    
    student_vector = student_vector_create();
    if(NULL == student_vector) {
        printf("Not enough memory to run program!\n");
        return 1;
    }
    
    for(;;) {
        printf("$ ");
        command = (char*)malloc(MAX_COMMAND + 1);
        if(NULL == command) {
            printf("ERROR! Out of memory;");
            continue;
        }
        number_of_fields_read = scanf("%10s", command);
        if(1 != number_of_fields_read) {
            printf("ERROR! Cannot process command\n");
            continue;
        }
        // exit
        if('e' == command[0] && 'x' == command[1] && 'i' == command[2] && 't' == command[3] && '\x00' == command[4]) {
            return 0;
        }
        // list
        if('l' == command[0] && 'i' == command[1] && 's' == command[2] && 't' == command[3] && '\x00' == command[4]) {
            for (i = 0; i < student_vector->count; i += 1) {
                printf("%3d.%30s%30s%10d%10.1f\n", i, student_vector->students[i]->name, student_vector->students[i]->surname, student_vector->students[i]->age, student_vector->students[i]->height);
            }
        }
        // add
        else if('a' == command[0] && 'd' == command[1] && 'd' == command[2] && '\x00' == command[3]) {
            struct student *new_student;
            
            new_student = (struct student*)malloc(sizeof(struct student));
            if (NULL == new_student) {
                printf("ERROR! Out of memory!\n");
                continue;
            }
            new_student->name = (char*)malloc(STUDENT_MAX_NAME + 1);
            new_student->surname = (char*)malloc(STUDENT_MAX_NAME + 1);
            if(NULL == new_student->name || NULL == new_student->surname) {
                printf("ERROR! Out of memory!\n");
                continue;
            }
            
            number_of_fields_read = scanf("%30s%30s%d%f", new_student->name, new_student->surname, &new_student->age, &new_student->height);
            if (4 != number_of_fields_read) {
                printf("ERROR! Invalid input!\n");
                continue;
            }
            success = student_vector_add(student_vector, new_student);
            if(success) {
                printf("Student %s %s added successfuly!\n", new_student->name, new_student->surname);
            } else {
                printf("ERROR! Could not add student %s %s!\n", new_student->name, new_student->surname);
            }
        }
        // del
        else if('d' == command[0] && 'e' == command[1] && 'l' == command[2] && '\x00' == command[3]) {
            number_of_fields_read = scanf("%d", &i);
            if (1 != number_of_fields_read) {
                printf("ERROR! Invalid input!\n");
                continue;
            }
            success = student_vector_remove(student_vector, i);
            if(success) {
                printf("Student %d removed successfully!\n", i);
            } else {
                printf("ERROR! Could not remove student %d!\n", i);
            }
        }
        // unknown command
        else {
            printf("ERROR! Bad command: %s\n", command);
        }
    }
}
