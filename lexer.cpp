#include <string>
#include "lexer.h"

#include <iostream>

using std::string;
using std::map;

lexer_t::lexer_t(std::istream& stream)
    :buffer(stream)
{
    terminals['+'] = OPERATOR;
    terminals['-'] = OPERATOR;
    terminals['*'] = OPERATOR;
    terminals['/'] = OPERATOR;
    terminals['^'] = OPERATOR;
    terminals['('] = BRACKET;
    terminals[')'] = BRACKET;
    next(); //get first token
}

token_t lexer_t::peek()
{
    return current_token;
}

void lexer_t::next()
{
    string token;
    bool number = false;
    while(true)
    {
        char current = buffer.peek();
        if(!number)
        {
            if(current == 0)
            {
                current_token.value = "";
                current_token.type = END_OF_FILE;
                break;
            }
            if(current == ' ' || current == '\n')
            {
                buffer.next();
                continue;
            }
            else if(isdigit(current))
            {
                number = true;
                token.push_back(current);
                buffer.next();
                continue;
            }
            else
            {
                map<char, token_type>::iterator it = terminals.find(current);
                if(it != terminals.end())
                {
                    current_token.type = it->second;
                    current_token.value = it->first;
                    buffer.next();
                    break;
                }
                else
                {
                    throw 1; //TODO
                }
            }

        }
        else
        {
            if(isdigit(current))
            {
                token.push_back(current);
                buffer.next();
                continue;
            }
            else
            {
                current_token.type = NUMBER;
                current_token.value = token;
                break;
            }
        }
    }
}
