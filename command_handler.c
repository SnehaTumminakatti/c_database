#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include"command_handler.h"
MetaCommandResult do_meta_command(InputBuffer* inputBuffer,Table* table){
    if(strcmp(inputBuffer->buffer,".exit")==0){
       close_input_buffer(inputBuffer);
       free_table(table);
       exit(EXIT_SUCCESS);
    }else {
        return META_COMMAND_UNRECONIZED_COMMAND;
    }
}


ExecuteResult prepare_insert(InputBuffer* input_buffer,Statement* statement){
    statement->type=STATEMENT_INSERT;
    char*  keyword = strtok(input_buffer->buffer," ");
    char* id_string = strtok(NULL," ");
    char* username = strtok(NULL," ");
    char* email = strtok(NULL," ");
    if(id_string==NULL || username == NULL || email==NULL){
        return PREPARE_SYNTAX_ERROR;
    }

    int id = atoi(id_string);
    if(id<0){
        return PREPARE_NEGATIVE_ID;
    }
    if(strlen(username)>COLUMN_USERNAME_SIZE){
        return PREPARE_STRING_TOO_LONG;
    }
    if(strlen(email)>COLUMN_EMAIL_SIZE){
        return  PREPARE_STRING_TOO_LONG;
    }
    statement->row_to_insert.id=id;
    strncpy(statement->row_to_insert.username,username,COLUMN_USERNAME_SIZE-1);
    strncpy(statement->row_to_insert.email,email,COLUMN_EMAIL_SIZE-1);
    statement->row_to_insert.username[COLUMN_USERNAME_SIZE-1]='\0';
    statement->row_to_insert.email[COLUMN_EMAIL_SIZE-1]='\0';

        return PREPARE_SUCCESS;

}

PrepareResult prepareStatement(InputBuffer* input_buffer,Statement* statement){
    if(strncmp(input_buffer->buffer,"insert",6)==0){
        // statement->type = STATEMENT_INSERT;

        // int args_assigned = sscanf(input_buffer->buffer,"insert %d %s %s",&(statement->row_to_insert.id),&(statement->row_to_insert.username),&(statement->row_to_insert.email));
        // if(args_assigned<3){
        //     return PREPARE_SYNTAX_ERROR;
        // }
        return prepare_insert( input_buffer,statement);

    }else if(strcmp(input_buffer->buffer,"select")==0){
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;

    }
    return PREPARE_UNRECONIZED_STATEMENT;
}

ExecuteResult execute_statement(Statement* statement,Table* table){
    switch(statement->type){
        case STATEMENT_INSERT:
        // printf("This is where u would insert");
            return execute_insert(statement, table);
        case STATEMENT_SELECT:
        return execute_select(statement, table);

    }
}