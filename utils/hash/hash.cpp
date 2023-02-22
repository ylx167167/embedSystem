#include "hash.hpp"

// #include <functional>

// // BKDR Hash Function
// unsigned int BKDRHash(char *str) {
//   unsigned int seed = 131;  // 31 131 1313 13131 131313 etc..
//   unsigned int hash = 0;

//   while (*str) {
//     hash = hash * seed + (*str++);
//   }

//   // return (hash & 0x7FFFFFFF);
//   return hash;
// }

unsigned int make_hash(const char *str) {
  if (!str) {
    return 0;
  }
  
  unsigned int seed = 131;  // 31 131 1313 13131 131313 etc..
  unsigned int hash = 0;

  while (*str) {
    hash = hash * seed + (*str++);
  }

  return (hash & 0x7FFFFFFF);
}

unsigned int make_hash(const std::string &str) {
  return make_hash(str.c_str());
}
