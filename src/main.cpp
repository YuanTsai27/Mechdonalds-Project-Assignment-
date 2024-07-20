#include <iostream>
#include <string>

#include "app/command.h"
#include "app/launcher.h"

using namespace std;

int main(int argc, char *argv[]) {
  Launcher app_launcher;
  app_launcher.Start();
  return 0;
}
