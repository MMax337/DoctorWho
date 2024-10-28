#include <iostream>
#include <iomanip>
#include <string>
#include <deque>
#include <map>
#include <string_view>

#include "../include/simulation.h"
#include "../include/game.h"
#include "../include/board.h"

#ifdef NPRINTING
bool constexpr printing = false;
#else
bool constexpr printing = true;
#endif

namespace 
{
  const std::string limiter(80, '=');
}

Simulation::Simulation(const char** file_names, size_t array_size)
{
  statistics = std::nullopt;
  times = std::nullopt;
  for (size_t i = 0; i < array_size; ++i) 
  {
    games.emplace_back(std::make_unique<Game>(file_names[i], statistics));
  }
}

Simulation::Simulation(const char* file_name, size_t times) 
: times(std::make_optional<size_t>(times))
{
  statistics = std::make_optional<stats_t>();
  games.emplace_back(std::make_unique<Game>(file_name, statistics));
  games[0]->player_fill();
}
void Simulation::run_simulations() 
{ 
  if (!statistics) 
  {
    size_t i = 1;
    while(!games.empty())
    {
      if constexpr (printing) std::cout << "==========================Game nr." << i++ 
                                        << "==========================\n";
      games[0]->play();
      games.pop_front();
    }
  }
  else 
  {
    for (size_t i = 0; i < *times; ++i)
    {
      if constexpr (printing) std::cout << "==========================Game nr." << i+1
                                        << "==========================\n";
        games[0]->play();
        games[0]->restart();
    }
  }
}

void Simulation::print_statistics() 
{ 
  if(!statistics) { std::cout<< "You must have provided the same game!\n"; return; }

  std::cout<< limiter << '\n';
  std::cout << std::setw(40) << "STATS\n";
  std::cout<< limiter << '\n';

  std::multimap<size_t, std::string_view, std::greater<size_t>> sorting_map;

  for(const auto& [player_name, number_of_wins] : *statistics)
  {
    sorting_map.emplace(number_of_wins, player_name);
  }

  for(const auto& [number_of_wins, player_name] : sorting_map)
  {
    std::cout<<player_name << "  Won  " << number_of_wins <<  "  times\n";
  }
}   


