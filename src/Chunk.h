#ifndef __CHUNK_H
#define __CHUNK_H

#include <vector>

#include "Block.h"
#include "Entity.h"
#include "TileEntity.h"

template <E>
class VectorHolder {
    private:
        vector<E> *vec;
    public:
        VectorHolder(*vector<E> v) {
            vec = v;
        }
        E &operator[] (int index) {
            return (*vec)[index];
        }
}

class Chunk {
    private:
        // blocks[x][y][z]
        Block blocks[16][128][16];
        byte heightMap[16][16];
        vector<Entity*> entities;
        bool owEntities; // Overwrite flag
        vector<TileEntity*> tileEntities;
        bool owTileEntities;
    public:
        NBT_Int xPos, yPos;
        
        Chunk();
        Chunk(TAG_Compound *root);
        //~Chunk();
        void loadFromCompound(TAG_Compound *root);
        Block[][] operator[](int index);
        
        void addEntity(Entity *ent);
        VectorHolder<Entity> getEntities();
        // Invalidates list
        void removeEntity(int index);
        
        void addTileEntity(TileEntity *tilEnt);
        VectorHolder<TileEntity> getTileEntities();
        // Invalidates list
        void removeTileEntity(int index);
        
        TAG_Compound *construct();
        TAG_Compound *construct(TAG_Compound *root);
};
