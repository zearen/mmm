#ifndef byte
#define byte unsigned char
#endif


#ifndef _nbt_defines
#define _nbt_defines

// 8 bits
#define NBT_BYTE unsigned char

#define NBT_BYTE_ARRAY unsigned char

// 64 bits, big endian, IEEE 754-2008, binary64
#define NBT_DOUBLE double

// 32 bits, big endian, IEEE 754-2008, binary32
#define NBT_FLOAT float

// 16 bits, big endian
#define NBT_SHORT short int

// 32 bits, bin endian
#define NBT_INT int

// 64 bits, big endian
#define NBT_LONG long int

    typedef struct NBT_StringHolder
    {
        NBT_SHORT length;
        char * value;
    };
    
typedef enum TagType {
    TAGTYPE_BYTE       = 1,
    TAGTYPE_BYTE_ARRAY = 7,
    TAGTYPE_COMPOUND   = 10,
    TAGTYPE_DOUBLE     = 6,
    TAGTYPE_END        = 0,
    TAGTYPE_FLOAT      = 5,
    TAGTYPE_INT        = 3,
    TAGTYPE_LIST       = 9,
    TAGTYPE_LONG       = 4,
    TAGTYPE_SHORT      = 2,
    TAGTYPE_STRING     = 8
    };



#endif
