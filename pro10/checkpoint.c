#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include"checkpoint.h"

int isInteger(char* strToken)
{
	if(strToken == NULL)
	{
		return 0;
	}
	for(int i = 0; i < strlen(strToken); ++i)
	{
		if(!isdigit(strToken[i]))
		{
			return 0;
		}
	}

	return 1;
}

int isDouble(char* strToken)
{
	int decimalPoint = 0;

	if(strToken == NULL)
	{
		return 0;
	}
	for(int i = 0; i < strlen(strToken); ++i)
	{
		if(!isdigit(strToken[i]) && strToken[i] != '.')
		{
			return 0;
		}
		if(strToken[i] == '.')
		{
			decimalPoint = 1;
		}
	}

	if(decimalPoint)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

struct Node *makeNode(void *key, void *value, int keyDataType, int valueDataType)
{
	int strSize = 0;
	struct Node* newNode = malloc(sizeof(struct Node));
	newNode->next = NULL;
	newNode->prev = NULL;
	newNode->keyDataType = keyDataType;
	newNode->valueDataType = valueDataType;


	if(keyDataType < 0)
	{
		newNode->key = malloc((keyDataType * -1) * sizeof(char));
		memmove(newNode->key, key, keyDataType * -1);
	}
	else
	{

		newNode->key = malloc(keyDataType);
		memmove(newNode->key, key, keyDataType);
	}
	if(valueDataType < 0)
	{
		newNode->value = malloc((valueDataType * -1) * sizeof(char));
		memmove(newNode->value, value, valueDataType * -1);
	}
	else
	{
		newNode->value = malloc(valueDataType);
		memmove(newNode->value, value, valueDataType);
	}

	return newNode;
}

void freeNode(struct Node *node)
{
	free(node->key);
	free(node->value);
	free(node);
}

void appendNode(struct Node* headNode, void *key, void *value, int sizek, int sizev)
{
	struct Node *nodeRef = headNode;
	struct Node *newNode = NULL;

	while(nodeRef->next != NULL)
	{
		nodeRef = nodeRef->next;
	}

	newNode = makeNode(key, value, sizek, sizev);
	newNode->prev = nodeRef;
	nodeRef->next = newNode;
}


char* strConcat(char* dest, char* src)
{
	if (src == NULL)
	{
		return dest;
	}
	if (dest == NULL)
	{
		dest = calloc(10, sizeof(char));
	}
	if (strlen(dest) + strlen(src) + 1 > sizeof(dest) / sizeof(char))
	{
		dest = realloc(dest, (strlen(dest) + strlen(src) + 1) * 2);
	}

	strcat(dest, src);
	return dest;
}

char* input()
{
	char character[2] = {'A', '\0'};
	char dummy = 'A';
	char *readStatus = &dummy;
	char* token = NULL;
	int loopKey = 1;



	while(readStatus != NULL && loopKey)
	{
		readStatus = fgets(character, 2, stdin);


		while(readStatus != NULL && isspace(character[0]))
		{
			readStatus = fgets(character, 2, stdin);
		}
		while(readStatus != NULL && !isspace(character[0]))
		{
			token = strConcat(token, character);
			readStatus = fgets(character, 2, stdin);
		}

		loopKey = !(isspace(character[0]));
	}

	return token;
}

void saveNode(struct Node* node, FILE* filePtr)
{
	if(node->keyDataType == INTEGER)//FILE *stream, const char *format, ...)
	{
		fprintf(filePtr, "<%d>", *((int*)(node->key)));
	}
	else if(node->keyDataType == DOUBLE)
	{
		fprintf(filePtr, "<%f>", *((float*)(node->key)));
	}
	else if(node->keyDataType < 0)
	{
		fprintf(filePtr, "<%s>", ((char*)(node->key)));
	}
	if(node->valueDataType == INTEGER)
	{
		fprintf(filePtr, "<%d>",  *((int*)(node->value)));
	}
	else if(node->valueDataType == DOUBLE)
	{
		fprintf(filePtr, "<%f>", *((float*)(node->value)));
	}
	else if(node->valueDataType < 0)
	{
		fprintf(filePtr, "<%s>", ((char*)(node->value)));
	}
}




char* parse(FILE *filePtr)
{
	char character[2] = {'A', '\0'};
	char dummy = 'A';
	char *readStatus = &dummy;
	char* token = NULL;
	int read = 0;


	while(readStatus != NULL)
	{
	
		while(readStatus != NULL && character[0] != '<')
		{	
			readStatus = fgets(character, 2, filePtr);
		}
		if(readStatus != NULL && character[0] == '<')
		{
			read = 1;
			readStatus = fgets(character, 2, filePtr);
		}
		while(readStatus != NULL && read && character[0] != '>')
		{
			token = strConcat(token, character);
			readStatus = fgets(character, 2, filePtr);
		}
		if(character[0] == '>')
		{
			break;
		}
	}

	return token;
}

void save(struct Node* headNode, char* filename)
{
	struct Node* nodeRef = headNode;
	struct Node* prevNode = NULL;
	FILE *filePtr = fopen(filename, "ab");

	while(nodeRef != NULL)
	{
		prevNode = nodeRef;
		nodeRef = nodeRef->next;
		if(prevNode->key != NULL && prevNode->value != NULL)
		{
			saveNode(prevNode, filePtr);
		}
		freeNode(prevNode);
	}

	fclose(filePtr);
}

void restoreNode(struct Node *headNode, char *keyTok, char* valueTok)
{
	void* key = NULL;
	void* value = NULL;
	int keyDataType = 0;
	int valueDataType = 0;

	int integral = 0;
	float floating = 0.0;

	if(isInteger(keyTok))
	{
		keyDataType = INTEGER;
		key = malloc(sizeof(int));
		integral = atoi(keyTok);
		memmove(key, &integral, sizeof(int));

	}
	else if(isDouble(keyTok))
	{
		keyDataType = DOUBLE;
		key = malloc(sizeof(float));
		floating = atof(keyTok);
		memmove(key,&floating, sizeof(float));
	}
	else 
	{
		keyDataType = -1 * (strlen(keyTok) + 1);
		key = malloc(sizeof(char) * (strlen(keyTok) + 1));
		memmove(key, keyTok, (strlen(keyTok) + 1) * sizeof(char));
	}
	if(isInteger(valueTok))
	{
		valueDataType = INTEGER;
		value = malloc(sizeof(int));
		integral = atoi(valueTok);
		memmove(value, &integral, sizeof(int));

	}
	else if(isDouble(valueTok))
	{
		valueDataType = DOUBLE;
		value = malloc(sizeof(float));
		floating = atof(valueTok);
		memmove(value, &floating, sizeof(float));
	}
	else 
	{
		valueDataType = -1 * (strlen(valueTok) + 1);
		value = malloc(sizeof(char) * (strlen(valueTok) + 1));
		memmove(value, valueTok, (strlen(valueTok) + 1) * sizeof(char));
	}

	 appendNode(headNode, key, value, keyDataType, valueDataType);
}

void restore(struct Node* headNode, char* filename)
{
	FILE *filePtr = fopen(filename, "rb");
	char* token = NULL;
	char* key = NULL;
	char* value = NULL;

	if(filePtr == NULL)
	{
		printf("Error: no such a file\n");
		exit(0);
	}

	do
	{
		token = parse(filePtr);
		key = token;
		
		token = parse(filePtr);
		value = token;

		if(key != NULL && value != NULL)
		{
			restoreNode(headNode, key, value);
			free(key);
			free(value);
			key = NULL;
			value = NULL;
		}
		
	} while(token != NULL);

	fclose(filePtr);
}

void printNode(struct Node* node, int mode)
{
	if(mode && node->key != NULL)
	{
		if(node->keyDataType == INTEGER)
		{
			printf("%d\n", *((int*)(node->key)));
		}
		else if(node->keyDataType == DOUBLE)
		{
			printf("%f\n", *((float*)(node->key)));
		}
		else if(node->keyDataType < 0)
		{
			printf("%s\n", (char*)(node->key));
		}
	}
	else if(node->value != NULL)
	{
		if(node->valueDataType == INTEGER)
		{
			printf("%d\n", *((int*)(node->value)));
		}
		else if(node->valueDataType == DOUBLE)
		{
			printf("%f\n", *((float*)(node->value)));
		}
		else if(node->valueDataType < 0)
		{
			printf("%s\n", (char*)(node->value));
		}	
	}
}

void print(struct Node* headNode, int mode)
{
	struct Node* nodeRef = headNode;

	if(mode)
	{
		printf("**********KEYS**********\n");
	}
	else
	{
		printf("**********VALUES**********\n");
	}

	while(nodeRef != NULL)
	{
		printNode(nodeRef, mode);

		nodeRef = nodeRef->next;
	}

	printf("************************\n");
}

int invalidEntry(char* entry)
{
	static const char RESERVED_CHAR1 = '<';
	static const char RESERVED_CHAR2 = '>';

	if(entry == NULL)
	{
		return 1;
	}
	for(int i = 0; i < strlen(entry); ++i)
	{
		if(entry[i] == RESERVED_CHAR2 || entry[i] == RESERVED_CHAR2)
		{
			return 1;
		}
	}

	return 0;
}

void insertKV(struct Node* headNode)
{
	char *keyToken = NULL;
	char *valueToken = NULL;

	printf("Insert key: ");
	keyToken = input();

	printf("Insert value: ");
	valueToken = input();

	if(invalidEntry(keyToken) || invalidEntry(valueToken))
	{
		printf("Error: Invalid entry, < and > are reserved characters and not allowed as key/value\n");
		exit(0);
	}

	restoreNode(headNode, keyToken, valueToken);

	free(keyToken);
	free(valueToken);
}

struct Node* makeHead()
{
	struct Node* head = malloc(sizeof(struct Node));
	head->prev = NULL;
	head->next = NULL;
	head->key = NULL;
	head->value = NULL;
	head->keyDataType = 55;
	head->valueDataType = 55;
}