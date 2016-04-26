#define DATA_TAG		"data"
#define RECORD_TAG	"record"
#define FIELD_ATTR	"field"

#define READ_BUF_SIZE	1024

int XML_WriteRecords(char *table, CStringList *pfields, CStringList *pvalues);
int XML_ReadRecords(CStatementData *p_statement_data);

// Called by CSV unit also
char *GenerateTempName(char *table_filename, char *temp_filename);
int DumpRecordToResultSet(CStatementData *p_statement_data);
