# sample-ODBC2Vertica
Sample program to connect to Vertica server from C++

This program uses unixODBC run on an Ubuntu machine

Step 1: Install unixODBC
On Ubuntu, type in:

```
sudo apt-get install unixodbc-dev unixodbc-bin unixodbc
```

Step 2: Create an ODBC DSN: 

Open the file 
```
/etc/odbc.ini 
```
then create a DSN as follow:

```
[ODBC Data Sources]
mydsn = "vmartdb"

[mydsn]
Description = Give it some description
Driver = /opt/vertica/lib64/libverticaodbc.so
Database = your_database
Servername = your_server_address
UID = your_username
PWD = your_password
Port = 5433

[ODBC]
Threading = 1 
```

Step 3:
Create a file named /etc/vertica.ini with the [Driver] section:

```
[Driver]
ODBCInstLib = /usr/lib/x86_64-linux-gnu/libodbcinst.a(libodbcinst.so.1)
DriverManagerEncoding = UTF-8
ErrorMessagesPath = /opt/vertica/lib64
LogLevel=4
LogPath = /tmp
Locale = en_US@collation=binary
```

This file doesn't have to be in a specific folder actually, users can put it everywhere and export the environment variable as follow:

```
export VERTICAINI=/etc/vertica.ini
```

Step 4: Test if Vertica can be connected through ODBC interactive client by typing in:

```
isql mydsn
```
if it shows something like this, it means you can start writing your C++ code.
```
+---------------------------------------+
| Connected!                            |
|                                       |
| sql-statement                         |
| help [tablename]                      |
| quit                                  |
|                                       |
+---------------------------------------+
SQL> 
```

Step 4: In your C++ program, provide the right dsn to the variable dsnName

Step 5: Compile the program as follow:
```
g++ your_program.cpp -lodbc
```
