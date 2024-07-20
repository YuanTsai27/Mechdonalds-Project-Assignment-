#ifndef APP_H
#define APP_H

#include <iostream>
#include <string>
#include <vector>

#include "account.h"
#include "burger.h"
#include "combo.h"
#include "drink.h"
#include "item.h"

using namespace std;

class App {
 private:
  vector<Account *> account_address_vector_;
  enum Mode { IDLE, ORDERING };
  Mode mode_;

  int next_order_count_;

  int current_account_index_;
  int current_order_num_;

  int order_total_;
  int burgers_in_order_;
  int drinks_in_order_;
  int combos_in_order_;

  int total_burgers_amount_;
  int total_drinks_amount_;

 public:
  App();
  void CreateNewAccount(const std::string &username,
                        const std::string &credit_amount);
  void TopUpAccount(const std::string &username,
                    const std::string &additional_credit);
  void PrintAccounts() const;
  void StartNewOrder(const std::string &username);
  void CancelCurrentOrder();
  void SubmitCurrentOrder();
  void AddBurgerToCurrentOrder(const std::string &burger_name,
                               const std::string &size);
  void AddDrinkToCurrentOrder(const std::string &drink_type);
  void AddComboToCurrentOrder(const std::string &burger_name,
                              const std::string &size,
                              const std::string &drink_type);
  void PrintAllSales() const;
};

#endif  // APP_H