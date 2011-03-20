#ifndef _UTIL_H
#define _UTIL_H


#include <string>
using namespace std;

class Utilities
{
      public:
            
      static bool IsLittleEndian;
      static void DetectBigEndian(void);
            
      static bool FileExists(string fileName);
      
};

// Fix endian crap

template <class E>
E endianSwap(E val) {
    union {
        E e;
        char c[sizeof(E)];
    } uVal;
    uVal.e = val;
    
    for(int i = 0; i < sizeof(E) >> 1; i++)
        swap(uVal.c[i], uVal.c[sizeof(E) - i - 1]);
    
    return uVal.e;
}

#endif
