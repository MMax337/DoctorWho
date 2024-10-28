#include <cstddef>
#include <vector>

#include "../include/doctor.h"
#include "../include/board.h"
#include "../include/dice.h"
#include "../include/field_factory.h"


#ifdef NPRINTING
bool constexpr printing = false;
#else
bool constexpr printing = true;
#endif


namespace 
{
  const uint8_t MAX_DICE_POINT = 6;
  const uint8_t MAX_INCARNATION = 13;
}
// =================================================================
// =                                                               =
// =             Doctor Implementation                             =
// =                                                               =
// =================================================================

void Doctor::move_forward() { jumped = false; board.move_forward(dice_throw(), this); }

void Doctor::teloport_forward(uint8_t roll) { jumped = true; board.teloport_forward(roll, this); }

void Doctor::teloport_backward(uint8_t roll) { jumped = true; board.teloport_backward(roll, this); }

void Doctor::restart() noexcept
{
  incarnation = 9;
  win = false;
  jumped = false;
  board_position = 0;
}

// =================================================================
// =                                                               =
// =                     Random Player                             =
// =                                                               =
// =================================================================

RandomPlayer::RandomPlayer(Board& board, const std::string &name,
                            const std::unordered_map<std::string, Dice *> &dices_)
  : Doctor(board, name, dices_), gen(std::random_device{}())
{
  dices.reserve(dices_.size());
  for (const auto &[dice_name, dice_ptr] : dices_) dices.push_back(dice_ptr);
}


uint8_t RandomPlayer::dice_throw() const 
{ 
  auto pick = gen() % dices.size();
  auto result = dices[pick]->dice_throw();
  if constexpr (printing) std::cout << "I roll " << *dices[pick] << "and get " << +result << '\n';
  return result; 
}

// =================================================================
// =                                                               =
// =                     Traditional Player                        =
// =                                                               =
// =================================================================

TraditionalPlayer::TraditionalPlayer(
          Board& board, const std::string &name, 
                                       const std::unordered_map<std::string, Dice *> &dices_)
    : Doctor(board, name, dices_)
{
  dices.push_back(dices_.at("UsualDice"));
}

uint8_t TraditionalPlayer::dice_throw() const 
{
  auto result = dices[0]->dice_throw();
  if constexpr (printing) std::cout << "I roll " << *dices[0] << "and get " << +result << '\n';
  return result; 
}



std::optional<uint8_t> PredictablePlayer::check_for_desired_fields() const 
{
  return board.check_for_fields(board_position, *desired_fields);
};




// =================================================================
// =                                                               =
// =                     Experimental Player                       =
// =                                                               =
// =================================================================



ExperimentalPlayer::ExperimentalPlayer(Board& board,
                  const std::string& name, const std::unordered_map<std::string, Dice*>& dices_)
: PredictablePlayer(board, name, dices_)
{
  desired_fields = RegenerationFieldMaker::get_maker()->get_instances();
  dices.push_back(dices_.at("WearingOutDice"));
}

uint8_t ExperimentalPlayer::dice_throw() const
{
  auto roll = dices[0]->dice_throw();
  if constexpr (printing) std::cout << "I roll " << *dices[0] << "and get " << +roll << '\n';
  if (roll == MAX_DICE_POINT and get_incarnation() != MAX_INCARNATION) 
  {
    if (auto new_roll = check_for_desired_fields(); new_roll) 
    {
      roll = *new_roll;
      if constexpr (printing) std::cout << "Desired field is " << +roll << " away, I go there!\n";
    }
  }
  return roll;
};



// =================================================================
// =                                                               =
// =                     Smart Player                              =
// =                                                               =
// =================================================================

SmartPlayer::SmartPlayer(Board& board,
                  const std::string& name, const std::unordered_map<std::string, Dice*>& dices_)
: PredictablePlayer(board, name, dices_), one_repeated(0), using_usual_dice(false)
{
  desired_fields = RegenerationFieldMaker::get_maker()->get_instances();
  dices.push_back(dices_.at("WearingOutDice"));
  dices.push_back(dices_.at("UsualDice"));
  dices.push_back(dices_.at("FaultyDice"));
}

uint8_t SmartPlayer::faulty_dice_throw(uint8_t roll) const 
{
  auto result = dices[2]->dice_throw();
  if constexpr (printing) std::cout << "My desired field is " << +roll 
                                    << " away. I roll " << *dices[2] 
                                    << "and get "<< +result << "\n";
  return result == MAX_DICE_POINT ? result : roll;
}


uint8_t SmartPlayer::usual_dice_throw() const 
{
  auto result = dices[1]->dice_throw();
  if constexpr (printing) std::cout<< "I roll " << *dices[1] << " and get " << +result << '\n';
  return result; 
}

uint8_t SmartPlayer::wearing_out_dice_throw() const 
{
  auto result = dices[0]->dice_throw();
  one_repeated = (result == 1) ? one_repeated + 1 : 0;
  // if one has repeated more than 4 times in a row SmartPlayer starts using usual dice
  if (one_repeated >= 4) using_usual_dice = true;

  if constexpr (printing) std::cout<< "I roll " << *dices[0] << " and get " << +result << '\n';
  return result;
}

uint8_t SmartPlayer::dice_throw() const
{
  if (auto roll = check_for_desired_fields(); roll) return faulty_dice_throw(*roll);

  else if (using_usual_dice) return usual_dice_throw();

  else return wearing_out_dice_throw(); 

};

 


