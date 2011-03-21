#include "NBTTree.h"
#include "NBTDefines.h"
#include <deque>
#include <iostream>



NBT_Tag::NBT_Tag(NBT_StringHolder name, TagType tagType)
{
    _isParsed = false;
    _name = name;
    _type = tagType;
}
NBT_Tag::NBT_Tag(TagType tagType)
{
    _isParsed = false;
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

bool NBT_Tag::isParsed()
{
    return _isParsed;
}




void NBT_Tag::setName(const string & name)
{
    
    // delete old name
    if(_name.value != NULL)
    {
        delete[] _name.value;
    }
    
    _name.length = name.length();
    _name.value = strdup(name.c_str());
};


void NBT_Tag::setName(NBT_StringHolder name)
{
    
    // delete old name
    if(_name.value != NULL)
    {
        delete[] _name.value;
    }
    
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


NBT_Tag * TAG_Compound::operator[] (NBT_INT i)
{
    return fields[i];
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


TAG_Compound * TAG_Compound::parseTag(NBT_BYTE ** data, bool named)
   {
        _isParsed = true;
        
        if(named)
        {
            setName(TAG_String::Parse(data));
        }
        
        bool doContinue = true;
        NBT_Tag * t;


        while(doContinue)
        {

            switch(*((*data)++))
            {
                // I MADE IT! :D
                case TAGTYPE_END:
                    doContinue = false;
                    break;
                    
                case TAGTYPE_COMPOUND:
                    t = (new TAG_Compound)->parseTag(data);
                    break;
                    
                case TAGTYPE_BYTE_ARRAY:
                    t = (new TAG_Byte_Array)->parseTag(data);
                    break;
                    
                case TAGTYPE_STRING:
                    t = (new TAG_String)->parseTag(data);
                    break;
                    
                case TAGTYPE_LIST:
                    t = (new TAG_List)->parseTag(data);
                    break;
                    
                // Atomics
                case TAGTYPE_SHORT:
                    t = (new TAG_Short)->parseTag(data);
                    break;
                    
                case TAGTYPE_INT:
                    t = (new TAG_Int)->parseTag(data);
                    break;

                case TAGTYPE_LONG:
                    t = (new TAG_Long)->parseTag(data);
                    break;

                case TAGTYPE_DOUBLE:
                    t = (new TAG_Double)->parseTag(data);
                    break;
                    
                case TAGTYPE_FLOAT:
                    t = (new TAG_Float)->parseTag(data);
                    break;
                    
                case TAGTYPE_BYTE:
                    t = (new TAG_Byte)->parseTag(data);
                    break;
                    
                default:
                    doContinue = false;
                    cout << "ERROR Reading - Unknown TAG Type: " << (short int)*((*data)-1) << endl;
                    break;

            }
            
            if(doContinue)
            {
                add(t);
            } 
        }
        
        return this;
    }

//*************************
// TAG_String
//*************************


TAG_String::TAG_String() : NBT_Tag(TAGTYPE_STRING)
{
    return;
};


NBT_StringHolder TAG_String::Parse(NBT_BYTE ** data)
{
        // grab a short from data
        NBT_SHORT len = TAG_Short::Parse(data);
        //data += sizeof(NBT_SHORT);
        char * name;
        
        if(len > 0)
        {
            name = new char[len+1];
            memcpy(name, *data, len);
            name[len] = 0; // terminate string!
        }
        else
        {
            name = new char[1];
            name[0] = 0;
        }
        
        // keep this
        (*data) += len;
        
        NBT_StringHolder v;
        v.length = len;
        v.value = name;
        
        return v;
}

NBT_StringHolder TAG_String::Rebuild(NBT_StringHolder payload)
{
    char * x = new char[payload.length + sizeof(NBT_SHORT)];

    NBT_StringHolder h = TAG_Short::Rebuild(payload.length);
    
    memcpy(x, h.value, sizeof(NBT_SHORT));
    delete [] h.value;
    
    x += sizeof(NBT_SHORT);
    
    memcpy(x, payload.value, payload.length);
    
    NBT_StringHolder nb;
    nb.length = payload.length + sizeof(NBT_SHORT);
    nb.value = x - sizeof(NBT_SHORT); // go back to beginning
    
    return nb;
    
    
}


NBT_StringHolder TAG_String::getPayload(void)
{
    return payload;
}

TAG_String * TAG_String::parseTag(NBT_BYTE ** data, bool named)
{
    _isParsed = true;
    
    if(named)
    {
        // grab name of tag
        setName(TAG_String::Parse(data));
        // offset past name of tag
        //data += getNameHolder().length + sizeof(NBT_SHORT);
    };


    //parse string itself
    payload =  TAG_String::Parse(data);
    
    return this; // + payload.length + sizeof(NBT_SHORT);
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



TAG_List * TAG_List::parseTag(NBT_BYTE ** data, bool named)
{
    _isParsed = true;

    if(named)
    {
        // grab name of tag
        setName(TAG_String::Parse(data));
        // offset past name of tag
        //data += getNameHolder().length + sizeof(NBT_SHORT);
    }
    
    // get element type
    TagType eleType = (TagType)TAG_Byte::Parse(data);
    NBT_SHORT bitSize = sizeOfTagType(eleType);
    //data += 1;
   
    
    NBT_Tag * nbt;
        
    // get # element in list
    NBT_INT numElements = TAG_Int::Parse(data);
    //data += sizeof(NBT_INT);
    
    _itemType = eleType;
    _numberOfElements = numElements;
    
    for(short int i = 0; i < numElements; i++)
    {
        switch(eleType)
        {
            case TAGTYPE_COMPOUND:
                // damn it.
                nbt = (new TAG_Compound)->parseTag(data,false);
                break;
                
            case TAGTYPE_STRING:
                nbt = (new TAG_String)->parseTag(data,false);
                break;
                
            case TAGTYPE_BYTE_ARRAY:
                nbt = (new TAG_Byte_Array)->parseTag(data,false);
                break;
                
            case TAGTYPE_LIST:
                nbt = (new TAG_List)->parseTag(data,false);
                break;

            case TAGTYPE_BYTE:
                nbt = (new TAG_Byte)->parseTag(data,false);
                break;
                                
            case TAGTYPE_DOUBLE:
                nbt = (new TAG_Double)->parseTag(data,false);
                break;
                
            case TAGTYPE_FLOAT:
                nbt = (new TAG_Float)->parseTag(data,false);
                break;
                
            case TAGTYPE_INT:
                nbt = (new TAG_Int)->parseTag(data,false);
                break;
                
            case TAGTYPE_LONG:
                nbt = (new TAG_Long)->parseTag(data,false);
                break;
                
            case TAGTYPE_SHORT:
                nbt = (new TAG_Short)->parseTag(data,false);
                break;
    
        };
        
        items.push_back(nbt);
    }
    
    // position data past the end of the string    
    return this;
}

NBT_Tag * TAG_List::operator[] (NBT_INT i)
{
    return items[i];
}


//*************************
// TAG_List
//*************************


TAG_Byte_Array::TAG_Byte_Array() : NBT_Tag(TAGTYPE_BYTE_ARRAY)
{
    return;
};

TAG_Byte_Array::~TAG_Byte_Array()
{
    delete [] items;
    return;
};

NBT_INT TAG_Byte_Array :: size(void)
{
    return _numberOfElements;
}

NBT_BYTE * TAG_Byte_Array :: getArray(void)
{
    return items;
}




TAG_Byte_Array * TAG_Byte_Array::parseTag(NBT_BYTE ** data, bool named)
{
    _isParsed = true;

    if(named)
    {
        // grab name of tag
        setName(TAG_String::Parse(data));
        // offset past name of tag
        //data += getNameHolder().length + sizeof(NBT_SHORT);
    }
    
    // get # element in list
    NBT_INT numElements = TAG_Int::Parse(data);
    //data += sizeof(NBT_INT);
    
    _numberOfElements = numElements;
    
    items = new NBT_BYTE[_numberOfElements];
    memcpy(items, *data, _numberOfElements);
    
    (*data) += _numberOfElements;
    
    return this;
}
NBT_StringHolder TAG_Byte_Array::Rebuild(TAG_Byte_Array * t)
{
    
    NBT_INT size = t->size();
    NBT_BYTE * data = t->getArray();
    
    NBT_StringHolder h = TAG_Int::Rebuild(size);
    
    char * x = new char[size + sizeof(NBT_INT)];
    char * start = x;
    memcpy(x, h.value, sizeof(NBT_INT));
    delete [] h.value;
    
    x += sizeof(NBT_INT);
    
    memcpy(x, data, size);
    
    NBT_StringHolder nb;
    nb.length = size + sizeof(NBT_INT);
    nb.value = start; // go back to beginning
    
    return nb;
}

NBT_BYTE TAG_Byte_Array::operator[] (NBT_INT i)
{
    return items[i];
}
