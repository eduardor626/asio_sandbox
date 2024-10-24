#pragma once
#include "CmdProcessor.hpp"


class DogProcessor : public IProcessor
{
    public:
    DogProcessor() = default;
    virtual std::string process(const std::string & cmd) override;

};

class CatProcessor : public IProcessor
{
    public:
    CatProcessor() = default;
    virtual std::string process(const std::string & cmd) override;

};