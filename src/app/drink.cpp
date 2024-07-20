#include "drink.h"

#include <iostream>

#include "message.h"
#include "utils.h"

using namespace std;

Drink::Drink(const Drink_Type drink_type) : Item() {
  drink_type_ = drink_type;
  type_ = Item::DRINK;

  if (drink_type == CAN) {
    price_ = 2;
  } else {
    price_ = 3;
  }
}

Drink::Drink_Type Drink::StringToDrinkType(
    const std::string &drink_type_string) {
  std::string drink_type_lower_cased = Utils::GetLowercase(drink_type_string);
  if (drink_type_lower_cased == "can") {
    return Drink_Type::CAN;
  } else {
    return Drink_Type::BOTTLE;
  }
}

int Drink::GetPrice() const { return price_; }

Item::Type Drink::GetType() const { return type_; }

Drink::~Drink() {}