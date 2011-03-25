#ifndef _NBT_H
#define _NBT_H

#include "NBTDefines.h"
#include "NBTTree.h"

class NBT
{
    private:
        static void DisplayList(TAG_List* src, string childStr);
        static void DisplayTag(NBT_Tag * src, string childStr);
        static void DisplayCompound(TAG_Compound* root, string childStr);

        static void PackTag(char *& pos, NBT_INT &len, NBT_Tag * tag, bool named);

        void parse(NBT_BYTE * data);

    public:
        NBT();
        ~NBT();
        TAG_Compound * getRoot(void);
        void saveToFile(string fileName);
        void saveToMemory(NBT_BYTE * buffer, NBT_INT * len);
        void displayToScreen(void);
        void decompressFile(string fileName);
        void decompressMemory(NBT_BYTE * buffer, NBT_INT length);
        
        static NBT_StringHolder MkStr(const char * str);
        void reset(void);
}


#endif
