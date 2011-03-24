#include <string.h>

#include "Entity.h"
#include "NBT.h"

Entity Entity::GetEntity(NBT_Tag *root) {
    // Eventually this will automatically decide between subclasses
}

Entity::Entity(char * id) {
    _id = new char [strlen(id) + 1];
    strcpy(_id, id);
}

Entity::~Entity() {
    if (_id) delete[] _id;
}

Entity::Entity(TAG_Compound *root, char * id) {
    _id = new char [strlen(id) + 1];
    strcpy(_id, id);
    loadFromCompound(root);
}

void Entity::loadFromCompound(TAG_Compound *root) {
    NBT_Tag *tmp;
    TAG_List *list;
    
    //_id = TAG_String::TryGetValue(root->getChild("Id"));
    
    
    fallDist = TAG_Float::TryGetValue(root->getChild("FallDistance"));
    fire = TAG_Short::TryGetValue(root->getChild("Fire"));
    air = TAG_Short::TryGetValue(root->getChild("Air"));
    if (TAG_Byte::TryGetValue(root->getChild("OnGround")))
        onGround = true;
    else
        onGround = false;
    
    tmp = root->getChild("Pos");
    if (tmp && tmp->getType() == TAGTYPE_LIST) {
        list = (TAG_List*) tmp;
        x = TAG_Double::TryGetValue(list->get(0));
        y = TAG_Double::TryGetValue(list->get(1));
        z = TAG_Double::TryGetValue(list->get(2));
    }
    else {
        x = y = z = 0;
    }
    
    tmp = root->getChild("Motion");
    if (tmp && tmp->getType() == TAGTYPE_LIST) {
        list = (TAG_List*) tmp;
        dx = TAG_Double::TryGetValue(list->get(0));
        dy = TAG_Double::TryGetValue(list->get(1));
        dz = TAG_Double::TryGetValue(list->get(2));
    }
    else {
        dx = dy = dz = 0;
    }
    
    tmp = root->getChild("Rotation");
    if (tmp && tmp->getType() == TAGTYPE_LIST) {
        list = (TAG_List*) tmp;
        theta = TAG_Double::TryGetValue(list->get(0));
        phi = TAG_Double::TryGetValue(list->get(1));
    }
    else {
        theta = phi = 0;
    }
}

TAG_Compound *Entity::construct() {
    return construct(new TAG_Compound);
}

TAG_Compound *Entity::construct(TAG_Compound *root) {
    TAG_List* tLst;
    NBT_Tag *nbt;
    
    root->update(nbt = new TAG_String("Id", NBT::MkStr(_id)));
    root->update(nbt = new TAG_Float("FallDistance", fallDist));
    root->update(nbt = new TAG_Short("Fire", fire));
    root->update(nbt = new TAG_Short("Air", air));
    root->update(nbt = new TAG_Byte("OnGround", onGround?1:0));
    
    tLst = new TAG_List("Pos", TAGTYPE_DOUBLE);
    tLst->add(new TAG_Double("", x));
    tLst->add(new TAG_Double("", y));
    tLst->add(new TAG_Double("", z));
    nbt = tLst;
    root->update(nbt);
    
    tLst = new TAG_List("Motion", TAGTYPE_DOUBLE);
    tLst->add(new TAG_Double("", dx));
    tLst->add(new TAG_Double("", dy));
    tLst->add(new TAG_Double("", dz));
    nbt =  tLst;
    root->update(nbt);
    
    tLst = new TAG_List("Rotation", TAGTYPE_DOUBLE);
    tLst->add(new TAG_Double("", theta));
    tLst->add(new TAG_Double("", phi));
    nbt = tLst;
    root->update(nbt);
    
    return root;
}

const char *Entity::getId() {
    return _id;
}
