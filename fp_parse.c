/* Randy Rollofson, CS 201, Program 2 */
/* fp_parser.c */

/* This programimplements an IEEE floating point parserfor an arbitrary number of
bit settings. The program takes 3 arguments from the command line: the number of
fraction bits, the number of exponent bits, and the hexadecimal value to be parsed.
*/


#include <stdio.h>
#include <math.h>


typedef unsigned int *word_pointer;


/* Function prototypes */
float get_final_frac(word_pointer start, int n);
float get_frac(int frac_shift, int n);
int calculate_sign_bit(word_pointer start, int n, int k);
int get_sign_bit(int sign_shift, int n, int k);
int check_hex(int hex, int n, int k);


/*
void show_bits(word_pointer start)
{
        unsigned int i,j;
	unsigned int mask=0x80000000;

	i=start[0];	
	for (j = 0; j < 32; j++){
		if (i & mask)
			printf("1");
		else
			printf("0");
		if (j==0 || j==8)
			printf(" ");
		mask >>= 1;
	}
        printf("\n");
}

void show_float(int x)
{
        show_bits((word_pointer) &x);
}
*/


/* Calculates the value of the fraction */
float get_final_frac(word_pointer start, int n)
{
    float frac = 0.0;
    unsigned int i, j;
    unsigned int mask = 0x80000000;
    
    i = start[0];
    mask >>= (32 - n);
    for (j = 0; j < n; ++j)
    {
        if (i & mask)
            frac += 1/(pow(2, (j + 1)));
        mask >>= 1;
    }
    return frac;
}


/* Sets up get_final_frac */
float get_frac(int frac_shift, int n)
{
    float frac = get_final_frac((word_pointer) &frac_shift, n);
    return frac;
}


/* Determines the value of the sign bit */
int calculate_sign_bit(word_pointer start, int n, int k)
{
    unsigned int i = start[0];
    unsigned int mask = 0x80000000;

    i = start[0];
    mask >>= 31;
    if (i & mask)
        return -1;
    return 0;
}


/* Sets up calculate_sign_bit */
int get_sign_bit(int sign_shift, int n, int k)
{
    signed int sign = calculate_sign_bit((word_pointer) &sign_shift, n, k);
    return sign;
}


/* Checks to see if the hex value uses more bits than (sign + exp + frac) */
int check_hex(int hex, int n, int k)
{
    unsigned int value = hex >> (n + k + 1);
    if (value == 0)
        return 1;
    return 0;   
}



int main(int argc, char * argv[])
{
        if (argc != 4)
            printf("Usage: fp_parse <# of frac_bits><# of exp_bits><hex_to_convert>\n\n");
        else
        {
            unsigned int n, k, hex;  /* Variables that store the number of exp bits, frac bits, and hex value */
            float value = 0.0;  /* Holds the final calculated value */
            sscanf(argv[1], "%d", &n);
            sscanf(argv[2], "%d", &k);
            sscanf(argv[3], "%x", &hex);
            if (n < 2 || n > 10)
                printf("Illegal number of fraction bits (%d). Should be between 2 and 10.\n\n", n);
            else if (k < 3 || k > 5)
                printf("Illegal number of exponent bits (%d). Should be between 3 and 5.\n\n", k);
            else if (!(check_hex(hex, n, k)))
                printf("Incompatible hex value.\n\n");    
            else
            {   
                unsigned int frac_shift = hex << (32 - n) >> (32 - n);  /* Isolates the fraction bits */
                float frac = get_frac(frac_shift, n);
                unsigned int exp = hex << (32 - (n + k)) >> (32 - k);   /* Isolates the exponent bits */ 
                unsigned int bias = (pow(2, (k - 1))) - 1;
                float M;  /* Variable for the significand */
                signed int E;
                if (exp == 0)
                {
                    E = 1 - bias;
                    M = frac;
                }
                else
                {
                    E = exp - bias;
                    M = 1 + frac; 
                }
                    
                int sign_shift = hex << (32 - (n + k + 1)) >> 31;  /* Isolates the sign bit */
                unsigned int sign = get_sign_bit(sign_shift, n, k);
                
                if (exp == ((1 << k) - 1))
                {
                    if (frac == 0)
                    {
                        if (sign == -1)
                            printf("-inf\n\n");
                        else
                            printf("+inf\n\n");      
                    }
                    else
                        printf("NaN\n\n");
                }
                else
                {        
                    if (sign == -1)
                        value = -1 * M * (pow(2, E));
                    else
                        value = M * (pow(2, E));
                    /*     
                    printf("n: %d\n", n);
                    printf("k: %d\n", k);
                    printf("frac: %f\n", frac);
                    printf("exp: %d\n", exp);
                    printf("bias: %d\n", bias);
                    printf("sign: %d\n", sign);
                    printf("M: %f\n", M);
                    printf("E: %d\n", E);
                    */
                    printf("%f\n\n", value);
                }
            }
        }

	return 0;
}
