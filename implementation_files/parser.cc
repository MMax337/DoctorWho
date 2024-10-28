#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

#include "../include/parser.h"
#include "../include/doctor.h"
#include "../include/field_factory.h"
#include "../include/player_factory.h"
#include "../include/board.h"



void Parser::field_parser_helper(const std::string& key,int& start_field_count, 
                                                int& regeneration_field_count,
                                                int& destination_field_count)
{
  if (key == "S") ++start_field_count;
  else if (key == "R") ++regeneration_field_count;
  else if (key == "D") ++destination_field_count;
}

void Parser::field_parser(Board* board) 
{
  std::string rest;
  std::string line;
  std::string key;

  std::getline(file, line);
  auto number_of_fields = std::stoll(line);
  assert(number_of_fields >= 3);

  
  board->reserve(number_of_fields);
  line.clear();
  
  int start_field_count = 0;
  int regeneration_field_count = 0;
  int destination_field_count = 0;

  for (long long i = 0; i < number_of_fields; ++i) 
  {
    std::getline(file, line);
    
    std::stringstream ss(line);
    ss>>key;
    if (i == 0) assert(key == "S" and "The First Field must Starting Field");
    field_parser_helper(key,start_field_count, regeneration_field_count,destination_field_count);
    rest.clear();
    std::getline(ss, rest);
    board->add_field(field_factory.produce(key, rest));
  }
  assert(start_field_count == 1 and "Only One Starting Field Possible");
  assert(regeneration_field_count < 4 and regeneration_field_count > 0 
                                      and "Wrong number of R fields");
  assert(destination_field_count > 0 and "Must be at least one D field");
}

void Parser::line_skip(size_t number_of_lines)
{
  for (size_t i = 0; i < number_of_lines; ++i) 
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  
}

void Parser::player_parser(std::vector<Doctor*>& players, 
                           const std::unordered_map<std::string, Dice*>& dices, Board* board)
{
  std::string doc;
  std::string name;
  std::string line;

  std::getline(file, line);
  auto number_of_players = std::stoll(line);
  for (long long i = 0; i < number_of_players; ++i)
  {
    std::getline(file, line);
    std::stringstream ss(line);
    ss >> doc;
    std::getline(ss, name);    
    
    players.push_back(player_factory.produce(doc, *board , name, dices));
  }
}

void Parser::parse(std::vector<Doctor*>& players,
                  const std::unordered_map<std::string, Dice*>& dices, Board* board) 
{
  std::string line;
  
  if (!file.is_open()) throw std::runtime_error("File is not open");
  
  std::getline(file, line);
  
  auto number_of_players = std::stoll(line);
  assert(number_of_players > 0);
  line_skip(number_of_players);

  field_parser(board);
  file.seekg(0, file.beg);
  player_parser(players, dices, board);
  file.close();
}


