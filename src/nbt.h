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
   bool _isReady;
   char * _decompressedStream;
   TAG_Compound _rootNode;
   
      
   
   public:

   NBT();
   ~NBT();
   
   bool IsReady(void);
   
   bool LoadFile(string fileName);
   
   void Decompress(string fileName);
   
   void Display();
   
   void Parse(NBT_BYTE * data);
   
   void CloseFile();
   
};

#endif

