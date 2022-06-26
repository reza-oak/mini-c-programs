#include<stdio.h>


#define MAX_WORD 100
#define MAX_WORD_LENGTH 21


void printString(char*, int);
void printLetter(char, int, int);
void function(char[MAX_WORD][MAX_WORD_LENGTH], int*, int, int, int);
void printLine(char words[MAX_WORD][MAX_WORD_LENGTH], int begin, int end, int letterNum, int lineLength);
void formatAndPrintParagraph(char* paragraph, int lineLength);


int main()
{
	formatAndPrintParagraph("Hi everyone. This is the 2nd assignment. Please make sure you start early as this is going to take some time!", 40);
	printf("\n\n\n");
	formatAndPrintParagraph("Hi everyone. This is the 2nd assignment. Please make sure you start early as this is going to take some time!", 25);
	return 0;
}

void printString(char *string, int newLine)
{
	for (int i = 0; string[i]; ++i)
	{
		printf("%c", string[i]);
	}
	if (newLine)
	{
		printf("\n");
	}
}

void printLetter(char letter, int printNum, int newLine)
{
	for (int i = 0; i < printNum; ++i)
	{
		printf("%c", letter);
	}
	if (newLine)
	{
		printf("\n");
	}

}

void printLine(char words[MAX_WORD][MAX_WORD_LENGTH], int begin, int end, int letterNum, int lineLength)
{
	int iterations = (end - begin) - 1;
	int spaces = lineLength - letterNum;
	int spacePerWord[iterations];
	int counter = begin;

	for (int i = 0; i < iterations; ++i)
	{
		spacePerWord[i] = 0;
	}
	for (int i = 0; spaces > 0; ++i)
	{
		if (i >= iterations)
		{
			i = 0;
		}
		spacePerWord[i]++;
		spaces--;
	}
	if (iterations)
	{
		for (int j = 0; counter < end - 1; ++counter, ++j)
		{
			printString(words[counter], 0);
			printLetter(' ', spacePerWord[j], 0);
			
		}
		printString(words[counter], 0);
	}
	else
	{
		printLetter(' ', spacePerWord[0] / 2, 0);
		printString(words[begin], 0);
		printLetter(' ', spacePerWord[0] / 2, 0);
	}
	printf("\n");
}

void formatAndPrintParagraph(char* paragraph, int lineLength)
{
	char words[MAX_WORD][MAX_WORD_LENGTH];
	int wordLengths[MAX_WORD];
	int lineSize = 0;
	int counter = 0;
	int index = 0;
	int letterNum = 0;

	for (int i = 0; i < MAX_WORD; ++i)//100 times
	{
		for (int j = 0; j < MAX_WORD_LENGTH; ++j)//21 times
		{
			words[i][j] = '\0';
		}
		wordLengths[i] = 0;
	}

	for (int i = 0, key = 0, wordCounter = 0, letterCounter = 0; paragraph[i]; ++i)
	{
		if (paragraph[i] != ' ' && paragraph[i] != '\n')
		{
			words[wordCounter][letterCounter++] = paragraph[i];
			wordLengths[wordCounter]++;
			key = 1;
		}
		else if (key && paragraph[i] == ' ' || paragraph[i] == '\n')
		{
			wordCounter++;
			letterCounter = 0;
			key = 0;
		}
	}

	lineSize += wordLengths[counter];
	letterNum += wordLengths[counter++];

	for (; wordLengths[counter]; ++counter)
	{
		if (lineSize + 1 + wordLengths[counter] <= lineLength)
		{
			lineSize += wordLengths[counter] + 1;
			letterNum += wordLengths[counter];
		}
		else if (lineSize + wordLengths[counter] <= lineLength)
		{
			lineSize += wordLengths[counter];
			letterNum += wordLengths[counter];
		}
		else
		{
			printLine(words, index, counter, letterNum, lineLength);

			lineSize = 0;
			letterNum = 0;
			index = counter;
			counter--;
		}
	}
	if (lineSize)
	{
		printLine(words, index, counter, letterNum, lineLength);
	}
}
