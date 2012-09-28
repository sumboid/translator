#ifndef LEXER_H
#define LEXER_H

#include <istream>
#include <map>
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

    std::map<char, token_type> terminals;
};

#endif /* end of include guard: LEXER_H */
