#include "NBTTree.h"
#include "NBTDefines.h"
#include <iostream>



NBT_Tag::NBT_Tag(NBT_StringHolder name, TagType tagType)
{
    _isParsed = false; // this isn't really used in anything. . . .
    _name = name;
    _type = tagType;
}

NBT_Tag::NBT_Tag(const char * name, TagType tagType)
{
    _isParsed = false;
    
    NBT_INT len = strlen(name);
    char * nv = new char[len+1];
    strcpy(nv, name);
    
    _name.length = len;
    _name.value = nv;
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

// NOTE: When using this function, must cast then delete
void * NBT_Tag::getValue(NBT_Tag * t)
{
    if(!_isParsed)
    {
        return NULL;
    }
    
    TagType tag = getType();
    void* val;
    
    switch(tag)
    {
        case TAGTYPE_COMPOUND:
            return NULL;
            
        case TAGTYPE_LIST:
            return NULL;
            
        case TAGTYPE_BYTE:
            val = new NBT_BYTE;
            *((NBT_BYTE*)val) = (((TAG_Byte*)t)->getPayload());
            return val;
            
        case TAGTYPE_BYTE_ARRAY:
            return ((TAG_Byte_Array*)t)->getArray();
            
        case TAGTYPE_DOUBLE:
            val = new NBT_DOUBLE;
            *((NBT_DOUBLE*)val) = (((TAG_Double*)t)->getPayload());
            return val;
            
        case TAGTYPE_FLOAT:
            
            val = new NBT_FLOAT;
            *((NBT_FLOAT*)val) = (((TAG_Float*)t)->getPayload());
            return val;
            
        case TAGTYPE_INT:
            val = new NBT_INT;
            *((NBT_INT*)val) = (((TAG_Int*)t)->getPayload());
            return val;
            
        case TAGTYPE_LONG:
            val = new NBT_LONG;
            *((NBT_LONG*)val) = (((TAG_Long*)t)->getPayload());
            return val;
            
        case TAGTYPE_SHORT:
            
            val = new NBT_SHORT;
            *((NBT_SHORT*)val) = (((TAG_Short*)t)->getPayload());
            return val;
            
        case TAGTYPE_STRING:
            
            return NULL;
    }

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
    if(_name.value != name.value && _name.value != NULL)
    {
        delete[] _name.value;
    }
    
    _name.length = name.length;
    _name.value = name.value;
};

//*************************
// TAG_Compound
//*************************

TAG_Compound::TAG_Compound() : NBT_Tag(TAGTYPE_COMPOUND)
{
    _isParsed = true;    
}

TAG_Compound::TAG_Compound(const char * name) : NBT_Tag(name, TAGTYPE_COMPOUND)
{
    _isParsed = true;
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


void TAG_Compound::remove(NBT_Tag* newItem)
{
    //TODO: put in middle, sort by tag
    for(NBT_INT i=0;i<fields.size();i++)
    {
        if(newItem == fields[i])
        {
            fields.erase(fields.begin() + i);
            return;
        }
    }
    
}

void TAG_Compound::remove(NBT_INT newItem)
{
    //TODO: put in middle, sort by tag

    fields.erase(fields.begin() + newItem);
}

NBT_Tag * TAG_Compound::operator[] (const char * name)
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

NBT_Tag * TAG_Compound::getChild(const char * name)
{
    return operator[](name);
}

NBT_Tag * TAG_Compound::operator[] (NBT_INT i)
{
    return fields[i];
}

NBT_Tag * TAG_Compound::getChild(NBT_INT i)
{
    return operator[](i);
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

TAG_String::TAG_String(const char * name) : NBT_Tag(name, TAGTYPE_STRING)
{
    return;
};

TAG_String::TAG_String(const char * name, NBT_StringHolder value) : NBT_Tag(name, TAGTYPE_STRING)
{
    _isParsed = true;
    payload = value;
    return;
};

char * TAG_String::getValue()
{
    return payload.value;
};


char * TAG_String::GetValue(NBT_Tag * t)
{
    if(t == NULL)
    {
        throw new NullException;
    }
    
    if(t->getType() == TAGTYPE_STRING)
    {
        return ((TAG_String*)t)->getValue();
    }
    else
    {
        throw new TypeMismatch(TAGTYPE_STRING, t->getType()); 
    }
}

char * TAG_String::TryGetValue(NBT_Tag * t)
{
    try {
        return GetValue(t);
    }
    catch(exception& e)
    {
        return NULL;
    }
}


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

void TAG_String::setPayload(NBT_StringHolder pl)
{
    if(payload.value != NULL)
        delete [] payload.value;
        
    payload = pl;
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
TAG_List::TAG_List(const char * name) : NBT_Tag(name, TAGTYPE_LIST)
{
    return;
};
TAG_List::TAG_List(const char * name, TagType itemType) : NBT_Tag(name, TAGTYPE_LIST)
{
    _isParsed = true;
    _numberOfElements = 0;
    _itemType = itemType;
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

NBT_Tag * TAG_List::get(NBT_INT i)
{
    return items.at(i);
}

void TAG_List::add(NBT_Tag * t)
{
    _numberOfElements++;
    items.push_back(t);
}
void TAG_List::clear(void)
{
    while(!items.empty())
    {
        delete items.back();
        items.pop_back();
    }
    _numberOfElements = 0;
}

void TAG_List::remove(NBT_INT i)
{
    if(i < items.size())
    {
        // delete from memory too
        NBT_Tag * delMe = items.at(i);
        delete delMe;
        
        items.erase(items.begin() + i);
        _numberOfElements--;
    }
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

void TAG_Byte_Array :: setByte(NBT_INT pos, NBT_BYTE val)
{
    *(items+pos) = val;
}

void TAG_Byte_Array :: setBlock(NBT_INT start, NBT_INT len, const NBT_BYTE * newData)
{
    memcpy(items, newData, start);
}

NBT_BYTE * TAG_Byte_Array::GetValue(NBT_Tag * t)
{
    if(t == NULL)
    {
        throw new NullException;
    }
    
    if(t->getType() == TAGTYPE_BYTE_ARRAY)
    {
        return ((TAG_Byte_Array*)t)->getArray();
    }
    else
    {
        throw new TypeMismatch(TAGTYPE_STRING, t->getType());
    }
}

NBT_BYTE * TAG_Byte_Array::TryGetValue(NBT_Tag * t)
{
    try {
        return GetValue(t);
    }
    catch(exception& e)
    {
        return NULL;
    }
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

NBT_BYTE TAG_Byte_Array::getByte (NBT_INT i)
{
    return items[i];
}
