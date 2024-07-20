
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <initializer_list>
#include <string>
#include <vector>

class Command {
 private:
  std::string name_;
  int num_args_;
  std::string message_;
  std::vector<std::string> option_prompts_;

  static std::initializer_list<Command> commands;

 public:
  static const Command NEW_ACCOUNT;
  static const Command TOP_UP_CREDIT;
  static const Command PRINT_ACCOUNTS;

  static const Command NEW_ORDER;
  static const Command CANCEL_ORDER;
  static const Command SUBMIT_ORDER;

  static const Command ADD_BURGER;
  static const Command ADD_DRINK;
  static const Command ADD_COMBO;
  static const Command PRINT_ALL_SALES;

  static const Command HELP;
  static const Command EXIT;
  static const Command INVALID;

  Command(const std::string &name, int num_args, const std::string &message);
  Command(const std::string &name, int num_args, const std::string &message,
          const std::vector<std::string> &option_prompts);

  std::string GetName() const;
  bool HasArguments() const;
  int GetNumArgs() const;
  bool HasOptions() const;
  int GetNumOptions() const;
  std::string GetMessage() const;
  std::string GetOptionPrompt(int index) const;

  bool operator==(const Command &other) const;
  bool operator!=(const Command &other) const;

  static std::string GetHelp();
  static bool IsValidCommand(const std::string &cmd_input);
  static const Command &Find(const std::string &cmd_input);
};

#endif  // COMMAND_HPP
