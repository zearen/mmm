#ifndef _nbt_tree
#define _nbt_tree

#include <string.h>
#include <vector>
#include <deque>
#include "NBTDefines.h"
#include <iostream>
#include "util.h"
#include <exception>


using namespace std;



class NBT_Tag
{
    private:
        NBT_StringHolder _name;
        TagType _type;
        int _memSize;

    public:
        bool _isParsed; // yea i know, temporary, too lazy to make setParsed()

        NBT_Tag(TagType tagType);
        NBT_Tag(NBT_StringHolder name, TagType tagType);
        NBT_Tag(const char * name, TagType tagType);
        
        ~NBT_Tag();
        bool isParsed(void);
        char * getName();
        NBT_StringHolder getNameHolder();
        void setName(const string &name);
        void setName(NBT_StringHolder name);
        TagType getType();
        bool isType(TagType tagType2);
        
        void * getValue(NBT_Tag * t);
};


class TAG_Compound : public NBT_Tag {
    private:
        deque<NBT_Tag*> fields;
        
    public:
        TAG_Compound();
        TAG_Compound(const char * name);
        ~TAG_Compound();
        
        
        void add(NBT_Tag *newItem);
        
        TAG_Compound * parseTag(NBT_BYTE ** data, bool named = true);
        
        NBT_Tag *operator[] (const char * name);
        NBT_Tag *operator[] (NBT_INT index);
        
        
        NBT_Tag * getChild (const char * name);
        NBT_Tag * getChild (NBT_INT index);
        
        
        void remove(NBT_Tag *newItem);
        void remove(NBT_INT index);
        
        NBT_Tag *update(NBT_Tag& *nbt) {
        
        NBT_INT size();
        char ** listTagNames();
        
};

// NOTE: Must be above TAG_Atom because it's referenced in template

class TAG_String : public NBT_Tag {
    private:
        NBT_StringHolder payload;    
    public:
        TAG_String();
        TAG_String(const char * name);
        TAG_String(const char * name, NBT_StringHolder value);
        
        // Auto set name/value
        TAG_String(const char * Name, const char * value);
        
        // Auto parsetag
        TAG_String(NBT_BYTE ** data, bool named = true);
        
        ~TAG_String();
        static NBT_StringHolder Parse(NBT_BYTE ** data);
        TAG_String * parseTag(NBT_BYTE ** data, bool named = true);
        NBT_StringHolder getPayload(void);
        void setPayload(NBT_StringHolder pl);
        char * getValue(void);
        static NBT_StringHolder Rebuild(NBT_StringHolder payload);
        static char * GetValue(NBT_Tag * t);
        static char * TryGetValue(NBT_Tag * t);

        
};


class TAG_Byte_Array : public NBT_Tag {
    private:
        NBT_INT _numberOfElements;
        NBT_BYTE * items;
        
    public:
        TAG_Byte_Array();
        TAG_Byte_Array(const char * name);
        TAG_Byte_Array(const char * name, NBT_INT size, NBT_BYTE * items);
        
        ~TAG_Byte_Array();
        NBT_BYTE operator [] (int index);
        NBT_INT size();
        static NBT_StringHolder Rebuild(TAG_Byte_Array * t);
        TAG_Byte_Array * parseTag(NBT_BYTE ** data, bool named = true);
        NBT_BYTE * getArray();
        NBT_BYTE getByte(NBT_INT index);
        
        void setByte (NBT_INT pos, NBT_BYTE val);
        void setBlock(NBT_INT start, NBT_INT len, const NBT_BYTE * newData);
        
        static NBT_BYTE * GetValue(NBT_Tag * t);
        static NBT_BYTE * TryGetValue(NBT_Tag * t);
        
};

class TAG_List : public NBT_Tag {
    private:
        TagType _itemType;
        NBT_INT _numberOfElements;
        vector<NBT_Tag*> items;
        
    public:
        TAG_List();
        TAG_List(const char * name);
        TAG_List(const char * name, TagType itemType);
        
        ~TAG_List();
        NBT_Tag *operator [] (NBT_INT index);
        NBT_Tag * get(NBT_INT index);
        void add(NBT_Tag * t);
        void clear(void);
        void remove(NBT_INT index);
        NBT_INT size();
        TagType getItemType();
        static NBT_StringHolder Rebuild(TAG_List * t);
        
        TAG_List * parseTag(NBT_BYTE ** data, bool named = true);
        
        

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
        
        TAG_Atom(E pl) : NBT_Tag(tagVal)
        {
            payload = pl;
            return;            
        }
        
        TAG_Atom(const char * name, E pl) : NBT_Tag(name, tagVal)
        {
            _isParsed = true;
            payload = pl;
            return;            
        }
        
        TAG_Atom<E,tagVal> * parseTag(NBT_BYTE ** data, bool named = true)
        {
             _isParsed = true;

            // get name
            /*
            len t = TAG_Short::Parse(data);
            start += sizeof(NBT_SHORT);
            // etc etc...
            */
            if(named)
            {
                setName(TAG_String::Parse(data));
                //data += getNameHolder().length + sizeof(NBT_SHORT);
            }
            // get payload
            payload = Parse(data);            
            //data += sizeof(E);
            return this;
            
        }
        
        static E Parse(NBT_BYTE ** data)
        {
            E t = *( (E*)(*data) );
            if(Utilities::IsLittleEndian)
            {
                t = endianSwap<E>(t);
            }
            
            // keep this
            (*data) += sizeof(E);
            
            return t;
        }
        
        // 
        TAG_Atom(char* newName, E newPayload) {
            NBT_Tag(newName, tagVal);
            setPayload(newPayload);
        }
        
        
        static E TryGetValue(NBT_Tag * t)
        {
            try {
                return GetValue(t);
            }
            catch(...)
            {
                return 0;
            }
        }
        
        static E GetValue(NBT_Tag * t)
        {
            // Failed.. return 0??
            if(t == NULL)
            {
                throw new NullException;
            }
            
            
            if(t->getType() == tagVal)
            {
                return ((TAG_Atom<E,tagVal>*)t)->getPayload();
            }
            else
            {
                throw new TypeMismatch(tagVal, t->getType());
            }
        }
        
    
        
        static NBT_StringHolder Rebuild(E payload, bool fixEndian = true)
        {
            
            if(Utilities::IsLittleEndian && fixEndian) 
            {
                payload = endianSwap<E>(payload);
            }
            E* pay2 = new E;
            *pay2 = payload;
            
            char *x = (char*)(pay2);
            
            if(Utilities::IsLittleEndian)
            {
                // move to proper index
                //x += (sizeof(E)-1);

                
            }
            
            NBT_StringHolder n;
            n.length = sizeof(E);
            n.value = x;
            return n;
            
            
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




#endif
