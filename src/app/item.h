#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>

using namespace std;

class Item {
 public:
  enum Type { BURGER, DRINK, COMBO };

 protected:
  int price_;
  Type type_;

 public:
  Item();
  virtual int GetPrice() const = 0;
  virtual Type GetType() const = 0;

  virtual ~Item();
};

#endif  // ITEM_H