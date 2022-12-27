/**
 * @file table.h
 * @author Hector Ochando <ochandoca@esat.alumni.com>
 * @brief 
 * 
 */

#ifndef __TABLE_H__
#define __TABLE_H__ 1

typedef struct Table Table;

enum DataType{
  UNDEFINED = -1,          //9??
  BIT = 1,                 //3
  INT = 2,                 //3
  BLOB = 3,                //4
  BOOL = 4,                //4
  BIGINT = 5,              //6
  BINARY = 6,              //6
  DECIMAL = 7,            //7
  TINYINT = 8,            //7
  BOOLEAN = 9,            //7
  INTEGER = 10,            //7
  TINYBLOB = 11,           //8
  SMALLINT = 12,           //8
  LONGBLOB = 13,           //8
  MEDIUMINT = 14,          //9
  MEDIUMBLOB = 15,         //10 
  DEC = 16,                 //3 *
  FLOAT = 17,               //5 *
  DOUBLE = 18,              //6 *
  DOUBLE_PRECISION = 19,   //16 *
  NUMERIC = 20,            //7 *
  DATE = 21,               //4---
  CHAR = 22,               //4--
  TEXT = 23,               //4--
  TIME = 24,               //4--
  YEAR = 25,               //4--
  VARCHAR = 26,            //7--
  NVARCHAR = 27,           //8--
  DATETIME = 28,           //8--
  TINYTEXT = 29,           //8--
  LONGTEXT = 30,           //8--
  VARBINARY = 31,          //9--
  TIMESTAMP = 32,          //9--
  MEDIUMTEXT = 33,         //10--
};

/**
 * @brief Create a Teable object
 * 
 * @param table Reference of the pointer to the Table object.
 * @param col_num Number of columns.
 * @param row_num Number of rows.
 * @return int Error: 0 == OK
 */
int  CreateTable(Table **table, int col_num, int reg_max_size);

/**
 * @brief Insert col_names in the Table
 * 
 * @param table Pointer to the Table where is gonna be inserted the column names
 * @param col_names Pointer to the column names.
 */
void InsertColNames(Table *table, char **col_names);

/**
 * @brief Insert the types of the columns
 * 
 * @param table Pointer to the Table where is gonna be inserted the column types
 * @param col_types Pointer with the code of the type
 */
void InsertColTypes(Table *table, int *col_types);

/**
 * @brief Insert a data row into the Table in the current position.
 * 
 * The current position can be modified with NextRow();
 *  
 * @param table Table is where is gonna be inserted row_data
 * @param index Index of the row on row_data is gonna be inseted
 * @param row_data Row data is gonna be inseted in the Table
 * @return int Errot: 0 == OK
 */
int InsertRow(Table *table, char**row_data);

/**
 * @brief This function run the Table and execute the callback.
 * 
 * The callback will recibe from the arguments the user_data into void *,
 * the column number into int, the row of values into the first char **,
 * and the col name int the second char **.
 * 
 * @param table The Table we whant to run.
 * @param callback The function is gonna be executed with the data.
 * @param user_data The user data you can pass to the callback
 * @return int Error: 0 == OK
 */
int  RunTable(Table *table, int (*callback)(Table *, void *, int, char **, char **), void *user_data);

/**
 * @brief This function run the Table and execute the callback.
 * 
 * The callback will recibe from the arguments the user_data into void *,
 * the column number into int, the row of values into the first char **,
 * and the col name int the second char **.
 * 
 * @param table The Table we whant to run.
 * @param callback The function is gonna be executed with the data.
 * @param user_data The user data you can pass to the callback
 * @param from The index of the row where the callback starts 
 * @param to The index of the row where the callback finish
 * @return int Error: 0 == OK
 */
int  RunTable(Table *table, int (*callback)(Table *, void *, int, char **, char **), void *user_data, unsigned int from, unsigned int to);


/**
 * @brief Get the columns names of the Table
 * 
 * @param table The Table we gonna be get the columns names
 * @return char** Pointer to the name of the columns
 */
char** GetColumnsNames(Table *table);

/**
 * @brief Get the Columns Type from the Table
 * 
 * @param table The Table we gonna be get the columns types
 * @return int* 
 */
int* GetColumnsType(Table *table);

/**
 * @brief Get the columns number of the Table
 * 
 * @param table Pointer to the Table we gonna have the columns number
 * @return int Number of the columns
 */
int GetColumnsNumber(Table *table);

/**
 * @brief Get the rows rumber of the Table
 * 
 * @return int Number of the rows
 */
int GetRowsNumber(Table *table);

/**
 * @brief This function free memory from the table and set the table to nullptr.
 * 
 * @param table Pointer to the Tabel is gonna be deleted.
 */
void DestroyTable(Table *table);

/**
 * @brief Set the Table Name object
 * 
 * @param table 
 * @param name 
 */
void SetTableName(Table *table, char *name);

/**
 * @brief Get the Table Name object
 * 
 * @param table 
 * @return char* 
 */
char* GetTableName(Table *table);

#endif