#pragma once

#include <string>
#include <string_view>

class Person {
private:
  std::string name;
  int age;

public:
  Person(std::string_view name, int age);

  std::string_view getName() const;
  void setName(std::string_view name);

  int getAge() const;
  void setAge(int age);
};

std::ostream &operator<<(std::ostream &out, Person person);
