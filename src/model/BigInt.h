#ifndef BIG_INT_H_
#define BIG_INT_H_

#include <gmpxx.h>

typedef mpz_class BigInt;

static std::string bigint_to_str(BigInt i) {
  std::string str_i = i.get_str();
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
