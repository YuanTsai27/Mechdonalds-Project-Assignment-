
# Mechdonalds Project Assignment (Completed September 2023)

This was the first project assignment that helped me to develop confidence with object-oriented programing principles, such as encapsulation, inheritance, polymorphism and classes. In this C++ based Mechdonalds Project, I have developed a robust OOP solution to simulate basic operations in a fast food restaurant e.g placing food orders, generating end-of-day sales report. 

The codebase features a structured OOP design with multiple classes including an abstract food item class, extended by 3 subclasses - burger, drink and combo classes which shares similar characteristics. Through this design choice of utilising inheritance and polymorphism, the codebase is modular and maintainable.


**Code Style**
Throughout this project, I have adhered to the Google C++ Style Guide, ensuring that the code is clean, consistent, and follows industry-standard practices.

**Feel free to check this project out!**
_To run this project, cmake is required._

# How to use (terminal)

* **Build:** Create a `build` directory for the output (if it doesn't already exist), generate the build files in it, then run it:
  ```bash
  mkdir build
  cd build
  cmake ..          # generates the build files
  cmake --build .   # execute the build and creates the executable(s)
  ```

* **Run:** Run the program interactively:
  ```bash
  cd build
  ./mechdonalds
  ```

* **Test:** Run the test cases:
  ```bash
  cd build
  ctest     # minimal output
  ctest -V  # verbose output
  ctest --output-on-failure  # verbose for failing test cases, but minimal for tests that pass
  ctest -V -R Task_1_ZeroAccounts # run a single specific test case in verbose mode
  ```

