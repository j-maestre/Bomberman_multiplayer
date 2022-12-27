#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <esat_extra/imgui.h>
#include "data_base_controller.h"
#include "table.h"
#include "nfd.h"



void HelpMarker(const char* desc){
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

int CallbackGetTablesName(void *names,int num_colums, char **data, char **colum_name){
  DataBaseController& dc_controller = DataBaseController::Instance();
  char **tables_name = (char **) names;
  for(int i=0; i < num_colums; i++){
    strcpy(tables_name[dc_controller.actual_pos_], data[i]);
    dc_controller.actual_pos_++;
  }
  return 0;
}

DataBaseController::DataBaseController(){
  db_opened_ = false;
  table_selected_ = false;
  table_created_ = false;
  actual_table_ = nullptr;
  cols_name_inserted_ = false;
  error_message_ = nullptr;
  query_table_ = nullptr;
  pagination = 20;
  edit_popup_open_ = false;
  max_colums = 30;
  row_data_copy_ = (char**) malloc(sizeof(char*)*max_colums);
  col_offset = 0;
  actual_pos_ = 0;
  edit_row_err_ = nullptr;
  for(unsigned int i = 0; i < max_colums; i++){
    // GUSTAVO: The calloc() call is wrong! The first argument
    // should be an integer. It is allocating zero bytes!
    row_data_copy_[i] = (char*) calloc(120,sizeof(char));
  }

  memset(query_,'\0',501);
  memset(query_aux_,'\0',501);
  //CallbackGetTablesName(&actual_pos_ref_,0,nullptr,nullptr);
}

DataBaseController::~DataBaseController(){
  sqlite3_close(db_);
  for (int i = 0; i < num_tables_; i++){
    free(tables_name_[i]);
  }
  free(tables_name_);
  DestroyTable(actual_table_);
  DestroyTable(query_table_);
  free(err_msg_);
}

DataBaseController& DataBaseController::Instance(){
  static DataBaseController db_controller;
  return db_controller;
};

int GetNumTables(void *notused,int num_colums, char **data, char **colum_name){
  int *num_tables = (int *) notused;
  *num_tables = atoi(data[0]);
  return 0;
}

int CallbackGetTotalColumns(void *_cols,int num_columns, char **data, char **colum_name){
  int *cols = (int*) _cols;
  *cols = num_columns;  

  return 0;
}

int CallbackInsertRows(void *table_,int num_columns, char **data, char **colum_name){
  Table **table = (Table**) table_;
  InsertRow(*table,data);
  InsertColNames(*table,colum_name);
  return 0;
}

int GetDataType(char *type){

  char type_tmp[17];
  snprintf(type_tmp,17,"%s",type);
  if(strcmp(type_tmp,"DOUBLE PRECISION") == 0){
    return DOUBLE_PRECISION;
  }
  //*******
  snprintf(type_tmp,11,"%s",type);
  if(strcmp(type_tmp,"MEDIUMBLOB") == 0){
    return MEDIUMBLOB;
  }
  if(strcmp(type_tmp,"MEDIUMTEXT") == 0){
    return MEDIUMTEXT;
  }

  //*******
  snprintf(type_tmp,10,"%s",type);
  if(strcmp(type_tmp,"TIMESTAMP") == 0){
    return TIMESTAMP;
  }
  if(strcmp(type_tmp,"VARBINARY") == 0){
    return VARBINARY;
  }
  if(strcmp(type_tmp,"MEDIUMINT") == 0){
    return MEDIUMINT;
  }

  //*******
  snprintf(type_tmp,9,"%s",type);
  if(strcmp(type_tmp,"LONGBLOB") == 0){
    return LONGBLOB;
  }
  if(strcmp(type_tmp,"SMALLINT") == 0){
    return SMALLINT;
  }
  if(strcmp(type_tmp,"LONGTEXT") == 0){
    return LONGTEXT;
  }
  if(strcmp(type_tmp,"TINYTEXT") == 0){
    return TINYTEXT;
  }
  if(strcmp(type_tmp,"DATETIME") == 0){
    return DATETIME;
  }
  if(strcmp(type_tmp,"TINYBLOB") == 0){
    return TINYBLOB;
  }
  if(strcmp(type_tmp,"NVARCHAR") == 0){
    return NVARCHAR;
  }
  //********
  snprintf(type_tmp,8,"%s",type);
  if(strcmp(type_tmp,"INTEGER") == 0){
    return INTEGER;
  }
  if(strcmp(type_tmp,"NUMERIC") == 0){
    return NUMERIC;
  }
  if(strcmp(type_tmp,"BOOLEAN") == 0){
    return BOOLEAN;
  }
  if(strcmp(type_tmp,"TINYINT") == 0){
    return TINYINT;
  }
  if(strcmp(type_tmp,"DECIMAL") == 0){
    return DECIMAL;
  }
  if(strcmp(type_tmp,"VARCHAR") == 0){
    return VARCHAR;
  }

  //*******
  snprintf(type_tmp,7,"%s",type);
  if(strcmp(type_tmp,"DOUBLE") == 0){
    return DOUBLE;
  }
  if(strcmp(type_tmp,"BINARY") == 0){
    return BINARY;
  }
  if(strcmp(type_tmp,"BIGINT") == 0){
    return BIGINT;
  }

  //*******
  snprintf(type_tmp,6,"%s",type);
  if(strcmp(type_tmp,"FLOAT") == 0){
    return FLOAT;
  }

  //******
  snprintf(type_tmp,5,"%s",type);
  if(strcmp(type_tmp,"YEAR") == 0){
    return YEAR;
  }
  if(strcmp(type_tmp,"TIME") == 0){
    return TIME;
  }
  if(strcmp(type_tmp,"BOOL") == 0){
    return BOOL;
  }
  if(strcmp(type_tmp,"BLOB") == 0){
    return BLOB;
  }
  if(strcmp(type_tmp,"TEXT") == 0){
    return TEXT;
  }
  if(strcmp(type_tmp,"DATE") == 0){
    return DATE;
  }
  if(strcmp(type_tmp,"CHAR") == 0){
    return CHAR;
  }

  //*******
  snprintf(type_tmp,4,"%s",type);
  if(strcmp(type_tmp,"DEC") == 0){
    return DEC;
  }
  if(strcmp(type_tmp,"INT") == 0){
    return INT;
  }
  if(strcmp(type_tmp,"BIT") == 0){
    return BIT;
  }

  return UNDEFINED;
}

int CallbackInsertColumnsDataType(void *types_,int num_columns, char **data, char **colum_name){
  //Entra una vez por cada columna
  DataBaseController& db_controller = DataBaseController::Instance();
  int *types = (int*) types_;

  for (int i = 0; i < num_columns; i++){
    int type;
    if(strcmp(colum_name[i],"type") == 0){
      type = GetDataType(data[i]);
      types[db_controller.col_offset] = type;
      db_controller.col_offset++;
    }

  }

  
  return 0;
}

bool DataBaseController::OpenDB(char *name){

  if(sqlite3_open_v2(name,&db_,SQLITE_OPEN_READONLY,NULL) == SQLITE_OK && !db_opened_){
    sqlite3_close(db_);
    sqlite3_open(name,&db_);

    // Get total tables
    sqlite3_exec(db_, "SELECT COUNT(name) FROM sqlite_master WHERE (type = 'table' AND name != 'sqlite_sequence' AND name != 'sqlite_stat1')",
                                              GetNumTables, &num_tables_, &err_msg_);
    tables_name_ = (char**) malloc(sizeof(char*) * (num_tables_));
    for(int i = 0; i < num_tables_; i++){
        tables_name_[i] = (char*) malloc(sizeof(char) * 50);
    }
    
    sqlite3_exec(db_,"SELECT name FROM sqlite_master WHERE (type = 'table' AND name != 'sqlite_sequence' AND name != 'sqlite_stat1')",
                                        CallbackGetTablesName, tables_name_,&err_msg_);

    db_opened_ = true;
    table_selected_ = false;
    return true;
  }

  return false;
}

void DataBaseController::MainWindow(){
  static bool open_popup = false;
  static bool open_error_popup = false;
  static bool open_file_explorer = false;

  ImGuiWindowFlags flags = ImGuiWindowFlags_::ImGuiWindowFlags_None;
  flags |= ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar;
  
  if(!ImGui::Begin("DB Viewer", NULL,flags)){

    
    ImGui::End();
    return;
  }

  // Menu db options
  if(ImGui::BeginMenuBar()){
    if(ImGui::BeginMenu("Options")){
      if(ImGui::MenuItem("Open","O")){
        open_popup = true;
        open_error_popup = false;
        open_file_explorer = true;
      }
      if(ImGui::MenuItem("Close")){
        //open_popup = true;
        //open_error_popup = false;
        //open_file_explorer = true;
        
        CloseDB();
      }

      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }


  // Pop up open data base
  if(open_popup){
    open_popup = false;
    open_file_explorer = false;
    ImGui::OpenPopup("Select DB");

    //Open file explorer
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog( NULL, NULL, &outPath );
    if ( result == NFD_OKAY ) {
        puts(outPath);
        strcpy(db_name_,outPath);
        free(outPath);
    }
    else if ( result == NFD_CANCEL ) {
        puts("User pressed cancel.");
    }
    else {
        printf("Error: %s\n", NFD_GetError() );
    }
    

  }
  ImVec2 center;
  center.x = ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x / 2);
  center.y = ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y / 2);
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if(ImGui::BeginPopupModal("Select DB", NULL, ImGuiWindowFlags_AlwaysAutoResize)){
    ImGui::Text("Open database\nWrite the path from the .exe file to hte database.\nMake sure write the extension of the file.");
    ImGui::Separator();
    ImGui::PushItemWidth(500);
    if(ImGui::InputText("##db_name",db_name_,500, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::Button("Acept")){

      // Controll errors
      if(strlen(db_name_) > 0){
        if(OpenDB(db_name_)){
          memset(db_name_,'\0',50);
          ImGui::CloseCurrentPopup();
        }else{
          // La bd ya está abierta
          open_error_popup = true;
        }
      }else{
        // El input esta vacio
        open_error_popup = true;
      }
      
    }
    ImGui::SameLine();
    if(ImGui::Button("Close")){
        ImGui::CloseCurrentPopup();
    }

    // Error messages
    if(open_error_popup){
      if (db_opened_) {
        ImGui::TextColored({255, 0, 0, 255}, "Can't open the database. \nDatabase already open. Close the database to open another.");
      } else {
        ImGui::TextColored({255, 0, 0, 255}, "Can't open the database. \nMake sure the path from the .exe file to the database is correct.");
      }
    }
    ImGui::EndPopup();
  }
  // End open data base


  TablesNameWindow();
  ShowTable();
  PreviewWindow();
  QueryWindow();

  ImGui::End();
}

void DataBaseController::QueryWindow(){
  ImVec2 vec = {0.0f,0.0f}; // 0 es para que ocupe el 100% del contenedor
  
  if(!ImGui::BeginChild("Query",vec,true)){
    
    ImGui::EndChild();
    return;
  }
  //ImGui::Text("SQL Sentence");
  ImGuiInputTextFlags flags = ImGuiInputTextFlags_::ImGuiInputTextFlags_None;
  //flags |= ImGuiInputTextFlags_EnterReturnsTrue;
  flags |= ImGuiInputTextFlags_AllowTabInput;


  char query_aux[1024];

  

  ImGui::InputTextMultiline("##sentece",query_,500,ImVec2(ImGui::GetWindowSize().x,0.0f),flags);
  if(ImGui::Button("Execute") && db_opened_){
    // execute user's query
    DestroyTable(query_table_);
    query_table_ = nullptr;
    strcpy(query_aux,query_);
    strcat(query_aux," LIMIT 1\n");

    int *numcols_tmp = new int;
    sqlite3_exec(db_,query_,CallbackGetTotalColumns,numcols_tmp,&error_message_);

    //Check is is a SELECT query
    char *check_select = (char*) malloc(sizeof(char)*7);
    snprintf(check_select,7,"%s",query_);
    _strupr(check_select);

    if((strcmp(check_select,"SELECT") == 0 || strcmp(check_select,"PRAGMA") == 0) && !error_message_){
      CreateTable(&query_table_, *numcols_tmp,120);
      sqlite3_exec(db_,query_,CallbackInsertRows,&query_table_,&error_message_);
    }


    strncpy(query_aux_,query_,1024);
    //memset(query_,'\0',501);
    table_created_ = false;
    delete numcols_tmp;

    for(int i = 0; i < num_tables_; i++){
      free(tables_name_[i]);
    }
    free(tables_name_);
    num_tables_ = 0;
    sqlite3_exec(db_, "SELECT COUNT(name) FROM sqlite_master WHERE (type = 'table' AND name != 'sqlite_sequence' AND name != 'sqlite_stat1')",
                                              GetNumTables, &num_tables_, &err_msg_);
    tables_name_ = (char**) malloc(sizeof(char*) * (num_tables_));
    for(int i = 0; i < num_tables_; i++){
        tables_name_[i] = (char*) malloc(sizeof(char) * 50);
    }
    actual_pos_ = 0;
    sqlite3_exec(db_,"SELECT name FROM sqlite_master WHERE (type = 'table' AND name != 'sqlite_sequence' AND name != 'sqlite_stat1')",
                                        CallbackGetTablesName, tables_name_,&err_msg_);
  }
  ImGui::EndChild();
}

void DeleteRow(char *table_name,char *colum_name, char *id, sqlite3* db){
  char query[100] = {"DELETE FROM "};
  strcat(query,table_name);
  strcat(query," WHERE ");
  strcat(query,colum_name);
  strcat(query," = ");
  strcat(query,id);
  //printf("Query-> %s\n",query);
  sqlite3_exec(db,query,nullptr,nullptr,nullptr);

}

int CallbackPreviewTable(Table *table,void *data_base, int num_colums, char **data, char **col_name){
  
  DataBaseController& db_controller = DataBaseController::Instance();
  
  char table_name[30];
  strcpy(table_name,GetTableName(table)?GetTableName(table):"NULL");
  sqlite3* db = (sqlite3*) data_base;
  
  ImGui::TableNextRow();  
  for(int i = 0; i < num_colums; i++){
    ImGui::TableSetColumnIndex(i);
    char tmp[120];
    snprintf(tmp,120,"##%d",rand()%200);
    strcat(tmp,col_name[i]);
    strcat(tmp,data[i]);
    //if(ImGui::InputText(tmp,data[i],70,ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue)){
    ImGui::Text(data[i]);
      // Update de la row
      // Mostrar pop up-> ¿Desea actualizar la tabla? -> Tabla actualizada, Error actuaizando
    //} 
  }

    //Pintar basura
    
    if(db_controller.actual_table_ == table){

      // Columna Delete & Edit

      ImGui::TableSetColumnIndex(num_colums);
      ImGui::Button("Delete");
      if(ImGui::IsItemClicked()){
        //IsMouseDoubleClicked
        //Delete row
        DeleteRow(table_name,col_name[0],data[0],db);
        db_controller.table_created_ = false;
      }
  

      ImGui::SameLine();
      ImGui::Button("Edit");
      if(ImGui::IsItemClicked()){
        db_controller.edit_popup_open_ = true;
        db_controller.row_data_ = data;
        

        //Copy of data
        int cols_num = GetColumnsNumber(db_controller.actual_table_);
        for(int i = 0; i < cols_num; i++){
          snprintf(db_controller.row_data_copy_[i],120,"%s",data[i]);
        }

      }
    }

  return 0;
} 

int GetTypeToInput(int type){
  if(type <0){
    return -1; // Its undefined
  }else if(type >=1 && type <= 15){
    return 1;  // Its a integer
  }else if(type >=16 && type <=20 ){
    return 2;  // Its a float
  }else{
    return 3; // Its a text
  }
}

void DataBaseController::PreviewWindow(){

  ImGuiTableFlags table_flags = ImGuiTableFlags_::ImGuiTableFlags_None;
  table_flags |= ImGuiTableFlags_::ImGuiTableFlags_RowBg;
  table_flags |= ImGuiTableFlags_::ImGuiTableFlags_Borders;
  table_flags |= ImGuiTableFlags_::ImGuiTableFlags_BordersH;
  table_flags |= ImGuiTableFlags_::ImGuiTableFlags_BordersOuterH;
  table_flags |= ImGuiTableFlags_::ImGuiTableFlags_BordersInnerH;
  table_flags |= ImGuiTableFlags_::ImGuiTableFlags_BordersV;
  table_flags |= ImGuiTableFlags_::ImGuiTableFlags_BordersOuterV;
  table_flags |= ImGuiTableFlags_::ImGuiTableFlags_BordersInnerV;
  table_flags |= ImGuiTableFlags_::ImGuiTableFlags_BordersOuter;
  table_flags |= ImGuiTableFlags_::ImGuiTableFlags_BordersInner;
  table_flags |= ImGuiTableFlags_::ImGuiTableFlags_Resizable;
  table_flags |= ImGuiTableFlags_::ImGuiTableFlags_ScrollX;
  table_flags |= ImGuiTableFlags_::ImGuiTableFlags_ScrollY;
  
  ImVec2 size = {0,0};
  size.x = 0;
  size.y = ImGui::GetWindowSize().y * 2.0f/3.0f;
  ImGui::SameLine();
  if(!ImGui::BeginChild("Table content",size,true)){
    ImGui::EndChild();
    return;
  }
  if (ImGui::BeginTabBar("ViewerTabs")) {
    if (ImGui::BeginTabItem("Data")) {
      if (table_selected_) {
        if(table_created_){
          // Callback to print data table
          int num_columns = GetColumnsNumber(actual_table_);
          char **col_names = GetColumnsNames(actual_table_);


          if(ImGui::BeginTable("##Table_content",num_columns+1,table_flags,ImVec2((1.175494351e-38F),0))){

            for(int i = 0; i<num_columns; i++){
              ImGui::TableSetupColumn(col_names[i]);
            }
            ImGui::TableSetupColumn("");
            ImGui::TableHeadersRow();
            RunTable(actual_table_,CallbackPreviewTable, db_);

            ImGui::EndTable();
          }
        }
      }
      static bool error_on_edit = false;
      // Edit Row popup
      if (edit_popup_open_ == true) {
        ImGui::OpenPopup("Edit Row");
        error_on_edit = false;
        edit_popup_open_ = false;
      }
      ImVec2 center;
      center.x = ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x / 2);
      center.y = ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y / 2);
      ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

      //Pop up data
      if(ImGui::BeginPopupModal("Edit Row", NULL, ImGuiWindowFlags_AlwaysAutoResize)){
        ImGui::Text("%-15s", "TableName: ");
        ImGui::SameLine();
        ImGui::Text(GetTableName(actual_table_));
        ImGui::Separator();
        int num_columns = GetColumnsNumber(actual_table_);
        char **colum_names = GetColumnsNames(actual_table_);
  
        // Set data in columns
        int *types = GetColumnsType(actual_table_);
        static int type_tmp = 0;
        static float type_f_tmp = 0;

        for (int i = 0; i < num_columns; i++){
          char label[256] = {"##Rowdata"};
          // GUSTAVO: Buffer overflow! Don't hardcode buffer sizes.
          // The buffer is 40 bytes long, but the snprintf call maxes at 120.
          // Please use something like sizeof(label) .
          snprintf(label,sizeof(label),"##%s",colum_names[i]);
          int type = GetTypeToInput(types[i]);
          ImGui::Text("%-15s \t", colum_names[i]);
          ImGui::SameLine();
          //Comprobar el tipo de datos
          if(type == 1){
            //Integer
            type_tmp = atoi(row_data_copy_[i]);
            ImGui::InputInt(label,&type_tmp);
            snprintf(row_data_copy_[i],120,"%d",type_tmp);

          }else if(type == 2){
            //Float
            type_f_tmp = std::stof(row_data_copy_[i]);
            ImGui::InputFloat(label,&type_f_tmp);
            snprintf(row_data_copy_[i],120,"%f",type_f_tmp);

          }else if(type == 3){
            // Texto
            ImGui::InputText(label,row_data_copy_[i],120);              
          }else{
            //Undefined

          }

        }
        ImGui::Separator();
        if(ImGui::Button("Save")){
          // Update en la DB
          char query[2048];
          snprintf(query,120,"%s%s%s","UPDATE ",GetTableName(actual_table_), " SET ");
          for(int i = 0; i<num_columns;i++){
            strncat(query,colum_names[i], 2048);
            strncat(query," = ", 2048);
            strncat(query,"'", 2048);
            strncat(query,row_data_copy_[i], 2048);
            strncat(query,"'", 2048);
            if(i<num_columns-1)strncat(query,", ", 2048);
          }

          // Condicion where del update
          strncat(query," WHERE ", 2048);
          for (int i = 0; i < num_columns; i++){
            strncat(query,colum_names[i], 2048);
            strncat(query," = ", 2048);
            strncat(query,"'", 2048);
            strncat(query,row_data_[i], 2048);
            strncat(query,"'", 2048);
            if(i<num_columns-1)strncat(query, " AND ", 2048);
          }

          //printf("%s\n",query);
          sqlite3_exec(db_,query,nullptr,nullptr,&edit_row_err_);
          if (edit_row_err_) {
            error_on_edit = true;
          } else {
            ImGui::CloseCurrentPopup();
            table_created_ = false;
            error_on_edit = false;
          }
        }
        ImGui::SameLine();
        if(ImGui::Button("Close")){
          error_on_edit = false;
          ImGui::CloseCurrentPopup();
        }
        if (error_on_edit) {
          ImGui::TextColored(ImVec4(1, 0, 0, 1), "[ERROR]");
          ImGui::SameLine();
          ImGui::Text(edit_row_err_);
        }

        ImGui::EndPopup();
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("SQL")) {
      //Preview of SQL Query window
      if(error_message_){
        ImGui::TextColored(ImVec4(255,0,0,255),"[ERROR]");
        ImGui::SameLine();
        ImGui::Text(error_message_);
      } else {
        if(strlen(query_aux_)>0){
          ImGui::TextColored(ImVec4(0,255,0,255),"[OK]");
          ImGui::SameLine();
          ImGui::Text(query_aux_);
        }else{
          ImGui::TextColored(ImVec4(238/255.0f,210/255.0f,2/255.0f,255),"[EMPTY]");
        }
      }
      if(query_table_ != nullptr){

        ImGui::Separator();
        //Show query table
        int num_query_columns = GetColumnsNumber(query_table_);

        if(ImGui::BeginTable("##QueryContent", num_query_columns, table_flags,ImVec2((1.175494351e-38F),0))){
          char **col_names = GetColumnsNames(query_table_);
          for (int i = 0; i < num_query_columns; i++){
            ImGui::TableSetupColumn(col_names[i]);
          }
          ImGui::TableHeadersRow();
          RunTable(query_table_,CallbackPreviewTable, db_);
          ImGui::EndTable();
        }
      }
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
  ImGui::EndChild();


}

void DataBaseController::ShowTable(){

  if(table_selected_ && !table_created_){
    // Reset de la tabla pa meter la nueva
    if(actual_table_ != nullptr)DestroyTable(actual_table_);
    char *err_msg;

    //Create table with num columns
    char query_rows_columns[50] = {"SELECT * FROM "};
    strcat(query_rows_columns,current_table_);
    strcat(query_rows_columns," LIMIT 1");

    int *num_cols_aux = new int;
    sqlite3_exec(db_,query_rows_columns,CallbackGetTotalColumns,num_cols_aux,&err_msg);
    CreateTable(&actual_table_, *num_cols_aux,120);
    SetTableName(actual_table_,current_table_);
    
    delete num_cols_aux;

    // Insert row data
    char query_data[50] = "SELECT * FROM ";
    strcat(query_data,current_table_);
    sqlite3_exec(db_,query_data,CallbackInsertRows,&actual_table_, &err_msg);

    // Insert col data type
    char query_data_columns[50];
    int colum_types[40];
    snprintf(query_data_columns,50,"%s%s%s","PRAGMA table_info('",current_table_,"')");
    col_offset = 0;
    sqlite3_exec(db_,query_data_columns,CallbackInsertColumnsDataType,colum_types, &err_msg);
    InsertColTypes(actual_table_,colum_types);

    table_created_ = true;

  }
}

void DataBaseController::TablesNameWindow(){
  ImVec2 size = {0,0};
  size.x = ImGui::GetWindowSize().x * 1.0f/3.0f;
  size.y = ImGui::GetWindowSize().y * 2.0f/3.0f;
  if(size.x > 215.0f)size.x = 215.0f;
  if(!ImGui::BeginChild("Tables",size,true)){
    ImGui::EndChild();
    return;
  }
  ImGui::Text("TABLAS");

  for(int i = 0; i < num_tables_; i++){
    if(ImGui::SmallButton(tables_name_[i])){
      table_selected_ = true;
      table_created_ = false;
      strcpy(current_table_,tables_name_[i]);
    }
    ImGui::Separator();
  }

  ImGui::EndChild();
}

void DataBaseController::CloseDB(){

  DestroyTable(actual_table_);
  DestroyTable(query_table_);
  actual_table_ = nullptr;
  query_table_ = nullptr;
  free(tables_name_);
  num_tables_ = 0;
  db_opened_ = false;
  table_selected_ = false;
  table_created_ = false;
  cols_name_inserted_ = false;
  sqlite3_close(db_);
  actual_pos_ = 0;
}
