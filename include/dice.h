#ifndef DICE_H
#define DICE_H

#include <random>
#include <iostream>
#include <vector>
#include <cstddef>
#include <optional>


class Dice
{
  protected:
    mutable std::mt19937 gen;
    Dice() : gen(std::random_device{}()) {}
    Dice(const Dice&) = delete;
    virtual void print_me(std::ostream&) const = 0;
  public:
    virtual uint8_t dice_throw() const = 0;
    virtual ~Dice() = default;
    virtual void restart() = 0;
    friend std::ostream& operator<<(std::ostream&, const Dice&);
};

inline std::ostream& operator<<(std::ostream& os, const Dice& dice) 
{
  dice.print_me(os);
  return os;
}


class UsualDice : public Dice
{
  mutable std::uniform_int_distribution<> dis;
  void print_me(std::ostream& os) const override {os << "Usual Dice ";}

  public:
    UsualDice() : dis(1, 6) {}
    UsualDice(const UsualDice&) = delete;
    uint8_t dice_throw() const override { return dis(gen); }
    void restart() override {}
    ~UsualDice() = default;

};

class FaultyDice : public Dice
{
 private:
  const double probability_of_one;
  mutable std::uniform_real_distribution<> dis;
  void print_me(std::ostream& os) const override {os << "Faulty Dice ";}
  
 public:
  FaultyDice() : probability_of_one(4/6.), dis(0.0, 1.0) {}
  FaultyDice(const FaultyDice&) = delete;
  // dis(gen) is the draw
  uint8_t dice_throw() const override { return dis(gen) < probability_of_one ? 1 : 6; }
  void restart() noexcept override {}
};

class WearingOutDice : public Dice
{
 private:
  const double max_probability;
  uint8_t favorite_number;
  const double step;
  mutable std::vector<double> probabilities;
  mutable std::discrete_distribution<unsigned short> dis; 

  void print_me(std::ostream& os) const override {os << "Wearing Out Dice ";}

 public:
  WearingOutDice();

  WearingOutDice(const WearingOutDice&) = delete;
  ~WearingOutDice() = default;
  void restart() override;
  uint8_t dice_throw() const override;

};


#endif