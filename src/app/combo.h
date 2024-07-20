#ifndef COMBO_H
#define COMBO_H

#include <string>
#include <vector>

#include "burger.h"
#include "drink.h"
#include "item.h"

using namespace std;

class Combo : public Item {
 public:
  Combo(Burger::Size burger_size, Drink::Drink_Type drink_type);
  int GetPrice() const override;
  Type GetType() const override;

  virtual ~Combo() noexcept(true);
};

#endif  // COMBO_H