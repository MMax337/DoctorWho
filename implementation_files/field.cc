
#include <list>
#include <utility>
#include <unordered_map> 
#include <memory>
#include <cassert>
#include <cmath>
#include "../include/doctor.h"
#include "../include/field.h"


#ifdef NPRINTING
bool constexpr printing = false;
#else
bool constexpr printing = true;
#endif

namespace 
{
  const uint8_t MAX_INCARNATION = 13;
}

void FieldWithInfo::player_move(Doctor* doctor) 
{
  auto it = list.find(doctor);
  assert(it != list.end());
  if (it != list.end()) {
      if (it->second >= 1) 
      {
        --it->second; // operator-> has higher presedence than operator--
        if constexpr(printing) doing(it->second);
      }
      else
      {
        list.erase(it);
        this->move_helper(doctor);
      }
  }
  else player_register(doctor);
}
void FieldWithInfo::full_print() const 
{
  if constexpr (printing) 
  {
    std::cout<< *this << ":   ";
    for (auto& [doc, num] : list) std::cout<< *doc << "  ";
    std::cout<< '\n';
  }
}


void EmptyField::player_move(Doctor* doctor) 
{ 
  doctor->move_forward();
  assert(doctor == players.front());
  players.pop_front();
}
void EmptyField::full_print() const 
{
  if constexpr (printing) 
  {
    std::cout<< *this << ":   ";
    for (auto doc : players) std::cout<< *doc << ",   ";
    std::cout<< "\n";
  }
}

void EmptyField::player_register(Doctor* doc) 
{ 
  if constexpr (printing) std::cout << *doc << " stepped on " << *this << '\n';
  players.push_back(doc); 
};


void WaitField::move_helper(Doctor* doctor) 
{ 
  if constexpr (printing) std::cout << "Waiting finished\n";
  doctor->move_forward(); 
}


void WaitField::player_register(Doctor* doctor) 
{
  if (auto it = list.find(doctor); it != list.end()) return;
  if constexpr (printing) std::cout << *doctor << " stepped on Waiting Field with description [" 
                                    << description
                                    << "] Waiting started and will last " 
                                    << waiting_time << " moves\n"; 
  
  list.emplace(doctor, waiting_time);
}


void RegenerationField::move_helper(Doctor* doctor)
{
  if(doctor->get_incarnation() < MAX_INCARNATION)  doctor->reincarnate();
  if constexpr (printing) std::cout << "Reincarnation finished, NEW IDENTITY: " 
                                    << +doctor->get_incarnation() << '\n'; 
  doctor->move_forward();
}



void RegenerationField::player_register(Doctor* doctor) 
{
  if (auto it = list.find(doctor); it != list.end()) return;
  if constexpr (printing) std::cout << *doctor << " steped on the Reincarnation Field." 
                                       " Reincarnation started and will last "
                                    << regeneration_time << " moves\n";
  list.emplace(doctor, regeneration_time);
}


void DestinationField::player_catch(Doctor* doctor)
{
  if (doctor->get_incarnation() >= MAX_INCARNATION) doctor->set_win();
}


void JumpField::player_register(Doctor* doctor)
{ 
  if (doctor->if_jumped()) 
  {
    if constexpr (printing) std::cout << *doctor << "jumped on Jump Field with description [" 
                                                  << description << "]\n"; 
     players.push_back(doctor); 
     return; 
  }
    
  if constexpr (printing) std::cout << *doctor << "stepped on Jump Field with description [" 
                                    << description << "] and jumps" 
                                    << (jump > 0 ? " forward " : " backward ") 
                                    << std::abs(jump) <<" tiles\n";
  if (jump > 0)  doctor->teloport_forward(jump);
  else doctor->teloport_backward(std::abs(jump));
}
