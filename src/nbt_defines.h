#ifndef byte
#define byte unsigned char
#endif


#ifndef _nbt_defines
#define _nbt_defines

#define NBT_BUFFER_SIZE 1048576 // how many bytes max? = 1mb chunk max...


// 8 bits
#define NBT_BYTE unsigned char

#define NBT_BYTE_ARRAY unsigned char

// 64 bits, big endian, IEEE 754-2008, binary64
#define NBT_DOUBLE double

// 32 bits, big endian, IEEE 754-2008, binary32
#define NBT_FLOAT float

// 16 bits, big endian
#define NBT_SHORT short signed int

// 32 bits, bin endian
#define NBT_INT signed int

// 64 bits, big endian
#define NBT_LONG long long signed int

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
    

inline short int sizeOfTagType(TagType t) {
    short int r = 0;
    switch(t)
    {
        case TAGTYPE_BYTE: r = 1; break;
        case TAGTYPE_SHORT: r = 2; break;
        case TAGTYPE_LONG: r = 8; break;
        case TAGTYPE_DOUBLE: r = 8; break;
        case TAGTYPE_FLOAT: r = 4; break;
        case TAGTYPE_INT: r = 4; break;
        default: r = 0; break;
    }
    return r;
}


#endif
