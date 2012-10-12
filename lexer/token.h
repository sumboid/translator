#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
    NUMBER,
    OPERATOR,
    BRACKET,     //not enought imagination
    END_OF_FILE
};

struct token_t
{
    token_type type;
    std::string value;
};


#endif /* end of include guard: TOKEN_H */
