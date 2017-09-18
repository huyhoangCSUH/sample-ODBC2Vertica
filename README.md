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