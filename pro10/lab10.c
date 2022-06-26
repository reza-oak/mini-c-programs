#include<stdio.h>
#include<stdlib.h>
#include"checkpoint.h"


struct Node *globalNode = NULL;

int main()
{
	int userChoice = 0;
	int loopKey = 1;
	char* filename = NULL;

	globalNode = makeHead();
	

	while(loopKey)
	{
		printf("1) Print Keys\n2) Print Values\n");
		printf("3) Insert KV Pair\n4) Save\n5) Restore\n6) Exit\n");

		printf("Enter: ");
		

		scanf("%d", &userChoice);



		switch(userChoice)
		{
			case 1: 
				print(globalNode, 1);
				break;
			case 2: 
				print(globalNode, 0);
				break;
			case 3: 
				insertKV(globalNode);
				break;
			case 4: 
				printf("Type the file name: ");
				filename = input();
				save(globalNode, filename);
				globalNode = NULL;
				free(filename);
				filename = NULL;
				break;
			case 5: 
				printf("Type the file name: ");
				filename = input();
				globalNode = makeHead();
				restore(globalNode, filename);
				free(filename);
				filename = NULL;
				break;
			case 6: 
				loopKey = 0;
				break;
			default:
				printf("Error: Invalid entry\n") ;
				exit(0);
				break;
		}
	}

	free(filename);
	filename = NULL;

	return 0;
}







