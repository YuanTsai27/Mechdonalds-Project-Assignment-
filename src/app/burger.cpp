#include "burger.h"

#include <iostream>

#include "message.h"
#include "utils.h"

using namespace std;

Burger::Burger(const Size size) : Item() {
  size_ = size;
  type_ = Item::BURGER;

  if (size == SMALL) {
    price_ = 8;
  } else if (size == REGULAR) {
    price_ = 10;
  } else {
    price_ = 12;
  }
}

Burger::Size Burger::StringToSize(const std::string &burger_size) {
  std::string size_lower_cased = Utils::GetLowercase(burger_size);
  if (size_lower_cased == "small") {
    return Size::SMALL;
  } else if (size_lower_cased == "regular") {
    return Size::REGULAR;
  } else {
    return Size::LARGE;
  }
}

int Burger::GetPrice() const { return price_; }

Item::Type Burger::GetType() const { return type_; }

Burger::~Burger() {}