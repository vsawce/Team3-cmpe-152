#include <iostream>
#include <string>

#include "../inc/Parser.h"
#include "../Tokens/Token.h" 

class Scanner   //special scanner for parser
{
public:
    Scanner(std::string filename);
        ~Scanner();
        Token read();
private:
    std::ifstream file;
        int line_number;
        char next_char;
        bool is_eof_reached;
        void check_file(std::ifstream& file, std::string filename);
        void close_file();
        void check_for_invalid_character(int state);
        void check_for_table_error(int state);
        void check_for_eof();
};

Parser::Parser(std::string filename)
{
    scanner = new Scanner(filename);
}

Parser::~Parser()
{
    delete scanner;
}

Node* Parser::parse()
{
    Node* root;
    token = scanner->read();
    root = start();
    if (token.is_eof()) {
        return root;
    }
    printError();
}

Node* Parser::start() //<start> -> program <vars> <block>
{
    int level = 1;
    if (KeywordToken::is_program_token(token)) {
        Node* node = Node::of(START, level);
        token = scanner->read();
        node->append_child(vars(level));
        node->append_child(block(level));
        return node;
    }
    printError();
}

Node* Parser::block(int level)  //<block> -> start <vars> <stats> end
{
    level++;
    if (KeywordToken::is_start_token(token)) {
        Node* node = Node::of(BLOCK, level);
        token = scanner->read();
        node->append_child(vars(level));
        node->append_child(stats(level));
        if (KeywordToken::is_end_token(token)) {
            token = scanner->read();
            return node;
        }
    }
    printError();
}

Node* Parser::vars(int level)   //<vars> -> var Identifier <vars> | empty
{
    if (KeywordToken::is_var_token(token)) {
        level++;
        Node* node = Node::of(VARS, level);
        token = scanner->read();
        if (token.is_identifier()) {
            node->append_token(token);
            token = scanner->read();
            node->append_child(vars(level));
            return node;
        } else {
            printError();
        }
    } else {
        return NULL;
    }
}

bool Parser::is_first_of_stats(Token token)
{
    return KeywordToken::is_read_token(token) ||
           KeywordToken::is_print_token(token) ||
           KeywordToken::is_start_token(token) ||
           KeywordToken::is_if_token(token) ||
           KeywordToken::is_iter_token(token) ||
           KeywordToken::is_let_token(token);
}

Node* Parser::stat(int level)   //<stat> -> <in> , | <out> , | <block> , | <ifstat> , | <loop> , | <assign>
{
    level++;
    Node* node = Node::of(STAT, level);
    if (KeywordToken::is_read_token(token)) {
        node->append_child(input(level));
        checkComma();
        return node;
    } else if (KeywordToken::is_print_token(token)) {
        node->append_child(out(level));
        checkComma();
        return node;
    } else if (KeywordToken::is_start_token(token)) {
        node->append_child(block(level));
        checkComma();
        return node;
    } else if (KeywordToken::is_if_token(token)) {
        node->append_child(ifstat(level));
        checkComma();
        return node;
    } else if (KeywordToken::is_iter_token(token)) {
        node->append_child(loop(level));
        checkComma();
        return node;
    } else if (KeywordToken::is_let_token(token)) {
        node->append_child(assign(level));
        checkComma();
        return node;
    }
    printError();
}

void Parser::checkComma()
{
    if (DelimiterToken::is_comma_token(token)) {
        token = scanner->read();
    } else {
        printError();
    }
}

Node* Parser::op(int level)   //<O> -> < | > | :
{
    level++;
    Node* node = Node::of(OPERATOR, level);
    if (is_op_token(token)) {
        node->append_token(token);
        token = scanner->read();
        return node;
    }
    printError();
}

bool Parser::is_op_token(Token token)
{
    return OperatorToken::is_less_than_token(token) ||
           OperatorToken::is_greater_than_token(token) ||
           OperatorToken::is_equals_token(token);
}

Node* Parser::expr(int level)   //<expr> -> <hash> + <expr> | <hash> - <expr> <expr> -> <hash> / <expr> | <hash> * <expr> <expr> -> <H>
{
    level++;
    Node* node = Node::of(EXPR, level);
    node->append_child(hash(level));
    if (OperatorToken::is_addition_token(token)) {
        node->append_token(token);
        token = scanner->read();
        node->append_child(expr(level));
        return node;
    } else if (OperatorToken::is_subtraction_token(token)) {
        node->append_token(token);
        token = scanner->read();
        node->append_child(expr(level));
        return node;
    } else if (OperatorToken::is_division_token(token)) {
        node->append_token(token);
        token = scanner->read();
        node->append_child(expr(level));
        return node;
    } else if (OperatorToken::is_multiplication_token(token)) {
        node->append_token(token);
        token = scanner->read();
        node->append_child(expr(level));
        return node;
    }
    return node;
}

Node* Parser::hash(int level)   //<hash> -> # <R> | <R>
{
    level++;
    Node* node = Node::of(HASH, level);
    if (OperatorToken::is_negation_token(token)) {
        node->append_token(token);
        token = scanner->read();
        node->append_child(R(level));
        return node;
    } else {
        node->append_child(R(level));
        return node;
    }
}

Node* Parser::R(int level)  //<R> -> ( <expr> ) | Identifier | Integer
{
    level++;
    Node* node = Node::of(R_LETTER, level);
    if (DelimiterToken::is_left_parentheses_token(token)) {
        token = scanner->read();
        node->append_child(expr(level));
        if (DelimiterToken::is_right_parentheses_token(token)) {
            token = scanner->read();
            return node;
        }
    } else if (token.is_identifier()) {
        node->append_token(token);
        token = scanner->read();
        return node;
    } else if (token.is_integer()) {
        node->append_token(token);
        token = scanner->read();
        return node;
    }
    printError();
}

Node* Parser::stats(int level)  //<stats> -> <stat> <m_stat>
{
    level++;
    Node* node = Node::of(STAT, level);
    node->append_child(stat(level));
    node->append_child(m_stat(level));
    return node;
}

Node* Parser::m_stat(int level) //<m_stat> -> <stats> | empty
{
    if (is_first_of_stats(token)) {
        level++;
        Node* node = Node::of(M_STAT, level);
        node->append_child(stats(level));
        return node;
    } else {
        return NULL;
    }
}

Node* Parser::input(int level) //<input> -> read Identifier
{
    if (KeywordToken::is_read_token(token)) {
        level++;
        Node* node = Node::of(INPUT, level);
        token = scanner->read();
        if (token.is_identifier()) {
            node->append_token(token);
            token = scanner->read();
            return node;
        }
    }
    printError();
}

Node* Parser::out(int level)    //<out> -> print <expr>
{
    level++;
    if (KeywordToken::is_print_token(token)) {
        Node* node = Node::of(OUT, level);
        token = scanner->read();
        node->append_child(expr(level));
        return node;
    }
    printError();
}

Node* Parser::ifstat(int level) //<ifstat> -> if ( <expr> <O> <expr> ) <stat>
{
    level++;
    if (KeywordToken::is_if_token(token)) {
        Node* node = Node::of(IFSTAT, level);
        token = scanner->read();
        if (DelimiterToken::is_left_parentheses_token(token)) {
            token = scanner->read();
            node->append_child(expr(level));
            node->append_child(op(level));
            node->append_child(expr(level));
            if (DelimiterToken::is_right_parentheses_token(token)) {
                token = scanner->read();
                node->append_child(stat(level));
                return node;
            }
        }
    }
    printError();
}

Node* Parser::loop(int level)   //<loop> -> iter ( <expr> <O> <expr> ) <stat>
{
    level++;
    if (KeywordToken::is_iter_token(token)) {
        Node* node = Node::of(LOOP, level);
        token = scanner->read();
        if (DelimiterToken::is_left_parentheses_token(token)) {
            token = scanner->read();
            node->append_child(expr(level));
            node->append_child(op(level));
            node->append_child(expr(level));
            if (DelimiterToken::is_right_parentheses_token(token)) {
                token = scanner->read();
                node->append_child(stat(level));
                return node;
            }
        }
    }
    printError();
}

Node* Parser::assign(int level) //<assign> -> let Identifier = <expr>
{
    level++;
    if (KeywordToken::is_let_token(token)) {
        Node* node = Node::of(ASSIGN, level);
        token = scanner->read();
        if (token.is_identifier()) {
            node->append_token(token);
            token = scanner->read();
            if (OperatorToken::is_assignment_token(token)) {
                token = scanner->read();
                node->append_child(expr(level));
                return node;
            }
        }
    }
    printError();
}

void Parser::printError()
{
    std::cerr << "Parser error: " << token << std::endl;
    exit(0);
}