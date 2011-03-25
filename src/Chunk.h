#ifndef __CHUNK_H
#define __CHUNK_H

#include <vector>

#include "Block.h"
#include "Entity.h"
#include "TileEntity.h"

class Chunk {
    private:
        // blocks[x][y][z]
        vector<vector<vector<Block>>> blocks;
        byte heightMap[16][16];
        vector<Entity*> entities;
        bool owEntities; // Overwrite flag
        vector<TileEntity*> tileEntities;
        bool owTileEntities;
        NBT_Int xPos, yPos;
    public:
        Chunk();
        Chunk(TAG_Compound *root);
        ~Chunk();
        void loadFromCompound(TAG_Compound *root);
        vector<vector<Block>> &operator[](int x);
        
        void addEntity(Entity ent);
        // Invalidates list
        vector<Entity*> &getEntity();
        
        void addTileEntity(TileEntity tilEnt);
        // Invalidates list
        vector<Entity*> filterEntity(const char * id);
        
        TAG_Compound *construct();
        TAG_Compound *construct(TAG_Compound *root);
};
