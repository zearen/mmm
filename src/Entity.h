#ifndef __ENTITY_H
#define __ENTITY_H
#include "NBTTree.h"
#include "Block.h"

    
class Entity {
    private:
        char *_id;
    public:
        double x, dx;
        double y, dy;
        double x, dz;
        double theta, phi;
        float fallDist;
        short fire, air;
        bool onGround;
        
        Entity();
        Entity(TAG_Compound *root);
        void loadFromCompound(TAG_Compound *root);
        TAG_Compound *construct();
        TAG_Compound *construct(NBT_Tag *root);
        const char *getId();
}

#endif
