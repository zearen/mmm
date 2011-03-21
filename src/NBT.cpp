#include <string>
#include <fstream>
#include "NBT.h"
#include "NBTDefines.h"
#include "util.h"

#include <iostream>

#include <zlib.h>


   NBT::NBT(void)
   {
   }
   

   TAG_Compound * NBT::getRoot(void)
   {
        return &_rootNode;
   }
   
   
   
   
   NBT::~NBT()
   {
        // destroy everything!
    }
    
    void NBT::DisplayList(TAG_List* src, string childStr)
    {
            int range=src->size();
            TagType type = src->getItemType();
            
            cout << range << " entries of type " << type << endl;
            cout << childStr << "{ " << endl;
            for(short int i=0;i<range;i++)
            {
                DisplayTag((*src)[i], childStr + "   ");
            }
            cout << childStr << "}" << endl;
    }
   
   void NBT::DisplayTag(NBT_Tag * src, string childStr)
   {
        
        // display tag to screen
        TagType t = src->getType();
        
        // use own handler, ignore it
        if(t == TAGTYPE_COMPOUND)
        {
            // Recursively parse compound tag
            DisplayCompound((TAG_Compound*)src, childStr);
            return;
        }
        
        cout << childStr;

        string tagName = "";
        
        if(src->getNameHolder().length > 0)
        {
        tagName = "(\"";
        tagName += src->getName();
        tagName += "\")";
        }
        switch(t)
        {
            case TAGTYPE_BYTE:
                cout << "TAG_Byte" << tagName.c_str() << ": ";
                cout << (unsigned short)(0 << 8 | ((TAG_Byte*)src)->getPayload());
                cout << endl;
                break;
                
                
            case TAGTYPE_BYTE_ARRAY:
                cout << "TAG_Byte_Array" << tagName.c_str() << ": [";
                cout << ((TAG_Byte_Array*)src)->size();
                cout << " bytes]" << endl;
                break;
                
            case TAGTYPE_DOUBLE:
                cout << "TAG_Double" << tagName.c_str() << ": ";
                cout << ((TAG_Double*)src)->getPayload();
                cout << endl;
                break;
                
                
            case TAGTYPE_FLOAT:
                cout << "TAG_Float" << tagName.c_str() << ": ";
                cout << ((TAG_Float*)src)->getPayload();
                cout << endl;
                break;
                
                
            case TAGTYPE_INT:
                cout << "TAG_Int" << tagName.c_str() << ": ";
                cout << ((TAG_Int*)src)->getPayload();
                cout << endl;
                break;
                
            case TAGTYPE_LONG:
                cout << "TAG_Long" << tagName.c_str() << ": ";
                cout << ((TAG_Long*)src)->getPayload();
                cout << endl;
                break;
                
                
            case TAGTYPE_SHORT:
                cout << "TAG_Short" << tagName.c_str() << ": ";
                cout << ((TAG_Short*)src)->getPayload();
                cout << endl;
                break;
                
            case TAGTYPE_STRING:
                cout << "TAG_String" << tagName.c_str() << ": ";
                cout << ((TAG_String*)src)->getPayload().value;
                cout << endl;
                break;
                
                
                
            case TAGTYPE_LIST:
                cout << "TAG_List" << tagName.c_str() << ": ";
                DisplayList((TAG_List*)src, childStr);
                cout << endl;

                break;
                
            
        }
        
    }
    

    void NBT::SaveToFile(string fileName)
    {
        
        // make 1MB buffer
        char * buffer = new char[NBT_BUFFER_SIZE];
        char * bufferStart = buffer;
        
        NBT_INT len = 0;
        PackTag(buffer, len, &_rootNode);
        
        cout << "Packing buffer (length " << len << ") to file . . ." << endl;
                
        gzFile nbtFile = gzopen(fileName.c_str(), "wb");
        gzwrite(nbtFile, bufferStart, len);
        gzclose(nbtFile);

        delete [] buffer;
        
    }
    
    
    void NBT::SaveToMemory(NBT_BYTE * buffer, NBT_INT * len)
    {
        
        // make 1MB buffer
        char * buf = new char[NBT_BUFFER_SIZE];
        char * bufferStart = buf;
        
        NBT_INT origLen = 0;
        PackTag(buf, origLen, &_rootNode);
        
        cout << "Packing buffer (length " << origLen << ") to memory . . ." << endl;
                

        uLongf written = NBT_BUFFER_SIZE;
        compress(buffer, &written, (NBT_BYTE*)bufferStart, origLen);
        
        delete [] buf;
        
    }
    

    
    void NBT::PackTag(char *& pos, NBT_INT &len, NBT_Tag * tag, bool named)
    {
        if(tag == NULL)
        {
            return;
        }
        
        if(named)
        {
            byte myType = tag->getType();
            
            memcpy(pos, &myType, 1);
            pos++;
            len++;
            
            NBT_StringHolder title = TAG_String::Rebuild(tag->getNameHolder());
                        
            memcpy(pos, title.value, title.length);
            len += title.length;
            pos += title.length;
            delete [] title.value;
        }
        
        // temp vars used sometimes
        NBT_INT size;
        NBT_Tag * sub;
        
        NBT_StringHolder displayMe;
        
        switch(tag->getType())
        {
            case TAGTYPE_COMPOUND:
                
                size = ((TAG_Compound*)tag)->size();
                
                for(NBT_INT i=0;i<size;i++)
                {
                    sub = (*(TAG_Compound*)tag)[i];
                
                    PackTag(pos, len, sub);
                }
            
                // TAG_End
                
                displayMe = TAG_Byte::Rebuild((NBT_BYTE)0);
                break;

            case TAGTYPE_LIST:
                
                // 1: byte type
                displayMe = TAG_Byte::Rebuild((NBT_BYTE)((TAG_List*)tag)->getItemType());
                memcpy(pos, displayMe.value, displayMe.length);
                pos += displayMe.length;
                len += displayMe.length;
                delete [] displayMe.value;
            
                size = ((TAG_List*)tag)->size();
                
                // 2: byte size
                displayMe = TAG_Int::Rebuild(size);
                memcpy(pos, displayMe.value, displayMe.length);
                pos += displayMe.length;
                len += displayMe.length;
                delete [] displayMe.value;
                
                for(NBT_INT i=0;i<size;i++)
                {
                    sub = (*(TAG_List*)tag)[i];
                
                    PackTag(pos, len, sub, false);
                }
                
                
                displayMe.length = 0;

                break;
                
            case TAGTYPE_BYTE:
                    displayMe = TAG_Byte::Rebuild(((TAG_Byte*)tag)->getPayload());
                break;

            case TAGTYPE_BYTE_ARRAY:
                    displayMe = TAG_Byte_Array::Rebuild((TAG_Byte_Array*)tag);
                break;

            case TAGTYPE_DOUBLE:
                    displayMe = TAG_Double::Rebuild(((TAG_Double*)tag)->getPayload());
                break;
                
            case TAGTYPE_FLOAT:
                    displayMe = TAG_Float::Rebuild(((TAG_Float*)tag)->getPayload());
                break;
                
            case TAGTYPE_LONG:
                    displayMe = TAG_Long::Rebuild(((TAG_Long*)tag)->getPayload());
                break;
                
            case TAGTYPE_INT:
                    displayMe = TAG_Int::Rebuild(((TAG_Int*)tag)->getPayload());
                break;

            case TAGTYPE_SHORT:
                    displayMe = TAG_Short::Rebuild(((TAG_Short*)tag)->getPayload());
                break;
                
            case TAGTYPE_STRING:
                    displayMe = TAG_String::Rebuild(((TAG_String*)tag)->getPayload());
                break;
                
            default:
                cout << "ERROR!!!" << endl;
                delete [] displayMe.value;
                return;
                break;
                
            }
            if(displayMe.length > 0)
            {
                memcpy(pos, displayMe.value, displayMe.length);
                pos += displayMe.length;
                len += displayMe.length;
            }
        
            delete [] displayMe.value;
    }
    
    
    void NBT::DisplayCompound(TAG_Compound* root, string childStr)
    {
        
        cout << childStr << "TAG_Compound";
        if(root->getNameHolder().length > 0)
        {
            cout << "(\"" << root->getName() << "\")";
        }
        cout << ": ";
        
        NBT_INT numNodes = root->size();
        
        cout << numNodes << " entries" << endl;
        cout << childStr << "{" << endl;
        
        char ** nodes = root->listTagNames();
        
        
        NBT_INT i = 0;
        while(i < numNodes)
        {
            NBT_Tag * t = (*root)[*nodes];
            if(t)
            {
                DisplayTag(t, childStr + "   ");
            }
            else
            {
                cout << childStr << "ERROR: Null tag result (not found in compound)." << endl;
            }
            i++;
            nodes++;
        }
        
         cout << childStr << "}" << endl;

        
        
        //delete nodes;
    }
    
   void NBT::DisplayToScreen(void)
   {
    
        if(!_rootNode.isParsed())
        {
            cout << "NBT is not parsed / ready. Cannot display to screen." << endl;
        }    
        else
        {
            DisplayCompound(&_rootNode);
        }
    }
    
    
    
    
   NBT * NBT::DecompressFile(string fileName)
   {
        cout << "Beginning to decompress file: " << fileName << endl;
        
        FILE*_nbtFile;
        
        if(!Utilities::FileExists(fileName))
        {
            return NULL;
        }
        
        _nbtFile = fopen(fileName.c_str(),  "rb");
        if(_nbtFile == NULL)
        {
            return NULL;
        }
        
        // get file size
        fseek(_nbtFile, -4, SEEK_END);
        unsigned int compressedSize = ftell(_nbtFile)+4;

        NBT_INT uncompressedSize;
        fread(&uncompressedSize, 4, 1, _nbtFile);
        
        /*
        // this is little endian!?
        
        if(Utilities::IsLittleEndian)
        {   
            
            uncompressedSize = endianSwap(uncompressedSize);
        };

        cout << "Decompressing (File size " << uncompressedSize << ") " << endl;
        cout.flush();
        */
        
        fclose(_nbtFile);
        
        NBT_BYTE * uncompressedFile =  new NBT_BYTE[uncompressedSize];
        gzFile myFile = gzopen(fileName.c_str(), "rb");
        if(myFile == NULL)
        {
            delete [] uncompressedFile;
            return NULL;
        }
        gzread(myFile, uncompressedFile, uncompressedSize);
        gzclose(myFile);
        
        //TEMP: Write to file
        
        ofstream outfile ("uncompressed.dat", ofstream::binary);
        outfile.write((char*)uncompressedFile, uncompressedSize);
        outfile.close();
        
        
        
        NBT * n = new NBT;
        n->Parse(uncompressedFile);
        
        // All done here!
        delete[] uncompressedFile;
        
        return n;
    }
    
    
   NBT * DecompressMemory(NBT_BYTE * buffer, NBT_INT length)
   {
        // use zlib with gzip
      z_stream zstream;
        zstream.zalloc    = (alloc_func)0;
        zstream.zfree     = (free_func)0;
        zstream.opaque    = (voidpf)0;
        zstream.next_in   = buffer;
        zstream.avail_in  = length;
        zstream.next_out  = 0;
        zstream.avail_out = 0;
        zstream.total_in  = 0;
        zstream.total_out = 0;
        zstream.data_type = Z_BINARY;
        inflateInit(&zstream);
        
        NBT_BYTE * chunkBuffer = new NBT_BYTE[NBT_BUFFER_SIZE + 1];
        
        zstream.next_out = chunkBuffer;
        zstream.avail_out = NBT_BUFFER_SIZE;

        int ret = 0;
        
        if(ret = inflate(&zstream, Z_FINISH) != Z_STREAM_END && ret != Z_BUF_ERROR)
        {
            cout << "Error decompressing memory stream. Sorry." << endl;
            delete [] chunkBuffer;
            return NULL;
        }
        
        inflateEnd(&zstream);
        
        
        NBT * n = new NBT;
        n->Parse(chunkBuffer);
        
        delete [] chunkBuffer;
        
        return n;
    }
    
    
      
   void NBT::Parse(NBT_BYTE * data)
   {
        NBT_BYTE ** dblPointer = new NBT_BYTE *(data);
        
        long int position = 0;
        
        if(*data == TAGTYPE_COMPOUND)
        {

            (*dblPointer)++;
            _rootNode.parseTag(dblPointer);
        }
        else
        {
            // No root node, fail.
            
            cout << "ERROR: Problem reading NBT! Root tag was not compound. It was actually type " << (unsigned short int)*data << endl;
        }
        
         return;
   }

