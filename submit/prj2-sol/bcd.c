#include "bcd.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>

/* Returns a BCD digit for a given BCD. */
Bcd get_bcd_digit(Bcd in, int index) {
  unsigned mask = ((1 << BCD_BITS) - 1) << (index * BCD_BITS);
  Bcd ret = (in & mask);
  for (int i = 0; i < (index * BCD_BITS); i++) {
    ret /= 2;
  }
  return ret; /// 2;
  //return in >> ((MAX_BCD_DIGITS - index - 1) * BCD_BITS) << (index * BCD_BITS) >> (index * BCD_BITS);
}

/** Return BCD encoding of binary (which has normal binary representation).
 *
 *  Examples: binary_to_bcd(0xc) => 0x12;
 *            binary_to_bcd(0xff) => 0x255
 *
 *  If error is not NULL, sets *error to OVERFLOW_ERR if binary is too\
 *  big for the Bcd type, otherwise *error is unchanged.
 */
Bcd
binary_to_bcd(Binary value, BcdError *error)
{
  // grabs the last digit from the bcd and formats appropriately
  Binary copy = value; // avoid side effects
  Bcd accum = 0;
  unsigned int length = 1;
  while (copy) {
    length++;
    if (length - 1 > MAX_BCD_DIGITS) {
      *error = OVERFLOW_ERR;
    }
    accum |= ((copy % 10) << ((length - 2) * BCD_BITS));
    copy /= 10;
  }
  return accum;
}

/** Return binary encoding of BCD value bcd.
 *
 *  Examples: bcd_to_binary(0x12) => 0xc;
 *            bcd_to_binary(0x255) => 0xff
 *
 *  If error is not NULL, sets *error to BAD_VALUE_ERR if bcd contains
 *  a bad BCD digit.
 *  Cannot overflow since Binary can represent larger values than Bcd
 */
Binary
bcd_to_binary(Bcd bcd, BcdError *error)
{
  Binary ret = 0;
  int first_int_flag = 1;
  printf("bcd: %llu\n", bcd);
  // for(int i = 0; i < MAX_BCD_DIGITS; i++) { // account for endianness
  for(int i = MAX_BCD_DIGITS - 1; i >= 0; i--) {
    Bcd digit = get_bcd_digit(bcd, i);
    printf("what is i: %d\n", i);
    printf("hello %d\n", digit);
    printf("hex %04x\n", digit);
    if (digit >= 10) {
      *error = BAD_VALUE_ERR;
    } else {
      if (first_int_flag) {
        ret = digit;
        first_int_flag = 0;
      } else {
        ret *= 10;
        ret += digit;
      }
    }
    printf("ret: %llu\n", ret);
  }
  printf("-----\n");
  return ret;
}

/** Return BCD encoding of decimal number corresponding to string s.
 *  Behavior undefined on overflow or if s contains a non-digit
 *  character.  Sets *p to point to first non-digit char in s.
 *  Rougly equivalent to strtol().
 *
 *  If error is not NULL, sets *error to OVERFLOW_ERR if binary is too
 *  big for the Bcd type, otherwise *error is unchanged.
 */
Bcd
str_to_bcd(const char *s, const char **p, BcdError *error)
{
  //@TODO
  return 0;
}

/** Convert bcd to a NUL-terminated string in buf[] without any
 *  non-significant leading zeros.  Never write more than bufSize
 *  characters into buf.  The return value is the number of characters
 *  written (excluding the NUL character used to terminate strings).
 *
 *  If error is not NULL, sets *error to BAD_VALUE_ERR is bcd contains
 *  a BCD digit which is greater than 9, OVERFLOW_ERR if bufSize bytes
 *  is less than BCD_BUF_SIZE, otherwise *error is unchanged.
 */
int
bcd_to_str(Bcd bcd, char buf[], size_t bufSize, BcdError *error)
{
  //@TODO
  return 0;
}

/** Return the BCD representation of the sum of BCD int's x and y.
 *
 *  If error is not NULL, sets *error to to BAD_VALUE_ERR is x or y
 *  contains a BCD digit which is greater than 9, OVERFLOW_ERR on
 *  overflow, otherwise *error is unchanged.
 */
Bcd
bcd_add(Bcd x, Bcd y, BcdError *error)
{
  //@TODO
  return 0;
}

/** Return the BCD representation of the product of BCD int's x and y.
 *
 * If error is not NULL, sets *error to to BAD_VALUE_ERR is x or y
 * contains a BCD digit which is greater than 9, OVERFLOW_ERR on
 * overflow, otherwise *error is unchanged.
 */
Bcd
bcd_multiply(Bcd x, Bcd y, BcdError *error)
{
  //@TODO
  return 0;
}
