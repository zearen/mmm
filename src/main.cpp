#include <cstdlib>
#include <iostream>


#include "NBT.h"
#include "NBTDefines.h"
#include "util.h"
#include "Region.h"
#include "MC_World.h"
#include "player.h"


using namespace std;

/* Load Server Configuration from server.properties */




int main(int argc, char *argv[])
{
    
    MC_World::LoadConfiguration();
    
    // Determine if system is big or little endian (sets Utilities::IsLittleEndian )
    Utilities::DetectBigEndian();    
    

        /*
        
        // TEST 1: 
    cout << "Using map: " << MC_World::WorldPath << endl;
       
    cout << "Trying to read bigtest.nbt..." << endl;
    NBT * player = NBT::DecompressFile(MC_World::PlayerPath + "zearen.dat");
    if  (player)
    {
        player->DisplayToScreen();
        //player->SaveToFile("bigtest_stream.txt");
    }else
    {
        cout << "There was an error reading file. Cannot process." << endl;
    }
    */
    /*
    
    // TEST 2:
    
    Region r("world/region/r.0.0.mcr");
    Region::chunk_t myChunk = r.read((byte)1,(byte)1);
    NBT * chunk = NBT::DecompressMemory((NBT_BYTE*)myChunk.data,(NBT_INT)myChunk.len);
    chunk->DisplayToScreen();
    
    TAG_Compound * rootNode = chunk -> getRoot();
    if(!rootNode)
    {
        cout << "Fail reading root node." << endl;
        return 0;
    }
    
    TAG_Compound * levelNode = (TAG_Compound*)rootNode->getChild("Level");
    if(!levelNode)
    {
        cout << "Fail reading level node." << endl;
        return 0;
    }
    
    NBT_INT x = TAG_Int::GetValue(levelNode->getChild("xPos"));
    
    NBT_INT z = TAG_Int::GetValue(levelNode->getChild("zPos"));
    
    cout << "Position: X " << x << "  Z " << z << endl;
    
    */
    
    Player newPlayer("zearen");
    newPlayer.loadFromFile();
    newPlayer.display();
    
    system("PAUSE");
    return EXIT_SUCCESS;
}

