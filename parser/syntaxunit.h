#ifndef SYNTAXUNIT_H
#define SYNTAXUNIT_H

#include <string>

class syntaxunit_t
{
public:
    syntaxunit_t();
    syntaxunit_t (const std::string& unit_name);
    syntaxunit_t (const std::string& unit_name,
                  const std::string& value);

    std::string& get_value();
    std::string& get_name();
    bool has_value();
private:
    std::string unit_name;
    std::string value;
};


#endif /* end of include guard: SYNTAXUNIT_H */
