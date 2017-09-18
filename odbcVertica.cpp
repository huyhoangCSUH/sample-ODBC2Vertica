#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <assert.h>
// Standard ODBC headers
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

using namespace std;
// Helper function to print SQL error messages.
template <typename HandleT>
void reportError(int handleTypeEnum, HandleT hdl)
{
    // Get the status records.
    SQLSMALLINT   i, MsgLen;
    SQLRETURN ret2;
    SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
    SQLINTEGER    NativeError;
    i = 1;
    cout << endl;
    while ((ret2 = SQLGetDiagRec(handleTypeEnum, hdl, i, SqlState, &NativeError,
                                Msg, sizeof(Msg), &MsgLen)) != SQL_NO_DATA) {
        cout << "error record #" << i++ << endl;
        cout << "sqlstate: " << SqlState << endl;
        cout << "detailed msg: " << Msg << endl;
        cout << "native error code: " << NativeError << endl;
    }
}

int main() {
	SQLRETURN ret;   // Stores return value from ODBC API calls
    SQLHENV hdlEnv;  // Handle for the SQL environment object
    // Allocate an a SQL environment object
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hdlEnv); 
    if(!SQL_SUCCEEDED(ret)) {
        printf("Could not allocate a handle.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Allocated an environment handle.\n");
    }
    // Set the ODBC version we are going to use to 
    // 3.
    ret = SQLSetEnvAttr(hdlEnv, SQL_ATTR_ODBC_VERSION,
            (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_UINTEGER);
    if(!SQL_SUCCEEDED(ret)) {
         printf("Could not set application version to ODBC 3.\n");
         exit(EXIT_FAILURE);
    } else {
         printf("Set application version to ODBC 3.\n");
    }
    // Allocate a database handle.
    SQLHDBC hdlDbc;
     ret = SQLAllocHandle(SQL_HANDLE_DBC, hdlEnv, &hdlDbc); 
     if(!SQL_SUCCEEDED(ret)) {
          printf("Could not allocate database handle.\n");
          exit(EXIT_FAILURE);
     } else {
          printf("Allocated Database handle.\n");
     }
    // Connect to the database using 
    // SQL Connect
    printf("Connecting to database.\n");
    const char *dsnName = "your_defined_DSN";
    const char* userID = "dbuser";
    const char* passwd = "thepassword";
    ret = SQLConnect(hdlDbc, (SQLCHAR*)dsnName,
        SQL_NTS,(SQLCHAR*)userID,SQL_NTS,
        (SQLCHAR*)passwd, SQL_NTS);
    if(!SQL_SUCCEEDED(ret)) {
        printf("Could not connect to database.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Connected to database.\n");
    }
    // This part is for executing SQL queries
    // Set up a statement handle
    SQLHSTMT hdlStmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hdlDbc, &hdlStmt);
    assert(SQL_SUCCEEDED(ret)); 
    
    // Execute a query to get the names and IDs of all tables in the schema
    // search p[ath (usually public).
    ret = SQLExecDirect( hdlStmt, (SQLCHAR*)"SELECT i,j "
        "FROM webgoogle LIMIT(10)", SQL_NTS );
    
    if(!SQL_SUCCEEDED(ret)) { 
        // Report error an go no further if statement failed.
        cout << "Error executing statement." << endl;
        reportError<SQLHDBC>(SQL_HANDLE_STMT, hdlStmt);
        exit(EXIT_FAILURE);
    } else {
        // Query succeeded, so bind two variables to the two colums in the 
        // result set,
        cout << "Fetching results..." << endl;
        SQLBIGINT table_id;       // Holds the ID of the table.
        SQLTCHAR table_name[256]; // buffer to hold name of table
        ret = SQLBindCol(hdlStmt, 1, SQL_C_SBIGINT, (SQLPOINTER)&table_id, 
            sizeof(table_id), NULL);
        ret = SQLBindCol(hdlStmt, 2, SQL_C_TCHAR, (SQLPOINTER)table_name, 
            sizeof(table_name), NULL);
        
        // Loop through the results, 
        while( SQL_SUCCEEDED(ret = SQLFetchScroll(hdlStmt, SQL_FETCH_NEXT,1))) {
            // Print the bound variables, which now contain the values from the
            // fetched row.
            cout << table_id << " | " << table_name << endl;
        }
        
        
        // See if loop exited for reasons other than running out of data
        if (ret != SQL_NO_DATA) {
            // Exited for a reason other than no more data... report the error.
            reportError<SQLHDBC>( SQL_HANDLE_STMT, hdlStmt );    
        }
    }
    // When done, free all of the handles    
    printf("Disconnecting and freeing handles.\n");
    ret = SQLDisconnect( hdlDbc );
    if(!SQL_SUCCEEDED(ret)) {
        printf("Error disconnecting from database. Transaction still open?\n");
        exit(EXIT_FAILURE);
    }
    
    SQLFreeHandle(SQL_HANDLE_DBC, hdlDbc); 
    SQLFreeHandle(SQL_HANDLE_ENV, hdlEnv);  
    exit(EXIT_SUCCESS);
}