#include "account.h"

#include <iostream>

#include "message.h"
#include "utils.h"

using namespace std;

int Account::next_account_number_ = 0;

Account::Account(const std::string &username,
                 const std::string &credit_amount) {
  username_ = username;
  credit_amount_ = credit_amount;
  account_number_ = next_account_number_;
  next_account_number_++;
  orders_cancelled_ = orders_completed_ = 0;
}

std::string Account::GetUsername() const { return username_; }

std::string Account::GetCredit() const { return credit_amount_; }

int Account::GetAccNumber() const { return account_number_; }

int Account::GetCompletedOrders() const { return orders_completed_; }

int Account::GetCancelledOrders() const { return orders_cancelled_; }

void Account::TopUpCredit(const std::string &credit_amount) {
  int new_credit_amount = stoi(credit_amount_) + stoi(credit_amount);
  credit_amount_ = to_string(new_credit_amount);
}

void Account::DeductCredit(int deduct_amount) {
  int new_credit_amount = stoi(credit_amount_) - deduct_amount;
  credit_amount_ = to_string(new_credit_amount);
}

void Account::IncrementCompletedOrders() { orders_completed_++; }
void Account::IncrementCancelledOrders() { orders_cancelled_++; }

Account::~Account() {}