#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

using namespace std;

class Account {
 private:
  std::string username_;
  std::string credit_amount_;
  int account_number_;
  static int next_account_number_;

  // checkpoint 2
  int orders_completed_;
  int orders_cancelled_;

 public:
  Account(const std::string &username, const std::string &credit_amount);

  // getters
  std::string GetUsername() const;
  std::string GetCredit() const;
  int GetAccNumber() const;
  int GetCompletedOrders() const;
  int GetCancelledOrders() const;

  void TopUpCredit(const std::string &credit_amount);
  void DeductCredit(int deduct_amount);

  // incrementers
  void IncrementCompletedOrders();
  void IncrementCancelledOrders();

  virtual ~Account();
};

#endif  // ACCOUNT_H