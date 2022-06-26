#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct 
{
	float gpa;
	char* firstname;
	struct Student *next;
} Student;

char* str_concat(char* dest, char* src);
void read_file(char* filename);
Student* make_student(char* firstname, float gpa);
void insert_student(char* firstname, float gpa);
void free_student_records(Student* head);
void free_student(Student* student);
void merge(Student** arr, int first, int middle, int end, int by_gpa);
void mergeSort(Student** arr, int first, int end, int by_gpa);
int validateStudentData(char* firstname, char* gpa);
Student** filter_students(Student* head, float gpaFilter);
int filter_count(Student* head, float gpaFilter);
void printStudentRecords(Student** arr, int studentCount);
void sortByFirstname(Student** arr, int length);


Student* global_head = NULL;

int main()
{
	Student** arr;
	int filterCount = 0;
	read_file("student.txt");
	filterCount = filter_count(global_head, 3.9);
	arr = filter_students(global_head, 3.9);
	mergeSort(arr, 0, filterCount - 1, 1);
	sortByFirstname(arr, filterCount);
	printStudentRecords(arr, filterCount);
	free(arr);
	free_student_records(global_head);

	
	return 0;
} 

char* str_concat(char* dest, char* src)
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

void read_file(char* filename)
{
	FILE* fileptr = fopen(filename, "r");
	char dummy = 'A';
	char* read_status = &dummy;
	char character[2] = { ' ', '\0' };
	char* firstname = malloc(sizeof(char) * 10);
	char* gpa = malloc(sizeof(char) * 10);
	int record_read = 0;


	if (fileptr == NULL)
	{
		printf("ERROR: Issues opening the %s\n", filename);
		exit(-1);
	}

	while (read_status != NULL)
	{
		strcpy(firstname, "\0");
		strcpy(gpa, "\0");

		

		//skip the white spaces
		while (read_status != NULL && isspace(character[0]))
		{
			read_status = fgets(character, 2, fileptr);
		}
		//either reading extra space at the end or no content at all 
		if (record_read && read_status == NULL)
		{
			free(firstname);
			free(gpa);
			free(fileptr);
			return;
		}
		//no content at all
		else if (read_status == NULL)
		{
			printf("ERROR: issues reading the file\n");
			exit(-1);
		}
		//read the alpha name
		while (read_status != NULL && isalpha(character[0]))
		{
			firstname = str_concat(firstname, character);
			read_status = fgets(character, 2, fileptr);
		}
		//not encoutered a space or other issues
		if (read_status == NULL || !isspace(character[0]))
		{
			printf("ERROR: file corrupted\n");
			exit(-1);
		}
		//skip the white spaces
		while (read_status != NULL && isspace(character[0]))
		{
			read_status = fgets(character, 2, fileptr);
		}
		//if end of file, record not complete. if char read not numberic but alpha invalid file
		if (read_status == NULL || !(isdigit(character[0]) || character[0] == '.'))
		{
			printf("ERROR: File corrupted\n");
			exit(-1);
		}
		//read the numeric gpa
		while (read_status != NULL && (isdigit(character[0]) || character[0] == '.'))
		{
			gpa = str_concat(gpa, character);
			read_status = fgets(character, 2, fileptr);
		}
		if (isalpha(character[0]))
		{
			printf("ERROR: File corrupted\n");
			exit(-1);
		}

		record_read = 1;
		if (validateStudentData(firstname, gpa))
		{
			insert_student(firstname, atof(gpa));
		}
		else
		{
			printf("ERROR: Invalid entry\n");
			exit(-1);
		}
	}

	free(firstname);
	free(gpa);
	free(fileptr);
}

Student* make_student(char* firstname, float gpa)
{
	Student *student_ref = malloc(sizeof(Student));
	student_ref->next = NULL;
	student_ref->firstname = malloc((strlen(firstname) + 1) * sizeof(char));
	strcpy(student_ref->firstname, firstname);
	student_ref->gpa = gpa;

	return student_ref;
}

void insert_student(char* firstname, float gpa)
{
	Student *ref = global_head;

	if(global_head == NULL)
	{
		
		global_head = make_student(firstname, gpa);
		return;
	}

	while(ref->next != NULL)
	{
		ref = (Student*)ref->next;
	}

	ref->next = (struct Student*)make_student(firstname, gpa);
}

void free_student_records(Student *head)
{
	Student *freeable = NULL; 

	while(head != NULL)
	{
		freeable = head;

		head = (Student*) head->next;

		free_student(freeable);
	}
}

void free_student(Student *student)
{
	if(student != NULL)
	{
		if(student->firstname != NULL)
		{
			free(student->firstname);
		}

		free(student);
	}
}


void merge(Student** arr, int first, int middle, int end, int by_gpa)
{
	int arrLength1 = middle - first + 1;
	int arrLength2 = end - middle;
	int compare_condition = 0;
	int arr1Counter = 0, arr2Counter = 0, mainArrCounter = first;
	Student* leftArr[arrLength1], * rightArr[arrLength2];

	for (int i = 0; i < arrLength1; i++)
	{
		leftArr[i] = arr[first + i];
	}
	for (int j = 0; j < arrLength2; j++)
	{
		rightArr[j] = arr[middle + 1 + j];
	}

	
	while (arr1Counter < arrLength1 && arr2Counter < arrLength2) 
	{

		if (by_gpa)
		{
			compare_condition = leftArr[arr1Counter]->gpa < rightArr[arr2Counter]->gpa;
		}
		else
		{
			compare_condition = strcmp(leftArr[arr1Counter]->firstname, rightArr[arr2Counter]->firstname) < 0;
		}

		if (compare_condition)
		{
			arr[mainArrCounter] = leftArr[arr1Counter];
			arr1Counter++;
		}
		else
		{
			arr[mainArrCounter] = rightArr[arr2Counter];
			arr2Counter++;
		}

		mainArrCounter++;
	}


	while (arr1Counter < arrLength1)
	{
		arr[mainArrCounter] = leftArr[arr1Counter];
		arr1Counter++;
		mainArrCounter++;
	}
	while (arr2Counter < arrLength2)
	{
		arr[mainArrCounter] = rightArr[arr2Counter];
		arr2Counter++;
		mainArrCounter++;
	}
}

void mergeSort(Student** arr, int first, int end, int by_gpa)
{
	if (first < end)
	{
		int middle = first + (end - first) / 2;

		mergeSort(arr, first, middle, by_gpa);
		mergeSort(arr, middle + 1, end, by_gpa);

		merge(arr, first, middle, end, by_gpa);
	}
}

int filter_count(Student* head, float gpaFilter)
{
	Student* ref = head;
	int studentCount = 0;

	while (ref != NULL)
	{
		if (ref->gpa > gpaFilter)
		{
			studentCount++;
		}
		ref = (Student*)ref->next;
	}

	return studentCount;
}

Student** filter_students(Student* head, float gpaFilter)
{
	Student* ref = head;
	int studentCount = filter_count(head, gpaFilter);
	Student** arr = malloc(sizeof(void **) * studentCount);

	for (int i = 0; i < studentCount && ref != NULL;)
	{
		if (ref->gpa > gpaFilter)
		{
			arr[i++] = ref;
		}
		ref = (Student*) ref->next;
	}

	return arr;
}


int validateStudentData(char* firstname, char* const gpa)
{
	static const float MIN_GPA = 0.0;
	static const float MAX_GPA = 4.0;
	static const int MIN_FIRSTNAME_LENGTH = 3;
	static const int MAX_FIRSTNAME_LENGTH = 15;
	static const int MIN_FLOAT_DIGIT_LENGTH = 3;
	static const int MAX_FLOAT_DIGIT_LENGTH = 5;

	int decimalPointExistence = 0;

	if (firstname == NULL || gpa == NULL)
	{
		return 0;
	}
	if (strlen(firstname) < MIN_FIRSTNAME_LENGTH || strlen(firstname) > MAX_FIRSTNAME_LENGTH)
	{
		return 0;
	}
	if(strlen(gpa) == 1 && isdigit(gpa[0]) && atoi(gpa) >= 0 && atoi(gpa) <= 4)
	{
		return 1;
	}
	if (strlen(gpa) > MAX_FLOAT_DIGIT_LENGTH || strlen(gpa) < MIN_FLOAT_DIGIT_LENGTH)
	{
		return 0;
	}
	if(gpa[0] == '.')
	{
		return 0;
	}
	if(gpa[1] != '.')
	{
		return 0;
	}
	for(int i = 0; i < strlen(gpa);++i)
	{
		if(decimalPointExistence && gpa[i] == '.')
		{
			return 0;
		}
		if(!decimalPointExistence && gpa[i] == '.')
		{
			decimalPointExistence = 1;
		}
		if(!(isdigit(gpa[i]) || gpa[i] == '.'))
		{
			return 0;
		}
	}
	if(atof(gpa) > MAX_GPA || atof(gpa) < MIN_GPA)
	{
		return 0;
	}

	return 1;
}

void printStudentRecords(Student **arr, int studentCount)
{
	for (int i = 0; i < studentCount; ++i)
	{
		printf("%s %.3f\n", arr[i]->firstname, arr[i]->gpa);
	}
}

void sortByFirstname(Student** arr, int length)
{
	int start_index = -1;

	for (int i = 0; i <= length - 1; ++i)
	{
		start_index = i;

		while (i <= length - 1 && arr[start_index]->gpa ==  arr[i]->gpa)
		{
			i++;
		}

		if (start_index != i && i > 0)
		{
			mergeSort(arr, start_index, i - 1, 0);
			i--;
		}
	}
}




