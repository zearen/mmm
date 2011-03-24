#ifndef __BLOCK_H
#define __BLOCK_H
#include "NBTTree.h"

class Block {
    private:
        // high bits are light, low bits are extra data
        byte data;
    public:
        byte id;
        
        Block(byte newId, byte light, byte extra);
        void setLight(byte light);
        byte getLight();
        void setExtra(byte extra);
        byte getExtra();
}
#endif 
