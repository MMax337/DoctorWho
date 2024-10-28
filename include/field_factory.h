#ifndef FIELD_FAC_H
#define FIELD_FAC_H

#include <iostream>
#include <memory>
#include <unordered_set>
#include <cassert>
#include "field.h"



class FieldMaker 
{
 public:
  [[nodiscard]] virtual Field* make(const std::string& params) = 0;
  virtual ~FieldMaker() = default;
};



class EmptyFieldMaker : public FieldMaker
{
 public:
  [[nodiscard]] Field* make(const std::string& params) override 
  {
    assert(params.empty() and "Invalid input parameter for Empty Field, must be empty");
    return new EmptyField();
  }
  EmptyFieldMaker() = default;
  EmptyFieldMaker(const EmptyFieldMaker&) = delete;
  virtual ~EmptyFieldMaker() = default;
};

class DestinationFieldMaker : public FieldMaker
{
 public:
  [[nodiscard]] Field* make(const std::string& params) override 
  {
    assert(params.empty() and "Invalid input parameter for Destination Field, must be empty");
    return new DestinationField();
  }
  DestinationFieldMaker() = default;
  DestinationFieldMaker(const DestinationFieldMaker&) = delete;
  virtual ~DestinationFieldMaker() = default;
};

class StartingFieldMaker : public FieldMaker
{
 public:
  [[nodiscard]] Field* make(const std::string& params) override 
  {
    assert(params.empty() and "Invalid input parameter for Starting Field, must be empty"); 
    return new StartingField();
  }
  StartingFieldMaker() = default;
  StartingFieldMaker(const DestinationFieldMaker&) = delete;
  virtual ~StartingFieldMaker() = default;
};

class WaitFieldMaker : public FieldMaker
{
 public:
  [[nodiscard]] Field* make(const std::string& params) override;
  WaitFieldMaker() = default;
  WaitFieldMaker(const DestinationFieldMaker&) = delete;
  virtual ~WaitFieldMaker() = default;
};


class JumpFieldMaker : public FieldMaker 
{
 public:
  [[nodiscard]] Field* make(const std::string& params) override;
  JumpFieldMaker() = default;
  JumpFieldMaker(const DestinationFieldMaker&) = delete;
  virtual ~JumpFieldMaker() = default;
};

class DesiredFieldMaker : public FieldMaker
{
 protected:
  std::shared_ptr<std::unordered_set<Field*>> desired_fields;
  DesiredFieldMaker() : desired_fields(std::make_shared<std::unordered_set<Field*>>()) {}
 public:
  virtual std::shared_ptr<std::unordered_set<Field*>> get_instances() const noexcept
        { return desired_fields; }

  DesiredFieldMaker(const DestinationFieldMaker&) = delete;
  virtual ~DesiredFieldMaker() = default;
};

class RegenerationFieldMaker : public DesiredFieldMaker
{
 private:
  static RegenerationFieldMaker* ptr;
  RegenerationFieldMaker() = default;
 public:
  RegenerationFieldMaker(const RegenerationFieldMaker&) = delete;
  static RegenerationFieldMaker* get_maker() 
  {
    if (ptr == nullptr) ptr = new RegenerationFieldMaker();
    return ptr;
  }
  [[nodiscard]] Field* make(const std::string& params) override;
  ~RegenerationFieldMaker() { ptr = nullptr; }
};

inline RegenerationFieldMaker* RegenerationFieldMaker::ptr = nullptr;


class FieldFactory 
{
 private:
  std::unordered_map<std::string, FieldMaker*> makers;
 
 public:
  FieldFactory();
  FieldFactory(const FieldFactory&) = delete;
  [[nodiscard]] Field* produce(const std::string& key, const std::string& params) 
      { return makers.at(key)->make(params); } 

  ~FieldFactory() { for (auto& [str, maker_ptr] : makers) delete maker_ptr; }
};

#endif