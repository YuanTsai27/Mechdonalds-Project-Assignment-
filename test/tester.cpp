
#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "app/launcher.h"
#include "app/utils.h"

using namespace std;

/*
*****************************
  DO NOT MODIFY EXISTING METHODS
  However, you are welcome to add additional test cases.
*****************************
*/

enum TestResult { TEST_RESULT_PASS, TEST_RESULT_FAIL };

#define RUN_COMMAND(command_with_args)                         \
  cout << app.GetCommandPrefix() << command_with_args << endl; \
  app.ProcessCommand(command_with_args);

#define __FILENAME__ \
  (std::strrchr(__FILE__, '/') ? std::strrchr(__FILE__, '/') + 1 : __FILE__)

#define EXPECT_CONTAINS(expected)                                              \
  if (still_capturing_cout) {                                                  \
    still_capturing_cout = false;                                              \
    output = buffer.str();                                                     \
    std::cout.rdbuf(old);                                                      \
    std::cout << output << std::endl;                                          \
  }                                                                            \
  if (output.find(expected) == std::string::npos) {                            \
    cerr << "FAILED Test " << test_number << " in " << __FILENAME__            \
         << " on line " << __LINE__ << endl                                    \
         << "\tExpected '" << expected                                         \
         << "' in the output, but it was not found." << endl                   \
         << endl                                                               \
         << "----------------------------------------------------------------" \
            "------------"                                                     \
         << endl                                                               \
         << endl;                                                              \
    return TEST_RESULT_FAIL;                                                   \
  }

#define EXPECT_DOES_NOT_CONTAINS(expected, ignore_case)             \
  output = buffer.str();                                            \
  if (Utils::Contains(output, expected, ignore_case)) {             \
    cerr << "FAILED Test " << test_number << " in " << __FILENAME__ \
         << " on line " << __LINE__ << endl                         \
         << "\tExpected '" << expected                              \
         << "' to NOT be in the output, but it was." << endl        \
         << endl;                                                   \
    return TEST_RESULT_FAIL;                                        \
  }

int main(int argc, char *argv[]) {
  Launcher app;
  string output = "";
  bool still_capturing_cout = true;
  std::stringstream buffer;
  std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

  int test_number = std::stoi(argv[1]);
  switch (test_number) {
    case 1:  // Task_1_ZeroAccounts
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("There are 0 accounts.");

      break;
    case 2:  // Task_1_CreateNewAccount
      RUN_COMMAND("NEW_ACCOUNT Alex 100");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Account successfully created for 'alex' with starting credit of "
          "$100.");
      EXPECT_DOES_NOT_CONTAINS("there is", true);
      EXPECT_DOES_NOT_CONTAINS("there are", true);
      break;
    case 3:  // Task_1_CreateNewAccountWithHeader
      RUN_COMMAND("NEW_ACCOUNT Alex 100");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Account successfully created for 'alex' with starting credit of "
          "$100.");
      EXPECT_CONTAINS("There is 1 account:");
      EXPECT_DOES_NOT_CONTAINS("There are 0 account", true);
      break;
    case 4:  // Task_1_CreateTwoNewAccountsWithoutPrint
      RUN_COMMAND("NEW_ACCOUNT Jenny 100");
      RUN_COMMAND("NEW_ACCOUNT Alex 25");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Account successfully created for 'jenny' with starting credit of "
          "$100");
      EXPECT_CONTAINS(
          "Account successfully created for 'alex' with starting credit of "
          "$25");

      EXPECT_DOES_NOT_CONTAINS("Cannot create account", true);
      EXPECT_DOES_NOT_CONTAINS("there is", true);
      EXPECT_DOES_NOT_CONTAINS("there are", true);
      break;
    case 5:  // Task_1_CreateTwoNewAccountsWithPrint
      RUN_COMMAND("NEW_ACCOUNT Jenny 100");
      RUN_COMMAND("NEW_ACCOUNT Alex 25");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Account successfully created for 'jenny' with starting credit of "
          "$100");
      EXPECT_CONTAINS(
          "Account successfully created for 'alex' with starting credit of "
          "$25");
      EXPECT_CONTAINS("There are 2 accounts:");

      EXPECT_DOES_NOT_CONTAINS("Cannot create account", true);
      EXPECT_DOES_NOT_CONTAINS("0 account", true);
      EXPECT_DOES_NOT_CONTAINS("1 account", true);
      break;
    case 6:  // Task_1_CreateNewAccountDuplicate
      RUN_COMMAND("NEW_ACCOUNT Alex 100");
      RUN_COMMAND("NEW_ACCOUNT Alex 25");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Account successfully created for 'alex' with starting credit of "
          "$100");
      EXPECT_CONTAINS(
          "Cannot create account for 'alex' with $25. Account already exists.");
      EXPECT_CONTAINS("There is 1 account:");

      EXPECT_DOES_NOT_CONTAINS("0 account", true);
      EXPECT_DOES_NOT_CONTAINS("2 account", true);
      break;
    case 7:  // Task_1_CreateNewAccountDuplicateCasing
      RUN_COMMAND("NEW_ACCOUNT alex 100");
      RUN_COMMAND("NEW_ACCOUNT AleX 25");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Account successfully created for 'alex' with starting credit of "
          "$100");
      EXPECT_CONTAINS(
          "Cannot create account for 'alex' with $25. Account already exists.");
      EXPECT_CONTAINS("There is 1 account:");

      EXPECT_DOES_NOT_CONTAINS("0 account", true);
      EXPECT_DOES_NOT_CONTAINS("2 account", true);
      break;
    case 8:  // Task_1_CreateNewAccountShortUsername
      RUN_COMMAND("NEW_ACCOUNT A 100");
      RUN_COMMAND("NEW_ACCOUNT b 25");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Cannot create account for 'a' with $100. Username must be at least "
          "2 characters long.");
      EXPECT_CONTAINS(
          "Cannot create account for 'b' with $25. Username must be at least 2 "
          "characters long.");
      EXPECT_CONTAINS("There are 0 accounts.");

      EXPECT_DOES_NOT_CONTAINS("Account successfully created", true);
      EXPECT_DOES_NOT_CONTAINS("1 account", true);
      EXPECT_DOES_NOT_CONTAINS("2 account", true);
      break;
    case 9:  // Task_1_CreateNewAccountNegative
      RUN_COMMAND("NEW_ACCOUNT Alex -10");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Cannot have a negative initial credit. Using $0 instead.");
      EXPECT_CONTAINS(
          "Account successfully created for 'alex' with starting credit of "
          "$0.");
      EXPECT_CONTAINS("There is 1 account:");

      EXPECT_DOES_NOT_CONTAINS("0 account", true);
      EXPECT_DOES_NOT_CONTAINS("starting credit of $-10", true);
      EXPECT_DOES_NOT_CONTAINS("invalid credit amount", true);
      break;
    case 10:  // Task_1_CreateNewAccountInvalid
      RUN_COMMAND("NEW_ACCOUNT Alex ten");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("'ten' is an invalid credit amount. Using $0 instead.");
      EXPECT_CONTAINS(
          "Account successfully created for 'alex' with starting credit of "
          "$0.");
      EXPECT_CONTAINS("There is 1 account:");

      EXPECT_DOES_NOT_CONTAINS("0 account", true);
      EXPECT_DOES_NOT_CONTAINS("starting credit of $ten", true);
      EXPECT_DOES_NOT_CONTAINS("negative initial credit", true);
      break;
    case 11:  // Task_1_CreateNewTwoAccountWithPrintAccountDetails
      RUN_COMMAND("NEW_ACCOUNT Jenny 100");
      RUN_COMMAND("NEW_ACCOUNT Alex 25");
      RUN_COMMAND("NEW_ACCOUNT Jen 55");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("There are 3 accounts:");
      EXPECT_CONTAINS(
          "*) 'jenny' has $100 credit and has completed 0 orders (0 "
          "cancelled)");
      EXPECT_CONTAINS(
          "*) 'alex' has $25 credit and has completed 0 orders (0 cancelled)");
      EXPECT_CONTAINS(
          "*) 'jen' has $55 credit and has completed 0 orders (0 cancelled)");

      EXPECT_DOES_NOT_CONTAINS("0 account", true);
      EXPECT_DOES_NOT_CONTAINS("1 account", true);
      EXPECT_DOES_NOT_CONTAINS("2 account", true);
      break;
    case 12:  // Task_1_CreateNewTwoAccountWithoutPrintAccountDetails
      RUN_COMMAND("NEW_ACCOUNT Jenny 100");
      RUN_COMMAND("NEW_ACCOUNT Alex 25");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Account successfully created");
      EXPECT_DOES_NOT_CONTAINS("There are", true);
      EXPECT_DOES_NOT_CONTAINS("There is", true);
      EXPECT_DOES_NOT_CONTAINS("has $", true);
      break;
    case 13:  // Task_1_TopUpValidWithoutPrintAccounts
      RUN_COMMAND("NEW_ACCOUNT Alex 100");
      RUN_COMMAND("TOP_UP_CREDIT Alex 50");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Account successfully created for 'alex' with starting credit of "
          "$100.");
      EXPECT_CONTAINS(
          "Successfully topped up 'alex' with $50. Current balance is $150.");

      EXPECT_DOES_NOT_CONTAINS("0 account", true);
      EXPECT_DOES_NOT_CONTAINS("1 account", true);
      EXPECT_DOES_NOT_CONTAINS("2 account", true);
      EXPECT_DOES_NOT_CONTAINS("Successfully topped up 'alex' with $100", true);
      EXPECT_DOES_NOT_CONTAINS("Current balance is $100", true);
      EXPECT_DOES_NOT_CONTAINS("Current balance is $200", true);
      EXPECT_DOES_NOT_CONTAINS("has $", true);
      break;
    case 14:  // Task_1_TopUpValid
      RUN_COMMAND("NEW_ACCOUNT Alex 100");
      RUN_COMMAND("TOP_UP_CREDIT Alex 50");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Account successfully created for 'alex' with starting credit of "
          "$100.");
      EXPECT_CONTAINS(
          "Successfully topped up 'alex' with $50. Current balance is $150.");
      EXPECT_CONTAINS("There is 1 account:");

      EXPECT_CONTAINS(
          "*) 'alex' has $150 credit and has completed 0 orders (0 cancelled)");

      EXPECT_DOES_NOT_CONTAINS("0 account", true);
      EXPECT_DOES_NOT_CONTAINS("2 account", true);
      EXPECT_DOES_NOT_CONTAINS("Successfully topped up 'alex' with $100", true);
      EXPECT_DOES_NOT_CONTAINS("Current balance is $100", true);
      EXPECT_DOES_NOT_CONTAINS("Current balance is $200", true);
      EXPECT_DOES_NOT_CONTAINS(
          "'alex' has $100 credit and has completed 0 orders (0 cancelled)",
          true);
      EXPECT_DOES_NOT_CONTAINS(
          "'alex' has $200 credit and has completed 0 orders (0 cancelled)",
          true);
      break;
    case 15:  // Task_1_TopUpNegativeWithPrintAccounts
      RUN_COMMAND("NEW_ACCOUNT Alex 100");
      RUN_COMMAND("TOP_UP_CREDIT Alex -50");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Cannot have a negative top up amount. Credit has not changed.");
      EXPECT_CONTAINS("There is 1 account:");
      EXPECT_CONTAINS(
          "*) 'alex' has $100 credit and has completed 0 orders (0 cancelled)");

      EXPECT_DOES_NOT_CONTAINS("0 account", true);
      EXPECT_DOES_NOT_CONTAINS("2 account", true);
      EXPECT_DOES_NOT_CONTAINS("Successfully topped up 'alex' with $100", true);
      EXPECT_DOES_NOT_CONTAINS("Current balance is $50", true);
      EXPECT_DOES_NOT_CONTAINS("Current balance is $150", true);
      EXPECT_DOES_NOT_CONTAINS(
          "'alex' has $50 credit and has completed 0 orders (0 cancelled)",
          true);
      EXPECT_DOES_NOT_CONTAINS(
          "'alex' has $150 credit and has completed 0 orders (0 cancelled)",
          true);
      EXPECT_DOES_NOT_CONTAINS("is an invalid top up amount", true);
      break;
    case 16:  // Task_1_TopUpNegativeWithoutPrintAccounts
      RUN_COMMAND("NEW_ACCOUNT Alex 100");
      RUN_COMMAND("TOP_UP_CREDIT Alex -50");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Cannot have a negative top up amount. Credit has not changed.");

      EXPECT_DOES_NOT_CONTAINS("0 account", true);
      EXPECT_DOES_NOT_CONTAINS("1 account", true);
      EXPECT_DOES_NOT_CONTAINS("2 account", true);
      EXPECT_DOES_NOT_CONTAINS("and has completed 0 orders", true);
      break;
    case 17:  // Task_1_TopUpUsernameNotFound
      RUN_COMMAND("NEW_ACCOUNT Alex 100");
      RUN_COMMAND("TOP_UP_CREDIT Jen 50");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Cannot find an account with username 'jen'.");

      EXPECT_DOES_NOT_CONTAINS("Successfully topped up", true);
      EXPECT_DOES_NOT_CONTAINS("Current balance is", true);
      break;
    case 18:  // Task_1_TopUpUsernameShouldBeFound
      RUN_COMMAND("NEW_ACCOUNT Alex 10");
      RUN_COMMAND("TOP_UP_CREDIT aLeX 20");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Account successfully created for 'alex' with starting credit of "
          "$10.");
      EXPECT_CONTAINS(
          "Successfully topped up 'alex' with $20. Current balance is $30.");

      EXPECT_DOES_NOT_CONTAINS("There is 1 account:", true);
      EXPECT_DOES_NOT_CONTAINS(
          "*) 'alex' has $30 credit and has completed 0 orders (0 cancelled)",
          true);

      EXPECT_DOES_NOT_CONTAINS("0 account", true);
      EXPECT_DOES_NOT_CONTAINS("2 account", true);
      EXPECT_DOES_NOT_CONTAINS("Successfully topped up 'Alex' with $100",
                               false);
      EXPECT_DOES_NOT_CONTAINS("Successfully topped up 'aLeX' with $100",
                               false);
      EXPECT_DOES_NOT_CONTAINS("Current balance is $10", true);
      EXPECT_DOES_NOT_CONTAINS("Current balance is $20", true);
      EXPECT_DOES_NOT_CONTAINS(
          "'alex' has $10 credit and has completed 0 orders (0 cancelled)",
          true);
      EXPECT_DOES_NOT_CONTAINS(
          "'alex' has $20 credit and has completed 0 orders (0 cancelled)",
          true);
      break;
    case 19:  // Task_2_StartNewOrderUsernameExists
      RUN_COMMAND("NEW_ACCOUNT Alex 10");
      RUN_COMMAND("NEW_ORDER Alex");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Order #1 initiated for 'alex'. Please add your items.");

      EXPECT_DOES_NOT_CONTAINS("Cannot find account with username", true);
      break;
    case 20:  // Task_2_StartNewOrderUsernameDoesNotExist
      RUN_COMMAND("NEW_ACCOUNT Alex 10");
      RUN_COMMAND("NEW_ORDER Jenny");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Cannot find an account with username 'jenny'.");

      EXPECT_DOES_NOT_CONTAINS("initiated for", true);
      EXPECT_DOES_NOT_CONTAINS("Please add your items", true);
      break;
    case 21:  // Task_2_CancelOrder
      RUN_COMMAND("NEW_ACCOUNT Alex 10");
      RUN_COMMAND("NEW_ORDER Alex");
      RUN_COMMAND("CANCEL_ORDER");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Order #1 initiated for 'alex'. Please add your items.");
      EXPECT_CONTAINS("Order #1 has been cancelled.");

      EXPECT_DOES_NOT_CONTAINS("Cannot find account with username", true);
      break;
    case 22:  // Task_2_FinishEmptyOrder
      RUN_COMMAND("NEW_ACCOUNT Alex 10");
      RUN_COMMAND("NEW_ORDER Alex");
      RUN_COMMAND("SUBMIT_ORDER");  // Nothing has been added, so treated as a
                                    // cancel order
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Order #1 initiated for 'alex'. Please add your items.");
      EXPECT_CONTAINS("Order #1 has been cancelled.");

      EXPECT_DOES_NOT_CONTAINS("Cannot find account with username", true);
      break;
    case 23:  // Task_2_NewOrderOpenInvalidCommands
      RUN_COMMAND("NEW_ACCOUNT Alex 10");
      RUN_COMMAND("NEW_ORDER Alex");

      // Invalid commands with open orders:
      RUN_COMMAND("NEW_ACCOUNT Jenny 10");
      RUN_COMMAND("TOP_UP_CREDIT Alex 10");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("NEW_ORDER Jenny");
      RUN_COMMAND("PRINT_ACCOUNT Alex");
      RUN_COMMAND("PRINT_ALL_SALES");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Invalid command. You cannot do this while an order is open.");

      EXPECT_DOES_NOT_CONTAINS("Account successfully created for 'jenny'",
                               true);
      EXPECT_DOES_NOT_CONTAINS("Successfully topped up", true);
      EXPECT_DOES_NOT_CONTAINS("There is", true);
      EXPECT_DOES_NOT_CONTAINS("There are", true);
      EXPECT_DOES_NOT_CONTAINS("Order #2 initiated", true);
      EXPECT_DOES_NOT_CONTAINS("initiated for 'jenny'", true);
      EXPECT_DOES_NOT_CONTAINS("has order", true);
      EXPECT_DOES_NOT_CONTAINS("total sales", true);
      break;
    case 24:  // Task_2_NoOrderOpenInvalidCommands
      RUN_COMMAND("NEW_ACCOUNT Alex 10");
      RUN_COMMAND("NEW_ORDER Alex");
      RUN_COMMAND("CANCEL_ORDER");

      // Invalid commands without an open order:
      RUN_COMMAND("CANCEL_ORDER");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("ADD_BURGER Vegemite small");
      RUN_COMMAND("ADD_DRINK can");
      RUN_COMMAND("ADD_COMBO Vegemite small can");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Invalid command. You cannot do this without an open order.");

      EXPECT_DOES_NOT_CONTAINS("2 has been cancelled", true);
      EXPECT_DOES_NOT_CONTAINS("has been submitted", true);
      EXPECT_DOES_NOT_CONTAINS("Added burger", true);
      EXPECT_DOES_NOT_CONTAINS("Added drink", true);
      EXPECT_DOES_NOT_CONTAINS("Added combo", true);
      break;
    case 25:  // Task_2_MultiOrdersDifferentUsers
      RUN_COMMAND("NEW_ACCOUNT Alex 10");
      RUN_COMMAND("NEW_ACCOUNT Jenny 10");
      RUN_COMMAND("NEW_ACCOUNT Jude 10");
      RUN_COMMAND("NEW_ORDER Alex");
      RUN_COMMAND("CANCEL_ORDER");
      RUN_COMMAND("NEW_ORDER Jenny");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("CANCEL_ORDER");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Order #1 initiated for 'alex'. Please add your items.");
      EXPECT_CONTAINS("Order #2 initiated for 'jenny'. Please add your items.");
      EXPECT_CONTAINS("Order #3 initiated for 'jude'. Please add your items.");

      EXPECT_CONTAINS("Order #1 has been cancelled.");
      EXPECT_CONTAINS("Order #2 has been cancelled.");
      EXPECT_CONTAINS("Order #3 has been cancelled.");
      break;
    case 26:  // Task_2_MultiOrdersRepeatUsers
      RUN_COMMAND("NEW_ACCOUNT Alex 10");
      RUN_COMMAND("NEW_ACCOUNT Jenny 10");
      RUN_COMMAND("NEW_ACCOUNT Jude 10");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("CANCEL_ORDER");
      RUN_COMMAND("NEW_ORDER Jenny");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("CANCEL_ORDER");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Order #1 initiated for 'jude'. Please add your items.");
      EXPECT_CONTAINS("Order #2 initiated for 'jenny'. Please add your items.");
      EXPECT_CONTAINS("Order #3 initiated for 'jude'. Please add your items.");

      EXPECT_CONTAINS("Order #1 has been cancelled.");
      EXPECT_CONTAINS("Order #2 has been cancelled.");
      EXPECT_CONTAINS("Order #3 has been cancelled.");
      break;
    case 27:  // Task_2_MultiOrdersDifferentUsersSavedOrderNumbers
      RUN_COMMAND("NEW_ACCOUNT Alex 10");
      RUN_COMMAND("NEW_ACCOUNT Jenny 20");
      RUN_COMMAND("NEW_ACCOUNT Jude 30");
      RUN_COMMAND("NEW_ORDER Alex");
      RUN_COMMAND("CANCEL_ORDER");
      RUN_COMMAND("NEW_ORDER Jenny");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("CANCEL_ORDER");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Order #1 initiated for 'alex'. Please add your items.");
      EXPECT_CONTAINS("Order #2 initiated for 'jenny'. Please add your items.");
      EXPECT_CONTAINS("Order #3 initiated for 'jude'. Please add your items.");

      EXPECT_CONTAINS("Order #1 has been cancelled.");
      EXPECT_CONTAINS("Order #2 has been cancelled.");
      EXPECT_CONTAINS("Order #3 has been cancelled.");

      EXPECT_CONTAINS("There are 3 accounts:");
      EXPECT_CONTAINS(
          "*) 'alex' has $10 credit and has completed 0 orders (1 cancelled)");
      EXPECT_CONTAINS(
          "*) 'jenny' has $20 credit and has completed 0 orders (1 cancelled)");
      EXPECT_CONTAINS(
          "*) 'jude' has $30 credit and has completed 0 orders (1 cancelled)");
      break;
    case 28:  // Task_2_MultiOrdersRepeatUsersSavedOrderNumbers
      RUN_COMMAND("NEW_ACCOUNT Alex 10");
      RUN_COMMAND("NEW_ACCOUNT Jenny 20");
      RUN_COMMAND("NEW_ACCOUNT Jude 30");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("CANCEL_ORDER");
      RUN_COMMAND("NEW_ORDER Jenny");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("CANCEL_ORDER");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Order #1 initiated for 'jude'. Please add your items.");
      EXPECT_CONTAINS("Order #2 initiated for 'jenny'. Please add your items.");
      EXPECT_CONTAINS("Order #3 initiated for 'jude'. Please add your items.");

      EXPECT_CONTAINS("Order #1 has been cancelled.");
      EXPECT_CONTAINS("Order #2 has been cancelled.");
      EXPECT_CONTAINS("Order #3 has been cancelled.");

      EXPECT_CONTAINS("There are 3 accounts:");
      EXPECT_CONTAINS(
          "*) 'alex' has $10 credit and has completed 0 orders (0 cancelled)");
      EXPECT_CONTAINS(
          "*) 'jenny' has $20 credit and has completed 0 orders (1 cancelled)");
      EXPECT_CONTAINS(
          "*) 'jude' has $30 credit and has completed 0 orders (2 cancelled)");
      break;
    case 29:  // Task_3_AddBurgerSmall
      RUN_COMMAND("NEW_ACCOUNT Jude 30");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("ADD_BURGER Vegemite small");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Order #1 initiated for 'jude'. Please add your items.");
      EXPECT_CONTAINS("Added burger 'Vegemite' (small) to order #1 for $8.");
      EXPECT_CONTAINS("Order #1 has been submitted with 1 item.");

      EXPECT_DOES_NOT_CONTAINS("has been cancelled", true);
      break;
    case 30:  // Task_3_AddBurgerSmallCloseSubmittedAccount
      RUN_COMMAND("NEW_ACCOUNT Jude 30");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("ADD_BURGER Vegemite small");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Order #1 initiated for 'jude'. Please add your items.");
      EXPECT_CONTAINS("Added burger 'Vegemite' (small) to order #1 for $8.");
      EXPECT_CONTAINS("Order #1 has been submitted with 1 item.");

      // The following will only work if correctly closed the order
      EXPECT_CONTAINS(
          "*) 'jude' has $22 credit and has completed 1 order (0 cancelled)");
      EXPECT_DOES_NOT_CONTAINS("Invalid command", true);

      EXPECT_DOES_NOT_CONTAINS("has been cancelled", true);
      EXPECT_DOES_NOT_CONTAINS("has $30 credit", true);
      break;
    case 31:  // Task_3_AddBurgersMixed
      RUN_COMMAND("NEW_ACCOUNT Jude 50");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("ADD_BURGER Vegemite regular");
      RUN_COMMAND("ADD_BURGER Yummy large");
      RUN_COMMAND("ADD_BURGER Yummy regular");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Order #1 initiated for 'jude'. Please add your items.");
      EXPECT_CONTAINS("Added burger 'Vegemite' (regular) to order #1 for $10.");
      EXPECT_CONTAINS("Added burger 'Yummy' (large) to order #1 for $12.");
      EXPECT_CONTAINS("Added burger 'Yummy' (regular) to order #1 for $10.");
      EXPECT_CONTAINS("Order #1 has been submitted with 3 items.");

      EXPECT_CONTAINS(
          "*) 'jude' has $18 credit and has completed 1 order (0 cancelled");

      EXPECT_DOES_NOT_CONTAINS("(small)", true);
      EXPECT_DOES_NOT_CONTAINS("has been cancelled", true);
      EXPECT_DOES_NOT_CONTAINS("has $30 credit", true);
      EXPECT_DOES_NOT_CONTAINS("made 0 order", true);
      EXPECT_DOES_NOT_CONTAINS("made 2 order", true);
      EXPECT_DOES_NOT_CONTAINS("made 3 order", true);
      EXPECT_DOES_NOT_CONTAINS("2 item", true);
      break;
    case 32:  // Task_3_AddDrinksSecondOrder
      RUN_COMMAND("NEW_ACCOUNT Jude 50");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("CANCEL_ORDER");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("ADD_DRINK can");
      RUN_COMMAND("ADD_DRINK bottle");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Order #1 initiated for 'jude'. Please add your items.");
      EXPECT_CONTAINS("Order #1 has been cancelled.");
      EXPECT_CONTAINS("Order #2 initiated for 'jude'. Please add your items.");
      EXPECT_CONTAINS("Added drink can to order #2 for $2.");
      EXPECT_CONTAINS("Added drink bottle to order #2 for $3.");
      EXPECT_CONTAINS("Order #2 has been submitted with 2 items.");

      EXPECT_CONTAINS(
          "*) 'jude' has $45 credit and has completed 1 order (1 cancelled)");

      EXPECT_DOES_NOT_CONTAINS("Order #1 has been submitted", true);
      EXPECT_DOES_NOT_CONTAINS("to order #1 for", true);
      EXPECT_DOES_NOT_CONTAINS("has $50 credit", true);
      EXPECT_DOES_NOT_CONTAINS("made 1 order", true);
      EXPECT_DOES_NOT_CONTAINS("made 3 order", true);
      break;
    case 33:  // Task_3_AddCombos
      RUN_COMMAND("NEW_ACCOUNT Jude 50");
      RUN_COMMAND("NEW_ACCOUNT Alex 50");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("ADD_COMBO Yummy regular bottle");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("NEW_ORDER Alex");
      RUN_COMMAND("ADD_COMBO Vegemite regular can");
      RUN_COMMAND("ADD_COMBO Yummy small can");
      RUN_COMMAND("ADD_COMBO Spicy large bottle");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Order #1 initiated for 'jude'. Please add your items.");
      EXPECT_CONTAINS(
          "Added combo burger 'Yummy' (regular) with drink bottle to order #1 "
          "for "
          "$12.");
      EXPECT_CONTAINS(
          "Added combo burger 'Vegemite' (regular) with drink can to "
          "order #2 for $11.");
      EXPECT_CONTAINS(
          "Added combo burger 'Yummy' (small) with drink can to order #2 "
          "for $9.");
      EXPECT_CONTAINS(
          "Added combo burger 'Spicy' (large) with drink bottle to order "
          "#2 for $14.");
      EXPECT_CONTAINS("Order #1 has been submitted with 1 item.");
      EXPECT_CONTAINS("Order #2 has been submitted with 3 items.");

      EXPECT_CONTAINS(
          "*) 'jude' has $38 credit and has completed 1 order (0 cancelled");
      EXPECT_CONTAINS(
          "*) 'alex' has $16 credit and has completed 1 order (0 cancelled");
      break;
    case 34:  // Task_3_AddMixedItems
      RUN_COMMAND("NEW_ACCOUNT Jude 50");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("ADD_BURGER Vegemite regular");
      RUN_COMMAND("ADD_DRINK can");
      RUN_COMMAND("ADD_COMBO Yummy small bottle");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Order #1 initiated for 'jude'. Please add your items.");
      EXPECT_CONTAINS("Added burger 'Vegemite' (regular) to order #1 for $10.");
      EXPECT_CONTAINS("Added drink can to order #1 for $2.");
      EXPECT_CONTAINS(
          "Added combo burger 'Yummy' (small) with drink bottle to order "
          "#1 for $10.");

      EXPECT_CONTAINS("Order #1 has been submitted with 3 items.");
      EXPECT_CONTAINS(
          "*) 'jude' has $28 credit and has completed 1 order (0 cancelled");

      EXPECT_DOES_NOT_CONTAINS("has $50 credit", true);
      EXPECT_DOES_NOT_CONTAINS("made 0 order", true);
      EXPECT_DOES_NOT_CONTAINS("made 2 order", true);
      break;
    case 35:  // Task_3_NotEnoughCredit
      RUN_COMMAND("NEW_ACCOUNT Jude 10");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("ADD_BURGER Vegemite regular");
      RUN_COMMAND("ADD_DRINK can");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Added burger 'Vegemite' (regular) to order #1 for $10.");
      EXPECT_CONTAINS("Added drink can to order #1 for $2.");

      EXPECT_CONTAINS("Insufficient credit to complete order #1 with 2 items.");
      EXPECT_CONTAINS("Order #1 has been cancelled.");
      EXPECT_CONTAINS(
          "*) 'jude' has $10 credit and has completed 0 orders (1 cancelled");
      EXPECT_DOES_NOT_CONTAINS("made 1 order", true);
      EXPECT_DOES_NOT_CONTAINS("has $-2 credit", true);
      EXPECT_DOES_NOT_CONTAINS("has been submitted", true);
      break;
    case 36:  // Task_3_NotEnoughCreditTopUpTryAgain
      RUN_COMMAND("NEW_ACCOUNT Jude 5");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("ADD_COMBO Vegemite regular can");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("TOP_UP_CREDIT Jude 10");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("ADD_COMBO Vegemite large bottle");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Added combo burger 'Vegemite' (regular) with drink can to order #1 "
          "for $11.");

      EXPECT_CONTAINS("Insufficient credit to complete order #1 with 1 item.");
      EXPECT_CONTAINS("Order #1 has been cancelled.");
      EXPECT_CONTAINS(
          "Added combo burger 'Vegemite' (large) with drink bottle "
          "to order #2 for $14.");

      EXPECT_CONTAINS("Order #2 has been submitted with 1 item.");
      EXPECT_CONTAINS(
          "*) 'jude' has $1 credit and has completed 1 order (1 cancelled");

      EXPECT_DOES_NOT_CONTAINS("Order #1 has been submitted", true);
      EXPECT_DOES_NOT_CONTAINS("Order #2 has been cancelled", true);
      EXPECT_DOES_NOT_CONTAINS("has $5 credit", true);
      EXPECT_DOES_NOT_CONTAINS("has $15 credit", true);
      break;
    case 37:  // Task_3_TopUpBeforeOrderToHaveEnoughCredit
      RUN_COMMAND("NEW_ACCOUNT Jude 5");
      RUN_COMMAND("TOP_UP_CREDIT Jude 9");
      RUN_COMMAND("NEW_ORDER Jude");
      RUN_COMMAND("ADD_COMBO Vegemite regular bottle");
      RUN_COMMAND("ADD_DRINK can");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("PRINT_ACCOUNTS");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS(
          "Successfully topped up 'jude' with $9. Current balance is $14.");

      EXPECT_CONTAINS(
          "Added combo burger 'Vegemite' (regular) with drink bottle to order "
          "#1 "
          "for $12.");
      EXPECT_CONTAINS("Added drink can to order #1 for $2.");
      EXPECT_CONTAINS("Order #1 has been submitted");

      EXPECT_CONTAINS(
          "*) 'jude' has $0 credit and has completed 1 order (0 cancelled");

      // EXPECT_DOES_NOT_CONTAINS("Order #1 has been submitted", true);
      EXPECT_DOES_NOT_CONTAINS("Insufficient credit to complete order", true);
      EXPECT_DOES_NOT_CONTAINS("has $14 credit", true);
      EXPECT_DOES_NOT_CONTAINS("has $9 credit", true);
      EXPECT_DOES_NOT_CONTAINS("has $5 credit", true);
      break;
    case 38:  // Task_3_PrintAllSalesOneOrder
      RUN_COMMAND("NEW_ACCOUNT Alex 35");
      RUN_COMMAND("NEW_ORDER Alex");
      RUN_COMMAND("ADD_BURGER Vegemite large");
      RUN_COMMAND("ADD_DRINK can");
      RUN_COMMAND("ADD_DRINK bottle");
      RUN_COMMAND("ADD_COMBO Yummy small bottle");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("PRINT_ALL_SALES");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Total burger sales: 2, Total drink sales: 3");
      break;
    case 39:  // Task_3_PrintAllSalesTwoOrdersSubmittedAcrossTwoAccounts
      RUN_COMMAND("NEW_ACCOUNT Alex 25");
      RUN_COMMAND("NEW_ACCOUNT Jenny 35");
      RUN_COMMAND("NEW_ORDER Jenny");
      RUN_COMMAND("ADD_BURGER Vegemite large");
      RUN_COMMAND("ADD_COMBO Yummy small bottle");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("NEW_ORDER Alex");
      RUN_COMMAND("ADD_DRINK can");
      RUN_COMMAND("ADD_DRINK bottle");
      RUN_COMMAND("ADD_COMBO Yummy small bottle");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("PRINT_ALL_SALES");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Total burger sales: 3, Total drink sales: 4");
      break;
    case 40:  // Task_3_PrintAllSalesMultipleOrdersSomeCancelled
      RUN_COMMAND("NEW_ACCOUNT Alex 200");
      RUN_COMMAND("NEW_ACCOUNT Jenny 200");
      RUN_COMMAND("NEW_ORDER Jenny");
      RUN_COMMAND("ADD_BURGER Vegemite large");
      RUN_COMMAND("ADD_COMBO Yummy small bottle");
      RUN_COMMAND("CANCEL_ORDER");
      RUN_COMMAND("NEW_ORDER Alex");
      RUN_COMMAND("ADD_DRINK can");
      RUN_COMMAND("ADD_DRINK bottle");
      RUN_COMMAND("ADD_DRINK bottle");
      RUN_COMMAND("ADD_COMBO Yummy small bottle");
      RUN_COMMAND("ADD_COMBO Spicy small bottle");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("NEW_ORDER Alex");
      RUN_COMMAND("ADD_BURGER Spicy small");
      RUN_COMMAND("ADD_BURGER Yummy regular");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("NEW_ORDER Jenny");
      RUN_COMMAND("ADD_COMBO Yummy small bottle");
      RUN_COMMAND("SUBMIT_ORDER");
      RUN_COMMAND("PRINT_ALL_SALES");
      RUN_COMMAND("EXIT");

      EXPECT_CONTAINS("Total burger sales: 5, Total drink sales: 6");
      break;
    default:
      cerr << "Test case not found: #" << test_number << endl;
      return TEST_RESULT_FAIL;
  }

  cerr << "PASSED Test " << test_number << "." << endl
       << endl
       << "--------------------------------------------------------------------"
          "--------"
       << endl
       << endl;
  return TEST_RESULT_PASS;
}
