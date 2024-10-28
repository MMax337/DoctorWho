#include <cstddef>
#include <chrono>
#include <cassert>
#include <iostream>
#include <string>
#include "../include/simulation.h"
#include "../include/board.h" 
#include "../include/game.h"



int main(int argc, const char** argv) 
{ 
  auto start = std::chrono::high_resolution_clock::now();
  assert(argc > 1);
  if (argc == 3)
  {
    long long times = 0;
    try 
    {
      times = std::stoll(argv[2]);
      assert(times > 0);
      Simulation my_game(argv[1], times);
      my_game.run_simulations();
      my_game.print_statistics();
    }
    catch (const std::out_of_range& e) 
    {
      throw std::logic_error("Too many simulations!!! Computer will crash :(\n");
    }
    catch(const std::invalid_argument& e) 
    {
      Simulation my_game(argv + 1, argc - 1);
      my_game.run_simulations();
    }
  }
  else 
  {
    Simulation my_game(argv + 1, argc - 1);
    my_game.run_simulations();
  }
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
 

  std::cout << "\nSimulation Lasted "
      << duration.count() << " microseconds" << std::endl;
}