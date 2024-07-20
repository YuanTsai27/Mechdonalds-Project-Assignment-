#ifndef BURGER_H
#define BURGER_H

#include <string>
#include <vector>

#include "item.h"

using namespace std;

class Burger : public Item {
 public:
  enum Size { SMALL, REGULAR, LARGE };
  static Size StringToSize(const std::string &size_string);

 private:
  Size size_;

 public:
  Burger(const Size size);
  int GetPrice() const override;
  Type GetType() const override;

  virtual ~Burger();
};

#endif  // BURGER_H