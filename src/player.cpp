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
    
    _OnGround = TAG_Byte::TryGetValue(root->getChild("OnGround"));
    _Sleeping = TAG_Byte::TryGetValue(root->getChild("Sleeping"));
    
    _HurtTime = TAG_Short::TryGetValue(root->getChild("HurtTime"));
    _Health = TAG_Short::TryGetValue(root->getChild("Health"));
    _Air = TAG_Short::TryGetValue(root->getChild("Air"));
    _SleepTimer = TAG_Short::TryGetValue(root->getChild("SleepTimer"));
    _Fire = TAG_Short::TryGetValue(root->getChild("Fire"));    
    _AttackTime = TAG_Short::TryGetValue(root->getChild("AttackTime"));
    _DeathTime = TAG_Short::TryGetValue(root->getChild("DeathTime"));
    
    _FallDistance = TAG_Float::TryGetValue(root->getChild("FallDistance"));
    
    _Dimension = TAG_Int::TryGetValue(root->getChild("Dimension"));
    
    TAG_List * PositionTag = (TAG_List*)(root->getChild("Pos"));
    if(PositionTag != NULL)
    {
        _Position[0] = TAG_Double::TryGetValue(PositionTag->get(0));
        _Position[1] = TAG_Double::TryGetValue(PositionTag->get(1));
        _Position[2] = TAG_Double::TryGetValue(PositionTag->get(2));
    }
    
    
    TAG_List * MotionTag = (TAG_List*)(root->getChild("Motion"));
    if(MotionTag != NULL)
    {
        _Motion[0] = TAG_Double::TryGetValue(MotionTag->get(0));
        _Motion[1] = TAG_Double::TryGetValue(MotionTag->get(1));
        _Motion[2] = TAG_Double::TryGetValue(MotionTag->get(2));
    }
    
    TAG_List * RotationTag = (TAG_List*)(root->getChild("Rotation"));
    if(RotationTag != NULL)
    {
        _Rotation[0] = TAG_Float::TryGetValue(RotationTag->get(0));
        _Rotation[1] = TAG_Float::TryGetValue(RotationTag->get(1));
    }
    
    // Load inventory
    
    inventory.clear();
    
    TAG_List * invTag = (TAG_List*)(root->getChild("Inventory"));
    if(RotationTag != NULL)
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
    NBT_Tag * src;
    // Master atomic types
    
    // byte OnGround
    src = root->getChild("OnGround");
    if(!src) {
        src = new TAG_Byte("OnGround", _OnGround);
        root->add(src);
    } else {
        ((TAG_Byte*)src)->setPayload(_OnGround);
    }
    
    
    // byte OnGround
    src = root->getChild("Sleeping");
    if(!src) {
        src = new TAG_Byte("Sleeping", _Sleeping);
        root->add(src);
    } else {
        ((TAG_Byte*)src)->setPayload(_Sleeping);
    }
    
    
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
    
    // short air
    src = root->getChild("Air");
    if(!src) {
        src = new TAG_Short("Air", _Air);
        root->add(src);
    } else {
        ((TAG_Short*)src)->setPayload(_Air);
    }
    
    // short sleeptimer
    src = root->getChild("SleepTimer");
    if(!src) {
        src = new TAG_Short("SleepTimer", _SleepTimer);
        root->add(src);
    } else {
        ((TAG_Short*)src)->setPayload(_SleepTimer);
    }

    // short fire
    src = root->getChild("Fire");
    if(!src) {
        src = new TAG_Short("Fire", _Fire);
        root->add(src);
    } else {
        ((TAG_Short*)src)->setPayload(_Fire);
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


    // short FallDistance
    src = root->getChild("FallDistance");
    if(!src) {
        src = new TAG_Float("FallDistance", _FallDistance);
        root->add(src);
    } else {
        ((TAG_Float*)src)->setPayload(_FallDistance);
    }
    
    
    // Set Position and Rotation
    
    src = (TAG_List*)(root->getChild("Pos"));
    if(!src) {
        src = new TAG_List("Pos",TAGTYPE_DOUBLE);
        ((TAG_List*)src)->add(new TAG_Double(_Position[0]));
        ((TAG_List*)src)->add(new TAG_Double(_Position[1]));
        ((TAG_List*)src)->add(new TAG_Double(_Position[2]));
        root->add(src);
    } else {
        ((TAG_Double*)((TAG_List*)src)->get(0))->setPayload(_Position[0]);
        ((TAG_Double*)((TAG_List*)src)->get(1))->setPayload(_Position[1]);
        ((TAG_Double*)((TAG_List*)src)->get(2))->setPayload(_Position[2]);
    }
    
    
    
    
    src = (TAG_List*)(root->getChild("Motion"));
    if(!src) {
        src = new TAG_List("Motion",TAGTYPE_DOUBLE);
        ((TAG_List*)src)->add(new TAG_Double(_Motion[0]));
        ((TAG_List*)src)->add(new TAG_Double(_Motion[1]));
        ((TAG_List*)src)->add(new TAG_Double(_Motion[2]));
        root->add(src);
    } else {
        ((TAG_Double*)((TAG_List*)src)->get(0))->setPayload(_Motion[0]);
        ((TAG_Double*)((TAG_List*)src)->get(1))->setPayload(_Motion[1]);
        ((TAG_Double*)((TAG_List*)src)->get(2))->setPayload(_Motion[2]);
    }
    
    src = (TAG_List*)(root->getChild("Rotation"));
    if(!src) {
        src = new TAG_List("Rotation",TAGTYPE_FLOAT);
        ((TAG_List*)src)->add(new TAG_Float(_Rotation[0]));
        ((TAG_List*)src)->add(new TAG_Float(_Rotation[1]));
        root->add(src);
    } else {
        ((TAG_Float*)((TAG_List*)src)->get(0))->setPayload(_Rotation[0]);
        ((TAG_Float*)((TAG_List*)src)->get(1))->setPayload(_Rotation[1]);
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

