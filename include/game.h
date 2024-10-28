#ifndef GAME_H
#define GAME_H


#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <optional>



class Field;
class Doctor;
class Dice;
class Board;


class Game
{
 private:
  using stats_t = std::unordered_map<std::string, size_t>; 
  std::optional<stats_t>& statistics; // win statistics, available only for repeated games
  Board* board;
  std::vector<Doctor*> players;
  std::unordered_map<std::string, Dice*> dices;
  bool game_over;
  size_t move;
 private:
  void run_game_move(); // helper method to run one move 
  void print_win(); // after the game is over prints the winners
 public:
  Game(const std::string& filename, std::optional<stats_t>& statistics);
  Game(const Game&) = delete;

  // fills the statistics with players
  void player_fill();
  // runs the game
  void play();
  // resets the game to its initial state
  void restart();

  ~Game(); 

};


#endif
