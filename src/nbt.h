#ifndef _nbt
#define _nbt

#include <fstream>
#include <string>
#include "zlib/stdafx.h"
#include "nbt_defines.h"
#include "NBTTree.h"

using namespace std;

class NBT 
{
   private:
        
   TAG_Compound _rootNode;
   
   
   public:
   NBT();
   ~NBT();

   void Parse(NBT_BYTE * data);

   static NBT * DecompressFile(string fileName);

   static NBT * DecompressMemory(NBT_BYTE * buffer, NBT_INT length);
   
   void DisplayToScreen();
   
};

#endif

