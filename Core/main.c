#include "declarations.h"

typedef struct{
	char *buffer;
	size_t buffer_len;
	ssize_t input_len;
} InputBuffer;

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
	
	input_buffer->input_lenght = byte_read -1;
	input_buffer->buffer[byte_read-1] = 0;
}

int main(int argc,char* argv[]){
	InputBuffer* input_buffer = new_input_buffer();
	while(True){
		print_prompt();
		read_input(input_buffer);
		if (strcmp(input_buffer->buffer,".exit") == 0){
			close_buffer(input_buffer);
			exit(EXIT_SUCCESS);
		}
		else{
			printf("Command not Recognized %s \n",input_buffer->buffer);
		}		
	}
}