%skeleton "lalr1.cc"
%require "3.5"
%language "c++"

%define api.namespace  { paracl::frontend }
%define api.parser.class { Parser }
%define api.value.type variant
%define parse.error verbose
%define parse.lac full

%glr-parser
%expect-rr 4

%code requires
{
    #include <stdexcept>
    #include "../../include/frontend/AST.hpp"

    namespace paracl::frontend
    {

    class Lexer;
    class Driver;

    } // namespace paracl::frontend
} // %code requires

%code top
{
    #include <string>
    #include <iostream>
    #include "../../include/frontend/Driver.hpp"
    #include "../../include/frontend/Lexer.hpp"

    static paracl::frontend::symbol_type yylex(paracl::frontend::Lexer* lexer, paracl::frontend::Driver*) {
        return lexer->getNextToken();
    }
    
} // %code top

%lex-param { paracl::frontend::Lexer* lexer }
%lex-param { paracl::frontend::Driver* driver }
%parse-param { paracl::frontend::Lexer* lexer }
%parse-param { paracl::frontend::Driver* driver }

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

/* Terminals */
%token
    <int>         CONSTANT   "constant"
    <std::string> IDENTIFIER "identifier"

/* Rules that model the AST */
%type <paracl::frontend::Expression*>
    primary_expression
    postfix_expression

//%type <paracl::frontend::UnaryExpression*>
    unary_expression

//%type <paracl::frontend::BinaryExpression*>
    multiplicative_expression
    additive_expression
    relational_expression
    equality_expression
    logical_and_expression
    logical_or_expression

%type <paracl::frontend::Expression*>
    conditional_expression
    assignment_expression
    expression




    primary_expression
        : IDENTIFIER                { $$ = driver_.createNode<VariableExpression>($1, @1); }
        | CONSTANT                  { $$ = driver_.createNode<ConstantExpression>($1, @1); }
        | QMARK                     { $$ = driver_.createNode<InputExpression>($1, @$); }
        | LPAREN expression RPAREN  { $$ = $2; }

    postfix_expression
        : primary_expression        { $$ = $1; }
        | postfix_expression INC_OP { $$ = driver_.createNode<UnaryExpression>(UnaryExpression::UN_POSTFIX_INC, $1, @$); }
        | postfix_expression DEC_OP { $$ = driver_.createNode<UnaryExpression>(UnaryExpression::UN_POSTFIX_DEC, $1, @$); }

    unary_expression
        : postfix_expression        { $$ = %1; }
        | INC_OP unary_expression   { $$ = driver_.createNode<UnaryExpression>(UnaryExpression::UN_PREFIX_INC, $1, @$); }
        | DEC_OP unary_expression   { $$ = driver_.createNode<UnaryExpression>(UnaryExpression::UN_PREFIX_DEC, $1, @$); }
        | ADD_OP unary_expression   { $$ = driver_.createNode<UnaryExpression>(UnaryExpression::UN_ADD, $1, @$); }
        | SUB_OP unary_expression   { $$ = driver_.createNode<UnaryExpression>(UnaryExpression::UN_SUB, $1, @$); }
        | NOT_OP unary_expression   { $$ = driver_.createNode<UnaryExpression>(UnaryExpression::UN_NOT, $1, @$); }

    multiplicative_expression
        : unary_expression                                  { $$ = %1; }
        | multiplicative_expression MUL_OP unary_expression { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_MUL, $1, $3, @$); }
        | multiplicative_expression DIV_OP unary_expression { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_DIV, $1, $3, @$); }
        | multiplicative_expression MOD_OP unary_expression { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_MOD, $1, $3, @$); }

    additive_expression
        : multiplicative_expression                             { $$ = %1; }
        | additive_expression ADD_OP multiplicative_expression  { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_ADD, $1, $3, @$); }
        | additive_expression SUB_OP multiplicative_expression  { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_SUB, $1, $3, @$); }

    relational_expression
        : additive_expression                               { $$ = %1; }
        | relational_expression L_OP additive_expression    { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_L, $1, $3, @$); }
        | relational_expression G_OP additive_expression    { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_G, $1, $3, @$); }
        | relational_expression LE_OP additive_expression   { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_LE, $1, $3, @$); }
        | relational_expression GE_OP additive_expression   { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_GE, $1, $3, @$); }

    equality_expression
        : relational_expression                             { $$ = %1; }
        | equality_expression EQ_OP relational_expression   { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_EQ, $1, $3, @$); }
        | equality_expression NE_OP relational_expression   { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_NE, $1, $3, @$); }

    logical_and_expression
        : equality_expression                               { $$ = %1; }
        | logical_and_expression AND_OP equality_expression { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_AND, $1, $3, @$); }

    logical_or_expression
        : equality_expression                               { $$ = %1; }
        | logical_or_expression OR_OP equality_expression   { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_OR, $1, $3, @$); }

    conditional_expression
        : logical_or_expression                                                 { $$ = %1; }
        | logical_or_expression QMARK expression COLON conditional_expression   { $$ = driver_.createNode<TernaryExpression>($1, $3, $5, @$); }

    assignment_expression
        : conditional_expression                        { $$ = %1; }
        | unary_expression ASSIGN assignment_expression { $$ = driver_.createNode<BinaryExpression>(BinaryExpression::BIN_ASSIGN, $1, $3, @$); }

    expression
        : assignment_expression
        | expression COMMA assignment

  multiplicative_expression
  unary_expression
  additive_expression
  comparison_expression
  equality_expression
  logical_expression
  expression

%type <paracl::frontend::Expression*>
  value_block
  function_call

%type <paracl::frontend::Node*>
  statement

%type <paracl::frontend::Statement*>
  if_statement
  while_statement

//%type <ast::return_statement *>
//  return_statement

%type <std::vector<ast::i_ast_node*>>
  statements_list

%type <paracl::frontend::BlockStatement*>
  statement_block


%type <paracl::frontend::AssignmentStatement*>
  chainable_assignment
  chainable_assignment_statement

%type <paracl::frontend::VariableExpression*>
  typified_identifier

//%type <ast::function_definition *>
//  function_def

//%type <types::generic_type>
//  builtin_type
//  function_type
//  type

//%type <std::vector<types::generic_type>> 
//  type_list

//%type <paracl::frontend::VariableExpression*>
//  arglist

//%type <std::vector<ast::i_expression *>>
//  param_list

%type eof_or_semicol

%precedence THEN
%precedence ELSE


%start program

%%
program:
  stmtList  { auto ptr = driver.createNode<paracl::frontend::BlockStatement>($1, @1); driver.setRoot(ptr); }

stmtList:
    stmtList stmt |
    empty


eof_or_semicol: 
  SEMICOL 
| EOF

statements_:


optional_semicol: 
  SEMICOL
| %empty

%%

void paracl::frontend::error(const location& loc, const std::string& msg) {
    if (std::string_view{message} == "memory exhausted") {
        throw std::runtime_error{"Bison memory exhausted"};
    }

    driver->reportError<>(message, loc);
}
