#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 155
#define size_of_attributes(Struct,Attribute) sizeof(((Struct*)0)->Attribute)


typedef struct{
	uint32_t id;
	char username[COLUMN_USERNAME_SIZE];
	char email[COLUMN_EMAIL_SIZE];
}Row;

const uint32_t ID_SIZE = size_of_attributes(Row,id);
const uint32_t USERNAME_SIZE = size_of_attributes(Row,username);
const uint32_t EMAIL_SIZE = size_of_attributes(Row,email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;



typedef struct{
	char* buffer;
	size_t buffer_length;
	ssize_t input_length;
}InputBuffer;

typedef enum{
	STATEMENT_INSERT,STATEMENT_SELECT
}StatementType;

typedef struct{
	StatementType type;
	Row row_to_insert;
}Statement;


typedef enum {
	META_COMMAND_SUCCESS,
	META_COMMAND_UNRECONISED_COMMAND
}MetaCommandResult;

typedef enum{
	PREPARE_SUCCESS,PREPARE_UNRECONISED_STATEMENT
}PrepareResult;



InputBuffer* new_input_buffer(){
	InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
	input_buffer -> buffer = NULL;
	input_buffer -> buffer_length = 0;
	input_buffer->input_length =0;
	return input_buffer;
}

void print_promt(){
	printf(" db > ");
}


void read_input(InputBuffer* input_buffer){
	ssize_t bytes_read = getline(&(input_buffer->buffer),&(input_buffer->buffer_length),stdin);
	if(bytes_read<=0){
		printf("Error reading input");
		exit(EXIT_FAILURE);
	}
	input_buffer->input_length=bytes_read-1;
	input_buffer->buffer[bytes_read-1]=0; //replaces \n with 0 
	// printf("%s",input_buffer->buffer);
	// printf("%ld input length",input_buffer->input_length);
	// printf("%ld input buffer len",input_buffer->buffer_length);
}

void close_input_buffer(InputBuffer* input_buffer){
	free(input_buffer->buffer);
	free(input_buffer);

}

MetaCommandResult do_meta_command(InputBuffer* input_buffer){
	if(strcmp(input_buffer->buffer,".exit")==0){
		printf("%u Row size",ROW_SIZE);

		exit(EXIT_SUCCESS);
	}else{
		return META_COMMAND_UNRECONISED_COMMAND; 
	}
}

PrepareResult prepare_statement(InputBuffer* input_buffer,Statement* statement){
	if(strncmp(input_buffer->buffer,"insert",6)==0){
		// int args_assigned =sscanf(input_buffer->buffer,"insert %d %s %s")
		statement->type = STATEMENT_INSERT;
		return PREPARE_SUCCESS;
	}
	if(strcmp(input_buffer->buffer,"select")==0){
		statement->type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	}
	return PREPARE_UNRECONISED_STATEMENT;
}

void execute_statement(Statement* statement){
	switch(statement->type){
		case(STATEMENT_SELECT):
			printf("This is where you would select.\n");
			break;
		case(STATEMENT_INSERT):
			printf("This is where you would insert.\n");
			break;

	}
}

int main(){
	InputBuffer* input_buffer = new_input_buffer();
	while(true){
		print_promt();
		read_input(input_buffer);

		if(input_buffer->buffer[0] == '.'){
			switch(do_meta_command(input_buffer)){
				case (META_COMMAND_SUCCESS):
					continue;
				case (META_COMMAND_UNRECONISED_COMMAND):
					printf("Unrecognised command %p \n",input_buffer);
			}
		}

		Statement* statement;
		switch(prepare_statement(input_buffer,statement)){
			case (PREPARE_SUCCESS):
				break;
			case(PREPARE_UNRECONISED_STATEMENT):
				printf("Unreconised keyword at start  of '%s'  .\n",input_buffer->buffer);
				continue;
		}

		execute_statement(statement);
		printf("Executed. \n");

		// if(strcmp(input_buffer->buffer,".exit") ==0){
		// 	close_input_buffer(input_buffer);
		// 	exit(EXIT_FAILURE);
		// }else{
		// 	printf("unrecognised command '%s'.\n ",input_buffer->buffer);
		// }
	}

	return 0;
}
