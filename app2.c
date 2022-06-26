#include <stdio.h>
#include <stdlib.h>

//returns the weight that the leftmost and the right most blocks are carrying
double straightLine(int row)
{
    if (row == 0)//if row 0 is reached return 0 as this row does not support any weight 
    {
        return 0;
    }
    else
    {
        return (straightLine(row - 1) + 200.0) / 2.0;//get the weight that the current block is supporting
    }
}

//returns the weight that a block in a pyramid is supporting 
double weightOnBackOf(int row, int col)
{
    //if row or col number is negative or if col number is greater than row number, terminate the program
    if (row < 0 || col < 0 || col > row)
    {
        printf("invalid row and/or col. Terminating the program\n");
        exit(0);
    }
    if (col == 0 || col == row)//if col 0 is reached or the row number equals the col number
    {
        return straightLine(row);//call this function as we have reached either either the leftmost block or the rightmost block
    }
    else
    {
        //the weight that the current block is supporting, i.e, half of the upper right block and half of the upper left block
        return (weightOnBackOf(row - 1, col - 1) + 200.0 + weightOnBackOf(row - 1, col) + 200.0) / 2.0;
    }
}

int main(int argc, char *argv[]){
  int row, col;
  row = atoi(argv[1]);
  col = atoi(argv[2]);
  
  double ret_val = weightOnBackOf(row, col);
  printf("%f\n", ret_val);
  
  return 0;
}
