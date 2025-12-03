#pragma once

#include "lib/types.hpp"
#include <string>
#include <string_view>

class GameState {
private:
  u64 id;
  std::string username;

public:
  u64 getId() const;
  void setId(u64 id);

  std::string_view getUsername() const;
  void setUsername(std::string_view username);
};

