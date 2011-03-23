#include <string>
#include <sys/stat.h> // used for 'file exists' stat
#include "util.h"
#include <fstream>

// default to true for little endian.
bool Utilities::IsLittleEndian = true;

void Utilities::DetectBigEndian(void)
{
    int num = 1;
    if(*(char *)&num == 1)
    {
        IsLittleEndian = true;
    }
    else
    {
        IsLittleEndian = false;
    }
}

bool Utilities::FileExists(string fileName)
{

     struct stat stFileInfo;
     bool doesExist = false;
     if(stat(fileName.c_str(), &stFileInfo) == 0)
     {
         doesExist = true;
     }
     return doesExist;
}

TypeMismatch::TypeMismatch(TagType exp, TagType gotted) {
    expected = exp;
    got = gotted;
}
