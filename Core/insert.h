#include "utility.h"
#include "declarations.h"

ExecuteResult execute_insert(Statement *statemet, Table *table){
	if (table->num_rows >= TABLE_MAX_ROWS) {
		return EXECUTE_TABLE_FULL;
	}
	
	Row *row_to_insert = &(statement->row_to_insert);
	
	serialize_row(row_to_insert, row_slot(table, table->num_rows));
	table->num_rows +=1;
	
	return EXECUTE_SUCCESS;
} 
