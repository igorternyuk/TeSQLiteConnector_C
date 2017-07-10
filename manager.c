#include "manager.h"

void manager_print_worker_info(const Worker *p)
{
    printf("\n------------------------\n");
    printf("ID:%d\nName:%s\nAge:%d\nSalary:%d", p->id, p->name, p->age, p->salary);
}

void manager_read_worker_from_console(Worker *p, bool withID)
{
    //Here must be digital data validation
    printf("Name: ");
    scanf("%s", p->name);
    int age;
    manager_read_digital("Age: ", &age, 100);
    p->age = age;
    int salary;
    manager_read_digital("Salary: ", &salary, 1000000000);
    p->salary = salary;
    if(withID)
    {
       int id;
       manager_read_digital("ID:\n", &id, 1000000000);
       p->id = id;
    }
    else
    {
        p->id = -1;
    }
}

int manager_handle_database_error(sqlite3 *db)
{
    fprintf(stderr, "Error: %s(%d)\n", sqlite3_errmsg(db), sqlite3_errcode(db));
    return sqlite3_errcode(db);
}

void manager_insert_into_database(Worker *list, sqlite3 *db)
{
    if(list == NULL)
    {
        return;
    }
    Worker *it = list;
    char sql[200];
    while(it != NULL)
    {
        sprintf(sql, "INSERT INTO Worker(name, age) VALUES('%s',%d);", it->name, it->age);
        if(sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK)
        {
            manager_handle_database_error(db);
            return;
        }
        it = it->next;
    }
}

int manager_callback(void* ptr, int colNumber, char** values, char** colName)
{
    (void)ptr;
    for(int i = 0; i < colNumber; ++i)
    {
        printf("%s: %s\n", colName[i], values[i]);
    }
    printf("---------------------\n");
    return 0;
}

int manager_callback_file(void* ptr, int colNumber, char** values, char** colName)
{
    (void)ptr;
    FILE *file = fopen("AllDataBaseRegisters.txt", "a");
    for(int i = 0; i < colNumber; ++i)
    {
        fprintf(file, "%s:%s\n", colName[i], values[i]);
    }
    fprintf(file, "---------------------\n");
    fclose(file);
    return 0;
}

bool manager_insert_register(sqlite3 *db, Worker *p)
{
    char sql[MAX_CHARS];
    sprintf(sql, SQL_INSERT_REGISTER, p->name, p->age, p->salary);
    if(sqlite3_exec(db, sql, manager_callback, NULL, NULL) != SQLITE_OK)
    {
        manager_handle_database_error(db);
        return false;
    }
    else
    {
        return true;
    }
}

bool manager_update_register(sqlite3 *db, Worker *p)
{
    char sql[MAX_CHARS];
    sprintf(sql, SQL_UPDATE_REGISTER, p->name, p->age, p->salary, p->id);
    if(sqlite3_exec(db, sql, manager_callback, NULL, NULL) != SQLITE_OK)
    {
        manager_handle_database_error(db);
        return false;
    }
    else
    {
        return true;
    }
}

bool manager_delete_register(sqlite3 *db, int id)
{
    char sql[MAX_CHARS];
    sprintf(sql, SQL_DELETE_REGISTER, id);
    if(sqlite3_exec(db, sql, manager_callback, NULL, NULL) != SQLITE_OK)
    {
        manager_handle_database_error(db);
        return false;
    }
    else
    {
        return true;
    }
}

void manager_read_by_id(sqlite3 *db, int id)
{
    char sql[MAX_CHARS];
    sprintf(sql, SQL_SELECT_BY_ID, id);
    if(sqlite3_exec(db, sql, manager_callback, NULL, NULL) != SQLITE_OK)
    {
        manager_handle_database_error(db);
    }
}

void manager_read_all(sqlite3 *db)
{
    if(sqlite3_exec(db, SQL_SELECT_ALL, manager_callback, NULL, NULL) != SQLITE_OK)
    {
        manager_handle_database_error(db);
    }
}

void manager_search_registers(sqlite3 *db, const char *regExpr)
{
    char sql[MAX_CHARS];
    sprintf(sql,"%s%s%s",SQL_SEARCH_BY_NAME, regExpr, "%';");
    printf("SQL = %s", sql);
    if(sqlite3_exec(db, sql, manager_callback, NULL, NULL) != SQLITE_OK)
    {
        manager_handle_database_error(db);
    }
}

void manager_insert_all_registers_from_file(sqlite3 *db, const char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if(file == NULL)
    {
        printf("\nCould not find file\n");
        return;
    }
    Worker w;
    while(!feof(file))
    {
        char name[MAX_CHARS];
        int age, salary;
        fscanf(file,"%s %d %d", name, &age, &salary);
        sprintf(w.name, "%s", name);
        w.age = age;
        w.salary = salary;
        manager_print_worker_info(&w);
        manager_insert_register(db, &w);
    }
    fclose(file);
}

void manager_write_all_registers_to_file(sqlite3 *db)
{
    if(sqlite3_exec(db, SQL_SELECT_ALL, manager_callback_file, NULL, NULL) != SQLITE_OK)
    {
        manager_handle_database_error(db);
    }
}

void manager_print_menu()
{
    printf("-------------------------------------\n");
    printf("************MANAGER MENU*************\n");
    printf("-------------------------------------\n");
    printf("1.Insert register;\n");
    printf("2.Update register;\n");
    printf("3.Delete register;\n");
    printf("4.Select by ID;\n");
    printf("5.Select all;\n");
    printf("6.Search by name;\n");
    printf("7.Add registers from file;\n");
    printf("8.Write registers to file;\n");
    printf("9.Quit;\n");
    printf(">>>");
}

void manager_start(sqlite3 *db)
{
    bool mayBeterminate = false;
    Worker w;
    int id;
    char regExpr[MAX_CHARS];
    char pathToFile[MAX_CHARS];
    while(!mayBeterminate)
    {
        int input;
        manager_print_menu();
        scanf("%d", &input);
        switch(input)
        {
            case 1:
                manager_read_worker_from_console(&w, false);
                if(manager_insert_register(db, &w))
                {
                    printf("\nNew register has been inserted successfully.\n");
                }
                else
                {
                    printf("\nTransaction failed\n");
                }
                break;
            case 2:
                manager_read_worker_from_console(&w, true);
                if(manager_update_register(db, &w))
                {
                    printf("\nRegister with id = %d was successfully updated.\n", w.id);
                }
                else
                {
                    printf("Transaction failed");
                }
                break;
            case 3:
                manager_read_digital("\nType id of the worker:\n", &id, 1000000000);
                if(manager_delete_register(db, id))
                {
                    printf("\nRegister with id = %d was successfully deleted.\n", w.id);
                }
                else
                {
                    printf("\nTransaction failed\n");
                }
                break;
            case 4:
                manager_read_digital("\nType id of the worker:\n", &id, 1000000000);
                manager_read_by_id(db, id);
                break;
            case 5:
                manager_read_all(db);
                break;
            case 6:
                printf("\nType part of the worker name:\n");
                scanf("%s", regExpr);
                manager_search_registers(db, regExpr);
                break;
            case 7:
                printf("\nType path to the file with data:\n");
                scanf("%s", pathToFile);
                manager_insert_all_registers_from_file(db, pathToFile);
                break;
            case 8:
                 manager_write_all_registers_to_file(db);
                break;
            case 9:
                mayBeterminate = true;
                printf("\nGood bye!\n");
                break;
           default:
                break;
        }
    }
}

void manager_read_digital(const char *msg, int *val, int limit)
{
    char buff[CHAR_BUFFER];
    bool isOK = false;
    do
    {
        printf("%s",msg);
        scanf("%s", buff);
        if(isdigit(buff[0]))
        {
            *val = atoi(buff);
            if(*val < limit)
            {
                isOK = true;
                //printf("Val = %d\n", *val);
            }
            else
            {
                printf("\nTo big value!Try again.\n");
            }
        }

    }while(!isOK);
}
