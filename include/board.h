#ifndef BOARD_H
#define BOARD_H


#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cstddef>
#include <iostream>
#include <optional>

class Field;
class Doctor;
class Dice;



class Board
{
 private:
  std::vector<Field*> fields;
 private:

 public:
  Board() = default;
  Board(const Board&) = delete;
  
  //prints all fields and players that are on them
  void print_board_state(size_t after_moves);

  // puts all players on the starting field
  void get_started(std::vector<Doctor*>& players);
  
  // Distinction between move and teleport is that move triggers the effects of 
  // the catching fields while teleport skips them 
  void move_forward(uint8_t roll, Doctor* doc);
  void teloport_forward(uint32_t roll, Doctor* doc);
  void teloport_backward(uint32_t roll, Doctor* doc);

  // helper funciton (reduces # of allocations which warms my heart <3)
  void reserve(size_t size) { fields.reserve(size); }
  void add_field(Field* field) { fields.push_back(field); }

  const Field* get_field(size_t index);

  // triggers the field at index to move the player
  void player_move(size_t index, Doctor* doc);

  // resets the board to its initial state
  void restart();

  std::optional<uint8_t> check_for_fields(size_t start_index,
                                 const std::unordered_set<Field*>& fields_to_find) const noexcept;

  ~Board(); 

};


#endif
