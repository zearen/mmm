#include "NBTTree.h"

NBT_Tag::NBT_Tag(NBT_StringHolder *name, TagType tagType)
{
    _name = name;
    _type = tagType;
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
