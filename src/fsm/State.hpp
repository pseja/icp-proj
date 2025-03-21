#pragma once

#include <functional>
#include <string>
#include <vector>

class State
{
  private:
    std::string name;
    // TODO needs more brainstorming with code gen
    std::vector<std::function<void()>> actions;

  public:
    State();
    State(std::string name);

    std::string getName();

    void setName(std::string new_name);
};
