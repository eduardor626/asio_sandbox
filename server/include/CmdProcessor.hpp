#pragma once
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>

class IProcessor
{
public:
  IProcessor() = default;
  ~IProcessor() = default;
  virtual std::string process(const std::string &input) = 0;
};

class ProcessorFactory
{

private:
  std::map<std::string, std::shared_ptr<IProcessor>> processors_;

public:
  ProcessorFactory() = default;

  void registerProcessor(std::string type, std::shared_ptr<IProcessor> processor)
  {
    std::cout << "Inserting " << type << " into processors_\n";
    processors_.emplace(type, processor);
  }

  std::shared_ptr<IProcessor> getProcessor(std::string type)
  {
    std::cout << "Looking for " << type << "in processors!\n";
    auto itr = processors_.find(type);
    if (itr != processors_.end()) {
      return itr->second;
    }
    return nullptr;
  }

  bool hasProcessors() const { return !processors_.empty(); }
};