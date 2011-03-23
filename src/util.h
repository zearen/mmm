#ifndef _UTIL_H
#define _UTIL_H
#include <exception>


#include <string>
#include "NBTDefines.h"
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

class FieldNotFoundError : public exception {
    
        virtual const char* what() const throw()
        {
          return "Field not found!";
        }
        
};
    
class NullException : public exception  {
    
        virtual const char* what() const throw()
        {
          return "Something was null!";
        }
        
};

class TypeMismatch : public exception  {
    protected:
        TagType expected;
        TagType got;
    public:
        TypeMismatch();
        TypeMismatch(TagType exp, TagType gotted);
        
        virtual const char* what() const throw()
        {
          return "Types did not agree!";
        }
};
/*
typedef exception TypeMismatch;
typedef exception NullException;
typedef exception FieldNotFoundException;
typedef exception FileNotFoundException;
*/
#endif
