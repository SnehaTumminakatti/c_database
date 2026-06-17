// #ifndef TABLE_STRUCTURE_H
// #define TABLE_STRUCTURE_H
// #include <stdint.h>
// #include <string.h>

// #include"command_handler.h"

// #define COLUMN_USERNAME_SIZE 32
// #define COLUMN_EMAIL_SIZE 255
// #define size_of_attribute(Struct,Attribute) sizeof(((Struct*)0)->Attribute)

// typedef enum{EXECUTE_SUCCESS,EXECUTE_TABLE_FULL} ExecuteResult;


// typedef struct {
//     uint32_t id;
//     char username[COLUMN_USERNAME_SIZE];
//     char email[COLUMN_EMAIL_SIZE];
// }Row;



// extern const uint32_t ID_SIZE  ;
// extern const uint32_t USERNAME_SIZE;
// extern const uint32_t EMAIL_SIZE ;
// extern const uint32_t ROW_SIZE;
// // OFFSET DEC

// extern const uint32_t ID_OFFSET ;
// extern const uint32_t USERNAME_OFFSET;
// extern const uint32_t EMAIL_OFFSET ;
// extern const uint32_t ROW_OFFSET ;

// //DATABASE STRUCTURE

// extern const uint32_t PAGE_SIZE;
// extern const uint32_t ROWS_PER_PAGE;
// extern const uint32_t TABLE_MAX_ROWS;

// Table* new_table();
// void free_table(Table* table);
// void serialize_row(Row* source,void* destination);
// void deserialize_row(void* source,Row* destination);
// void row_slot(uint32_t row_num,Table* table);
// ExecuteResult execute_insert(Statement* statement,Table* table);
// ExecuteResult execute_select(Statement* statement,Table* table);
// #endif