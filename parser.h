#ifndef PARSER_H
#define PARSER_H

class parser_t {
public:
    parser_t (std::istream& stream);
    ~parser_t();

    int parse();

private:
    int parse_expr();
    int parse_fact();
    int parse_pow();
    int parse_number();

    lexer_t* lexer;
};
#endif /* end of include guard: PARSER_H */
