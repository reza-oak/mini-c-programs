#include<stdio.h>


void printMultiplicationTable();

int isPalindrome(int elements[], int size);


int main()
{
	printMultiplicationTable();
	
	int arr1[] = {1, 2, 3, 4, 3, 2, 1};//Palindrome

	int arr2[] = {1, 2, 3, 4, 1, 2, 3};// not Palindrome

	int arr3[] = {1, 2, 3, 55, 1, 2, 3};// not Palindrome

	int arr4[] = {11, 22, 33, 33, 22, 11};// Palindrome

	printf("[1, 2, 3, 4, 3, 2, 1] is %s Palindrome\n", (isPalindrome(arr1, sizeof(arr1)/sizeof(int))) ? "a" : "not");
	
	printf("[1, 2, 3, 4, 3, 2, 1] is %s Palindrome\n", (isPalindrome(arr2, sizeof(arr2)/sizeof(int))) ? "a" : "not");

	printf("[1, 2, 3, 4, 3, 2, 1] is %s Palindrome\n", (isPalindrome(arr3, sizeof(arr3)/sizeof(int))) ? "a" : "not");

	printf("[1, 2, 3, 4, 3, 2, 1] is %s Palindrome\n", (isPalindrome(arr4, sizeof(arr4)/sizeof(int))) ? "a" : "not");
	


	return 0;
}





void printMultiplicationTable()
{
	int i = 1, j = 1;

	for(; i < 10; ++i)
	{
		for(; j < 10; ++j)
		{
			printf("%d * %d = %d\n", i, j, i * j);
		}

		j = 1;

		printf("***********\n");		
	}
}

//My compier does not recognize the return type 'bool', so I replaced it with an int
int isPalindrome(int elements[], int size)
{
	int low = 0;

	int high = size - 1;

	int loopRun = 1;

	while(high > low && loopRun)
	{
		if(elements[high--] != elements[low++])
		{
			loopRun = 0;
		}
	}

	return loopRun;// if 'loopRun' was set to 0 in the loop, it means the input array is not a Palindrome
}