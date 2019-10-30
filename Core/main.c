#include "utility.h"

int main(int argc,char* argv[]){
	InputBuffer* input_buffer = new_input_buffer();
	while(true){
		Table* table = new_table();
		print_prompt();
		read_input(input_buffer);
		if (input_buffer->buffer[0] == '.' ){
			switch(do_meta_command(input_buffer)){
				case(META_COMMAND_SUCCESS):
					continue;
				case(META_COMMAND_FAILURE):
					continue;
			}
		}
		
		Statement statement;
		switch (prepare_statement(input_buffer, &statement)){
			case(PREPARE_SUCCESS):
				break;
			case(PREPARE_SYNTAX_ERROR):
				printf("syntax erro. please try again \n");
				continue;
			case(PREPARE_FAILURE):
				printf("Command unrecognized %s \n", input_buffer->buffer);
				continue;
		}
		
		switch (execute_statement(&statement, table)) {
			case (EXECUTE_SUCCESS):
				printf("Query executed successfully \n");
				break;
			case (EXECUTE_TABLE_FULL):
				printf("Error : table full \n");
				break;
		}
	}
}
