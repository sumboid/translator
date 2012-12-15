#include <string>
#include "lexer.h"

#include <iostream>

using std::string;
using std::map;

    lexer_t::lexer_t(std::istream& stream)
:buffer(stream)
{
    terminals["+"] = OPERATOR;
    terminals["-"] = OPERATOR;
    terminals["*"] = OPERATOR;
    terminals["/"] = OPERATOR;
    terminals["^"] = OPERATOR;

    terminals["("] = BRACKET;
    terminals[")"] = BRACKET;
    terminals["{"] = BRACKET;
    terminals["}"] = BRACKET;

    terminals[";"] = DELIMITER;

    terminals["void"] = TYPE;
    terminals["int"] = TYPE;
    terminals["double"] = TYPE;

    terminals["="] = ASSIGN;
    terminals[","] = COMMA;

    terminals["if"] = IF;
    terminals["<"] = LESS;
    terminals[">"] = MORE;
    terminals["=="] = EQUALS;

    terminals["return"] = RETURN;

    next(); //get first token
}

token_t lexer_t::peek()
{
    return current_token;
}

void lexer_t::next()
{
    flush_spaces();
    bool token_finded = check_eof()                      ||
                        check_number()                   ||
                        check_one_symbol_terminal()      ||
                        check_several_symbols_terminal() ;
    if(!token_finded)
    {
        throw 1;
    }
}

bool lexer_t::check_several_symbols_terminal()
{
    string token;

    while(true)
    {
        char current = buffer.peek();
        string current_str;
        current_str.push_back(current);
        if(current == 0 || current == ' ' || current == '\n' || current == '\t' || is_terminal(current_str))
        {
            if(token.empty())
            {
                return false; //XXX: logic error
            }
            else
            {
                current_token.type = VARIABLE;
                current_token.value = token;
                return true;
            }
        }

        token.push_back(current);
        if(is_terminal(token))
        {
            map<string, token_type>::iterator it = terminals.find(token);
            current_token.type = it->second;
            current_token.value = it->first;
            buffer.next();
            return true;
        }

        buffer.next();
    }
}

bool lexer_t::check_one_symbol_terminal()
{
    string token;
    token.push_back(buffer.peek());

    if(is_terminal(token))
    {
        map<string, token_type>::iterator it = terminals.find(token);
        current_token.type = it->second;
        current_token.value = it->first;
        buffer.next();
        return true;
    }

    return false;
}

bool lexer_t::is_terminal(const string& terminal)
{
    map<string, token_type>::iterator it = terminals.find(terminal);
    if(it != terminals.end())
    {
        return true;
    }
    return false;
}

bool lexer_t::check_number()
{
    string token;
    while(true)
    {
        char current = buffer.peek();
        if(isdigit(current))
        {
            token.push_back(current);
        }
        else if(token.empty())
        {
            return false;
        }
        else
        {
            current_token.type = NUMBER;
            current_token.value = token;
            return true;
        }
        buffer.next();
    }

}

bool lexer_t::check_eof()
{
    char current = buffer.peek();

    if(current == -1)
    {
        current_token.value = "";
        current_token.type = END_OF_FILE;
        return true;
    }

    return false;
}

void lexer_t::flush_spaces()
{
    //std::cout << (void*)buffer.peek() << '?' << std::endl;
    while(buffer.peek() == '\n' || buffer.peek() == '\r' || buffer.peek() == '\t' || buffer.peek() == ' ')
    {
      //  std::cout << "flush: " << (void*)buffer.peek() << std::endl;
        buffer.next();
    }
}
