#ifndef _PLAYER
#define _PLAYER

#include "NBTDefines.h"
#include "NBT.h"
#include <vector>

class Player
{
    private:
        
    public:
        
        
        NBT_BYTE _OnGround; 
        NBT_BYTE _Sleeping;
        
        NBT_SHORT _HurtTime;
        NBT_SHORT _Health;
        NBT_SHORT _Air;
        NBT_SHORT _SleepTimer;
        NBT_SHORT _Fire;
        NBT_SHORT _AttackTime;
        NBT_SHORT _DeathTime;
        
        NBT_FLOAT _FallDistance;
        
        NBT_INT _Dimension;
        
        NBT_DOUBLE  _Position[3];
        NBT_DOUBLE  _Motion[3];
        NBT_FLOAT  _Rotation[2];
        
        vector<InventoryT> inventory;
        
        Player();
        Player(TAG_Compound * t);
        ~Player();
        
        TAG_Compound * construct();
        TAG_Compound * construct(TAG_Compound * root);
        
        void loadFromCompound(TAG_Compound * root);
};

#endif
