#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

void validateBitWidthRange(int bitwidth);
long long signed_min(int bitwidth);
long long signed_max(int bitwidth);
long long sat_add(long long operand1, long long operand2, int bitwidth);
void printRange(int bitwidth);

int main(int argc, char* argv[])
{
	 if(argc == 2)
	 {

	 	validateBitWidthRange(atoi(argv[1]));

	 	printRange(atoi(argv[1]));
	 }
	 else if(argc == 4)
	 {
	 	printf("%d + %d = %lld\n", atoi(argv[2]), atoi(argv[3]), sat_add(atoi(argv[2]), atoi(argv[3]), atoi(argv[1])));
	 }

	return 0;
}



//checks whether the number of bits is within the range of 4-64
void validateBitWidthRange(int bitwidth)
{
	static const int MIN_BIT_WIDTH = 4, MAX_BIT_WIDTH = 64;

	//if out of range terminate the program
	if(bitwidth < MIN_BIT_WIDTH || bitwidth > MAX_BIT_WIDTH)
	{
		printf("Error: Bit number out of range. Min = %d and Max = %d\n", MIN_BIT_WIDTH, MAX_BIT_WIDTH);
		exit(0);
	}
}


//takes the number of bits and returns the largest positive number
//representable by two's complement system
long long signed_max(int bitwidth)
{
	//set LSB
	long long mask = 1;
	//move LSB to MSB
	mask = mask << (bitwidth - 1);
	//set all bits before MSB and unset MSB
	mask = mask - 1ll;
	//return result
	return mask;
}

//takes the number of bits and returns the lowest(largest negative value)
//representable by two's complement system
long long signed_min(int bitwidth)
{
	//LSB is set and all other bits are unset
	long long mask = 1;
	//check the number of bits
	validateBitWidthRange(bitwidth);
	//shift the LSB to the MSB
	mask = mask << (bitwidth - 1);
	//negate the result
	return -mask;
}

// If the sum of two positive numbers yields a negative result, the sum has overflowed. 
//If the sum of two negative numbers yields a positive result, the sum has overflowed.

//takes two long arguments which represent the decimal value of numbers to be 
//added in saturation addition system. The two long arguments must be 
//representable in two's complement of a <bitwidth> bits number.
long long sat_add(long long operand1, long long operand2, int bitwidth)
{
	//LSB is set
	long long mask = 1;
	//check the number of bits
	validateBitWidthRange(bitwidth);
	//shift LSB to MSB
	mask = mask << (bitwidth - 1);
	//add the two operands normally
	long long result = operand1 + operand2;

	//***if the two operands are negative, but the result of addition is positive
	//that implies negative overflow***

	//if the two operands are negative
	if((mask & operand1) && (mask & operand2))
	{
		//and if the result of addition is positive 
		if(!(result & mask))
		{
			//return the largest negative number represent able in a <bitwidth> number
			return signed_min(bitwidth);
		}
	}

	//if the two operands are positive 
	if(!(mask & operand1) && !(mask & operand2))//both numbers positive
	{
		//and if the result of addition is negative
		if(result & mask)
		{
			//return the largest number represent able in a <bitwidth> number.
			return signed_max(bitwidth);
		}
	}

	//if none of the if above statements execute that means neither 
	//negaive or positive overflow happens once the two numbers are added.
	//in that case simply return the result of the normal addition.

	//return the result of the normal addition.
	return result;

}

//prints the negative and positive limits of the given <bitwidth> bit two's complement
//this function will not work if the bitwidth is greater than 32. This is because %x only works 
//for the data type int which is made of 32 bits.
void printRange(int bitwidth)
{
	printf("%d-bit signed integer range\nmin: %lld\t0x%016llX\nmax:  %lld\t0X%016llx\n", bitwidth, signed_min(bitwidth), signed_min(bitwidth), signed_max(bitwidth), signed_max(bitwidth));
}