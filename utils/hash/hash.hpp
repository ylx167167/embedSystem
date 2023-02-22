#ifndef __BKDR_HASH_HPP
#define __BKDR_HASH_HPP

#include <string>

// unsigned int BKDRHash(char *str);

unsigned int make_hash(const char *str);
unsigned int make_hash(const std::string &str);

#endif  //__BKDR_HASH_HPP
