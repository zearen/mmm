#include <cstdlib>
#include <iostream>

#include <fstream>

#include "nbt.h"
#include "util.h"


using namespace std;

static string WORLD_PATH = "";
static string PLAYER_PATH = "";


/* Load Server Configuration from server.properties */

void LoadConfiguration(void)
{    // load server properties
    
    if(!Utilities::FileExists("server.properties"))
    {
        cout << "ERROR: Cannot open server.properties... Are you in the minecraft folder?" << endl;
        cout << "Assuming default world path of 'world' for now." << endl;
        cout << "WARNING: Program will crash if world folder doesn't exist.";
        WORLD_PATH = "world";
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
              PLAYER_PATH = WORLD_PATH + "/players/";
           }

        }
    }

}



int main(int argc, char *argv[])
{    
    
    
    cout << "Reading server properties . . . " << endl;
    
    LoadConfiguration();
        
    cout << "Using map: " << WORLD_PATH << endl;  
       
    cout << "Lol!" << endl;
    
    NBT player;
    //player.LoadFile("spackula.dat");
    player.Decompress("spackula.dat");
    //player.CloseFile();
    player.Display();
       
    system("PAUSE");
    return EXIT_SUCCESS;
}

