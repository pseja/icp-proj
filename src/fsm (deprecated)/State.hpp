#pragma once

#include <functional>
#include <string>
#include <vector>

class State
{
  private:
    std::string name;
    std::vector<std::function<void()>> actions;

  public:
    State(std::string name);

    std::string getName();

    void setName(std::string new_name);
};
