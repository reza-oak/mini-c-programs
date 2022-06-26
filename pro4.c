#include <stdio.h>
#include<stdlib.h>

//function to find the min distance between two identical characters 
void findMinDistance(char *inputString){
  
	char character = 0;//hold the identical character
	int distance = -1;//holds the min distance between the two identical characters
	
	//loop through the characters of the input string
	for (int i = 0; inputString[i]; ++i)
	{
		//loop though the rest of the string
		for (int j = i + 1; inputString[j]; ++j)
		{
			//if no identical characters have not been found yet, and now two identical characters are found
			if (distance  == -1 && inputString[j] == inputString[i])
			{
				character = inputString[j];//store the character

				distance = j - i -1;//get the distance between the two characters

			}
		//if two identical characters are found and their distance is lower than the lowest distance so far
			else if (inputString[j] == inputString[i] && distance > j - i - 1)
			{
				character = inputString[j];//store the character

				distance = j - i - 1;//store the distance between the two characters 
			}
		}
	}

	if (distance == -1)//if no two identical character were found 
	{
		printf("No repeats found!\n");//print the message 
	}
	else//if two identical characters are found
	{
		//print the identical character and the lowest distance between the two
		printf("Repeated char =\"%c\", min distance = %d\n", character, distance);
	}

	
}

//main function
int main( int argc, char *argv[]) {

	if (argc  <= 1)//if no command line argument is given notify the user and terminate the program
	{
		printf("where are the command line arguments???\n");
		exit(-1);
	}

	//call the function
	findMinDistance(argv[1]);

	

  return 0;
}
