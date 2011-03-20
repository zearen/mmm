#include "NBTTree.h"
#include "nbt_defines.h"
#include <deque>
#include <iostream>

NBT_Tag::NBT_Tag(NBT_StringHolder *name, TagType tagType)
{
    _name = name;
    _type = tagType;
}
NBT_Tag::NBT_Tag(TagType tagType)
{
    NBT_StringHolder blank;
    blank.length = 0;
    blank.value=NULL;
    _name = &blank;
    _type = tagType;
}

NBT_Tag::~NBT_Tag()
{
    // destroy NBT_StringHolder
    if(_name != NULL)
    {
        if(_name->value != NULL)
        {
            delete[] _name->value;
        }
        delete _name;
    
    }
    
    //TODO: Delete root node
    //TODO: Delete file reference
}


NBT_StringHolder* NBT_Tag::getName()
{
    return _name;
}



void NBT_Tag::setName(NBT_StringHolder name)
{
    _name->length = name.length;
    _name->value = name.value;
};

void NBT_Tag::setName(NBT_StringHolder *name)
{
    
    
    // delete old name
    if(_name->value != NULL)
    {
        delete[] _name->value;
    }
    
    
    // set new one
    _name->length = name->length;
    _name->value  = name->value;
};

//*************************
// TAG_Compound
//*************************

TAG_Compound::TAG_Compound() : NBT_Tag(TAGTYPE_COMPOUND)
{
    //_name=blank;
    // _type = TAGTYPE_Compound;
    // NBT_Tag(blank, TAGTYPE_COMPOUND);
}

TAG_Compound::~TAG_Compound()
{
    // todo: delete fields queue
}


void TAG_Compound::add(NBT_Tag* newItem)
{
    //TODO: put in middle, sort by tag
    
    fields.push_back(newItem);
}


NBT_BYTE * TAG_Compound::parseTag(NBT_BYTE * data)
   {
        cout << "I love you!" << endl;
        cout.flush();
        
        NBT_BYTE * start = data;
        
        NBT_StringHolder* name = TAG_String::Parse(data);
        cout << "Compound tag name: " << *name->value << endl;
        cout << "Compound tag length: " << name->length << endl;
        cout << "Compound tag short size: " << sizeof(NBT_SHORT) << endl;
        cout.flush();
        
        // apply name
        setName(name);
        
        cout << "Set name " << endl;
        cout.flush();
        
        data += name->length + sizeof(NBT_SHORT);
        cout << "Moved ahead" << endl;
        cout.flush();
        
        bool doContinue = true;
        NBT_Tag * t;
        cout << "Gonna loop now!" << endl;
        cout.flush();
        while(doContinue)
        {
            cout << "Reading byte " << (short int)(*data) << endl;
            switch(*data)
            {
                // I MADE IT! :D
                case TAGTYPE_END:
                    
                    data++; // position after this end marker
                    doContinue = false;
                    break;
                    
                // I HAS A BABY!
                case TAGTYPE_COMPOUND:
                    t = new TAG_Compound;
                    data = ((TAG_Compound*)t)->parseTag(data);
                    add(t);
                    break;
                    
                // Atomics
                case TAGTYPE_SHORT:
                    t = new TAG_Short;
                    data = ((TAG_Short*)t)->parseTag(data);
                    add(t);
                    break;
                case TAGTYPE_INT:
                    t = new TAG_Int;
                    data = ((TAG_Int*)t)->parseTag(data);
                    add(t);
                    break;
                case TAGTYPE_LONG:
                    t = new TAG_Long;
                    data = ((TAG_Long*)t)->parseTag(data);
                    add(t);
                    break;
                case TAGTYPE_DOUBLE:
                    t = new TAG_Double;
                    data = ((TAG_Double*)t)->parseTag(data);
                    add(t);
                    break;
                    
                case TAGTYPE_FLOAT:
                    t = new TAG_Float;
                    data = ((TAG_Float*)t)->parseTag(data);
                    add(t);
                    break;
                    
                case TAGTYPE_BYTE:
                    t = new TAG_Byte;
                    data = ((TAG_Byte*)t)->parseTag(data);
                    add(t);
                    break;
                case TAGTYPE_STRING:
                    t = new TAG_String;
                    data = ((TAG_String*)t)->parseTag(data);
                    add(t);
                    break;
                default:
                    cout << "ERROR Reading - Unknown TAG Type: " << (short int)*data << endl;
                    break;

            };
            

        }
        
        return data;
    }

//*************************
// TAG_String
//*************************


TAG_String::TAG_String() : NBT_Tag(TAGTYPE_STRING)
{
    return;
};


NBT_StringHolder* TAG_String::Parse(NBT_BYTE * data)
{
        // grab a short from data
        NBT_SHORT len = TAG_Short::Parse(data);
        data += sizeof(NBT_SHORT);
        
        char * name = new char[len+1];
        memcpy(name, data, len);
        name[len] = 0; // terminate string!
        
        NBT_StringHolder * v = new NBT_StringHolder;
        v->length = len;
        v->value = name;
        
        return v;
}


NBT_BYTE * TAG_String::parseTag(NBT_BYTE * data)
{
    
    // grab name of tag
    setName(TAG_String::Parse(data));
    // offset past name of tag
    data += getName()->length + sizeof(NBT_SHORT);
    
    // get length of string
    payload.length = TAG_Short::Parse(data);
    
    // offset to beginning of string
    data += sizeof(NBT_SHORT);
    
    // copy length bytes
    char * myVal = new char[payload.length + 1];
    memcpy(myVal, data, payload.length);
    
    // null terminate the string
    myVal[payload.length] = 0;

    // copy new char[] into "Payload"
    payload.value = myVal;

    // position data past the end of the string    
    return data + payload.length;
}


