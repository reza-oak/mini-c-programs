#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define  BUFFERSIZE 128

//struct representing a MP3 song that 
//contains the name of the artist and the duration of the music in 
//seconds
struct node_t{
    char* artist;//artist name
    int lengthSeconds;//length in seconds

    struct node_t* next;//ptr to the next song
    struct node_t* prev;//ptr to the previous song

};

//global variable representing the 
//head of the MP3 list
struct node_t *head = NULL;

//this function creates a MP3 with the given artist name 
//and song duration and returns a pointer to that MP3 created
struct node_t* make_node_t(char* name, int num);
//this functon accepts a pointer to a MP3 and frees the memory 
//associated with the MP3
void free_node_t(struct node_t *ptr);
//deletes alll instances of MP3 songs with the given artist name
void delete_node_t(struct node_t * node_t_ref);
//deletes a single instance of MP3 song with the givn artist name
void delete(char *name);
//inserts an MP3 song into the list
void insert(char *name, int data);
//prints all MP3 songs in the list 
void print();
//fully frees the MP3 list and deallocates all memory taken
void freeList();
void sortSongLength(struct node_t** arr, int l, int r);
void mergeSort(struct node_t** arr, int l, int r, int by_name);
void merge(struct node_t** arr, int p, int q, int r, int by_name);
void freeSongArray(struct node_t** node_t_arr, int size);
struct node_t** songArray(struct node_t* head);
int node_count(struct node_t* head);


int main()
{
  int i, num, len;
  struct node *n;
  char buffer[BUFFERSIZE], c;

  head = NULL;

  while (1) {
    printf("\nList Operations\n");
    printf("===============\n");
    printf("(1) Insert\n");
    printf("(2) Display\n");
    printf("(3) Delete\n");
    printf("(4) Exit\n");
    printf("Enter your choice : ");
    if (scanf("%d%c", &i, &c) <= 0) {          // use c to capture \n
        printf("Enter only an integer...\n");
        exit(0);
    } else {
        switch(i)
        {
        case 1: printf("Enter the name to insert : ");
		if (fgets(buffer, BUFFERSIZE , stdin) != NULL) {
                  len = strlen(buffer);
                  buffer[len - 1] = '\0';   // override \n to become \0
                } else {
                    printf("wrong name...");
                    exit(-1);
                  }
                printf("Enter the number to insert : ");
                scanf("%d%c", &num, &c);  // use c to capture \n
                printf("[%s] [%d]\n", buffer, num);
                insert(buffer, num);
                break;
        case 2: if (head == NULL)
                  printf("List is Empty\n");
                else
                  print();
                break;
        case 3: printf("Enter the name to insert : ");
		if (fgets(buffer, BUFFERSIZE , stdin) != NULL) {
                  len = strlen(buffer);
                  buffer[len - 1] = '\0';   // override \n to become \0
                } else {
                    printf("wrong name...");
                    exit(-1);
		}
		delete(buffer);
                break;
        case 4: freeList();
                return 0;
        default: printf("Invalid option\n");
        }
    }
  }
  return 0;
}


void insert(char *name, int data)
{
    //refernce to the head node
    struct node_t* node_t_ref = head;
    //variable for the node that is going to
    //be added to the list
    struct node_t* node_t_new = NULL;

    //if the length of the song is invalid, terminate the program.
    if(data <= 0)
    {
        printf("Invalid song length entered\nterminating the program.\n");
        exit(-1);
    }

    //if the head node is NULL, or, in other words, the
    //list is empty,
    if(node_t_ref == NULL)
    {
        //make a node
        node_t_ref = make_node_t(name, data);
        //store the node made in the head of the list
        head = node_t_ref;
        //and return from function
        return;
    }

    //if the head node is not empty, go through the while loop
    while(1)
    {
        //if there is no node after the current node
        if(node_t_ref->next == NULL)
        {
            break;//get out of the loop
        }

        //if there is a node after the current node, keep
        //traversing the list
        node_t_ref = node_t_ref->next ;
    }

    //when reached the end of the list,
    //add the new node to the end of the list
    node_t_ref->next = make_node_t(name, data);
    node_t_new = node_t_ref->next;
    node_t_new->prev = node_t_ref;
}

void print()
{
    //store the head node
    struct node_t* node_t_ref = head;



    struct node_t** node_t_arr;
    int node_arr_count = 0;
    

    //for getting input from user. Asking 
    //user whether they want the list printed from head to tail 
    //or tail to head.
    int i = 0;
    char c;

    //promot the user for their choice
    printf("(1) Head to tail\n(2) Tail to head\n(3) By name\nEnter your choice: ");

    //get the user input and validate it. If there is something 
    //wrong with the input, terminate the program
    if (scanf("%d%c", &i, &c) <= 0 || (i != 1 && i != 2 && i != 3)) 
    {      
        printf("Enter only an integer...\n");
        exit(0);
    }

    if(i == 3)
    {
        node_arr_count = node_count(head);

        node_t_arr = songArray(head);

        mergeSort(node_t_arr, 0, node_arr_count - 1, 1);

        sortSongLength(node_t_arr, 0, node_arr_count - 1);

        for(int i = 0; i < node_arr_count; ++i)
        {       
            printf("Artist: %s  Length: %ds\n", node_t_arr[i]->artist, node_t_arr[i]->lengthSeconds);
        }

        free(node_t_arr);

        //freeSongArray(node_t_arr, node_arr_count);

        return;
    }

    //loop through the list from head to tail,
    while(node_t_ref != NULL)
    {
        //if user want the list printed from head to tail
        //print the items here as we are looping through 
        //the list from head to tail
        if(i == 1)
        {
            printf("Artist: %s  Length: %ds\n", node_t_ref->artist, node_t_ref->lengthSeconds);
        }

        //if reached the last item within the list break
        //out of the loop
        if(node_t_ref->next == NULL)
        {
            break;
        }

        //go to the next node in the list
        node_t_ref = node_t_ref->next;
    }


    //now that we have reached the last item in the list
    //or the tail of the list we can print the list from tail
    //to head if the user wants.

    //loop through the list from tail to head, if the user wants it
    while(node_t_ref != NULL && i == 2)
    {
        //print the record
        printf("Artist: %s  Length: %ds\n", node_t_ref->artist, node_t_ref->lengthSeconds);
        
        //if reached the head of the list, break out of the loop.
        if(node_t_ref->prev == NULL)
        {
            break;
        }

        //go to the next item
        node_t_ref = node_t_ref->prev;
    }
}

void freeList()
{
    //refernce to the head of the list
    struct node_t* node_t_ref = head;
    //refernce to the next tie within the list
    struct node_t* node_t_next = NULL;
   
   //if the head of the list is not null
    while(node_t_ref != NULL)
    {
        //store the next item of the list
        node_t_next = node_t_ref->next;
        //delete the current item of the list
	    free_node_t(node_t_ref);
        //go to the next item within the list
        node_t_ref = node_t_next;
    }
}

void delete(char *name)
{
    //store the head of the list
    struct node_t* node_t_ref = head; 
    
    //go through the list
    while(node_t_ref != NULL)
    {
        //if the name given as the function argumen is the same as 
        //the name of the current record's artist, delete the current
        //record
        if(strcmp(node_t_ref->artist, name) == 0)
        {
            delete_node_t(node_t_ref);
        }

        //go to the next item within the list
        node_t_ref = node_t_ref->next ;
    }
}

void delete_node_t(struct node_t * node_t_ref)
{
    //reference to the next node
    struct node_t* node_t_next = NULL;
    //refernce to the previous node
    struct node_t* node_t_prev = NULL;

    //if the current node is not null
    if(node_t_ref != NULL)
    {
        //if the next item is not null, store the next item
        if(node_t_ref->next != NULL)
        {
            node_t_next = node_t_ref->next;
        }
        //if the previous item is not null, store the previous item
        if(node_t_ref->prev != NULL)
        {
            node_t_prev = node_t_ref->prev;
        }

        //if neither the previous item nor the next item is null, 
        //connect the prevois node of the current node
        //to the next node of the current node
        if(node_t_prev != NULL && node_t_next != NULL)
        {
            node_t_prev->next = node_t_next;
            node_t_next->prev = node_t_prev; 
        }
        //else if the previous node of the current node is null, but
        //the next node of the current node is not null, set the 
        //prevous node of the next node to null, and assign the 
        //next node as the head of the list
        else if(node_t_prev == NULL && node_t_next != NULL)
        {
            node_t_next->prev = NULL;
            head = node_t_next;
        }
        //else if the previous node is not null, but the next node is null,
        //set the previous node of the next node to null.
        else if(node_t_prev != NULL && node_t_next == NULL)
        {
            node_t_prev->next = NULL;
        }
        //if both the next node and the previous node are null,
        //set the head node to null
        else if(node_t_prev == NULL && node_t_next == NULL)
        {
            head = NULL;
        }

        //free the current noder
        free_node_t(node_t_ref);
    }
}


void free_node_t(struct node_t *ptr)
{
    if(ptr != NULL)//if the ptr is not null
    {
        if(ptr->artist != NULL)//if the artist ptr is not null
        {
            free(ptr->artist);//free artist ptr
        }

        free(ptr);//free the ptr
    }
}

struct node_t* make_node_t(char* name, int num)
{
    //create a MP3 ptr and set it to null
    struct node_t* node_t_ref = NULL;
    //allocate the space require by a MP3 node
    node_t_ref = malloc(sizeof(struct node_t));
    //allocate enough space to store the name of the 
    //artist
    node_t_ref->artist = malloc(sizeof(char) * (strlen(name) + 1));
    //copy the name of the artist
    strcpy(node_t_ref->artist, name);
    //store the duration of the MP3 song
    node_t_ref->lengthSeconds = num;
    //set the previous node and the next node to null
    node_t_ref->prev = NULL;
    node_t_ref->next = NULL;

    //return a ptr to the MP3 node created
    return node_t_ref;
}

int node_count(struct node_t* head)
{
    int count = 0;
    while(head != NULL)
    {
        count++;
        head = head->next;
    }

    return count;
}

struct node_t** songArray(struct node_t* head)
{
    struct node_t* head_ref = head;
    struct node_t** node_t_arr;
    int count = node_count(head_ref);

    node_t_arr = malloc(sizeof(void**) * count);

    for(int i = 0; i < count && head_ref != NULL; ++i)
    {
        node_t_arr[i] = head_ref;

        head_ref = head_ref->next;
    }

    return  node_t_arr;
}


// p = first    q = middle     r = end
void merge(struct node_t** arr, int first, int middle, int end, int by_name) 
{

  int arrLength1 = middle - first + 1;
  int arrLength2 = end - middle;
  int compare_condition = 0;

  struct node_t *leftArr[arrLength1], *rightArr[arrLength2];

  for (int i = 0; i < arrLength1; i++)
      leftArr[i] = arr[first + i];
  for (int j = 0; j < arrLength2; j++)
      rightArr[j] = arr[middle + 1 + j];

  // i = arr1Counter         j = arr2Counter   k = mainArrCounter
  int arr1Counter = 0, arr2Counter = 0, mainArrCounter = first;

  while (arr1Counter < arrLength1 && arr2Counter < arrLength2) {

    if(by_name)
    {
        compare_condition = strcmp(leftArr[arr1Counter]->artist, rightArr[arr2Counter]->artist) < 0;
    }
    else
    {
        compare_condition = leftArr[arr1Counter]->lengthSeconds > rightArr[arr2Counter]->lengthSeconds;
    }
    
    if(compare_condition) 
    {
      arr[mainArrCounter] = leftArr[arr1Counter];
      arr1Counter++;
    } else 
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

//l = first   r = end
void mergeSort(struct node_t** arr, int first, int end, int by_name) 
{
  if (first < end) 
  {
    int middle = first + (end - first) / 2;

    mergeSort(arr, first, middle, by_name);
    mergeSort(arr, middle + 1, end, by_name);

    merge(arr, first, middle, end, by_name);
  }
}

void sortSongLength(struct node_t** arr, int l, int r)
{
    int start_index = -1;
 
    for(int i = 0; i <= r; ++i)
    {
        start_index = i;

        while(i <= r && (strcmp(arr[start_index]->artist, arr[i]->artist) == 0))
        {
            i++;
        }

        if(start_index != i && i > 0)
        {
            mergeSort(arr,start_index, i - 1, 0);
            i--;
        }
    }
}

