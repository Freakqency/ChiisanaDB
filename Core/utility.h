#include "declarations.h"

typedef struct{
	char *buffer;
	size_t buffer_len;
	ssize_t input_len;
} InputBuffer;

typedef enum{
	META_COMMAND_SUCCESS,
	META_COMMAND_FAILURE
} MetaCommandResult;

typedef enum{
	PREPARE_SUCCESS,
	PREPARE_FAILURE
} PrepareResult;

typedef enum{
	STATEMENT_INSERT,
	STATEMENT_SELECT
} StatementType;

typedef struct{
	StatementType type;
} Statement;

//Our front end
InputBuffer *new_input_buffer(){
	InputBuffer *input_buffer = malloc(sizeof(InputBuffer));
	input_buffer->buffer = NULL;
	input_buffer->buffer_len = 0;
	input_buffer->input_len = 0;
	return input_buffer;
}

void print_prompt(){
	printf("db > ");
}

void read_input(InputBuffer *input_buffer){
	ssize_t byte_read = getline(&(input_buffer->buffer),&(input_buffer->buffer_len),stdin);
	
	if (byte_read <=0){
		printf("Error in Input");
		exit(EXIT_FAILURE);
	}
	
	input_buffer->input_len = byte_read -1;
	input_buffer->buffer[byte_read-1] = 0;
}

void close_buffer(InputBuffer *input_buffer){
	free(input_buffer->buffer);
	free(input_buffer->buffer);
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer){
	if (strcmp(input_buffer->buffer, ".exit") == 0){
		close_buffer(input_buffer);
		exit(EXIT_SUCCESS);
	}
	else{
		return META_COMMAND_FAILURE;
	}
}

//Our SQL Compiler
PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement){
	if (strncmp(input_buffer->buffer, "insert", 6 ) == 0){
		statement->type = STATEMENT_INSERT;
		return PREPARE_SUCCESS;
	}
	if (strcmp(input_buffer->buffer, "select") == 0){
		statement->type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	}
	
	return PREPARE_FAILURE;
}

//Our SQL Virtual Machine
void execute_statement(Statement *statement){
	switch(statement->type){
		case(STATEMENT_INSERT):
			printf("Insert statement called \n");
			break;
		case(STATEMENT_SELECT):
			printf("Select statement called \n");
			break;
	}
}

