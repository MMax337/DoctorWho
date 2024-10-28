#include "../include/dice.h"


WearingOutDice::WearingOutDice() :
  max_probability(0.5),
  favorite_number(gen()%6 + 1),
  step(0.01),
  probabilities(6, 1/6.),
  dis(probabilities.begin(), probabilities.end())
{}


void WearingOutDice::restart() 
{
  favorite_number = gen()%6 + 1;
  std::vector<double> temp(6, 1/6.);
  std::swap(probabilities, temp);
  std::discrete_distribution<unsigned short> temp_dis(probabilities.begin(), probabilities.end());
  std::swap(dis, temp_dis);
}

uint8_t WearingOutDice::dice_throw() const 
{
  uint8_t roll = dis(gen) + 1;
  // favorite number serveы as an index of probabilities
  if (probabilities[favorite_number - 1] >= max_probability) return roll;

  // if fav num probability is not max_prob increase the probability of favorite number
  probabilities[favorite_number - 1] += step;
  // increasing fav num probability, we need to decrease uniformly the probabilities of other numbers
  double other_prob = (1 - probabilities[favorite_number - 1])/5;

  for(size_t i = 0; i < probabilities.size(); ++i)
  {
    if (i + 1 != favorite_number) probabilities[i] = other_prob;
  }
  dis = std::discrete_distribution<unsigned short>(probabilities.begin(), probabilities.end());
  return roll;
}
