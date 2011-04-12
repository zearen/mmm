#include <time.h>

#include "Chunk.h"

Chunk::Chunk() {}

Chunk::Chunk(TAG_Compound *root) {
    owEntities = false;
    owTileEntities = false;
    loadFromCompound(root);
}

TAG_Compound *Chunk::construct() {
    return construct(new TAG_Compound());
}

/*
Block[][] Chunk::operator[](int index) {
    return blocks[index];
}
*/

void Chunk::addEntity(Entity *ent) {
    entities.push_back(ent);
}

VectorHolder<Entity*> Chunk::getEntities() {
    return VectorHolder<Entity*>(&entities);
}

void Chunk::removeEntity(int index) {
    owEntities = true;
    entities.erase(entities.begin() + index);
}

void Chunk::addTileEntity(TileEntity *tilEnt) {
    tileEntities.push_back(tilEnt);
}

VectorHolder<TileEntity*> Chunk::getTileEntities() {
    return VectorHolder<TileEntity*>(&tileEntities);
}

void Chunk::removeTileEntity(int index) {
    owTileEntities = true;
    tileEntities.erase(tileEntities.begin() + index);
}

void Chunk::loadFromCompound(TAG_Compound *root) {
    TAG_List *lst;
    NBT_BYTE *blks, *dta, *sLight, *bLight;
    // This is very trusting, probably too much so.
    blks = TAG_Byte_Array::TryGetValue(root->getChild("Blocks"));
    dta = TAG_Byte_Array::TryGetValue(root->getChild("Data"));
    sLight = TAG_Byte_Array::TryGetValue(root->getChild("SkyLight"));
    bLight = TAG_Byte_Array::TryGetValue(root->getChild("BlockLight"));
    if (blks && dta && sLight && bLight) {
        int loc;
        for (int x=0; x<16; x++)
        for (int z=0; z<16; z++)
        for (int y=0; y<64; y++) { // 64 = 128 / 2 we iterate two at a time
            loc = y + z*64 + x*16*64;
            blocks[x][y*2][z] = Block(blks[loc*2],
                                    sLight[loc] >> 4,
                                    bLight[loc] >> 4,
                                    dta[loc] >> 4);
            blocks[x][y*2][z] = Block(blks[loc*2+1],
                                    sLight[loc],
                                    bLight[loc],
                                    dta[loc]);
        }
    }
    lst = (TAG_List*)root->getChild("Entities");
    TAG_Compound *cmp;
    entities.reserve(lst->size());
    for (int i=0; i < lst->size(); i++) {
        cmp = (TAG_Compound*)(*lst)[i];
        entities.push_back(new Entity(cmp, 
            TAG_String::TryGetValue(cmp->getChild("Id"))));
    }
    
    lst = (TAG_List*)root->getChild("TileEntities");
    tileEntities.reserve(lst->size());
    for (int i=0; i < lst->size(); i++) {
        cmp = (TAG_Compound*)(*lst)[i];
        tileEntities.push_back(new TileEntity(cmp, 
            TAG_String::TryGetValue(cmp->getChild("Id"))));
    }
    
    xPos = TAG_Int::TryGetValue(root->getChild("xPos"));
    yPos = TAG_Int::TryGetValue(root->getChild("yPos"));
    
    if (TAG_Byte::TryGetValue(root->getChild("TerrainPopulated"))) {
        populated = true;
    }
    else {
        populated = false;
    }
}

TAG_Compound *Chunk::construct(TAG_Compound *root) {
    // Write height map
    NBT_BYTE hMap[256];
    NBT_Tag *nbt;
    for (int x=0; x<16; x++)
    for (int z=0; z<16; z++) {
        int y = 127;
        while (y > 0 && blocks[x][y][z].id) y--;
        hMap[z+16*x] = y;
    }
    root->update(nbt = new TAG_Byte_Array("HeightMap", 256, hMap));
    
    // calcLighting();
    
    //write block data
    NBT_BYTE blkData[32768];
    NBT_BYTE skyData[16384];
    NBT_BYTE blkLghtData[16384];
    NBT_BYTE extraData[16384];
    for (int x=0; x < 16; x++)
    for (int z=0; z < 16; z++)
    for (int y=0; y < 128; y+=2) {
        int index = (y/2) + 128 * z + 128 * 16 * x;
        blkData[index*2] = blocks[x][y][z].id;
        blkData[index*2+1] = blocks[x][y+1][z].id;
        extraData[index] = (blocks[x][y][z].getExtra() << 4)
            & blocks[x][y+1][z].getExtra();
        skyData[index] = (blocks[x][y][z].getSkyLight() << 4)
            & blocks[x][y+1][z].getSkyLight();
        extraData[index] = (blocks[x][y][z].getBlockLight() << 4)
            & blocks[x][y+1][z].getBlockLight();
    }
    root->update(nbt = new TAG_Byte_Array("Blocks", 32768, blkData));
    root->update(nbt = new TAG_Byte_Array("Data", 16384, extraData));
    root->update(nbt = new TAG_Byte_Array("SkyLight", 16384, skyData));
    root->update(nbt = new TAG_Byte_Array("BlockLight", 16384, blkLghtData));
    
    TAG_List *lst;
    int i;
    
    lst = (TAG_List* ) root->getChild("Entities");
    if (!lst) {
        lst = new TAG_List("Entities", TAGTYPE_COMPOUND);
        root->add(lst);
    }
    if (owEntities) {
        lst->clear();
    }
    for (i = 0; i < lst->size(); i++) {
        entities[i]->construct((TAG_Compound*)(*lst)[i]);
    }
    for (;i < entities.size(); i++) {
        lst->add(entities[i]->construct());
    }
     
    lst = (TAG_List* ) root->getChild("TileEntities");
    if (!lst) {
        lst = new TAG_List("TileEntities", TAGTYPE_COMPOUND);
        root->add(lst);
    }
    if (owTileEntities) {
        lst->clear();
    }
    for (i = 0; i < lst->size(); i++) {
        tileEntities[i]->construct((TAG_Compound*)(*lst)[i]);
    }
    for (;i < entities.size(); i++) {
        lst->add(tileEntities[i]->construct());
    }
    
    root->update(nbt = new TAG_Long("LastUpdate", time(NULL)));
    root->update(nbt = new TAG_Byte("TerrainPopulated", populated?1:0));
}
