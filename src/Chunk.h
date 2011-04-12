#ifndef __CHUNK_H
#define __CHUNK_H

#include <vector>

#include "Block.h"
#include "Entity.h"
#include "TileEntity.h"

template <class E>
class VectorHolder {
    private:
        vector<E> *vec;
    public:
        VectorHolder(vector<E> *v) {
            vec = v;
        }
        E &operator[] (int index) {
            return (*vec)[index];
        }
};

class Chunk {
    private:
        // blocks[x][y][z]
        byte heightMap[16][16];
        vector<Entity*> entities;
        bool owEntities; // Overwrite flag
        vector<TileEntity*> tileEntities;
        bool owTileEntities;
        bool populated;
    public:
        NBT_INT xPos, yPos;
        Block blocks[16][128][16];
        
        Chunk();
        Chunk(TAG_Compound *root);
        //~Chunk();
        void loadFromCompound(TAG_Compound *root);
        
        void addEntity(Entity *ent);
        VectorHolder<Entity*> getEntities();
        // Invalidates list
        void removeEntity(int index);
        
        void addTileEntity(TileEntity *tilEnt);
        VectorHolder<TileEntity*> getTileEntities();
        // Invalidates list
        void removeTileEntity(int index);
        
        TAG_Compound *construct();
        TAG_Compound *construct(TAG_Compound *root);
};
#endif
