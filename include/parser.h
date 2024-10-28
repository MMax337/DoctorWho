#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include "player_factory.h"
#include "field_factory.h"

class Board;

class Parser
{
 private:
  FieldFactory field_factory;
  PlayerFactory player_factory;
  std::ifstream file;

  // needed to assert that the validness of the board
  void field_parser_helper(const std::string& key,int& start_field_count, 
                                                  int& regeneration_field_count,
                                                  int& destination_field_count);
  void field_parser(Board* board);
  void line_skip(size_t number_of_lines);

  void player_parser(std::vector<Doctor*>& players, 
                      const std::unordered_map<std::string, Dice*>& dices, Board* board);
  
 public:
  Parser(const std::string& filename) { file.open(filename); }
  Parser(const Parser&) = delete;
  ~Parser() { file.close(); };

  // parses the given filename
  void parse(std::vector<Doctor*>& players,
                   const std::unordered_map<std::string, Dice*>& dices, Board* board);
};

#endif