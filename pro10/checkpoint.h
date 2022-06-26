#ifndef CHECKPOINT_H
#define CHECKPOINT_H 

//const size_t INTEGER = sizeof(int);
//const size_t DOUBLE = sizeof(double);
#define INTEGER sizeof(int)
#define DOUBLE sizeof(double)

struct Node{
	void *key;
	void *value;
	int keyDataType;
	int valueDataType;

	struct Node *next;
	struct Node *prev;
};


struct Node* makeHead();
void print(struct Node* headNode, int mode);
void insertKV(struct Node* headNode);
void restore(struct Node* headNode, char* filename);
void save(struct Node* headNode, char* filename);
char* input();

#endif