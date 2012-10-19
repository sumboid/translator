#ifndef LEXER_H
#define LEXER_H

#include <istream>
#include <map>
#include <string>
#include "buffer.h"
#include "token.h"

class lexer_t
{
public:
    lexer_t(std::istream& steam);

    token_t peek();
    void next();

private:
    token_t current_token;
    buffer_t buffer;

    std::map<std::string, token_type> terminals;

    bool check_several_symbols_terminal();
    bool check_one_symbol_terminal();
    bool check_number();
    bool check_eof();
    bool is_terminal(const std::string&);
    void flush_spaces();
};

#endif /* end of include guard: LEXER_H */
