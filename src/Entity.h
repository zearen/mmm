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
        NBT_DOUBLE z, dz;
        NBT_DOUBLE theta, phi;
        NBT_FLOAT fallDist;
        NBT_SHORT fire, air;
        bool onGround;
        
        Entity(char * id = "");
        Entity(TAG_Compound *root, char * id = "");
        ~Entity();
        virtual void loadFromCompound(TAG_Compound *root);
        TAG_Compound *construct();
        virtual TAG_Compound *construct(TAG_Compound *root);
        const char *getId();
        
        // what are you doing!?
        static Entity GetEntity(NBT_Tag *root);
};

#endif
