#include "person.hpp"
#include <ostream>
#include <string_view>

Person::Person(std::string_view name, int age) : name(name), age(age) {}

std::string_view Person::getName() const { return name; }
void Person::setName(std::string_view name) { this->name = name; }

int Person::getAge() const { return age; }
void Person::setAge(int age) { this->age = age; }

std::ostream &operator<<(std::ostream &out, Person person) {
  return out << "Person(" << person.getName() << ", " << person.getAge() << ")";
}
