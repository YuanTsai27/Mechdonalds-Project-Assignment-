#ifndef DRINK_H
#define DRINK_H

#include <string>
#include <vector>

#include "item.h"

using namespace std;

class Drink : public Item {
 public:
  enum Drink_Type { CAN, BOTTLE };
  static Drink_Type StringToDrinkType(const std::string &drink_type_string);

 private:
  Drink_Type drink_type_;

 public:
  Drink(const Drink_Type drink_type);
  int GetPrice() const override;
  Type GetType() const override;

  virtual ~Drink();
};

#endif  // DRINK_H