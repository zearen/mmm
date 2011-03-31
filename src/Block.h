#ifndef __BLOCK_H
#define __BLOCK_H
#include "NBTTree.h"

class Block {
    private:
        // high bits are skylight, low bits are blocklicht
        byte light;
        byte data;
    public:
        byte id;

        Block(byte newId, byte skylight, byte blocklight, byte extra);
        void setSkyLight(byte light);
        byte getSkyLight();
        void setBlockLight(byte light);
        byte getBlockLight();
        void setExtra(byte extra);
        byte getExtra();
};
#endif
