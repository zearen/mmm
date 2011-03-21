#include "NBTTree.h"
#include "nbt_defines.h"
#include <deque>
#include <iostream>



NBT_Tag::NBT_Tag(NBT_StringHolder name, TagType tagType)
{
    _name = name;
    _type = tagType;
}
NBT_Tag::NBT_Tag(TagType tagType)
{
    
    _name.length = 0;
    _name.value = new char[0];
    _type = tagType;
}

NBT_Tag::~NBT_Tag()
{
    // destroy NBT_StringHolder
    
        if(_name.value != NULL)
        {
            delete[] _name.value;
        }
        
    
    //TODO: Delete root node
    //TODO: Delete file reference
}


NBT_StringHolder NBT_Tag::getNameHolder()
{
    return _name;
}
char * NBT_Tag::getName()
{
    return _name.value;
}

TagType NBT_Tag::getType()
{
    return _type;
}




void NBT_Tag::setName(const string & name)
{
    _name.length = name.length();
    _name.value = strdup(name.c_str());
};


void NBT_Tag::setName(NBT_StringHolder name)
{
    _name.length = name.length;
    _name.value = name.value;
};

void NBT_Tag::setName(NBT_StringHolder *name)
{
    
    
    // delete old name
    if(_name.value != NULL)
    {
        delete[] _name.value;
    }
    
    
    // set new one
    _name.length = name->length;
    _name.value  = name->value;
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

NBT_Tag * TAG_Compound::operator[] (char * name)
{
    
      deque<NBT_Tag *>::iterator it;
        it = fields.begin();

      while (it != fields.end())
        {
            NBT_Tag * test = *it++;
            if (strcmp(test->getName(),name) == 0)
            {
                return test;
            }
        }


      return NULL;
}


NBT_INT TAG_Compound::size()
{
    return fields.size();
}


char ** TAG_Compound:: listTagNames()
{
    
      char ** ret = new char * [size()];
      
      
      deque<NBT_Tag *>::iterator it;
        it = fields.begin();
    int i = 0;
    
      while (it != fields.end())
        {
            NBT_Tag * test = *it++;
            ret[i++] = test->getName();
        }


      return ret;
}


NBT_BYTE * TAG_Compound::parseTag(NBT_BYTE * data, bool named)
   {

        NBT_BYTE * start = data;
        
        if(named)
        {
            NBT_StringHolder name = TAG_String::Parse(data);
            // apply name
            setName(name);
            data += name.length + sizeof(NBT_SHORT);
        }
        
        bool doContinue = true;
        NBT_Tag * t;


        while(doContinue)
        {

            switch(*data++)
            {
                // I MADE IT! :D
                case TAGTYPE_END:
                    doContinue = false;
                    break;
                    
                // I HAS A BABY!
                case TAGTYPE_COMPOUND:
                    t = new TAG_Compound;
                    data = ((TAG_Compound*)t)->parseTag(data);
                    add((TAG_Compound*)t);
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
                    
                case TAGTYPE_LIST:
                    t = new TAG_List;
                    data = ((TAG_List*)t)->parseTag(data);
                    add(t);
                    break;
                    
                default:
                    cout << "ERROR Reading - Unknown TAG Type: " << (short int)*(data-1) << endl;
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


NBT_StringHolder TAG_String::Parse(NBT_BYTE * data)
{
        // grab a short from data
        NBT_SHORT len = TAG_Short::Parse(data);
        data += sizeof(NBT_SHORT);
        
        char * name = new char[len+1];
        memcpy(name, data, len);
        name[len] = 0; // terminate string!
        
        NBT_StringHolder v;
        v.length = len;
        v.value = name;
        
        return v;
}

NBT_StringHolder * TAG_String::getPayload(void)
{
    return &payload;
}

NBT_BYTE * TAG_String::parseTag(NBT_BYTE * data, bool named)
{
    
    if(named)
    {
        // grab name of tag
        setName(TAG_String::Parse(data));
        // offset past name of tag
        data += getNameHolder().length + sizeof(NBT_SHORT);
    };

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


//*************************
// TAG_List
//*************************


TAG_List::TAG_List() : NBT_Tag(TAGTYPE_LIST)
{
    return;
};

TAG_List::~TAG_List()
{
    return;
};

NBT_INT TAG_List :: size(void)
{
    return _numberOfElements;
}


TagType TAG_List :: getItemType(void)
{
    return _itemType;
}



NBT_BYTE * TAG_List::parseTag(NBT_BYTE * data, bool named)
{
    if(named)
    {
        // grab name of tag
        setName(TAG_String::Parse(data));
        // offset past name of tag
        data += getNameHolder().length + sizeof(NBT_SHORT);
    }
    
    // get element type
    TagType eleType = (TagType)TAG_Byte::Parse(data);
    NBT_SHORT bitSize = sizeOfTagType(eleType);
    data += 1;
   
    
    NBT_Tag * nbt;
        
    // get # element in list
    NBT_INT numElements = TAG_Int::Parse(data);
    data += sizeof(NBT_INT);
    
    _itemType = eleType;
    _numberOfElements = numElements;
    
    for(short int i = 0; i < numElements; i++)
    {
        switch(eleType)
        {
            case TAGTYPE_COMPOUND:
                // damn it.
                nbt = new TAG_Compound;
                data = ((TAG_Compound*)nbt)->parseTag(data,false);
                break;
                
            case TAGTYPE_STRING:
                nbt = new TAG_String;
                data = ((TAG_String*)nbt)->parseTag(data,false);
                break;
                
            case TAGTYPE_BYTE:
                nbt = new TAG_Byte;
                data = ((TAG_Byte*)nbt)->parseTag(data,false);
                break;
                
            case TAGTYPE_DOUBLE:
                nbt = new TAG_Double;
                data = ((TAG_Double*)nbt)->parseTag(data,false);
                break;
                
            case TAGTYPE_FLOAT:
                nbt = new TAG_Float;
                data = ((TAG_Float*)nbt)->parseTag(data,false);
                break;
                
            case TAGTYPE_INT:
                nbt = new TAG_Int;
                data = ((TAG_Int*)nbt)->parseTag(data,false);
                break;
                
            case TAGTYPE_LONG:
                nbt = new TAG_Long;
                data = ((TAG_Long*)nbt)->parseTag(data,false);
                break;
                
            case TAGTYPE_SHORT:
                nbt = new TAG_Short;
                data = ((TAG_Short*)nbt)->parseTag(data,false);
                break;
    
        };
        
        items.push_back(nbt);
    }
    
    // position data past the end of the string    
    return data;
}

NBT_Tag * TAG_List::operator[] (NBT_INT i)
{
    return items[i];
}

