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
   FILE* _nbtFile;
   char * _decompressedStream;
   TAG_Compound * _rootNode;
   
   
   NBT_SHORT ParseShort(NBT_BYTE * data, long int start, long int maxSize);
   NBT_StringHolder ParseString(NBT_BYTE * data, long int start, long int maxSize);
   TAG_Compound * ParseCompoundTag(NBT_BYTE * data, long int start, long int maxSize);
   
   
   public:

   NBT();
   ~NBT();
   
   bool IsReady(void);
   
   bool LoadFile(string fileName);
   
   void Decompress();
   
   void Parse(NBT_BYTE * data, long int length);
   
   void CloseFile();
   
};

#endif

