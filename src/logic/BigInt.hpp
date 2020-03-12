#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include <string>
#include <gmpxx.h>

typedef mpz_class BigInt;

static std::string bigint_to_str(BigInt i)
{
    std::string str_i = i.get_str();
    std::string result;
    if(str_i.size() <= 20){
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

#endif // BIG_INT_HPP
