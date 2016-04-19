
#include "util.h"

std::string ToLower(std::string inStr)
{
    std::string outStr(inStr);
    for (int i = 0; i < outStr.length(); i++)
    {
        outStr[i] = tolower(outStr[i]);
    }
    return outStr;
}
