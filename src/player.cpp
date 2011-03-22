#include <string>
#include <exception>

#include "player.h"
#include "MC_World.h"
#include "util.h"

using namespace std;

Player::Player(const char * name)
{
    playerName = NBT::MkStr(name);
    root = nbtProcessor.getRoot();
}

Player::~Player()
{
    return;
}

void Player::reset()
{
    OnGround = 0;
    
    // reset tag_compound..somehow
    
    nbtProcessor.reset();
    
}

void Player::display()
{
    nbtProcessor.displayToScreen();
}
void Player::loadFromFile()
{
    string playerPath = (MC_World::PlayerPath + playerName.value) + ".dat";
    
    if(!Utilities::FileExists(playerPath))
    {
        cout << "Player does not exist!!!";
        throw new FileNotFoundException;
        return;
    }
    
    nbtProcessor.decompressFile(playerPath);
    
    OnGround = TAG_Byte::TryGetValue(root->getChild("OnGround"));
    
    
}
