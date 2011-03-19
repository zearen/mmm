#ifndef _nbt_tree
#define _nbt_tree

#include <string.h>
#include <deque>
#include "nbt_defines.h"



using namespace std;


class NBT_Tag {
    private:
        NBT_StringHolder *_name;
        TagType _type;
    public:
        NBT_Tag(NBT_StringHolder *name, TagType tagType);
        ~NBT_Tag();
        NBT_StringHolder* GetName();
        void SetName(NBT_StringHolder *name);
        TagType getType();
        bool isType(TagType tagType2);
};

class TAG_End : NBT_Tag
{
    public:
    TAG_End();
};

template <class E, TagType tagVal>
class TAG_Atom : public NBT_Tag {
    private:
        E payload;
    public:
        TagType getType() {
            return tagVal;
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
typedef TAG_Atom<NBT_BYTE *, TAGTYPE_BYTE_ARRAY> TAG_Byte_Array;
typedef TAG_Atom<NBT_DOUBLE, TAGTYPE_DOUBLE> TAG_Double;
typedef TAG_Atom<NBT_FLOAT, TAGTYPE_FLOAT> TAG_Float;
typedef TAG_Atom<NBT_INT, TAGTYPE_INT> TAG_Int;
typedef TAG_Atom<NBT_LONG, TAGTYPE_LONG> TAG_Long;
typedef TAG_Atom<NBT_SHORT, TAGTYPE_SHORT> TAG_Short;
typedef TAG_Atom<NBT_StringHolder, TAGTYPE_STRING> TAG_String;

class TAG_List : public NBT_Tag {
    private:
        TagType itemType;
        int len;
        NBT_Tag **items;
    public:
        TAG_List(char* newName, NBT_BYTE itemType, int length);
        ~TAG_List();
        NBT_Tag *operator [] (int index);
        int length();
};

class FieldNotFoundError {};
class TAG_Compound : public NBT_Tag {
    private:
        deque<NBT_Tag> fields;
    public:
        TAG_Compound();
        ~TAG_Compound();
        NBT_Tag *operator[] (char *field);
        void add(NBT_Tag *newItem);
};

//NBT_Tag *parseNBT(byte *NBT_Data);

#endif
