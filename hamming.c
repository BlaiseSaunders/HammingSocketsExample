#include <stdlib.h>
#include <stdio.h>
#include <string.h>



#define CHECK_LEN 11
#define GEN_LEN 7
#define ERR_DIG 9


/* Setup a simple bool */
typedef enum
{
	FALSE,
	TRUE
} bool;


void print_help(char *argv[])
{
	printf("Usage: %s [-g, -c] data\n", argv[0]);
	printf("\t-g, generate a hamming code based off 'data' (data should be 7 bits)\n");
	printf("\t-c, check a hamming code based off 'data' (data should be 11 bits)\n");
	/*printf("\n");*/

}


bool validate_input(char *argv[])
{
	char *tmp = argv[2]; /* For validating the 'data' */


	/* Make sure the data is the right length */
	if (argv[1][1] == 'g' && strlen(tmp) != GEN_LEN)
		return FALSE;
	if (argv[1][1] == 'c' && strlen(tmp) != CHECK_LEN)
		return FALSE;


	/* Iterate through the 'string' until we hit a null pointer (end of 'string') */
	while (*tmp != '\0')
	{
		/* Check to see if the char is something it shouldn't be */
		if (*tmp != '1' && *tmp != '0')
			return FALSE;
		tmp++;
	}

	return TRUE;
}


int* convert_to_int_array(char *data)
{
	int i, *arr = malloc(sizeof (int) * CHECK_LEN); /* CHECK_LEN was used because both
	                                                   functions need an array big enough
	                                                   to store a complete hamming code */

	if (arr == NULL)
	{
		printf("Error: Out of Memory");
		exit(EXIT_FAILURE); /* Save wasted code propogating error for niche case*/
	}

	/* Iterate through the 'string' until we hit a null pointer (end of 'string') */
	for (i = 0; data[i] != '\0'; i++)
		arr[i] = data[i] - '0';

	/* Fill the rest of the array with an easily recognisable error digit */
	for (; i < CHECK_LEN; i++)
		arr[i] = ERR_DIG;

	return arr;

}

void pretty_dump_int_array(int *arr)
{
	int i;

/*	printf("+--");
	for (i = 0; i < CHECK_LEN-2; i++)
		printf("--");
	printf("-+\n");*/
	printf("+--------------------------------+\n"
	       "|r1|r2|D1|r3|D2|D3|D4|r4|D5|D6|D7|\n"
	       "+--------------------------------+\n");
	for (i = 0; i < CHECK_LEN; i++)
		printf("| %d", arr[i]);
	printf("|\n");
	printf("+--");
	for (i = 0; i < CHECK_LEN-2; i++)
		printf("---");
	printf("---+\n");
}

void dump_int_array(int *arr)
{
	int i;

	for (i = 0; i < CHECK_LEN; i++)
		printf("%d", arr[i]);
	printf("\n");
}

void dump_int_array_d_bits(int *arr)
{
	printf("%d", arr[2]);
	printf("%d", arr[4]);
	printf("%d", arr[5]);
	printf("%d", arr[6]);
	printf("%d", arr[8]);
	printf("%d", arr[9]);
	printf("%d", arr[10]);
	printf("\n");
}


void generate_hamming(char *data)
{
	int *arr = convert_to_int_array(data);

	/* Move elements to final location */

	/* Conversion table */
	/* +--------------------------------+
	 * |D1|D2|D3|D4|D5|D6|D7|  |  |  |  |
	 * +--------------------------------+
	 * | 0| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|
	 * +--------------------------------+
	 * |r1|r2|D1|r3|D2|D3|D4|r4|D5|D6|D7|
	 * +--------------------------------+
	 */

	/*pretty_dump_int_array(arr);*/


	arr[10] = arr[6];
	arr[9] = arr[5];
	arr[8] = arr[4];
	arr[6] = arr[3];
	arr[5] = arr[2];
	arr[4] = arr[1];
	arr[2] = arr[0];

	/* D1 D3 D5 D7 */
	/*r1*/ arr[0] = (arr[2]+arr[4]+arr[6]+arr[8]+arr[10])%2; /* mod 2 to check if number is even */

	/* D2 D3 D6 D7 */
	/*r2*/ arr[1] = (arr[2]+arr[5]+arr[6]+arr[9]+arr[10])%2;

	/* D4 D5 D6 D7 */
	/*r3*/ arr[3] = (arr[4]+arr[5]+arr[6])%2;

	/* D8+ */
	/*r4*/ arr[7] = (arr[8]+arr[9]+arr[10])%2;;

	dump_int_array(arr);

}

void check_hamming(char *data)
{
	int *arr = convert_to_int_array(data);
	int re_comp[4]; /* For storing recomputed vars */
	int total_error = 0;

	/*pretty_dump_int_array(arr);*/


	/* Conversion table */
	/* +--------------------------------+
	 * |D1|D2|D3|D4|D5|D6|D7|  |  |  |  |
	 * +--------------------------------+
	 * | 0| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|
	 * +--------------------------------+
	 * |r1|r2|D1|r3|D2|D3|D4|r4|D5|D6|D7|
	 * +--------------------------------+
	 */

	/* D1 D3 D5 D7 */
	/*r1*/ re_comp[0] = (arr[2]+arr[4]+arr[6]+arr[8]+arr[10])%2;

	/* D2 D3 D6 D7 */
	/*r2*/ re_comp[1] = (arr[2]+arr[5]+arr[6]+arr[9]+arr[10])%2;

	/* D4 D5 D6 D7 */
	/*r3*/ re_comp[2] = (arr[4]+arr[5]+arr[6])%2;

	/* D8+ */
	/*r4*/ re_comp[3] = (arr[8]+arr[9]+arr[10])%2;


	/* Compare recomputed bits against recieved bits */
	if (re_comp[0] != arr[0] || re_comp[1] != arr[1] ||
	    re_comp[2] != arr[3] || re_comp[3] != arr[7])
	{
		fprintf(stderr, "Check failed!\n");

		if (re_comp[0] != arr[0])
			total_error += 1;
		if (re_comp[1] != arr[1])
			total_error += 2;
		if (re_comp[2] != arr[3])
			total_error += 4;
		if (re_comp[3] != arr[7])
			total_error += 8;

		fprintf(stderr, "Errored bit is %d!\n",
		       total_error);

		/* Attempt to correct the error */
		total_error--; /* For array adressing */
		arr[total_error] = (arr[total_error]+1)%2; /* Flip errored bit */
	}

	/*pretty_dump_int_array(arr);*/

	dump_int_array_d_bits(arr);
}


int main(int argc, char *argv[])
{
	/* Valid input will always be 3 arguments:
	 * 	prog name, choice, data
	 * And the second argument's second char should be 'g' or 'c'
	 */
	if (argc != 3 || (argv[1][1] != 'c' && argv[1][1] != 'g'))
	{
		printf("Error: Invalid input.\n\n");
		print_help(argv);
		return EXIT_FAILURE;
	}
	if (!validate_input(argv))
	{
		printf("Error: Invalid data.\n\n");
		print_help(argv);
		return EXIT_FAILURE;
	}

	/* Check the second char of the second argument to see what the user
	 * wants from the program this run */
	if (argv[1][1] == 'g')
		generate_hamming(argv[2]);
	else
		check_hamming(argv[2]);


	return EXIT_SUCCESS;
}
