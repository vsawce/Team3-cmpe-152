#ifndef PARSER_H
#define PARSER_H

#include <string>

#include "../inc/scanner.h"
#include "../inc/node.h"
#include "../Tokens/Token.h"

class Parser
{
    public:
     Parser(std::string filename);
        ~Parser();
        Node* parse();

    private:
        Scanner* scanner;
        Token token;

        Node* start();
        Node* block(int level);
        Node* vars(int level);
        bool is_first_of_stats(Token token);
        Node* stat(int level);
        Node* op(int level);
        bool is_op_token(Token token);
        Node* expr(int level);
        Node* hash(int level);
        Node* R(int level);
        Node* stats(int level);
        Node* m_stat(int level);
        Node* input(int level);
        Node* out(int level);
        Node* ifstat(int level);
        Node* loop(int level);
        Node* assign(int level);
        void printError();
        void checkComma();
};

#endif