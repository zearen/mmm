#ifndef _PLAYER
#define _PLAYER

#include "Entity.h"
#include "NBTDefines.h"
#include "NBT.h"
#include <vector>

class Player : public Entity
{
    private:
        
    public:
        
        
        NBT_SHORT _HurtTime;
        NBT_SHORT _Health;
        NBT_SHORT _SleepTimer;
        NBT_SHORT _AttackTime;
        NBT_SHORT _DeathTime;
        
        NBT_INT _Dimension;
        
        vector<InventoryT> inventory;
        
        Player();
        Player(TAG_Compound * t);
        ~Player();
        
        TAG_Compound * construct();
        TAG_Compound * construct(TAG_Compound * root);
        
        virtual void loadFromCompound(TAG_Compound * root);
};

#endif
