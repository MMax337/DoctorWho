#ifndef FIELD_H
#define FIELD_H

#include <list>
#include <utility>
#include <iostream>
#include <unordered_map> 
#include <memory>
#include <cassert>


class Doctor;


class Field
{
 protected:
  virtual void print_me(std::ostream& os) const = 0;
  

 public:
  Field() = default;
  Field(const Field&) = delete;
  
  // do the appropriate logic to decide when player should move
  // and what kind of move shoult it be (regular or teleport)
  virtual void player_move(Doctor*) = 0;
  // the catchable fields must override this method to do something nontrivial
  virtual void player_catch(Doctor*) {};
  // registers player on this field, is called when player steps on the field
  virtual void player_register(Doctor* doc) = 0;

  // prints the name of the field and all players standing on it
  virtual void full_print() const = 0;
  // resets the state to the initial
  virtual void restart() = 0;
  virtual ~Field() = default;

  friend std::ostream& operator<<(std::ostream&, const Field&);
};

inline std::ostream& operator<<(std::ostream& os, const Field& field)  
{
  field.print_me(os);
  return os;
}



class FieldWithInfo : public Field
{
 protected:
  std::unordered_map<Doctor*, uint8_t> list;
  virtual void move_helper(Doctor*) = 0;
  virtual void doing(uint8_t left) = 0;

 public:
  FieldWithInfo() = default;
  FieldWithInfo(const FieldWithInfo&) = delete;
  virtual ~FieldWithInfo() = default;
  void player_move(Doctor* doctor) override;
  void restart() noexcept override final {list.clear();}
  void full_print() const override final;
};



class EmptyField : public Field
{
 protected:
  std::list<Doctor*> players;
  void print_me(std::ostream& os) const override {os << "Empty Field ";}
 public:
  EmptyField() = default;
  EmptyField(const EmptyField&) = delete;
  virtual ~EmptyField() = default;
  void player_move(Doctor* doctor) override;
  void player_register(Doctor* doc) override;
  void restart() noexcept override final {players.clear();}
  
  void full_print() const override final;
  
};

class WaitField : public FieldWithInfo
{
 private:
  const int waiting_time;
  const std::string description;
  virtual void move_helper(Doctor* doctor) override;
 protected:
  void print_me(std::ostream& os) const override {os << "Wait Field ";}
  void doing(uint8_t left) override {std::cout << "Waiting. Moves left: "<< +left << "\n"; }
 public:
  WaitField(int waiting_time, const std::string& description)
  : waiting_time(waiting_time), description(description) {}

  WaitField(const WaitField&) = delete;
  virtual ~WaitField() = default;
  
  void player_register(Doctor* doctor) override;
};

class RegenerationField : public FieldWithInfo
{
 protected:
  void print_me(std::ostream& os) const override {os << "Reincarnation Field ";}
  void doing(uint8_t left) override {std::cout << "Reincarnating. Moves left: "<< +left << "\n"; }
 private:
  const int regeneration_time;
  virtual void move_helper(Doctor* doctor) override;
  void player_register(Doctor* doctor) override;
 public:
  RegenerationField(int time) : regeneration_time(time) {}
  RegenerationField(const RegenerationField&) = delete;
  virtual ~RegenerationField() = default;
};

class StartingField: public EmptyField
{
 protected:
  void print_me(std::ostream& os) const override {os << "Starting Field ";}
 public:
  StartingField() = default;
  StartingField(const StartingField&) = delete;
  virtual ~StartingField() = default;

};


class DestinationField: public EmptyField
{
 protected:
  void print_me(std::ostream& os) const override {os << "Destination Field ";}
 public:
  DestinationField() = default;
  DestinationField(const DestinationField&) = delete;
  virtual ~DestinationField() = default;

  void player_catch(Doctor* doctor) override;
};

class JumpField : public EmptyField
{
 private:
  const int jump;
  const std::string description;

 protected:
  void print_me(std::ostream& os) const override {os << "Jump Field ";}
 public:
  void player_register(Doctor* doctor) override;

  JumpField(int jump, const std::string& description) : jump(jump), description(description) {}
  JumpField(const JumpField&) = delete;
  virtual ~JumpField() = default;
};


#endif





