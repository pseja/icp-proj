#pragma once

#include <string>

class Variable
{
  private:
    std::string name;
    std::string type;
    std::string value;

  public:
    Variable(std::string name, std::string type, std::string value);

    std::string getName();
    std::string getType();
    std::string getValue();

    void setName(std::string new_name);
    void setType(std::string new_type);
    void setValue(std::string new_value);
};
