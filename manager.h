#ifndef MANAGER_H
#define MANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sqlite3.h"

#define MAX_CHARS 500
#define CHAR_BUFFER 20
#define CREATE_TABLE_WORKER \
    "CREATE TABLE IF NOT EXISTS worker (\
        id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
        name TEXT NOT NULL,\
        age INTEGER NOT NULL,\
        salary DOUBLE\
    );"
#define SQL_INSERT_REGISTER "INSERT INTO worker VALUES(default,'%s',%d,%d);"
#define SQL_UPDATE_REGISTER "UPDATE worker SET name = '%s', age = %d, salary = %d WHERE id = %d;"
#define SQL_DELETE_REGISTER "DELETE FROM worker WHERE id = %d;"
#define SQL_SELECT_BY_ID "SELECT * FROM worker WHERE id = %d;"
#define SQL_SELECT_ALL "SELECT * FROM worker;"
#define SQL_SEARCH_BY_NAME "SELECT * FROM worker WHERE name like '%"

typedef struct Worker Worker;

struct Worker
{
    int id;
    char name[MAX_CHARS];
    int age;
    int salary;
    Worker *next;
};

void manager_print_worker_info(const Worker *p);

void manager_read_worker_from_console(Worker *p, bool withID);

int manager_handle_database_error(sqlite3 *db);

void manager_insert_into_database(Worker *list, sqlite3 *db);

int manager_callback(void* ptr, int colNumber, char** values, char** colName);

int manager_callback_file(void* ptr, int colNumber, char** values, char** colName);

bool manager_insert_register(sqlite3 *db, Worker *p);

bool manager_update_register(sqlite3 *db, Worker *p);

bool manager_delete_register(sqlite3 *db, int id);

void manager_read_by_id(sqlite3 *db, int id);

void manager_read_all(sqlite3 *db);

void manager_search_registers(sqlite3 *db, const char *regExpr);

void manager_insert_all_registers_from_file(sqlite3 *db, const char *fileName);

void manager_write_all_registers_to_file(sqlite3 *db);

void manager_print_menu();

void manager_start(sqlite3 *db);

void manager_read_digital(const char *msg, int *val, int limit);

#endif // MANAGER_H
