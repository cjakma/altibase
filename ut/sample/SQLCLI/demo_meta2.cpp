#include <sqlcli.h>
#include <stdio.h>
#include <stdlib.h>



#define USERNAME            "SYS"
#define PASSWORD            "MANAGER"

#define MAX_OBJECT_NAME_LEN 128



#define PRINT_DIAGNOSTIC(handle_type, handle, str) print_diagnostic_core(handle_type, handle, __LINE__, str)
static void print_diagnostic_core(SQLSMALLINT handle_type, SQLHANDLE handle, int line, const char *str);

SQLRETURN execute_columns(SQLHDBC dbc);



int main()
{
    SQLHENV   env = SQL_NULL_HENV;
    SQLHDBC   dbc = SQL_NULL_HDBC;
    SQLRETURN rc;

    /* allocate Environment handle */
    rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    if (!SQL_SUCCEEDED(rc))
    {
        printf("SQLAllocHandle(SQL_HANDLE_ENV) error!!\n");
        goto EXIT;
    }

    /* allocate Connection handle */
    rc = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_ENV, env, "SQLAllocHandle(SQL_HANDLE_DBC)");
        goto EXIT_ENV;
    }

    /* establish connection */
    rc = SQLDriverConnect(dbc, NULL,
                          (SQLCHAR *)"Server=127.0.0.1;User=" USERNAME ";Password=" PASSWORD, SQL_NTS,
                          NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_DBC, dbc, "SQLDriverConnect");
        goto EXIT_DBC;
    }

    rc = execute_columns(dbc);
    if (!SQL_SUCCEEDED(rc))
    {
        goto EXIT_DBC;
    }

    EXIT_DBC:

    if (dbc != SQL_NULL_HDBC)
    {
        (void)SQLDisconnect(dbc);
        (void)SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    }

    EXIT_ENV:

    if (env != SQL_NULL_HENV)
    {
        (void)SQLFreeHandle(SQL_HANDLE_ENV, env);
    }

    EXIT:

    return 0;
}



static void print_diagnostic_core(SQLSMALLINT handle_type, SQLHANDLE handle, int line, const char *str)
{
    SQLRETURN   rc;
    SQLSMALLINT rec_num;
    SQLCHAR     sqlstate[6];
    SQLCHAR     message[2048];
    SQLSMALLINT message_len;
    SQLINTEGER  native_error;

    printf("Error : %d : %s\n", line, str);
    for (rec_num = 1; ; rec_num++)
    {
        rc = SQLGetDiagRec(handle_type,
                           handle,
                           rec_num,
                           sqlstate,
                           &native_error,
                           message,
                           sizeof(message),
                           &message_len);
        if (rc == SQL_NO_DATA || !SQL_SUCCEEDED(rc))
        {
            break;
        }

        printf("  Diagnostic Record %d\n", rec_num);
        printf("    SQLSTATE     : %s\n", sqlstate);
        printf("    Message text : %s\n", message);
        printf("    Message len  : %d\n", message_len);
        printf("    Native error : 0x%X\n", native_error);
    }
}



SQLRETURN execute_columns(SQLHDBC dbc)
{
    SQLHSTMT    stmt = SQL_NULL_HSTMT;
    SQLRETURN   rc;

    SQLCHAR     catalog[MAX_OBJECT_NAME_LEN + 1];
    SQLCHAR     schema[MAX_OBJECT_NAME_LEN + 1];
    SQLCHAR     table_name[MAX_OBJECT_NAME_LEN + 1];
    SQLCHAR     column_name[MAX_OBJECT_NAME_LEN + 1];
    SQLCHAR     type_name[MAX_OBJECT_NAME_LEN + 1];
    SQLCHAR     is_nullable[3 + 1];
    SQLINTEGER  column_size, buffer_len, ordinal_position;
    SQLSMALLINT data_type, decimal_digits, num_prec_radix, nullable;

    SQLLEN      catalog_len, schema_len, table_name_len, column_name_len;
    SQLLEN      data_type_ind, type_name_len, column_size_ind, buffer_len_ind;
    SQLLEN      decimal_digits_ind, num_prec_radix_ind, nullable_ind;
    SQLLEN      ordinal_position_ind, is_nullable_len;

    /* allocate Statement handle */
    rc = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_DBC, dbc, "SQLAllocHandle(SQL_HANDLE_STMT)");
        goto EXIT;
    }

    rc = SQLColumns(stmt,NULL, 0,
                    (SQLCHAR *)USERNAME, SQL_NTS,
                    (SQLCHAR *)"DEMO_META2", SQL_NTS,
                    NULL, 0);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLColumns");
        goto EXIT_STMT;
    }

    rc = SQLBindCol(stmt, 1, SQL_C_CHAR, catalog, sizeof(catalog), &catalog_len);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLBindCol");
        goto EXIT_STMT;
    }
    rc = SQLBindCol(stmt, 2, SQL_C_CHAR, schema, sizeof(schema), &schema_len);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLBindCol");
        goto EXIT_STMT;
    }
    rc = SQLBindCol(stmt, 3, SQL_C_CHAR, table_name, sizeof(table_name), &table_name_len);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLBindCol");
        goto EXIT_STMT;
    }
    rc = SQLBindCol(stmt, 4, SQL_C_CHAR, column_name, sizeof(column_name), &column_name_len);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLBindCol");
        goto EXIT_STMT;
    }
    rc = SQLBindCol(stmt, 5, SQL_C_SSHORT, &data_type, 0, &data_type_ind);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLBindCol");
        goto EXIT_STMT;
    }
    rc = SQLBindCol(stmt, 6, SQL_C_CHAR, type_name, sizeof(type_name), &type_name_len);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLBindCol");
        goto EXIT_STMT;
    }
    rc = SQLBindCol(stmt, 7, SQL_C_SLONG, &column_size, 0, &column_size_ind);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLBindCol");
        goto EXIT_STMT;
    }
    rc = SQLBindCol(stmt, 8, SQL_C_SLONG, &buffer_len, 0, &buffer_len_ind);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLBindCol");
        goto EXIT_STMT;
    }
    rc = SQLBindCol(stmt, 9, SQL_C_SSHORT, &decimal_digits, 0, &decimal_digits_ind);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLBindCol");
        goto EXIT_STMT;
    }
    rc = SQLBindCol(stmt, 10, SQL_C_SSHORT, &num_prec_radix, 0, &num_prec_radix_ind);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLBindCol");
        goto EXIT_STMT;
    }
    rc = SQLBindCol(stmt, 11, SQL_C_SSHORT, &nullable, 0, &nullable_ind);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLBindCol");
        goto EXIT_STMT;
    }
    rc = SQLBindCol(stmt, 17, SQL_C_SLONG, &ordinal_position, 0, &ordinal_position_ind);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLBindCol");
        goto EXIT_STMT;
    }
    rc = SQLBindCol(stmt, 18, SQL_C_CHAR, is_nullable, sizeof(is_nullable), &is_nullable_len);
    if (!SQL_SUCCEEDED(rc))
    {
        PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLBindCol");
        goto EXIT_STMT;
    }

    /* fetches the next rowset of data from the result set and print to stdout */
    printf("POSITION COL_NAME DATA_TYPE PRECISION SCALE IS_NULLABLE\n"
           "=======================================================\n");
    while ((rc = SQLFetch(stmt)) != SQL_NO_DATA)
    {
        if (rc == SQL_ERROR)
        {
            PRINT_DIAGNOSTIC(SQL_HANDLE_STMT, stmt, "SQLFetch");
            goto EXIT_STMT;
        }
        printf("%-8d %-8s %-9s %-9d %-5d %s\n",
               ordinal_position,
               column_name, type_name, column_size,
               decimal_digits, is_nullable);
    }
    rc = SQL_SUCCESS;

    EXIT_STMT:

    if (stmt != SQL_NULL_HSTMT)
    {
        (void)SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    }

    EXIT:

    return rc;
}
