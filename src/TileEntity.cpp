#include "TileEntity.h"

TileEntity::TileEntity(char * id) {
    _id = new char[strlen(id)];
    strcpy(_id, id);
}


TileEntity::TileEntity(TAG_Compound *root, char * id) {
    _id = new char[strlen(id)];
    strcpy(_id, id);
    loadFromCompound(root);
}

TileEntity::~TileEntity() {
    if (_id) {
        delete[] _id;
    }
}

void TileEntity::loadFromCompound(TAG_Compound *root) {
    x = TAG_Int::TryGetValue(root->getChild("x"));    
    y = TAG_Int::TryGetValue(root->getChild("y"));    
    z = TAG_Int::TryGetValue(root->getChild("z"));    
}

TAG_Compound *TileEntity::construct() {
    return construct(new TAG_Compound);
}

TAG_Compound *TileEntity::construct(TAG_Compound *root) {
    NBT_Tag *nbt;
    root->update(nbt = new TAG_String("Id", NBT::MkStr(_id)));
    root->update(nbt = new TAG_Int("x", x));
    root->update(nbt = new TAG_Int("y", y));
    root->update(nbt = new TAG_Int("z", z));
    return root;
}

const char *TileEntity::getId() {
    return _id;
}
