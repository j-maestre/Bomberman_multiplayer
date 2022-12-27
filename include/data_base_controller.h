/**
 * @file data_base_controller.h
// GUSTAVO: Do NOT state two authors on the same file. The should only be one
// responsible author for each file.
 * @author Xema Maestre <maestrequi@esat-alumni.com>
 * @brief 
 * 
 */


#ifndef __DATA_BASE_CONTROLLER_H__ 
#define __DATA_BASE_CONTROLLER_H__ 1 

#include <esat_extra/sqlite3.h>
#include "table.h"

class DataBaseController{
 public:
  // Public functions
  /**
   * @brief Create ImGui Row in table for every row in the table selected
   * 
   * @param table Table selected
   * @param data_base sqlite3 database instance
   * @param num_colums Number of columns of the table
   * @param data Data of every column
   * @param col_name Name of the colum
   * @return int 
   */
  friend int CallbackPreviewTable(Table *table,void *data_base, int num_colums, char **data, char **col_name);

  /**
   * @brief Callback to set the type of the data of every column in the tabe selected
   * 
   * @param types_ Pointer to save the types
   * @param num_columns Number of columns
   * @param data Type of the data
   * @param colum_name Name of the column
   * @return int 
   */
  friend int CallbackInsertColumnsDataType(void *types_,int num_columns, char **data, char **colum_name);

  /**
   * @brief Callabck to save the names of all tables in database
   * 
   * @param names Double pointer to save the names 
   * @param num_colums Number of columns
   * @param data Data who contains the names of the columns
   * @param colum_name Name of colum
   * @return int 
   */
  friend int CallbackGetTablesName(void *names,int num_colums, char **data, char **colum_name);

  /**
   * @brief Static instance of the class 
   * 
   * @return DataBaseController& 
   */
  static DataBaseController& Instance();

  // Constructor de copia
  /**
   * @brief Construct a new Data Base Controller object
   * 
   */
  DataBaseController(const DataBaseController&) = delete; // Palabra clave, para decirle que no exista ese constructor. Tambien se puede poner default en vez de delete para que lo haga el compilador por defecto

  /**
   * @brief Destroy the Data Base Controller object
   * 
   */
  ~DataBaseController(); // Destructor

  /**
   * @brief 
   * 
   */
  void operator = (const DataBaseController&) = delete; // Sobre carga de operador para la clase

    /**
   * @brief Open main window with all subpages
   * 
   */
  void MainWindow();

  /**
   * @brief This function opens the database, save the number of tables in database and the names
   * 
   * @param name Name of database file
   * @return true if can open the db
   * @return false if can't open the db
   */
  bool OpenDB(char *name);
  
  /**
   * @brief Close the database and resets all the variables used
   * 
   */
  void CloseDB();

 private:
  // Private functions
  /**
   * @brief Number of tables on DataBase
   */
  int num_tables_;
  /**
   * @brief  Names of tables on DataBase
   */
  char **tables_name_; 

  /**
   * @brief Error message returned of sqlite3_exec
   */
  char *err_msg_;

  /**
   * @brief Offset to save tables names
   */
  int actual_pos_; 
  
  /**
   * @brief Bool to save if db is opened or not
   */
  bool db_opened_;

  /**
   * @brief Name of DataBase
   */
  char db_name_[500];

  /**
   * @brief Name of table selected to view
   */
  char current_table_[20];

  /**
   * @brief Bool to say if a table is selected
   */
  bool table_selected_;

  /**
   * @brief Table var who contains all the data of the table selected
   */
  Table *actual_table_; // 20 bytes

  /**
   * @brief Table var who contains all the data of the table returned by user's query
   */
  Table *query_table_;
  
  /**
   * @brief Bool to say if Table *actual_table_ is correctly created
   */
  bool table_created_;

  /**
   * @brief Bool to say if names of columns are correctly inserted on Table *actual_table_
   */
  bool cols_name_inserted_;

  /**
   * @brief Error message returned by user's query on sqlite3_exec
   * 
   */
  char *error_message_;

  /**
   * @brief Bool to say if the edit pop up is opened or not
   */
  bool edit_popup_open_;

  /**
   * @brief Offset of pagination
   */
  unsigned int pagination;

  /**
   * @brief Data of table's row
   */
  char **row_data_;

  /**
   * @brief Copy of Data table's row for edit
   */
  char **row_data_copy_;

  /**
   * @brief The maxium columns permited
   */
  unsigned int max_colums;

  /**
   * @brief Offset for insert type of data of a table
   */
  unsigned int col_offset;

  /**
   * @brief Error of the edit row query.
   * 
   */
  char *edit_row_err_;

  /**
   * @brief Array to save the user's query
   */
  char query_[1024];

  /**
   * @brief Copy of user's query
   */
  char query_aux_[1024];

  /**
   * @brief Sqlite3 var who contains the DataBase
   */
  sqlite3* db_;

  /**
   * @brief View of the user's query and execute it
   * 
   */
  void QueryWindow();
  

  /**
   * @brief Construct a new Data Base Controller object
   * 
   */
  DataBaseController();

  /**
   * @brief When select a table, this function get the number of columns, the name of the columns, the type of data columns and set the necesaries values to show it 
   * 
   */
  void ShowTable();


  /**
   * @brief Select table window
   * 
   */
  void TablesNameWindow();
  /**
   * @brief Shows content of table or the result of user's query
   * 
   */
  void PreviewWindow();

};


#endif
