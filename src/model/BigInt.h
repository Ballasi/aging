#ifndef BIG_INT_H_
#define BIG_INT_H_

/* !
   \file BigInt.h
   \brief An alias to GMP's mpz_class
   \author aging-team
 */

#include <gmpxx.h>
#include <string>

/*!
 * \class BigInt
 * \brief A class to represent arbitrary sized integers
 *
 * It is a simple alias to GMP's mpz_class
 * and thus provides many highly optimized operations
 */
typedef mpz_class BigInt;

/*!
 * \brief Converts a bigint to a reasonably sized string
 *
 * Chooses between classical and exponent notation to prevent
 * the notation to exceed 12 characters in length
 *
 * \param bigint: a bigint to turn to a string
 * \return a string of length < 12 representing the bigint
 */
static std::string bigint_to_str(const BigInt &bigint) {
  std::string str_i = bigint.get_str();
  std::string result;
  if (str_i.size() <= 12) {
    return str_i;
  } else {
    result += str_i[0];
    result += '.';
    result += str_i[1];
    result += str_i[2];
    result += 'e';
    result += std::to_string(str_i.size() - 1);
  }
  return result;
}

#endif // BIG_INT_H_
