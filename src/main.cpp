#include "person.hpp"
#include <iostream>
#include <string>

int main() {
  std::cout << "Successfully connected via SSH, taking over..." << std::endl;

  std::string name;
  int age;

  std::cout << "What's your name? ";
  std::cin >> name;

  std::cout << "What's your age? ";
  std::cin >> age;

  Person person(name, age);
  std::cout << "Welcome aboard, " << person << std::endl;
  return 0;
}
