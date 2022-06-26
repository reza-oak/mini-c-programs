#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define outputfile "summary.txt"
#define inputfile  "hotellog.txt"

//struct represeting a hotel room
struct Room{
	int roomNum;
	int occupantNum;
	char *lastName;
	int empty;//indicates whether the room is empty or it has been booked
};

void freeRoom(struct Room *room);
char* strConcat(char* dest, char* src);
int search(struct Room **rooms, char* lastName);
void print(struct Room **rooms, FILE *filePtr);
struct Room* getRoom(int occupantNum, char* lastName);
void cleanRoom(struct Room **rooms, int roomNum);
int checkout(struct Room **rooms, char* lastName);
int checkin(struct Room **rooms, char* lastName, int occupantNum);
char* substring(char* str, int first, int last);
int tokenCount(char* str);
char** strTok(char *str);
void processCommand(struct Room **rooms, FILE *filePtr, char *command);
void process(struct Room **rooms, FILE *inputPtr, FILE *outputPtr);



int main()
{
	int roomCount = 10;//ten rooms was mentioned in the lab handout
	//create an array that contains the addresses of the rooms.
	//size that of ten pointers plus one more slot to store the terminating null value
	struct Room **rooms = malloc(sizeof(void**) * (roomCount + 1));

	FILE *outputPtr = NULL;//for the output file where we print the logs
	FILE *inputPtr = NULL;//for the input file from which we read the commands

	outputPtr = fopen(outputfile, "a");//append mood
	inputPtr = fopen(inputfile, "r");//read mood

	//if either streams is null, throw an error
	if(outputfile == NULL || inputPtr == NULL)
	{
		printf("ERRPR: Issues opening input/output file(S)\n");
		exit(0);
	}

	//generate ten rooms
	for(int i = 0; i < roomCount; ++i)
	{
		rooms[i] = getRoom(0, "0123456789ABCDE");

	}
	//set to null so we know when the arry is over
	rooms[roomCount] = NULL;

    //start processing the commands
	process(rooms, inputPtr, outputPtr);

    //free the rooms that were generated
	for(int i = 0; i < roomCount; ++i)
	{
		freeRoom(rooms[i]);
	}

	//close the files
	fclose(outputPtr);
	fclose(inputPtr);
	//free the array that contained the addresses of the rooms
	free(rooms);


	return 0;
}


//Takes a ptr to a room object and frees it 
void freeRoom(struct Room *room)
{
	if(room != NULL)
	{
		if(room->lastName != NULL)
		{	
			free(room->lastName);
		}

		free(room);
	}
}


//dynamically concats the second string to the first string.
char* strConcat(char* dest, char* src)
{
	if (src == NULL)
	{
		return dest;
	}
	if (dest == NULL)//if the first string is null
	{
		dest = calloc(10, sizeof(char));//allocate some space
	}
	//if the content in the first string plus the content in the second string 
	//plus the terminating character occupy more space than what the first 
	//string has. Allocate more space so the two strings can be concatenated together.
	if (strlen(dest) + strlen(src) + 1 > sizeof(dest) / sizeof(char))
	{
		dest = realloc(dest, (strlen(dest) + strlen(src) + 1) * 2);
	}

	strcat(dest, src);//concate the two strings 

	return dest;//return the concated result
}


//returns the index of the room that 'lastName' is staying in. And if there is no room
//book under 'lastName' it returns -1.
int search(struct Room **rooms, char* lastName)
{
	int i = 0;
	for(int i = 0; rooms[i] != NULL; ++i)
	{
		//if there is a match between the last names and the room is booked,
		if(strcmp(rooms[i]->lastName, lastName) == 0 && !rooms[i]->empty)
		{
			return i;//return the room number.
		}
	}

	return -1;//no room found under the last name, return -1 to indicate that.
}

//prints a log of the rooms
void print(struct Room **rooms, FILE* filePtr)
{
	int i = 0;

	//print the header of for the log table to be printed.
	fprintf(filePtr, "Name%15sRoom%4sNumber%2sNumber of Occupants\n", "", "", "");

	//for each room that has been booked,
	for(int i = 0; rooms[i] != NULL; ++i)
	{
		if(!rooms[i]->empty)
		{
			//print the details.

			fprintf(filePtr, "%s%-15s%-4s%d%-7s%d\n", "", rooms[i]->lastName, "", 
				rooms[i]->roomNum, "", rooms[i]->occupantNum);
		}
	}
	fprintf(filePtr, "\n");
}

//creates a room with the param values and returns a ptr to it.
struct Room* getRoom(int occupantNum, char* lastName)
{
	//represents the room number starting at 0.
	static int roomCount = 0;

	struct Room *room_ref = malloc(sizeof(struct Room));

	room_ref->roomNum = roomCount;
	room_ref->occupantNum = occupantNum;
	room_ref->lastName = malloc((strlen(lastName) + 1) * sizeof(char));
	strcpy(room_ref->lastName, lastName);
	room_ref->empty = 1;//1 if the room is not bookded

	//static local var indicating the room number.
	roomCount++;

	return room_ref;
}

//sets a room to the default values when someone checks out
void cleanRoom(struct Room **rooms, int roomNum)
{
	//default value for last name
	static const char* const defaultLastName = "0123456789ABCDE";
	//default number for the number of occupants
	static const int defaultOccupantNum = 0;

	struct Room* room = rooms[roomNum];//get the room based on the room number
	//set the default values
	strcpy(room->lastName, defaultLastName);
	room->roomNum = defaultOccupantNum;
	room->empty = 1;//set to 1 to indicate that the room is no longer booked.
}

//checks out 'lastName' of the their room.
int checkout(struct Room **rooms, char* lastName)
{
	//find out the room number where 'lastName' is staying
	int roomNum = search(rooms, lastName);
	//if a room is in fact booked under the name 'lastName'
	if(roomNum != -1)
	{
		cleanRoom(rooms, roomNum);//clean the room

		//return the room that was checked out
		return roomNum;
	}

	//if there is no record of 'lastName' booking a room
	//return -1 to indocate that.
	return -1;
}

//checks in a room under the name 'lastName'.
int checkin(struct Room **rooms, char* lastName, int occupantNum)
{
	int i = 0;
	for(;rooms[i] != NULL; ++i)//find an empty room
	{
		if(rooms[i]->empty)
		{
			break;
		}
	}

	//book the empty room found
	if(rooms[i] != NULL && rooms[i]->empty)
	{
		strcpy(rooms[i]->lastName, lastName);
		rooms[i]->occupantNum = occupantNum;
		rooms[i]->empty = 0;

		return i;//return the number of the room that was booked
	}

	//if there are no rooms to be booked(all rooms are already booked), return 
	//-1 to indicate that.
	return -1;
}	



//returns the substring from 'first' index to 'last' index inclusive
char* substring(char* str, int first, int last)
{
	int strSize = last - first + 1;//get the length of the substring
	//allocate memory for the substring
	char *result = malloc((strSize + 1) * sizeof(char));
	//used to advance the index of the substring
	int counter = 0;

	//go one char at a time and copy the char into the substring that was just allocated
	for(int i = first; i <= last; ++i)
	{
		result[counter++] = str[i];
	}

	//append a terminating character at the end
	result[counter] = '\0';

	//return the substring
	return result;

}


//returns the number of the tokens the param string has.
int tokenCount(char* str)
{
	//used to count the number of substrings
	int count = 0;
	int j = 0;

	for(int i = 0; i < strlen(str); ++i)
	{
		
		while(j < strlen(str) && isspace(str[j]))//skip the white spaces
		{
			j++;
		}

		if(j < strlen(str) && !isspace(str[j]))//if we have encounterd a non-white space character
		{
			count++;//increment the number of the tokens
		}

		//skip the rest of the token that was encountered
		while(j < strlen(str) && !isspace(str[j]))
		{
			j++;
		}

		//index pointing to the location after the token that was read.
		//we must decrement it by one so when the i is incremented by the for loop
		//we do not miss one character.
		i = j -1;
	}

	//return the number of the tokens present in the param string
	return count;
}

//returns an array containing the tokens present in the param string
char** strTok(char *str)
{
	int count = tokenCount(str);//get the number of tokens present 
	//allocate an array capable of storing pointers to strings with the size of the tokens plus
	//one. The extra slot at the end stores a null value so it can be used to tell
	//when we have reached the end of the array that stores all tokens. 
	char **tokArr = malloc(sizeof(void**) * (count + 1));//last num
	int j = 0;
	int begin = -1;//points to the first character of a token
	int tokenIndex = 0;//used to increment indices for the token array

	for(int i = 0; i < strlen(str); ++i)
	{
		j = i;

		//skip all white spaces
		while(j < strlen(str) && isspace(str[j]))
		{
			j++;
		}

		//if we have encoutered a token,
		if(j < strlen(str) && !isspace(str[j]))
		{
			begin = j;//get the index of the first character of the token found.
		}

		//go through the entire token
		while(j < strlen(str) && !isspace(str[j]))
		{
			j++;
		}

		//if we have encounterd a token and the size of the token is not only one char long*
		if(begin > -1 && j - 1 > -1)
		{
			tokArr[tokenIndex++] = substring(str, begin, j - 1);//store the token
		}


		i = j - 1;//decrement so when the for loop incremenets it, we do not miss one char
		begin = -1;//set to the default value -1
		j = 0;//set to the default value zero
	}

	//set to null so we cann tell when the 'tokArr' has ended. Its kind of like 
	//the null character '\0' in strings.
	tokArr[count] = NULL;

	return tokArr;//return the token array
}

//processes commands enterd by the user and output logs to a file.
void processCommand(struct Room **rooms, FILE *filePtr, char *command)
{
	//available commands
	static const char* checkinCommand = "CHECKIN";
	static const char* checkoutCommand = "CHECKOUT";
	static const char* printCommand = "PRINTOCCUPANCY";
	static const char* searchCommand = "SEARCH";

	//The number of tokens for a command. That is the command itself plus the arguments 
	//required make up the number of tokens for a command.
	static const int checkin_tok_num = 3;//CheckIn + lastName + occpuantNum = 3
	static const int checkout_tok_num = 2;//Checkout + lastName = 2
	static const int print_tok_num = 1;//printOccupany = 1
	static const int search_tok_num = 2;//Search + lastName = 2

	//store the tokens present in a single line
	char** commandToks = strTok(command);
	int occupantNum = 0;//used to store occupant num
	char* lastName = NULL;//used to store lastName
	int tokenNum = tokenCount(command);//get the number of tokens present in a singe line
	int roomNum = -1;//used to store room number. Initialy set to -1.


    //if the number of tokens present in the line is euqal to the number of tokens 
    //for a check in command,
	if(tokenNum == checkin_tok_num)
	{
		if(strcmp(commandToks[0], checkinCommand) != 0)
		{
			printf("ERROR: invalid commands/arguments1\n");
			exit(0);
		}

		lastName = commandToks[1];//store firstname
		occupantNum = atoi(commandToks[2]);//store occpuant num

		//if there is a free room, book it and notify the user.
		if((roomNum = checkin(rooms, lastName, occupantNum)) != -1)
		{
			fprintf(filePtr, "%s was checked into room %d.\n\n", lastName, roomNum);
		}
		else//if there all rooms have been booked, inform the user.
		{
			fprintf(filePtr, "Sorry, no room was available for %s.\n\n", lastName);
		}
	}
	//if the number of tokens present in the line is euqal to the number of tokens 
    //for a check out command and the command is the same as checkout command,
	else if(tokenNum == checkout_tok_num && strcmp(commandToks[0], checkoutCommand) == 0)
	{
		
		lastName = commandToks[1];//store the lastname

		//if there is a room booked under the name, free the room
		if((roomNum = checkout(rooms, lastName)) != -1)
		{
			fprintf(filePtr, "%s was checked out of room %d.\n\n", lastName, roomNum);
		}
		else//if no room has been booked under that name, inform the under about it.
		{
			fprintf(filePtr, "Sorry, there is no occupant named %s.\n\n", lastName);
		}

	}
	//if the number of tokens present in the line is euqal to the number of tokens 
    //for a print log command,
	else if(tokenNum == print_tok_num)
	{

		print(rooms, filePtr);//print the rooms' details.
	}
	//if the number of tokens present in the line is euqal to the number of tokens 
    //for a search command and the command is the same as search command,
	else if(tokenNum == search_tok_num && (strcmp(commandToks[0], searchCommand) == 0))
	{
		lastName = commandToks[1];//store the lastname

		//if there is a room booked under the lastname
		if((roomNum = search(rooms, lastName)) != -1)
		{
			//print the details
			fprintf(filePtr, "%s is currently staying in room %d.\n\n", lastName, roomNum);
		}
		else
		{
			//otherwise inform the user that no room has been booked under that name.
			fprintf(filePtr, "Sorry, there is no occupant named %s.\n\n", lastName);
		}
	}	
	//if the number of tokens present in the line is not eqaul to any of the excpeted commands,
	//that's an error.
	else
	{
		//print the error message, and terminate the program
		printf("ERROR: invalid commands/arguments\n");
		exit(0);
	}

	//free the strings used to store the tokens present in the line.
	for(int i = 0; i < tokenNum; ++i)
	{
		free(commandToks[i]);
	}

    //free the pointer to char pointer itself. That is free the string array itself.
	free(commandToks);
}


//used to process the commands present in the inputfile
void process(struct Room **rooms, FILE *inputPtr, FILE *outputPtr)
{
	//used to store a single character from the input file. For now the value
	//of the character in question will be A.
	char character[2] = {'A', '\0'};
	char* line = NULL;//stores the line from the file.
	//used so that 'readStatus' ptr inititally points to a location that is not null
	char dummy = 'A';
	//make the ptr point to the dummay variable.
	char *readStatus = &dummy;
	int commandNumber = 0;

	//read the first integer, i.e., the number of commands in the file and ignore it.
	//This program does not need to know the number of commmand lines to work.
	//If fscanf fails to read the first integer in the file represeting the number of
	//commands in the file, an error is thrown.
	if(	fscanf(inputPtr, "%d", &commandNumber) == 0)
	{
		printf("Error: File corrupted\n");    
		exit(0);
	}

	//while no errors have been encountered and EOF has not been reached
	while(readStatus != NULL)
	{
		//read a single character from the file and store it in 'character'
		readStatus = fgets(character, 2, inputPtr);

		//while no errors have been encountered and EOF has not been reached
		//and the character read is not a new line or a carriage return(13 is ASCII value
		//for carriage return),  
		while(readStatus != NULL && character[0] != '\n' && character[0] != 13)
		{
			//concat the character read to the part of the line that has already been read
			line = strConcat(line, character);

			//read a character from the file
			readStatus = fgets(character, 2, inputPtr);
		}

		//if line is not null or in other words a line has been read from the file.
		if(line != NULL)
		{
			//process the line read
			processCommand(rooms, outputPtr, line);
		}

		//before moving on to the next line free the char* that stores the line
		free(line);
		//and set it to null
		line = NULL;
	}
}

