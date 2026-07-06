#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H
#include <stdint.h>
#include <string.h>
#include"input.h"
#include <fcntl.h>
#include <errno.h>

#define TABLE_MAX_PAGES 100
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define size_of_attribute(Struct,Attribute) sizeof(((Struct*)0)->Attribute)


typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECONIZED_COMMAND
}MetaCommandResult;

typedef enum{
    PREPARE_SUCCESS,
    PREPARE_UNRECONIZED_STATEMENT,
    PREPARE_SYNTAX_ERROR,
    PREPARE_NEGATIVE_ID,
    PREPARE_STRING_TOO_LONG,
}PrepareResult;

// Statement command declarations

typedef enum{
    STATEMENT_INSERT,
    STATEMENT_SELECT
}StatementType;

typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE+1];
    char email[COLUMN_EMAIL_SIZE+1];
}Row;

typedef struct{
    StatementType type;
    Row row_to_insert;
}Statement;

typedef struct{
    int file_descriptor;
    uint32_t file_length;
    void* pages[TABLE_MAX_PAGES];
}Pager;

typedef struct{
    uint32_t num_rows;
    Pager* pager;
}Table;


typedef enum{EXECUTE_SUCCESS,EXECUTE_TABLE_FULL} ExecuteResult;



extern const uint32_t ID_SIZE ;
 extern const uint32_t USERNAME_SIZE;
 extern const uint32_t EMAIL_SIZE;
 extern const uint32_t ROW_SIZE ;


 extern const uint32_t ID_OFFSET;
 extern const uint32_t USERNAME_OFFSET;
 extern const uint32_t EMAIL_OFFSET;
 extern const uint32_t ROW_OFFSET;

 extern const uint32_t PAGE_SIZE ;
 extern const uint32_t ROWS_PER_PAGE ;
 extern const uint32_t TABLE_MAX_ROWS;



Table* new_table();
void free_table(Table* table);
void serialize_row(Row* source,void* destination);
void deserialize_row(void* source,Row* destination);
void* row_slot(uint32_t row_num,Table* table);
ExecuteResult execute_insert(Statement* statement,Table* table);
ExecuteResult execute_select(Statement* statement,Table* table);

// Meta Command declarations

MetaCommandResult do_meta_command(InputBuffer* inputBuffer,Table* table);

// Statement command declarations

ExecuteResult prepare_insert(InputBuffer* input_buffer,Statement* statement);
PrepareResult prepareStatement(InputBuffer* input_buffer,Statement* statement);
ExecuteResult execute_statement(Statement* statement,Table* table);
void pager_flush(Pager* pager,uint32_t page_num,uint32_t size);

Table* db_open(const char* filename);
Pager* pager_open(const char* filename);
void* get_page(Pager* pager ,uint32_t page_num);
void db_close(Table* table);
#endif