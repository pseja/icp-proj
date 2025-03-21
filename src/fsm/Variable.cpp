#include "Variable.hpp"

Variable::Variable(std::string name, std::string type, std::string value) : name(name), type(type), value(value)
{
}

std::string Variable::getName()
{
    return name;
}

std::string Variable::getType()
{
    return type;
}

std::string Variable::getValue()
{
    return value;
}

void Variable::setName(std::string new_name)
{
    name = new_name;
}

void Variable::setType(std::string new_type)
{
    type = new_type;
}

void Variable::setValue(std::string new_value)
{
    value = new_value;
}
