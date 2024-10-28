#ifndef DOCTOR_H
#define DOCTOR_H

#include <cstddef>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <memory>
#include <random>
#include <cstddef>
#include <optional>


class Field;
class Dice;
class Board;




class Doctor
{
 private:
  uint8_t incarnation;
  bool win;
  bool jumped;

 protected:
  const std::string name;
  Board& board;
  std::size_t board_position; 
  std::vector<Dice *> dices;

  Doctor(Board &board, const std::string &name,
         const std::unordered_map<std::string, Dice *> &)
      : incarnation(9), win(false), jumped(false), name(name), board(board), board_position(0) {}

  virtual void print_me(std::ostream &os) const = 0;
 
 protected:
  // player will throw the dice he wants
  virtual uint8_t dice_throw() const = 0;
 
 public:
  friend std::ostream &operator<<(std::ostream &os, const Doctor &p);
  Doctor(const Doctor&) = delete;

  virtual ~Doctor() = default;
  
  uint8_t get_incarnation() const noexcept { return incarnation; }

  std::size_t get_position() const noexcept { return board_position; }

  void set_position(std::size_t position) noexcept { board_position = position; }

  const std::string& get_name() const noexcept { return name; }
  
  // move, teleport delegate to the board to move this player!
  void move_forward();
  // teleport is perfomed by jump fields which passes a value into it
  void teloport_forward(uint8_t roll);

  void teloport_backward(uint8_t roll);

  void reincarnate() noexcept { ++incarnation; }

  // resets player to its initial state
  void restart() noexcept;
  bool if_jumped() noexcept { bool result = jumped; jumped = false; return result; }

  void set_win() noexcept { win = true; }

  bool check_if_won() const noexcept { return win; }

};

inline std::ostream &operator<<(std::ostream &out, const Doctor &player)
{
  player.print_me(out);
  return out;
}

class RandomPlayer : public Doctor
{
 private:
  mutable std::mt19937 gen;

 protected:
  void print_me(std::ostream &os) const override { os << "Random Player, " << name << ", "; }
  uint8_t dice_throw() const override;

 public:
  RandomPlayer(Board& board, const std::string &name,
                                            const std::unordered_map<std::string, Dice *> &dices_);
  RandomPlayer(const RandomPlayer&) = delete;
  virtual ~RandomPlayer() = default;

};

class TraditionalPlayer : public Doctor
{
 protected:
  void print_me(std::ostream &os) const override { os << "Traditional Player, " << name << ", "; }

  uint8_t dice_throw() const override;

 public:
  TraditionalPlayer(Board& board,
                    const std::string &name, const std::unordered_map<std::string, Dice *> &dices_);
  
  TraditionalPlayer(const TraditionalPlayer&) = delete;
  

  virtual ~TraditionalPlayer() = default;
};

class PredictablePlayer : public Doctor
{
 protected:
  std::shared_ptr<std::unordered_set<Field *>> desired_fields;
  PredictablePlayer(Board& board,
                    const std::string &name, const std::unordered_map<std::string, Dice *> &dices_)
      : Doctor(board, name, dices_)
  {}

  virtual std::optional<uint8_t> check_for_desired_fields() const;
 public:
  PredictablePlayer(const PredictablePlayer& ) = delete;
  virtual ~PredictablePlayer() = default;
};

class ExperimentalPlayer : public PredictablePlayer
{
 protected:
  void print_me(std::ostream &os) const override { os << "Experimental Player, " << name << ", "; }
  uint8_t dice_throw() const override;

 public:
  ExperimentalPlayer(Board& board,
                    const std::string &name, const std::unordered_map<std::string, Dice*> &dices_);

  ExperimentalPlayer(const ExperimentalPlayer& ) = delete;
  virtual ~ExperimentalPlayer() = default;

};

class SmartPlayer : public PredictablePlayer
{
 private:
  mutable unsigned int one_repeated;
  mutable bool using_usual_dice;
  
  // helper methods to throw an appropriate dice
  uint8_t faulty_dice_throw(uint8_t) const;
  uint8_t usual_dice_throw() const;
  uint8_t wearing_out_dice_throw() const;

 protected:
  void print_me(std::ostream &os) const override { os << "Smart Player, " << name << ", "; }
  uint8_t dice_throw() const override;

 public:
  SmartPlayer(Board &board, const std::string &name,
              const std::unordered_map<std::string, Dice *> &dices_);
  SmartPlayer(const SmartPlayer&) = delete;
  virtual ~SmartPlayer() = default;
};

#endif



