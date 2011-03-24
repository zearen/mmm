#include <string>
#include <exception>

#include "player.h"
#include "MC_World.h"
#include "util.h"
#include "NBT.h"
#include "NBTTree.h"

using namespace std;




Player::Player()
{
    // set default values
    
}

Player::Player(TAG_Compound * t)
{    
    // load
    loadFromCompound(t);
}

Player::~Player()
{
    
}

void Player::loadFromCompound(TAG_Compound * root)
{
    
    Entity::loadFromCompound(root);
    
    
    _HurtTime = TAG_Short::TryGetValue(root->getChild("HurtTime"));
    _Health = TAG_Short::TryGetValue(root->getChild("Health"));
    
    _SleepTimer = TAG_Short::TryGetValue(root->getChild("SleepTimer"));
    
    _AttackTime = TAG_Short::TryGetValue(root->getChild("AttackTime"));
    _DeathTime = TAG_Short::TryGetValue(root->getChild("DeathTime"));
    
    
    
    _Dimension = TAG_Int::TryGetValue(root->getChild("Dimension"));
    
    
    // Load inventory
    
    inventory.clear();
    
    TAG_List * invTag = (TAG_List*)(root->getChild("Inventory"));
    if(invTag != NULL)
    {
        NBT_INT size = invTag->size();
        
        for(NBT_INT i = 0;i<size;i++)
        {
            TAG_Compound * invItem = (TAG_Compound*)invTag->get(i);
            InventoryT i;
            i.id = TAG_Short::TryGetValue(invItem->getChild("id"));
            i.Damage = TAG_Short::TryGetValue(invItem->getChild("Damage"));
            i.Count = TAG_Byte::TryGetValue(invItem->getChild("Count"));
            i.Slot = TAG_Byte::TryGetValue(invItem->getChild("Slot"));
            inventory.push_back(i);
        }
    }
}

TAG_Compound * Player::construct()
{
    // create tag compound from nothing...?
    TAG_Compound * t = new TAG_Compound;
    return construct(t);
}

TAG_Compound * Player::construct(TAG_Compound * root)
{
    Entity::construct(root);
    
    
    NBT_Tag * src;
    // Master atomic types
    
    
    // short HurtTime
    src = root->getChild("HurtTime");
    if(!src) {
        src = new TAG_Short("HurtTime", _HurtTime);
        root->add(src);
    } else {
        ((TAG_Short*)src)->setPayload(_HurtTime);
    }
    
    // short Health
    src = root->getChild("Health");
    if(!src) {
        src = new TAG_Short("Health", _Health);
        root->add(src);
    } else {
        ((TAG_Short*)src)->setPayload(_Health);
    }
    
    
    // short sleeptimer
    src = root->getChild("SleepTimer");
    if(!src) {
        src = new TAG_Short("SleepTimer", _SleepTimer);
        root->add(src);
    } else {
        ((TAG_Short*)src)->setPayload(_SleepTimer);
    }
    
    // short AttackTime
    src = root->getChild("AttackTime");
    if(!src) {
        src = new TAG_Short("AttackTime", _AttackTime);
        root->add(src);
    } else {
        ((TAG_Short*)src)->setPayload(_AttackTime);
    }
    
    // short DeathTime
    src = root->getChild("DeathTime");
    if(!src) {
        src = new TAG_Short("DeathTime", _DeathTime);
        root->add(src);
    } else {
        ((TAG_Short*)src)->setPayload(_DeathTime);
    }

    // short Dimension
    src = root->getChild("Dimension");
    if(!src) {
        src = new TAG_Int("Dimension", _Dimension);
        root->add(src);
    } else {
        ((TAG_Int*)src)->setPayload(_Dimension);
    }


    src = (TAG_List*)(root->getChild("Inventory"));
    if(!src) {
        src = new TAG_List("Inventory",TAGTYPE_COMPOUND);
        root->add(src);
    }
    
    
    ((TAG_List*)src)->clear();
    
    vector<InventoryT>::iterator pos = inventory.begin();
    int invSize = inventory.size();
    
    for(int i=0;i<invSize;i++)
    {
        // create compound for me
        
        
        InventoryT me = inventory[i];
        // convert to tagCompound. . . .
        TAG_Compound * invItem = new TAG_Compound();
        invItem->add(new TAG_Short("id", me.id));
        invItem->add(new TAG_Short("Damage", me.Damage));
        invItem->add(new TAG_Byte("Count", me.Count));
        invItem->add(new TAG_Byte("Slot", me.Slot));
        
        ((TAG_List*)src)->add(invItem); 
    }
    
    
    return root;
    
    
}

