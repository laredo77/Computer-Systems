// 311547087 Itamar Laredo
#include <stdio.h>

/*
 * The function checks whether the computer is working in the big endian
 * or little endian.The function uses a union structure that defines int
 * variable i and char array c in size of long it.
 * In the body of the function, it initialize val i = 1 and then checks
 * whether the first place of the array is placed with 1 or 0.
 * if 1, we know the memory is kind of little endian, otherwise big endian.
*/
int is_big_endian() {
    // Define a union structure
    union {
        long int i;
        char c[sizeof(long int)];
    } x;

    x.i = 1; // Initizlize x.i to 1
    if(x.c[0] == 1)
        return 0; // Case little endian
    else 
        return 1; // Case big endian
    
}
/* The function gets two numbers in the hexa representation, takes the first half
 * of the first number's and the last half's of the second number and concatenate
 * it to a new number. The function works with shifts to move the bits.
 * In order to get the first half of the number use ((sizeof(x) * 8) / 2) that
 * equal to 32 bits right. Then we fix the number in te memory place by shifting the 
 * other size back. The case of the other number is the oposite to the first number.
 * last thing is to add x and y and return the result.
 * Important thing to know when working with shifting, the result in big and little endian
 * is the same.
*/
unsigned long merge_bytes(unsigned long x, unsigned long int y) {

    unsigned long shifted_x = x >> ((sizeof(x) * 8) / 2); // Shifting x to it first half
    unsigned long shifted_y = y << ((sizeof(y) * 8) / 2); // Shifting y to it second half
    unsigned long fixed_shifted_x = shifted_x << ((sizeof(x) * 8) / 2); // Fixing the first half
    unsigned long fixed_shifted_y = shifted_y >> ((sizeof(y) * 8) / 2); // Fixing the second half

    unsigned long result = fixed_shifted_x + fixed_shifted_y; // addition fixed x and fixed y
    return result;
}

/*
 * Similarly, in this function we work with shifts on the bytes.
 * First it find where the spot of b should be in memory. Similar to the previous section,
 * The function do a shift operation on the memory of x to get only the prefix,
 * then it will concatenate to the prefix var b and finally it will concatenate
 * the rest of the x number. and return the number.
*/
unsigned long put_byte(unsigned long x, unsigned char b, int i) {

    int len = sizeof(x) * 8; // The bits length of x
    int spot = sizeof(x) * i; // Where b should be spotted by bits

    unsigned long shifted_x = x >> (len - spot); // Shifting to the place
    shifted_x = shifted_x << sizeof(b) * 8; // Fixing the shifting

    unsigned long sx_concatenated = shifted_x + b; // concatenate b to the memory
    sx_concatenated = sx_concatenated << (len - spot - sizeof(b) * 8);

    unsigned long rest_of_x = x << (spot + sizeof(b) * 8); // concatenate the rest to the memory
    rest_of_x = rest_of_x >> (spot + sizeof(b) * 8); 

    unsigned long result = sx_concatenated + rest_of_x; 
    return result;
}