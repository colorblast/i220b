#include "hamming.h"
#include <limits.h>
#include <assert.h>

/**
  All bitIndex'es are numbered starting at the LSB which is given index 1

  ** denotes exponentiation; note that 2**n == (1 << n)
*/

/** Return bit at bitIndex from word. */
static inline unsigned
get_bit(HammingWord word, int bitIndex)
{
  assert(bitIndex > 0);
  int length = sizeof(HammingWord) * CHAR_BIT;
  unsigned shifted = ((word >> (bitIndex - 1)) << (length - 1)) >> (length - 1);
  return shifted;
}

/** Return word with bit at bitIndex in word set to bitValue. */
static inline HammingWord
set_bit(HammingWord word, int bitIndex, unsigned bitValue)
{
  assert(bitIndex > 0);
  assert(bitValue == 0 || bitValue == 1);
  unsigned long long switched = word & ~(1ULL << (bitIndex - 1));
  switched |= (bitValue << (bitIndex - 1));

  return switched;
}

/** Given a Hamming code with nParityBits, return 2**nParityBits - 1,
 *  i.e. the max # of bits in an encoded word (# data bits + # parity
 *  bits).
 */
static inline unsigned
get_n_encoded_bits(unsigned nParityBits)
{
  unsigned int i;
  unsigned int ret = 1;
  for (i = 0; i < nParityBits; i++) {
    ret *= 2;
  }
  return ret - 1;
}

/** Return non-zero if bitIndex indexes a bit which will be used for a
 *  Hamming parity bit; i.e. the bit representation of bitIndex
 *  contains only a single 1.
 */
static inline int
is_parity_position(int bitIndex)
{
  assert(bitIndex > 0);
  unsigned int number = (unsigned int) bitIndex;
  unsigned ones = 0;
  while (number > 0) {
    number &= number - 1;
    ones++;
  }
  return ones == 1;
}

/** Return the parity over the data bits in word specified by the
 *  parity bit bitIndex.  The word contains a total of nBits bits.
 *  Equivalently, return parity over all data bits whose bit-index has
 *  a 1 in the same position as in bitIndex.
 */
static int
compute_parity(HammingWord word, int bitIndex, unsigned nBits)
{
  assert(bitIndex > 0);
  int res_flag = 0;
  int res;
  for (unsigned i = 1; i < nBits + 1; i++) {
    if (!is_parity_position(i)) {
      if (get_bit(i, bitIndex) == 1) {
        if (res_flag) {
          unsigned val = get_bit(word, i);
          res ^= val;
        } else {
          res = get_bit(word, i);
          res_flag = 1;
        }
      }
    }
  }
  return res;
}

/** Encode data using nParityBits Hamming code parity bits.
 *  Assumes data is within range of values which can be encoded using
 *  nParityBits.
 */
HammingWord
hamming_encode(HammingWord data, unsigned nParityBits)
{
  HammingWord encoded = 0;
  unsigned nBits = get_n_encoded_bits(nParityBits);

  int parity_bits = 0;
  // load the data into the correct positions for encoded
  for (unsigned int i = 1; i < nBits+1; i++) {
    if (is_parity_position(i)) {
      parity_bits++;
    } else {
      int val = get_bit(data, i - parity_bits);
      encoded = set_bit(encoded, i, val);
    }
  }

  // do parity bit stuff
  for (unsigned int i = 1; i < nBits+1; i++) {
    if (is_parity_position(i)) {
      parity_bits++;
      int pos = 0;
      unsigned int k;
      int count = 0;
      // compute parity of the indices
      for (k = 1; k < nBits + 1; k++) {
        if (get_bit(i, k) == 1) {
          pos = k;
        }
        count++;
      }
      int val = compute_parity(encoded, pos, count);
      encoded = set_bit(encoded, i, val);
    }
  }

  return encoded;
}

/** Decode encoded using nParityBits Hamming code parity bits.
 *  Set *hasError if an error was corrected.
 *  Assumes that data is within range of values which can be decoded
 *  using nParityBits.
 */
HammingWord
hamming_decode(HammingWord encoded, unsigned nParityBits,
                           int *hasError)
{
  unsigned int error_syndrome = 0;
  int error_flag = 0;

  HammingWord decoded = 0;

  unsigned nBits = get_n_encoded_bits(nParityBits);
  // do parity computations to find errors
  for (unsigned int i = 1; i < nBits + 1; i++) {
    if (is_parity_position(i)) {
      int pos = 0;
      unsigned int k;
      int count = 0;
      // compute parity of the indices
      for (k = 1; k < nBits + 1; k++) {
        if (get_bit(i, k) == 1) {
          pos = k;
        }
        count++;
      }
      int val = compute_parity(encoded, pos, count);
      if (val != get_bit(encoded, i)) {
        if (error_flag) {
          error_syndrome |= i;
        } else {
          error_syndrome = i;
          error_flag = 1;
        }
      }
    }
  }

  HammingWord fixed = encoded; // avoid mutating existing state
  // fix the syndrome error if present
  if (error_syndrome) {
    unsigned int swap = get_bit(encoded, error_syndrome);
    swap ^= 1ULL;
    fixed = set_bit(encoded, error_syndrome, swap);
    *hasError = 1;
  }

  // store in decoded
  int parity_bits = 0;
  for (unsigned int i = 1; i < nBits + 1; i++) {
    if(!is_parity_position(i)) {
      decoded = set_bit(decoded, i - parity_bits, get_bit(fixed, i));
    } else {
      parity_bits++;
    }
  }

  return decoded;
}
