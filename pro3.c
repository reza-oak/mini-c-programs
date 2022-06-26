#include<stdio.h>

void setDefault(char *string, int stringLength, char defaultChar);
void printString(char *string);
void printTokens(char *inputString);

int main()
{
	printTokens("Hi there! How is it going?");

	printf("\n********************************************\n");

	printTokens("   I hate         in-person         classess         ");

	printf("********************************************\n");

	printTokens("      Online       classes        are       much      better ");



	return 0;
}

void printTokens(char *inputString)
{
	int MAX_WORD_LENGTH = 100 + 1;//couting the terminating char
	int key = 0;
	char singleWord[MAX_WORD_LENGTH];

	setDefault(singleWord, MAX_WORD_LENGTH, '\0');

	for(int i = 0, wordCounter = 0, letterCounter = 0; inputString[i]; ++i)
	{
		if(inputString[i] != ' ' && inputString[i] != '\n')
		{	
			key = 1;
			
			singleWord[letterCounter++] = inputString[i];
			
		}
		else if(key && inputString[i] == ' ' || inputString[i] == '\n')
		{
			key = 0;

			letterCounter = 0;

			wordCounter++;

			printString(singleWord);

			printf("\n");

			setDefault(singleWord, MAX_WORD_LENGTH, '\0');

		}
	}

	if (key)
	{
		printString(singleWord);
	}
}

void printString(char *str)
{
	for(int i = 0; str[i]; ++i)
	{	
		printf("%c", str[i]);
	}
}

void setDefault(char *string, int stringLength, char defaultChar)
{
	for(int i = 0; i < stringLength; ++i)
	{
		string[i] = defaultChar;
	}
}