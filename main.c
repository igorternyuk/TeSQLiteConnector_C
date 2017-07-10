#include "manager.h"
#include "ctype.h"

int main(void)
{
    printf("-------------------------------------\n");
    printf("*******QSLite database manager*******\n");
    printf("-------------------------------------\n");
    //char *fileNameDB = "/home/igor/DB/SQLite/persona.db";
    bool isFileNameOK = false;
    char fileNameDB[500];
    do
    {
        printf("Type full path to the database:\n");
        scanf("%s", fileNameDB);
        FILE * file = fopen(fileNameDB, "r");
        if(file != NULL)
        {
            fclose(file);
            isFileNameOK = true;
        }
        else
        {
            printf("File path you've entered does not exist!Try again, please!\n");
        }
    } while(!isFileNameOK);

    sqlite3 *db = NULL;

    if(sqlite3_open(fileNameDB,&db) != SQLITE_OK) {
        return manager_handle_database_error(db);
    }

    manager_start(db);
    sqlite3_close(db);

    return 0;
}

