#include <string.h>

using namespace std;

typedef unsigned char byte;

class NBT_Tag {
    private:
        char *name;
    public:
        NBT_Tag(char *name);
        ~NBT_Tag();
        char *getName();
        byte getType()=0;
        bool isType(byte tagType2);
};

class TAG_End : NBT_Tag {
    public
        NBTTag();
};

template <class E, byte tagVal>
class TAG_Atom() : NBT_Tag {
    private:
        E payload;
    public:
        byte getType() {
            return tagVal;
        }
        
        // 
        TAG_Atom(char* newName, E newPayload) {
            NBT_Tag(newName);
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

typedef TAG_Atom<byte, 1> TAG_Byte;
//...
typedef TAG_Atom<char*, 8> TAG_String;

class TAG_List : NBT_Tag {
    private:
        byte tagType;
        int len;
        NBT_Tag **list;
    public:
        TAG_List(char* newName, byte tagVal, int length);
        ~TAG_List();
        NBT_Tag *operator [] (int index);
        int length();
};

class FieldNotFoundError {};
class TAG_Compound : NBT_Tag {
    private:
        deque<*NBT_Tag> fields;
    public:
        ~TAG_Compound;
        NBT_Tag *operator[] (char *field);
        void add(NBT_Tag *newItem);
};

NBT_Tag *parseNBT(byte *NBT_Data);
