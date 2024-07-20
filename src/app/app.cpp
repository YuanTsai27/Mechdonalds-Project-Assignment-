#include "app.h"

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "message.h"
#include "utils.h"

using namespace std;

App::App() {
  account_address_vector_ = {};
  mode_ = IDLE;
  next_order_count_ = 1;
  total_burgers_amount_ = total_drinks_amount_ = 0;
}

void App::CreateNewAccount(const std::string &username_input,
                           const std::string &credit_amount) {
  /* possible rejection scenarios
  - username less than 2 letters
  - account username already exists
  - input credit not whole number (but account creation proceeds)
  */

  if (mode_ == ORDERING) {
    Message::INVALID_COMMAND_ORDER_OPEN.PrintMessage();
    return;
  }

  string m1 = "initialisation";  // initialisating messages to print
  string m2 = "initialisation";
  bool is_username_duplicated =
      false;  // setting up boolean variables as conditionals
  bool is_credit_invalid = false;
  int credit_int = 0;  // setting credit number to 0
  std::string username_lower_cased =
      Utils::GetLowercase(username_input);  // lowercasing username

  /*
hierachy of error messages
1. username less than 2 letters
2. username already exists
3. credit is negative
4. credit is invalid

*/

  // process to check if input credit is valid

  for (int i = 0; i < credit_amount.length(); i++) {
    if ((i == 0 && credit_amount[i] == '-')) {  // checking if input is negative
      is_credit_invalid = true;
      m1 = Message::ERROR_CREATE_ACCOUNT_NEGATIVE_CREDIT.GetMessage(
          {to_string(credit_int)});
      break;
    } else if (credit_amount[i] < '0' ||
               credit_amount[i] > '9') {  // checking if input has any other
                                          // non-number characters
      is_credit_invalid = true;
      m1 = Message::ERROR_CREATE_ACCOUNT_INVALID_CREDIT.GetMessage(
          {credit_amount, to_string(credit_int)});
      break;
    }
  }

  if (!is_credit_invalid) {
    credit_int = std::stoi(credit_amount);  // initiallising the valid credit
                                            // input for print output

    // if credit input is not valid, credit_int remains as 0 as initialised
  }

  // process to check if account is duplicated
  for (int i = 0; i < account_address_vector_.size(); i++) {
    if ((account_address_vector_[i]->Account::GetUsername() ==
         username_lower_cased))
      is_username_duplicated = true;
  }

  if (username_input.length() < 2) {
    m2 = Message::ERROR_CREATE_ACCOUNT_SHORT_USERNAME.GetMessage(
        {username_lower_cased,
         to_string(credit_int)});  // setting appropriate string message
  } else if (is_username_duplicated) {
    m2 = Message::ERROR_CREATE_ACCOUNT_DUPLICATE_USERNAME.GetMessage(
        {username_lower_cased, to_string(credit_int)});
  } else {
    Account *new_account = new Account(
        username_lower_cased,
        to_string(credit_int));  // creating account instance on heap

    account_address_vector_.push_back(
        new_account);  // registering instance pointer into static vector

    m2 = Message::ACCOUNT_CREATED.GetMessage(
        {username_lower_cased, to_string(credit_int)});
  }

  // printing relevant outputs
  if (is_credit_invalid) {
    cout << m1 << endl;  // printing 2 strings for invalid credit input,
                         // otherwise just one.
  }

  cout << m2 << endl;
}

void App::TopUpAccount(const std::string &username_input,
                       const std::string &additional_credit) {
  if (mode_ == ORDERING) {
    Message::INVALID_COMMAND_ORDER_OPEN.PrintMessage();
    return;
  }

  bool does_username_exist = false;
  bool is_credit_invalid = false;

  int index;
  std::string username_lower_cased = Utils::GetLowercase(username_input);
  std::string additional_credit_copy = additional_credit;

  string m = Message::USERNAME_NOT_FOUND.GetMessage({username_lower_cased});
  // initialisation, m will change is username is valid.

  /*hierachy of error messages
  1. username not found
  2. negative topup
  3. invalid topup
  */

  for (int i = 0; i < account_address_vector_.size(); i++) {
    if ((account_address_vector_[i]->Account::GetUsername() ==
         username_lower_cased))
      does_username_exist = true;
    index = i;  // store this index position for later use in accessing the
                // correct address for topping up credit value.

    break;
  }

  if (does_username_exist) {  // if username is valid, proceed to topup credit
                              // check

    for (int i = 0; i < additional_credit.length(); i++) {
      if ((i == 0 && additional_credit[i] ==
                         '-')) {  // checking if input credit is negative
        is_credit_invalid = true;
        m = Message::ERROR_TOPUP_NEGATIVE_CREDIT.GetMessage({});
        break;
      } else if (additional_credit[i] < '0' ||
                 additional_credit[i] >
                     '9') {  // checking for any other non-number characters
        is_credit_invalid = true;
        m = Message::ERROR_TOPUP_INVALID_CREDIT.GetMessage({additional_credit});
        break;
      }
    }
  }

  if (!is_credit_invalid &&
      does_username_exist) {  // if conditions are satisfied, then top up
    account_address_vector_[index]->Account::TopUpCredit(
        additional_credit_copy);

    // existing_balance_int = credit_balance_vector_.at(index);
    // new_credit_amount = existing_balance_int + stoi(additional_credit);

    // credit_balance_vector_.at(index) = new_credit_amount;

    m = Message::TOP_UP_SUCCESSFUL.GetMessage(
        {username_lower_cased, additional_credit,
         account_address_vector_[index]->Account::GetCredit()});
  }

  cout << m << endl;
}

void App::PrintAccounts() const {
  if (mode_ == ORDERING) {
    Message::INVALID_COMMAND_ORDER_OPEN.PrintMessage();
    return;
  }

  string a, b, c, d, e;  // %s inputs for string templates
  string m1, m2;

  std::string username, credit_balance;
  int num_accounts = account_address_vector_.size();

  int orders_completed;
  int orders_cancelled;

  // b represents number of accounts made
  b = to_string(account_address_vector_.size());

  // setting a and c based on plural grammars
  if (num_accounts == 1) {
    a = "is";
    c = "";
  } else {
    a = "are";
    c = "s";
  }

  // setting d based on punctuation grammar

  if (num_accounts > 0) {
    d = ":";
  } else {
    d = ".";
  }

  m1 = Message::PRINT_ACCOUNTS_HEADER.GetMessage({a, b, c, d});
  cout << m1 << endl;

  for (int i = 0; i < num_accounts;
       i++) {  // printing out each account with their respective details line
               // by line
    credit_balance = account_address_vector_[i]->Account::GetCredit();
    username = account_address_vector_[i]->Account::GetUsername();
    orders_completed =
        account_address_vector_[i]->Account::GetCompletedOrders();
    orders_cancelled =
        account_address_vector_[i]->Account::GetCancelledOrders();

    if (orders_completed == 1) {
      e = "";
    } else {
      e = "s";
    }

    m2 = Message::PRINT_ACCOUNT_ENTRY.GetMessage(
        {username, credit_balance, to_string(orders_completed), e,
         to_string(orders_cancelled)});

    cout << m2 << endl;
  }
}

void App::StartNewOrder(const std::string &username) {
  if (mode_ == ORDERING) {
    Message::INVALID_COMMAND_ORDER_OPEN.PrintMessage();
    return;
  }

  bool does_username_exist = false;
  int index;

  std::string username_lower_cased = Utils::GetLowercase(username);

  string m = Message::USERNAME_NOT_FOUND.GetMessage({username_lower_cased});
  // initialisation, m will change is username is valid.

  // check if account username exists, otherwise return message

  for (int i = 0; i < account_address_vector_.size(); i++) {
    if ((account_address_vector_[i]->Account::GetUsername() ==
         username_lower_cased)) {
      does_username_exist = true;
      index = i;  // store this index position for later use in accessing the
                  // correct address

      break;
    }
  }

  if (does_username_exist) {
    // enter system into Ordering mode
    mode_ = ORDERING;
    current_order_num_ = next_order_count_;
    m = Message::ORDER_STARTED.GetMessage(
        {to_string(current_order_num_), username_lower_cased});
    next_order_count_++;
    current_account_index_ = index;

    order_total_ = burgers_in_order_ = drinks_in_order_ = combos_in_order_ = 0;
  }
  cout << m << endl;
}

void App::CancelCurrentOrder() {
  if (mode_ == IDLE) {
    Message::INVALID_COMMAND_ORDER_CLOSED.PrintMessage();
    return;
  }

  account_address_vector_[current_account_index_]
      ->Account::IncrementCancelledOrders();

  Message::ORDER_CANCELLED.PrintMessage({to_string(current_order_num_)});

  mode_ = IDLE;
}

void App::SubmitCurrentOrder() {
  if (mode_ == IDLE) {
    Message::INVALID_COMMAND_ORDER_CLOSED.PrintMessage();
    return;
  }

  std::string m =
      Message::ORDER_CANCELLED.GetMessage({to_string(current_order_num_)});
  bool sufficient_funds = (stoi(account_address_vector_[current_account_index_]
                                    ->Account::GetCredit()) >= order_total_);

  std::string a = "s";

  if ((burgers_in_order_ + drinks_in_order_ + combos_in_order_) == 1) {
    a = "";
  }

  if ((order_total_ == 0) || !sufficient_funds) {
    account_address_vector_[current_account_index_]
        ->Account::IncrementCancelledOrders();

    if (!sufficient_funds) {
      std::string order_num_string = to_string(current_order_num_);
      std::string total_items_string =
          to_string(burgers_in_order_ + drinks_in_order_ + combos_in_order_);
      Message::ORDER_SUBMIT_INSUFFICIENT_CREDIT.PrintMessage(
          {order_num_string, total_items_string, a});
    }
  } else {
    account_address_vector_[current_account_index_]
        ->Account::IncrementCompletedOrders();
    account_address_vector_[current_account_index_]->Account::DeductCredit(
        order_total_);

    total_burgers_amount_ += (burgers_in_order_ + combos_in_order_);
    total_drinks_amount_ += (drinks_in_order_ + combos_in_order_);

    std::string order_num_string = to_string(current_order_num_);
    std::string total_items_string =
        to_string(burgers_in_order_ + drinks_in_order_ + combos_in_order_);

    m = Message::ORDER_SUBMITTED.GetMessage(
        {order_num_string, total_items_string, a});
  }

  cout << m << endl;

  mode_ = IDLE;
}

void App::AddBurgerToCurrentOrder(const std::string &burger_name,
                                  const std::string &size) {
  if (mode_ == IDLE) {
    Message::INVALID_COMMAND_ORDER_CLOSED.PrintMessage();
    return;
  }

  Burger new_burger(Burger::StringToSize(size));
  order_total_ += new_burger.GetPrice();
  burgers_in_order_++;

  Message::ADDED_BURGER.PrintMessage({burger_name, Utils::GetLowercase(size),
                                      to_string(current_order_num_),
                                      to_string(new_burger.GetPrice())});
}

void App::AddDrinkToCurrentOrder(const std::string &drink_type) {
  if (mode_ == IDLE) {
    Message::INVALID_COMMAND_ORDER_CLOSED.PrintMessage();
    return;
  }

  Drink new_drink(Drink::StringToDrinkType(drink_type));
  order_total_ += new_drink.GetPrice();
  drinks_in_order_++;

  Message::ADDED_DRINK.PrintMessage({drink_type, to_string(current_order_num_),
                                     to_string(new_drink.GetPrice())});
}

void App::AddComboToCurrentOrder(const std::string &burger_name,
                                 const std::string &size,
                                 const std::string &drink_type) {
  if (mode_ == IDLE) {
    Message::INVALID_COMMAND_ORDER_CLOSED.PrintMessage();
    return;
  }

  Combo new_combo(Burger::StringToSize(size),
                  Drink::StringToDrinkType(drink_type));
  order_total_ += new_combo.GetPrice();
  combos_in_order_++;

  Message::ADDED_COMBO.PrintMessage({burger_name, size, drink_type,
                                     to_string(current_order_num_),
                                     to_string(new_combo.GetPrice())});
}

void App::PrintAllSales() const {
  if (mode_ == ORDERING) {
    Message::INVALID_COMMAND_ORDER_CLOSED.PrintMessage();
    return;
  }

  Message::PRINT_TOTAL_SALES.PrintMessage(
      {to_string(total_burgers_amount_), to_string(total_drinks_amount_)});
}