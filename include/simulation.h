#ifndef SIMULATION_H
#define SIMULATION_H


#include <deque>
#include <string>
#include <memory>
#include <optional>
#include <unordered_map>


class Game;

class Simulation
{
 private:
  using stats_t = std::unordered_map<std::string, size_t>; 
  std::deque<std::unique_ptr<Game>> games;
  std::optional<stats_t> statistics;
  std::optional<size_t> times;

 public:
  // takes arrays of file names to run everything once
  Simulation(const char** file_names, size_t array_size);
  // takes one file and how many times it should run.
  //Statistics available only for this option
  Simulation(const char* file_name, size_t times);
  Simulation(const Simulation&) = delete;
  ~Simulation() = default;
  
  void run_simulations();
  void print_statistics();
};

#endif