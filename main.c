#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

#define MAX_CHARS 500
#define CREATE_TABLE_PERSON \
    "CREATE TABLE IF NOT EXISTS person (\
        id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
        name TEXT NOT NULL,\
        age INTEGER NOT NULL\
    );"

#define SELECT_ALL "SELECT * FROM person;"

typedef struct Person Person;

struct Person
{
    int id;
    char name[MAX_CHARS];
    int age;
    Person *next;
};

void print_person(const Person *p)
{
    printf("\n------------------------\n");
    printf("ID:%d\nName:%s\nAge:%d\n", p->id, p->name, p->age);
}

Person *create_new_person(const Person *other)
{
    Person *p = (Person*)malloc(sizeof(Person));
    p->id = -1;
    strcpy(p->name, other->name);
    p->age = other->age;
    p->next = NULL;
    return p;
}

void add_person(Person **list, const Person *person)
{
    if(list == NULL)
    {
        //If the list has not been initialized
        //we will return
        return;
    }
    if(*list == NULL)
    {
        //If out list is empty we assign him the new person address
        *list = create_new_person(person);
        return;
    }
    Person *it = *list;
    while(it->next != NULL)
    {
        it = it->next;
    }
    it->next = create_new_person(person);
}

void print_list(Person *list)
{
    if(list == NULL)
    {
        printf("List is empty");
        return;
    }
    Person *it = list;
    while(it != NULL)
    {
        it = it->next;
        print_person(it);
    }
}

int manage_database_error(sqlite3 *db)
{
    fprintf(stderr, "Error: %s(%d)\n", sqlite3_errmsg(db), sqlite3_errcode(db));
    return sqlite3_errcode(db);
}

void insert_into_database(Person *list, sqlite3 *db)
{
    if(list == NULL)
    {
        return;
    }
    Person *it = list;
    char sql[200];
    while(it != NULL)
    {
        sprintf(sql, "INSERT INTO person(name, age) VALUES('%s',%d);", it->name, it->age);
        if(sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK)
        {
            manageError(db);
            return;
        }
        it = it->next;
    }
}

int callback(void* ptr, int colNumber, char** values, char** colName)
{
    (void)ptr;
    for(int i = 0; i < colNumber; ++i)
    {
        printf("%s:%s\n", colName[i], values[i]);
    }
    printf("---------------------\n");
    return 0;
}

void read_database(sqlite3 *db)
{
    sqlite3_exec(db, SELECT_ALL, callback, NULL, NULL);
}

int main(void)
{
    const char *fileNameDB = "/home/igor/DB/SQLite/persona.db";
    const char *pathToDataFile = "datos.txt";
    FILE *fileNameData = NULL;
    sqlite3 *db = NULL;
    Person *list = NULL;

    //open db

    if(sqlite3_open(fileNameDB,&db) != SQLITE_OK) {
        return manageError(db);
    }

    //configure database

    /*if(sqlite3_exec(db, CREATE_TABLE_PERSON, NULL, NULL, NULL) != SQLITE_OK){
        return manageError(db);
    }*/

    //read the list

    fileNameData = fopen(pathToDataFile, "r");
    if(fileNameData != NULL)
    {
        Person p;
        while(!feof(fileNameData))
        {
            fscanf(fileNameData, "%s%d", &p.name, &p.age);
            add_person(&list, &p);
        }
        fclose(fileNameData);
    }
    else
    {
        fprintf(stderr, "File error");
        return ferror(fileNameData);
    }

    //insertDate(list, db);
    //print_list(list);

    read_database(db);

    sqlite3_close(db);

    return 0;
}
