#include "Processors.hpp"

std::string DogProcessor::process(const std::string & cmd)
{
    std::cout<<"inside DogProcessor!\n";
    return {"Woof!"};
}

std::string CatProcessor::process(const std::string & cmd)
{
    std::cout<<"inside CatProcessor\n";
    return {"Meow!"};
}