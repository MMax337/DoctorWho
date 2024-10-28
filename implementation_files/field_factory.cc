#include <iostream>
#include <memory>
#include <sstream>

#include "../include/field.h"
#include "../include/field_factory.h"





[[nodiscard]] Field* WaitFieldMaker::make(const std::string& params) 
{
  int wait = 2;
  std::stringstream ss;
  std::string description;
  if (!params.empty()) {
    try { 
      ss.str(params);
      ss >> wait;
      assert(wait > 0);
      std::getline(ss, description); 
    } catch (...) {wait = 2;}
  }
  return new WaitField(wait, description);
}

[[nodiscard]] Field* JumpFieldMaker::make(const std::string& params) 
{
  int jump = 4;
  std::stringstream ss;
  std::string description;
  if (!params.empty()) {
    ss.str(params);
    ss >> jump;
    assert(jump > 0 or jump < 0);
    std::getline(ss, description); 
  }  
  return new JumpField(jump, description); 
}


[[nodiscard]] Field* RegenerationFieldMaker::make(const std::string& params) 
{
  int regeneration_time = 2;
  if (!params.empty()) {
    try { 
      regeneration_time = std::stoi(params); 
      assert(regeneration_time > 0);
    } catch (...) {regeneration_time = 2;}
  }
  auto ptr = new RegenerationField(regeneration_time);
  desired_fields->insert(ptr);
  return ptr;
}



FieldFactory::FieldFactory()
  {
    makers.emplace("S", new StartingFieldMaker());
    makers.emplace("D", new DestinationFieldMaker());
    makers.emplace(".", new EmptyFieldMaker());
    makers.emplace("R", RegenerationFieldMaker::get_maker());
    makers.emplace("P", new JumpFieldMaker());
    makers.emplace("O", new WaitFieldMaker()); 
  }