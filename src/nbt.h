#ifndef _nbt
#define _nbt

#include <fstream>
#include <string>
#include "nbt_defines.h"
#include "NBTTree.h"

using namespace std;

class NBT 
{
   private:
        
   TAG_Compound _rootNode;
   static void DisplayTag(NBT_Tag * src, string childStr = "");
   static void DisplayCompound(TAG_Compound* root, string childStr = "");
   static void DisplayList(TAG_List* src, string childStr = "");
   
   public:
   NBT();
   ~NBT();

   void Parse(NBT_BYTE * data);

   static NBT * DecompressFile(string fileName);

   static NBT * DecompressMemory(NBT_BYTE * buffer, NBT_INT length);
   
   
   
   void DisplayToScreen();
   
};

#endif

