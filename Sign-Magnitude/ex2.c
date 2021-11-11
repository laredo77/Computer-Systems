// Itamar Laredo
#include <stdio.h>
#include <limits.h>

typedef struct {
    int num;
} magnitude;

/*
 * Get magnitude representation of a int 4 byte number.
 */
int getMagnitudeRepresentation(int a) {
    int ret_value;
    int sign = 0;
    if (a < 0) {
        a *= -1;
        sign = (1 << 31);
    }
    ret_value = (sign | a);
    return ret_value;
}

/*
 * Gets two magnitude numbers and return the sum of both of them.
 * Each number, convert to magnitude representation whit bit for
 * sign and the rest of the bits to value.
 * If the number is negative, The function add 2^31 to the number
 * in purpose to make the sign bit 0, then multiply in -1.
 */
magnitude add(magnitude a, magnitude b) {
    magnitude ret;
    int mag_a = a.num;
    int mag_b = b.num;
    int overflow_flag = 0;
    if (a.num < 0) {
        mag_a += 2147483648; // 2^31
        mag_a *= -1;
    }
    if (b.num < 0) {
        mag_b += 2147483648; // 2^31
        mag_b *= -1;
    }
    // check overflow
    if ((mag_b > 0) && (mag_a > INT_MAX - mag_b)) {
        overflow_flag = 1;
    }
    int res = mag_a + mag_b;
    if (res < 0) {
        res += 2147483648;
        res *= -1;
    }
    // deal with overflow i.e if the res is negative keep the sign 1 otherwise 0.
    if (overflow_flag) {
        if ((a.num + b.num) < 0 ) {
            res |= 1UL << 31;
        }
        else
            res *= -1;
    }
    ret.num = res;
    return ret;
}

/*
 * Gets two magnitude numbers and return the first minute the second,
 * magnitude representation.
 * Each number, convert to magnitude representation whit bit for
 * sign and the rest of the bits to value.
 * If the number is negative, The function add 2^31 to the number
 * in purpose to make the sign bit 0, then multiply in -1.
 */
magnitude sub(magnitude a, magnitude b) {
    magnitude ret;
    int mag_a = a.num;
    int mag_b = b.num;
    int res;
    int overflow_flag = 0;
    if (a.num < 0) {
        mag_a += 2147483648; // 2^31
        mag_a *= -1;
    }
    if (b.num < 0) {
        mag_b += 2147483648; // 2^31
        mag_b *= -1;
    }
    // check overflow
    if ((mag_b < 0) && (mag_a > INT_MAX + mag_b))  {
        overflow_flag = 1;
    }
    res = mag_a - mag_b;
    if (res < 0) {
        res += 2147483648;
        res *= -1;
    }
    // deal with overflow i.e if the res is negative keep the sign 1 otherwise 0.
    if (overflow_flag) {
        if ((a.num - b.num) < 0 ) {
            res |= 1UL << 31;
        }
        else
            res *= -1;
    }
    ret.num = res;
    return ret;
}
/*
 * Gets two magnitude numbers and return the first multiply the second,
 * magnitude representation.
 * Each number, convert to magnitude representation whit bit for
 * sign and the rest of the bits to value.
 * If the number is negative, The function add 2^31 to the number
 * in purpose to make the sign bit 0, then multiply in -1.
 */
magnitude multi(magnitude a, magnitude b) {
    magnitude ret;
    int mag_a = a.num;
    int mag_b = b.num;
    int overflow_flag = 0;
    if (a.num < 0) {
        mag_a += 2147483648; // 2^31
        mag_a *= -1;
    }
    if (b.num < 0) {
        mag_b += 2147483648; // 2^31
        mag_b *= -1;
    }
    // check overflow
    if ((mag_b > 0) && (mag_a > INT_MAX - mag_b)) {
        overflow_flag = 1;
    }
    int res = mag_a * mag_b;
    if (res < 0) {
        res += 2147483648;
        res *= -1;
    }
    // deal with overflow i.e if the res is negative keep the sign 1 otherwise 0.
    if (overflow_flag) {
        if ((a.num * b.num) < 0 ) {
            res |= 1UL << 31;
        }
        else
            res *= -1;
    }
    ret.num = res;
    return ret;
}
/*
 * First this function get the magnitude representation of the numbers,
 * Then it compare the numbers. If all the bits are equal, it return true
 * and false otherwise.
 */
int equal(magnitude a, magnitude b) {
    int a_bit_representation = getMagnitudeRepresentation(a.num);
    int b_bit_representation = getMagnitudeRepresentation(b.num);
    if (a_bit_representation == b_bit_representation)
        return 1;
    return 0;
}
/*
 * Convert the two magnitude number and check if the first
 * parameter is greater then the second. If so, return true
 * and false otherwise.
 */
int greater(magnitude a, magnitude b) {
    int a_bit_representation = getMagnitudeRepresentation(a.num);
    int b_bit_representation = getMagnitudeRepresentation(b.num);
    if (a_bit_representation > b_bit_representation)
        return 1;
    return 0;
}

