#include "declarations.h"
#include "utility.h"

ExecuteResult execute_select(Statement *statement, Table *table){
	Row row;
	
	for (uint32_t i = 0; i < table->num_rows; i++){
		deserialize_row(row_slot(table,i), &row);
		print_row(&row);
	}
	return EXECUTE_SUCCESS;
}

