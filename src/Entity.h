#ifndef __ENTITY_H
#define __ENTITY_H
#include "NBTTree.h"
#include "Block.h"

    
class Entity {
    private:
        char *_id;
    public:
        NBT_DOUBLE x, dx;
        NBT_DOUBLE y, dy;
        NBT_DOUBLE x, dz;
        NBT_DOUBLE theta, phi;
        NBT_FLOAT fallDist;
        NBT_SHORT fire, air;
        bool onGround;
        
        Entity();
        Entity(TAG_Compound *root);
        virtual void loadFromCompound(TAG_Compound *root);
        TAG_Compound *construct();
        virtual TAG_Compound *construct(NBT_Tag *root);
        const char *getId();
        
        static Entity Entity(NBT_Tag *root);
}

#endif
