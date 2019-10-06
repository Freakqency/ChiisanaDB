#include "declarations.h"

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
#define uint32_t ID_SIZE = size_of_attribute(Row, id);
#define uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
#define uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
#define uint32_t ID_OFFSET = 0
#define uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
#define uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
#define uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const uint32_t PAGE_SIZE = 4096 //same as size of os page 
#define TABLE_MAXX_PAGES 100
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

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
	uint32 id;
	char username[COLUMN_USERNAME_SIZE];
	char email[COLUMN_EMAIL_SIZE];
} Row;

typedef struct{
	StatementType type;
	Row row_to_insert;
} Statement;

typedef struct{
	uint32_t num_rows;
	void *pages[TABLE_MAX_PAGES];
} Table;

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

void serialize_row(Row *source, void *destination){
	memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE );
	memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
	memcpy(destination +EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize(void *source, Row *destination){
	memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
	memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
	memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

void *row_slot(Table* table, uint32_t row_num){
	uint32_t page_num = row_num / ROWS_PER_PAGE;
	void *page = table->pages[page_num];
	if (page == NULL){
		page = table->pages[page_num] = malloc(PAGE_SIZE);
	}
	uint32_t row_offset = row_num % ROWS_PER_PAGE;
	uint32_t byte_offset = row_offset * ROW_SIZE;

	return page + byte_offset;
}



//Our SQL Compiler
PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement){
	if (strncmp(input_buffer->buffer, "insert", 6 ) == 0){
		statement->type = STATEMENT_INSERT;
		int args_assigned = sscanf(input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id),
		&(statement->row_to_insert.username), &(statement->row_to_insert.email));
			
		if (args_assigned < 3){
			return PREPARE_FAILURE;
		}
		
		return PREPARE_SUCCESS;
	}
	if (strcmp(input_buffer->buffer, "select") == 0){
		statement->type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	}
	
	return PREPARE_FAILURE;
}

//Our SQL Virtual Machine
ExecuteResult execute_statement(Statement *statement, Table *table){
	switch(statement->type){
		case(STATEMENT_INSERT):
			return execute_insert(statement, table);
		case(STATEMENT_SELECT):
			return execute_select(statement, table);
	}
}
