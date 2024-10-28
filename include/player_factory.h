#ifndef PLAYER_FACTORY_H
#define PLAYER_FACTORY_H

#include "doctor.h"
#include "field_factory.h"

class PlayerMaker
{
 public:
  PlayerMaker() = default;
  PlayerMaker(const PlayerMaker&) = delete;

  [[nodiscard]] virtual Doctor* make(Board& , const std::string &,
                       const std::unordered_map<std::string, Dice*> &) = 0;
  virtual ~PlayerMaker() = default;
};

class RandomPlayerMaker : public PlayerMaker
{
 public:
  RandomPlayerMaker() = default;
  RandomPlayerMaker(const RandomPlayerMaker&) = delete;
  virtual ~RandomPlayerMaker() = default;


  [[nodiscard]] Doctor* make(Board& fields, const std::string &name,
               const std::unordered_map<std::string, Dice *> &dices)
  {
    return new RandomPlayer(fields, name, dices);
  }
};

class TraditionalPlayerMaker : public PlayerMaker
{
 public:
  TraditionalPlayerMaker() = default;
  TraditionalPlayerMaker(const TraditionalPlayerMaker&) = delete;
  virtual ~TraditionalPlayerMaker() = default;

  [[nodiscard]] Doctor* make(Board& fields, const std::string &name,
               const std::unordered_map<std::string, Dice *> &dices)
  {
    return new TraditionalPlayer(fields, name, dices);
  }
};

class PredictablePlayerMaker : public PlayerMaker
{
private:
  DesiredFieldMaker *field_maker;

protected:
  virtual void maker_helper() = 0;

  [[nodiscard]] Doctor* make(Board& fields, const std::string &name,
               const std::unordered_map<std::string, Dice *> &dices)
  {
    field_maker->get_instances();
    return new TraditionalPlayer(fields, name, dices);
  }
 public:
  PredictablePlayerMaker() = default;
  PredictablePlayerMaker(const PredictablePlayerMaker&) = delete;
  virtual ~PredictablePlayerMaker() = default;
};

class ExperimentalPlayerMaker : public PlayerMaker
{
public:
  ExperimentalPlayerMaker() = default;
  ExperimentalPlayerMaker(const ExperimentalPlayerMaker&) = delete;
  virtual ~ExperimentalPlayerMaker() = default;

  [[nodiscard]] Doctor* make(Board& fields, const std::string &name,
               const std::unordered_map<std::string, Dice *> &dices)
  {
    return new ExperimentalPlayer(fields, name, dices);
  }
};
class SmartPlayerMaker : public PlayerMaker
{
 public:
  SmartPlayerMaker() = default;
  SmartPlayerMaker(const SmartPlayerMaker&) = delete;
  virtual ~SmartPlayerMaker() = default;

  [[nodiscard]] Doctor* make(Board& fields, const std::string &name,
               const std::unordered_map<std::string, Dice *> &dices)
  {
    return new SmartPlayer(fields, name, dices);
  }
};

class PlayerFactory
{
 private:
  std::unordered_map<std::string, PlayerMaker *> makers;

 public:
  PlayerFactory()
  {
    makers.emplace("L", new RandomPlayerMaker());
    makers.emplace("T", new TraditionalPlayerMaker());
    makers.emplace("E", new ExperimentalPlayerMaker());
    makers.emplace("R", new SmartPlayerMaker());
  }
  PlayerFactory(const PlayerFactory& ) = delete;
  [[nodiscard]] Doctor* produce(const std::string &doctor,
                                Board& fields, const std::string &name,
                                const std::unordered_map<std::string, Dice *> &dices)
  {
    return makers.at(doctor)->make(fields, name, dices);
  }

  ~PlayerFactory() { for (auto &[str, maker_ptr] : makers) delete maker_ptr; }
};

#endif