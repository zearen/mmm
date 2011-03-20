#include <string>
#include <fstream>
#include "nbt.h"
#include "utilities.h"
#include "nbt_defines.h"

#include "zlib/stdafx.h"
#include "zlib/deflate.h"
#include <iostream>


   NBT::NBT(void)
   {
      _isReady = false;
   }
   
   NBT::~NBT()
   {
        // destroy everything!
    }
   
   bool NBT::IsReady(void)
   {
        return _isReady;
    }
   
   bool NBT::LoadFile(string fileName)
   {
        return false;
        /*// close already loaded file
        if (_isReady)
        {
            CloseFile();
        }
        
        bool didLoad = false;
        if(Utilities::FileExists(fileName))
        {
            _nbtFile = fopen(fileName.c_str(),  "rb");
            _isReady = true;
            didLoad = true;
        }
        
        return didLoad;*/
    }
   
   void NBT::Display()
   {
        cout << _rootNode.getName() << endl;
    }
    
   void NBT::Decompress(string fileName)
   {
        FILE*_nbtFile;
        
        if(!Utilities::FileExists(fileName))
        {
            return;
        }
        
        _nbtFile = fopen(fileName.c_str(),  "rb");
        cout << "File opened! " << endl;
        cout.flush();
        
        // get file size
        fseek(_nbtFile, 0, SEEK_END);
        int fileSize = ftell(_nbtFile);
        
        // reset
        fseek(_nbtFile, 0, SEEK_SET);
        
        Byte * compressedFile = (Byte *)malloc(sizeof(Byte)*fileSize);
        if(compressedFile == NULL) cout << "Denied Love!" << endl;
        
        int len;

        cout << "Reading " << fileSize << " byte file into buffer!!!" << endl;
        cout.flush();
        
        len = fread(compressedFile, 1, fileSize, _nbtFile);
        
        cout << "Actually read " << len << endl;
        cout << "Read file into buffer!!!" << endl;
        cout << "Print: " << compressedFile << endl;
        // decompress now!
        
        // set to nbt root
        //_rootTag = ParseCompoundTag(decompressedData, 0, decompressedSize);
        
        cout << "B4 Root name is: '" << *_rootNode.getName()->value << "' and balls!";
        cout.flush();
                
        Parse(compressedFile);
        cout << "After Root name is: '" << *_rootNode.getName()->value << "' and balls!";
        cout.flush();
        
        fclose(_nbtFile);
        return;
    }
    
      
      
   void NBT::Parse(NBT_BYTE * data)
   {
        long int position = 0;
        
        
        
        if(*data == TAGTYPE_COMPOUND)
        {
            _rootNode.parseTag(++data);
        }
        else
        {
            // No root node, fail.
            
            cout << "NO!!!!!!! BAD FILE IS BAD!: SAY: " << (unsigned short int)*data << endl; 
        }
        
         return;
   }
   
   void NBT::CloseFile()
   {
        return;
        /*
        if(_isReady)
        {
            fclose(_nbtFile);
            _isReady = false;
        }*/
    }
