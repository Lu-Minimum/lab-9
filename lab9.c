#include <stdio.h>
#include <stdlib.h>

/*
Hash based on id
id ranges from 0 to 99999
hash result ranges from 0 to 149
*/

#define MAX_HASH_RESULT 149

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType* record;
	struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
	return ((x%23)+((x/23)%23))%23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	//Deal with null
	if (pHashArray==NULL) {
		return;
	}

	int i;

	for (i=0;i<hashSz;++i)
	{
		//Skip empty
		if (pHashArray[i].record==NULL) {
			continue;
		}

		//Prepare to print records
		printf ("Hash index %d:\n", i);
		struct HashType* curr = pHashArray+i;

		//Loop through records
		while (curr!=NULL&&curr->record!=NULL) {
			printf("\t%d %c %d\n", curr->record->id, curr->record->name, curr->record->order);
			curr = curr->next;
		}
	}
}

void addHashRecord(struct HashType* pHashArray, struct RecordType* record) {
	//Deal with null
	if (pHashArray==NULL||record==NULL) {
		return;
	}

	//Find index in hash array
	struct HashType* curr = pHashArray + hash(record->id);

	//Find end of linked list
	while (curr->record!=NULL&&curr->next!=NULL) {
		curr = curr->next;
	}

	//Add new record
	if (curr->record==NULL) {
		curr->record = record;
	} else {
		curr->next = (struct HashType*)malloc(sizeof(struct HashType));
		if (curr->next==NULL) {
			printf("Couldn't allocate memory for hash.\n");
			exit(-1);
		}

		curr->next->next = NULL;
		curr->next->record = record;
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	//Allocate hash array
	struct HashType* pHashes = (struct HashType*)malloc((MAX_HASH_RESULT+1)*sizeof(struct HashType));
	if (pHashes==NULL) {
		printf("Couldn't allocate memory for hash array.\n");
		exit(-1);
	}

	//Initialize to null
	for (int i = 0; i<=MAX_HASH_RESULT; i++) {
		pHashes[i].next = NULL;
		pHashes[i].record = NULL;
	}

	//Add records to hash
	for (int i = 0; i<recordSz; i++) {
		addHashRecord(pHashes, pRecords+i);
	}

	//Print
	displayRecordsInHash(pHashes, MAX_HASH_RESULT+1);
}