#include <cstdlib>
#include <iostream>

#include <fstream>

#include "NBT.h"
#include "util.h"


using namespace std;

static string WORLD_PATH = "";
static string PLAYER_PATH = "";
static string REGION_PATH = "";


/* Load Server Configuration from server.properties */

void LoadConfiguration(void)
{    // load server properties
    
    WORLD_PATH = "world";

    if(!Utilities::FileExists("server.properties"))
    {
        cout << "ERROR: Cannot open server.properties... Are you in the minecraft folder?" << endl;
    }
    else
    {
        ifstream serverProperties ("server.properties", ifstream::binary);
        string dataFileLine;
    
        while(serverProperties.good())
        {
            getline(serverProperties, dataFileLine);
           // find it
           remove(dataFileLine.begin(),dataFileLine.end(),' ');
           if(dataFileLine.compare(0,11,"level-name=") == 0)
           {
              // found the info you want!
              
            WORLD_PATH = dataFileLine.substr(11);
           }

        }
        
    }
    WORLD_PATH = WORLD_PATH + "/";
    PLAYER_PATH = WORLD_PATH + "players/";
    REGION_PATH = WORLD_PATH + "region/";


}



int main(int argc, char *argv[])
{
    
    // Determine if system is big or little endian (sets Utilities::IsLittleEndian )
    Utilities::DetectBigEndian();    
    
    
    cout << "Reading server properties . . . " << endl;
    
    LoadConfiguration();
        
    cout << "Using map: " << WORLD_PATH << endl;
       
    NBT * player = NBT::DecompressFile(WORLD_PATH + "bigtest.nbt");
    if  (player)
    {
        player->DisplayToScreen();
        player->SaveToFile("bigtest_stream.txt");
    }else
    {
        cout << "There was an error reading file. Cannot process." << endl;
    }
    
    system("PAUSE");
    return EXIT_SUCCESS;
}

