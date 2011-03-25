#ifndef __TILE_ENTITY_H
#define __TILE_ENTITY_H

#include <string.h>

#include "NBT.h"
#include "NBTTree.h"

class TileEntity {
    private:
        char * _id;
    public:
        int x, y, z;
        
        TileEntity(char * id = "");
        TileEntity(TAG_Compound *root, char * id = "");
        ~TileEntity();
        virtual void loadFromCompound(TAG_Compound *root);
        TAG_Compound *construct();
        virtual TAG_Compound *construct(TAG_Compound *root);
        const char *getId();
};
#endif
