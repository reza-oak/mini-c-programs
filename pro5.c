#include <stdio.h>
#include<stdlib.h>

//this function takes a binary string and the size of that binary string excluding the termintating character
int toDecimal(char* str, int size)
{
	int placeValue = 1;//value for every slot in a binary format. Ex. 1, 2, 4, 8, etc.
	int result = 0;//the decimal number returned
	int i = size - 1;

	if (str[i--] == '1')//if the leftmost character is one add one to the total
	{
		result += placeValue;
	}
	for (; i >= 0; --i)//loop through the binary string and add values accordingly
	{
		placeValue *= 2;

		if (str[i] == '1')
		{
			result += placeValue;
		}
		else if(str[i] != '0')//if there a character other and 0 or 1 in the binary string,
		{
			printf("Invalid character encountered; terminating the program\n");//terminate the program
			exit(0);
		}
	}

	return result;
}

//this function extracts the binary digits from a file
void getData(char* buffer, int bufferSize, FILE *stream)
{
	char character[2];//pointer used to extract one character at a time
	int counter = 0;
	char dummy;
	char* returnVal = &dummy;//to check if fgets function returned a NULL or not

	while (returnVal != NULL && counter < bufferSize - 1)//if fgets did not return null and limit was not exceeded
	{
		returnVal = fgets(character, 2, stream);//get a character

		if (character[0] == '0' || character[0] == '1' && returnVal != NULL)//if it is either 0 or 1
		{
			buffer[counter++] = character[0];//store the character in the input buffer
		}
		//if the inputted character is not 0 or 1 or a white space, terminate the program
		if (!(character[0] == '0' || character[0] == '1'|| character[0] == '\n'
			|| character[0] == ' ' || character[0] == '\r' || character[0] == '\t'))
		{
			printf("Invalid character encountered; terminating the program\n");
			exit(0);
		}
	}
	if (counter < bufferSize - 1)//if the extracted characters are less than the number expceted, termintate the program
	{
		printf("invalid number of columns; terminating the program\n");
		exit(0);
	}
	buffer[bufferSize - 1] = '\0';//add a null char to the end
}


//this function accepts a file name and converts the contents of the file to decimal digits
void toBinary(char* inputString) {

	FILE* fptr;
	int row = -1, col = -1;
	char* line;//ptr used to get a line of the file at a tine
	int i = 0;
	
	if (inputString[0] == '\0')//if filename is null terminate the program
	{
		printf("invalid filename; terminating the program\n");
		exit(0);
	}

	fptr = fopen(inputString, "r");

	if (fptr == NULL)//if fopen returns null for any reason, terminate the program
	{
		printf("issues opening the file; terminating the program\n");
		exit(0);
	}

	fscanf(fptr, "%d %d", &row, &col);//get the number of rows and columns 

	//if the program fails to get the number of rows and cols or if the numbers are invalid, i.e., negative numbers
	//terminate the program
	if (row == -1 || col == -1 || row < 1 || col < 1)
	{
		printf("issues reading the file; terminating the program\n");
		exit(0);
	}
	
	//allocate the space needed to hold one line of the file
	line = calloc(col + 1, sizeof(char));

	//get the lines of the file and print their respective decimal number
	for (; i < row; ++i)
	{
		 getData(line, col + 1, fptr);

		 printf("%d\n", toDecimal(line, col));
	}
	if (i < row)//if the number of rows read is less than the number of rows expected terminate the program
	{
		printf("invalid number of rows; terminating the program\n");
		exit(0);
	}

	//free the resources
	free(line);
	fclose(fptr); 
}

int main( int argc, char *argv[]) {
  toBinary(argv[1]);

  return 0;
}
