#include "Entity.h"

static Entity Entity::GetEntity(NBT_Tag *root) {
    // Eventually this will automatically decide between subclasses
}

Entity::Entity() {
}

Entity::Entity(TAG_Compound *root) {
    loadFromCompound(root);
}

virtual void Entity::loadFromCompound(TAG_Compound *root) {
    NBT_Tag *tmp;
    TAG_List *list;
    
    _id = TAG_String::TryGetValue(root->getChild("Id"));
    
    fallDist = TAG_Float::TryGetValue(root->getChild("FallDistance"));
    fire = TAG_Short::TryGetValue(root->getChild("Fire"));
    air = TAG_Short::TryGetValue(root->getChild("Air"));
    if (TAG_Byte::TryGetValue(root->getChild("OnGround")))
        onGround = true;
    else
        onGround = false;
    
    tmp = root->getChild("Pos");
    if (tmp && type.getType == TAGTYPE_LIST) {
        list = (TAG_List*) tmp;
        x = TAG_Double::TryGetValue(list->get[0]);
        y = TAG_Double::TryGetValue(list->get[1]);
        z = TAG_Double::TryGetValue(list->get[2]);
    }
    else {
        x = y = z = 0;
    }
    
    tmp = root->getChild("Motion");
    if (tmp && type.getType == TAGTYPE_LIST) {
        list = (TAG_List*) tmp;
        dx = TAG_Double::TryGetValue(list->get[0]);
        dy = TAG_Double::TryGetValue(list->get[1]);
        dz = TAG_Double::TryGetValue(list->get[2]);
    }
    else {
        dx = dy = dz = 0;
    }
    
    tmp = root->getChild("Rotation");
    if (tmp && type.getType == TAGTYPE_LIST) {
        list = (TAG_List*) tmp;
        theta = TAG_Double::TryGetValue(list->get[0]);
        phi = TAG_Double::TryGetValue(list->get[1]);
    }
    else {
        theta = phi = 0;
    }
}

TAG_Compound *Entity::construct() {
    return construct(new TAG_Compound);
}

virtual TAG_Compound *Entity::construct(TAG_Compound *root) {
        double x, dx;
        double y, dy;
        double x, dz;
        double theta, phi;
        float fallDist;
        short fire, air;
        bool onGround;
    TAG_Double* tDbl;
    TAG_Short* tSht;
    TAG_Float* tFlt;
    TAG_String* tStr;
    TAG_List* tLst;
    TAG_Byte* tByt
    
    root->update(tStr = new TAG_String("Id", _id));
    root->update(tFlt = new TAG_Float("FallDistance"));
    root->update(tSht = new TAG_Short("Fire", fire));
    root->update(tSht = new TAG_Short("Air", air));
    root->update(tByt = new TAG_Byte("OnGround", onGround?1:0));
    
    lLst = new TAG_List("Pos", TAGTYPE_DOUBLE);
    lLst->add(new TAG_Double("", x));
    lLst->add(new TAG_Double("", y));
    lLst->add(new TAG_Double("", z));
    root->update(lLst);
    
    lLst = new TAG_List("Motion", TAGTYPE_DOUBLE);
    lLst->add(new TAG_Double("", dx));
    lLst->add(new TAG_Double("", dy));
    lLst->add(new TAG_Double("", dz));
    root->update(lLst);
    
    lLst = new TAG_List("Rotation", TAGTYPE_DOUBLE);
    lLst->add(new TAG_Double("", theta));
    lLst->add(new TAG_Double("", phi));
    root->update(lLst);
    
}

const char *Entity::getId() {
    return _id;
}
