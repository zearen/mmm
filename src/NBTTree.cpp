#include "NBTTree.h"
#include "nbt_defines.h"
#include <deque>

NBT_Tag::NBT_Tag(NBT_StringHolder *name, TagType tagType)
{
    _name = name;
    _type = tagType;
}
NBT_Tag::NBT_Tag(TagType tagType)
{
    NBT_StringHolder blank;
    blank.length = 0;
    blank.value=new char[1];
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


NBT_StringHolder* NBT_Tag::GetName()
{
    return _name;
}




void NBT_Tag::SetName(NBT_StringHolder *name)
{
    // delete old name
    if(_name != NULL)
    {
        if(_name->value != NULL)
        {
            delete[] _name->value;
        }
        delete _name;
    
    }
    
    // set new one
    _name = name;
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
