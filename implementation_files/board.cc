#include <iostream>
#include <iomanip>
#include <cstddef>

#include "../include/doctor.h"
#include "../include/field.h"
#include "../include/board.h"


#ifdef NPRINTING
bool constexpr printing = false;
#else
bool constexpr printing = true;
#endif

namespace 
{
  const std::string limiter(80,'=');
}



Board::~Board() { for (size_t i = 0; i < fields.size(); ++i) { delete fields[i]; } };
void Board::get_started(std::vector<Doctor*>& players) 
{ 
  for (auto& player : players) fields[0]->player_register(player);
}
  

const Field* Board::get_field(size_t index) { return fields[index]; }

void Board::restart() { for (auto field : fields) field->restart(); }


void Board::player_move(size_t index, Doctor* doc) { fields[index]->player_move(doc); }



void Board::move_forward(uint8_t roll, Doctor* doc) 
{
  auto board_position = doc->get_position();
  if (board_position + roll < fields.size())
  {
    for (size_t i = board_position + 1; i <= board_position + roll; ++i)
    {
      fields[i]->player_catch(doc);
    }
    board_position += roll;
  }
  else 
  {
    size_t i = board_position + 1;
    for (uint8_t moved = 0; moved < roll; ++i, ++moved)
    {
      if (i >= fields.size()) i = 0;
      fields[i]->player_catch(doc);
    }
    board_position = i-1; // On the last iteration i increases by 1
  }
  doc->set_position(board_position);
  fields[board_position]->player_register(doc);
}


void Board::teloport_forward(uint32_t roll, Doctor* doc) 
{
  auto board_position = doc->get_position();
  board_position += roll;
  if (board_position >= fields.size()) board_position %= fields.size();
  doc->set_position(board_position);
  fields[board_position]->player_register(doc);
}

void Board::teloport_backward(uint32_t roll, Doctor* doc) 
{
  auto board_position = doc->get_position();
  if (board_position >= roll) board_position -= roll;
  else 
  {
    int temp = roll - board_position - 1; 
    board_position = fields.size() - temp%fields.size() - 1;
  }
  doc->set_position(board_position);
  fields[board_position]->player_register(doc);
}



void Board::print_board_state(size_t move) 
{
  if constexpr (printing) 
  {
    std::cout << limiter << '\n' 
              << std::setw(40) << "Game state after " << move  << " moves\n" 
              << limiter << '\n';
  }
  for (auto field : fields) field->full_print();
}

std::optional<uint8_t> Board::check_for_fields(size_t start_index, 
                                  const std::unordered_set<Field*>& fields_to_find) const noexcept
{
  uint8_t moved = 1;
  for (size_t i = start_index + 1; moved <= 6; ++moved, ++i)
  {
    if (i >= fields.size()) i = 0;
    if (fields_to_find.find(fields[i]) != fields_to_find.end())
        return std::make_optional<uint8_t>(moved);
  }
  return std::nullopt;
}







