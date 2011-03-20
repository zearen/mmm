#ifndef _nbt_tree
#define _nbt_tree

#include <string.h>
#include <deque>
#include "nbt_defines.h"
#include <iostream>
#include "util.h"

using namespace std;


class NBT_Tag {
    private:
        NBT_StringHolder _name;
        TagType _type;
        int _memSize;
    public:
        NBT_Tag(TagType tagType);
        NBT_Tag(NBT_StringHolder name, TagType tagType);
        ~NBT_Tag();
        NBT_StringHolder getName();
        void setName(NBT_StringHolder name);
        void setName(NBT_StringHolder *name);
        TagType getType();
        bool isType(TagType tagType2);
};

class TAG_End : NBT_Tag
{
    public:
    TAG_End();
};

// NOTE: Must be above TAG_Atom because it's referenced in template

class TAG_String : public NBT_Tag {
    private:
        NBT_StringHolder payload;    
    public:
        TAG_String();
        ~TAG_String();
        static NBT_StringHolder Parse(NBT_BYTE * data);
        NBT_BYTE * parseTag(NBT_BYTE * data);
        
};

template <class E, TagType tagVal>
class TAG_Atom : public NBT_Tag {
    private:
        E payload;
    public:
        
        TAG_Atom() : NBT_Tag(tagVal)
        {
            return;            
        }
        
        TagType getType() {
            return tagVal;
        }
        
        NBT_BYTE * parseTag(NBT_BYTE * data)
        {
            // get name
            /*
            len t = TAG_Short::Parse(data);
            start += sizeof(NBT_SHORT);
            // etc etc...
            */
            setName(TAG_String::Parse(data));
            data += getName().length + 2;
            
            // get payload
            payload = Parse(data);
            data += sizeof(E);
            return data;
            
        }
        
        static E Parse(NBT_BYTE * data)
        {
            E t = *((E*)data);
            if(!Utilities::IsBigEndianSystem())
            {
                t = endianSwap<E>(t);
            }
            
            return t;
        }
        
        // 
        TAG_Atom(char* newName, E newPayload) {
            NBT_Tag(newName, tagVal);
            setPayload(newPayload);
        }
            
        E getPayload() {
            return payload;
        }
        
        void setPayload(E newPayload) {
            payload = newPayload;
        }
};


// We may need a template specialization for TAG_String
typedef TAG_Atom<NBT_BYTE, TAGTYPE_BYTE> TAG_Byte;
typedef TAG_Atom<NBT_DOUBLE, TAGTYPE_DOUBLE> TAG_Double;
typedef TAG_Atom<NBT_FLOAT, TAGTYPE_FLOAT> TAG_Float;
typedef TAG_Atom<NBT_INT, TAGTYPE_INT> TAG_Int;
typedef TAG_Atom<NBT_LONG, TAGTYPE_LONG> TAG_Long;
typedef TAG_Atom<NBT_SHORT, TAGTYPE_SHORT> TAG_Short;




typedef TAG_Atom<NBT_BYTE *, TAGTYPE_BYTE_ARRAY> TAG_Byte_Array;

class TAG_List : public NBT_Tag {
    private:
        TagType itemType;
        NBT_INT _numberOfElements;
        byte * items;
        
    public:
        TAG_List();
        ~TAG_List();
        NBT_Tag *operator [] (int index);
        int length();
        
        NBT_BYTE * parseTag(NBT_BYTE * data);

};

class FieldNotFoundError {};
class TAG_Compound : public NBT_Tag {
    private:
        deque<NBT_Tag*> fields;
    public:
        TAG_Compound();
        ~TAG_Compound();
        NBT_Tag *operator[] (char *field);
        void add(NBT_Tag *newItem);
        NBT_BYTE * parseTag(NBT_BYTE * data);
};

//NBT_Tag *parseNBT(byte *NBT_Data);

#endif
