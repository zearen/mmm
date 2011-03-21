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
        char * getName();
        NBT_StringHolder getNameHolder();
        void setName(const string &name);
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
        NBT_BYTE * parseTag(NBT_BYTE * data, bool named = true);
        NBT_StringHolder * getPayload(void);
        
};

template <class E, TagType tagVal>
class TAG_Atom : public NBT_Tag {
    private:
        E payload;
    protected:
    public:
        
        TAG_Atom() : NBT_Tag(tagVal)
        {
            return;            
        }
        
        NBT_BYTE * parseTag(NBT_BYTE * data, bool named = true)
        {
            // get name
            /*
            len t = TAG_Short::Parse(data);
            start += sizeof(NBT_SHORT);
            // etc etc...
            */
            if(named)
            {
                setName(TAG_String::Parse(data));
                data += getNameHolder().length + sizeof(NBT_SHORT);
            }
            // get payload
            payload = Parse(data);            
            data += sizeof(E);
            return data;
            
        }
        
        static E Parse(NBT_BYTE * data)
        {
            E t = *((E*)data);
            if(Utilities::IsLittleEndian)
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


typedef TAG_Atom<NBT_BYTE, TAGTYPE_BYTE> TAG_Byte;
typedef TAG_Atom<NBT_DOUBLE, TAGTYPE_DOUBLE> TAG_Double;
typedef TAG_Atom<NBT_FLOAT, TAGTYPE_FLOAT> TAG_Float;
typedef TAG_Atom<NBT_INT, TAGTYPE_INT> TAG_Int;
typedef TAG_Atom<NBT_LONG, TAGTYPE_LONG> TAG_Long;
typedef TAG_Atom<NBT_SHORT, TAGTYPE_SHORT> TAG_Short;




typedef TAG_Atom<NBT_BYTE *, TAGTYPE_BYTE_ARRAY> TAG_Byte_Array;

class TAG_List : public NBT_Tag {
    private:
        TagType _itemType;
        NBT_INT _numberOfElements;
        deque<NBT_Tag *> items;
        
    public:
        TAG_List();
        ~TAG_List();
        NBT_Tag *operator [] (int index);
        NBT_INT size();
        TagType getItemType();
        
        NBT_BYTE * parseTag(NBT_BYTE * data, bool named = true);

};

class FieldNotFoundError {};
class TAG_Compound : public NBT_Tag {
    private:
        deque<NBT_Tag*> fields;
    public:
        TAG_Compound();
        ~TAG_Compound();
        void add(NBT_Tag *newItem);
        
        NBT_BYTE * parseTag(NBT_BYTE * data, bool named = true);
        
        NBT_Tag *operator[] (char * name);
        NBT_INT size();
        char ** listTagNames();
                
        
};

//NBT_Tag *parseNBT(byte *NBT_Data);

#endif
