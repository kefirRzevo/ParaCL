%skeleton "lalr1.cc"
%require "3.5"
%language "c++"

%defines

%define api.namespace  { paracl::frontend }
%define api.parser.class { Parser }
%define api.token.constructor
%define api.value.type variant
%define parse.error verbose
%define parse.lac full

%glr-parser
%expect-rr 0

%code requires
{
    #include <stdexcept>
    #include "frontend/AST.hpp"

    namespace paracl::frontend
    {

    class Lexer;
    class Driver;

    } // namespace paracl::frontend
} // %code requires

%code
{
    #include <string>
    #include <iostream>
    #include "frontend/Driver.hpp"
    #include "frontend/Lexer.hpp"

    namespace paracl::frontend
    {
    
    inline paracl::frontend::Parser::symbol_type yylex(paracl::frontend::Lexer& lexer, paracl::frontend::Driver&) {
        return lexer.getNextToken();
    }

    } // namespace paracl::frontend
} // %code

%lex-param { paracl::frontend::Lexer& lexer }
%lex-param { paracl::frontend::Driver& driver }
%parse-param { paracl::frontend::Lexer& lexer }
%parse-param { paracl::frontend::Driver& driver }

%define api.token.prefix {TOKEN_}
%locations
%define api.location.file "location.hpp"

%token
    BREAK       "break"
    CONTINUE    "continue"
    DO          "do"
    ELSE        "else"
    FOR         "for"
    IF          "if"
    INT         "int"
    PRINT       "print"
    WHILE       "while"

    ASSIGN      "="
//    ADD_ASSIGN  "+="
//    SUB_ASSIGN  "-="
//    MUL_ASSIGN  "*="
//    DIV_ASSIGN  "/="
//    MOD_ASSIGN  "%="
    INC_OP      "++"
    DEC_OP      "--"
    AND_OP      "&&"
    OR_OP       "||"
    LE_OP       "<="
    GE_OP       ">="
    G_OP        ">"
    L_OP        "<"
    EQ_OP       "=="
    NE_OP       "!="
    SEMICOL     ";"
    COMMA       ","
    COLON       ":"
    LSQUARE     "["
    RSQUARE     "]"
    LPAREN      "("
    RPAREN      ")"
    LCURLY      "{"
    RCURLY      "}"
    NOT_OP      "!"
    QMARK       "?"
    ADD_OP      "+"
    SUB_OP      "-"
    MUL_OP      "*"
    DIV_OP      "/"
    MOD_OP      "%"

    EOF 0       "end of file"

/* Terminals */
%token
    <int>         CONSTANT   "constant"
    <std::string> IDENTIFIER "identifier"

/* Rules that model the AST */
%type <paracl::frontend::Expression*>
    primary_expression
    postfix_expression
    unary_expression
    multiplicative_expression
    additive_expression
    relational_expression
    equality_expression
    logical_and_expression
    logical_or_expression
    conditional_expression
    assignment_expression
    expression

%type <paracl::frontend::Statement*>
    statement
    compound_statement

%type <paracl::frontend::BlockStatement*>
    statement_list

%type <paracl::frontend::Statement*>
    expression_statement
    selection_statement
    iteration_statement
    jump_statement

%type <paracl::frontend::Statement*>
    translation_unit

%nonassoc IFX
%nonassoc ELSE

%start translation_unit

%%

    primary_expression
        : IDENTIFIER                { $$ = driver.createNode<VariableExpression>($1, @1); }
        | CONSTANT                  { $$ = driver.createNode<ConstantExpression>($1, @1); }
        | QMARK                     { $$ = driver.createNode<InputExpression>(@$); }
        | LPAREN expression RPAREN  { $$ = $2; }

    postfix_expression
        : primary_expression        { $$ = $1; }
        | postfix_expression INC_OP { $$ = driver.createNode<UnaryExpression>(UnaryOperation::UN_POSTFIX_INC, $1, @$); }
        | postfix_expression DEC_OP { $$ = driver.createNode<UnaryExpression>(UnaryOperation::UN_POSTFIX_DEC, $1, @$); }

    unary_expression
        : postfix_expression        { $$ = $1; }
        | INC_OP unary_expression   { $$ = driver.createNode<UnaryExpression>(UnaryOperation::UN_PREFIX_INC, $2, @$); }
        | DEC_OP unary_expression   { $$ = driver.createNode<UnaryExpression>(UnaryOperation::UN_PREFIX_DEC, $2, @$); }
        | ADD_OP unary_expression   { $$ = driver.createNode<UnaryExpression>(UnaryOperation::UN_ADD, $2, @$); }
        | SUB_OP unary_expression   { $$ = driver.createNode<UnaryExpression>(UnaryOperation::UN_SUB, $2, @$); }
        | NOT_OP unary_expression   { $$ = driver.createNode<UnaryExpression>(UnaryOperation::UN_NOT, $2, @$); }

    multiplicative_expression
        : unary_expression                                  { $$ = $1; }
        | multiplicative_expression MUL_OP unary_expression { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_MUL, $1, $3, @$); }
        | multiplicative_expression DIV_OP unary_expression { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_DIV, $1, $3, @$); }
        | multiplicative_expression MOD_OP unary_expression { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_MOD, $1, $3, @$); }

    additive_expression
        : multiplicative_expression                             { $$ = $1; }
        | additive_expression ADD_OP multiplicative_expression  { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_ADD, $1, $3, @$); }
        | additive_expression SUB_OP multiplicative_expression  { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_SUB, $1, $3, @$); }

    relational_expression
        : additive_expression                               { $$ = $1; }
        | relational_expression L_OP additive_expression    { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_L, $1, $3, @$); }
        | relational_expression G_OP additive_expression    { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_G, $1, $3, @$); }
        | relational_expression LE_OP additive_expression   { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_LE, $1, $3, @$); }
        | relational_expression GE_OP additive_expression   { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_GE, $1, $3, @$); }

    equality_expression
        : relational_expression                             { $$ = $1; }
        | equality_expression EQ_OP relational_expression   { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_EQ, $1, $3, @$); }
        | equality_expression NE_OP relational_expression   { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_NE, $1, $3, @$); }

    logical_and_expression
        : equality_expression                               { $$ = $1; }
        | logical_and_expression AND_OP equality_expression { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_AND, $1, $3, @$); }

    logical_or_expression
        : logical_and_expression                                { $$ = $1; }
        | logical_or_expression OR_OP logical_and_expression    { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_OR, $1, $3, @$); }

    conditional_expression
        : logical_or_expression                                                 { $$ = $1; }
        | logical_or_expression QMARK expression COLON conditional_expression   { $$ = driver.createNode<TernaryExpression>($1, $3, $5, @$); }

    assignment_expression
        : conditional_expression                        { $$ = $1; }
        | unary_expression ASSIGN assignment_expression { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_ASSIGN, $1, $3, @$); }

    expression
        : assignment_expression                     { $$ = $1; }
        | expression COMMA assignment_expression    { $$ = driver.createNode<BinaryExpression>(BinaryOperation::BIN_COMMA, $1, $3, @$); }

    statement
        : compound_statement        { $$ = $1; }
        | expression_statement      { $$ = $1; }
        | selection_statement       { $$ = $1; }
        | iteration_statement       { $$ = $1; }
        | jump_statement            { $$ = $1; }
        | PRINT expression SEMICOL  { $$ = driver.createNode<OutputStatement>($2, @$); }

    compound_statement
        : LCURLY RCURLY                 { $$ = driver.createNode<BlockStatement>(@$); }
        | LCURLY statement_list RCURLY  { $$ = $2; }

    statement_list
        : statement                 { auto block = driver.createNode<BlockStatement>(@$); block->statements_.push_back($1); $$ = block; }
        | statement_list statement  { $1->statements_.push_back($2); $$ = $1; }

    expression_statement
        : SEMICOL               { }
        | expression SEMICOL    { $$ = driver.createNode<ExpressionStatement>($1, @$); }

    selection_statement
        : IF LPAREN expression RPAREN statement %prec IFX       { $$ = driver.createNode<IfStatement>($3, $5, @$); }
        | IF LPAREN expression RPAREN statement ELSE statement  { $$ = driver.createNode<IfStatement>($3, $5, $7, @$); }

    iteration_statement
        : WHILE LPAREN expression RPAREN statement  { $$ = driver.createNode<WhileStatement>($3, $5, @$); }

    jump_statement
        : CONTINUE SEMICOL  { $$ = driver.createNode<ContinueStatement>(@$); }
        | BREAK SEMICOL     { $$ = driver.createNode<BreakStatement>(@$); }

    translation_unit
        : statement_list { $$ = $1; driver.setRoot($$); }
%%

void paracl::frontend::Parser::error(const location& loc, const std::string& msg) {
    if (std::string_view{msg} == "memory exhausted") {
        throw std::runtime_error{"Bison memory exhausted"};
    }
    driver.reportError<Error>(loc);
}
