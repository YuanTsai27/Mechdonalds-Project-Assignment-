#include "combo.h"

#include <iostream>

#include "message.h"
#include "utils.h"

using namespace std;

Combo::Combo(Burger::Size burger_size, Drink::Drink_Type drink_type) : Item() {
  Burger Burger(burger_size);  // creating combo items on the stack
  Drink Drink(drink_type);

  type_ = Item::COMBO;
  price_ = Drink.GetPrice() + Burger.GetPrice() - 1;
}

int Combo::GetPrice() const { return price_; }

Item::Type Combo::GetType() const { return type_; }

Combo::~Combo() {}