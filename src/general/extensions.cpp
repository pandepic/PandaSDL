#include "extensions.h"

std::vector<std::string> PandaSDL::SplitString(std::string input, std::string delimiter)
{
    std::vector<std::string> result;

    if (input.length() == 0)
        return result;
    
    if (input.find(delimiter) == std::string::npos)
    {
        result.push_back(input);
        return result;
    }
    
    size_t last = 0;
    size_t next = 0;
    
    while ((next = input.find(delimiter, last)) != std::string::npos)
    {
        result.push_back(input.substr(last, next-last));
        last = next + 1;
    }

    result.push_back(input.substr(last, next-last));

    return result;
}
