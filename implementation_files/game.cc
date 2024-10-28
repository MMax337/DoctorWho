#include <iostream>
#include <iomanip>

#include "../include/game.h"
#include "../include/doctor.h"
#include "../include/field.h"
#include "../include/dice.h"
#include "../include/parser.h"
#include "../include/board.h"


#ifdef NPRINTING
bool constexpr printing = false;
#else
bool constexpr printing = true;
#endif


namespace
{
  const std::string limiter(80,'=');
  const std::string limiter2(80,'-');

}

Game::Game(const std::string& filename, std::optional<stats_t>& statistics) 
:statistics(statistics), board(new Board()), game_over(false), move(1)  
{
  dices.emplace("UsualDice", new UsualDice());
  dices.emplace("FaultyDice", new FaultyDice());
  dices.emplace("WearingOutDice", new WearingOutDice());

  Parser parser(filename);
  parser.parse(players, dices, board);
}


void Game::run_game_move() 
{
  if constexpr (printing) std::cout<<limiter <<'\n'<<std::setw(36)<<"Move: "<< move <<'\n' <<limiter <<'\n';
  for(auto player : players) 
  {
    auto index = player->get_position();
    if constexpr (printing) std::cout << *player << "position "<< index 
                                      << ", field: "<< *board->get_field(index) << '\n';
    board->player_move(index, player);
    
    if (player->check_if_won()) 
    {
      game_over = true;
      if (statistics) ++(statistics->at(player->get_name()));
    }
    if constexpr (printing) std::cout<< limiter2 <<'\n';
  }
}

void Game::print_win() 
{
  if constexpr (printing) 
  {
    for(auto player : players)
    {
      if (player->check_if_won()) std::cout << *player << "won!!!\n";
    }
  }
}

void Game::player_fill() 
{
  if (statistics) for(auto player : players) statistics->emplace(player->get_name(), 0);
}


void Game::play()
{
  board->get_started(players);
  while (!game_over)
  {
    run_game_move();
    board->print_board_state(move);
    ++move;
  }
  print_win(); 
}

void Game::restart()
{
  board->restart();
  for(auto& player : players) player->restart();
  for (auto& [name, dice] : dices) dice->restart();
  game_over = false;
  move = 1;
}

Game::~Game() 
{
  delete board;
  for (auto& [str, dice_ptr] : dices) delete dice_ptr;
  for (auto& player_ptr : players) delete player_ptr;
} 


