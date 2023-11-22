
%{

#include "bison_paracl_parser.hpp"
#include "../../include/frontend/Lexer.hpp"

#include <iostream>
#include <string>
#include <sstream>

%}

%option noyywrap nounput noinput nodefault 
%option c++
%option yyclass="Lexer"
%option nounistd

number      (0|[1-9][0-9]*)
identifier  [a-zA-Z_][a-zA-Z_0-9]*
comment     ([/][/].*)

%%
"break"         { return paracl::frontend::make_BREAK(updateLocation()); }
"continue"      { return paracl::frontend::make_CONTINUE(updateLocation()); }
"do"            { return paracl::frontend::make_DO(updateLocation()); }
"else"          { return paracl::frontend::make_ELSE(updateLocation()); }
"for"           { return paracl::frontend::make_FOR(updateLocation()); }
"if"            { return parser::make_IF(updateLocation()); }
"int"           { return parser::make_INT(updateLocation()); }
"print"         { return parser::make_PRINT(updateLocation()); }
"while"         { return parser::make_WHILE(updateLocation()); }

//"+="            { return parser::make_ADD_ASSIGN(updateLocation()); }
//"-="            { return parser::make_SUB_ASSIGN(updateLocation()); } 
//"*="            { return parser::make_MUL_ASSIGN(updateLocation()); }
//"/="            { return parser::make_DIV_ASSIGN(updateLocation()); }
//"%="            { return parser::make_MOD_ASSIGN(updateLocation()); }
"++"            { return parser::make_INC_OP(updateLocation()); }
"--"            { return parser::make_DEC_OP(updateLocation()); }
"&&"            { return parser::make_AND_OP(updateLocation()); }
"||"            { return parser::make_OR_OP(updateLocation()); }
"<="            { return parser::make_LE_OP(updateLocation()); }
">="            { return parser::make_GE_OP(updateLocation()); }
">"             { return parser::make_G_OP(updateLocation()); }
"<"             { return parser::make_L_OP(updateLocation()); }
"=="            { return parser::make_EQ_OP(updateLocation()); }
"!="            { return parser::make_NE_OP(updateLocation()); }
";"             { return parser::make_SEMICOL(updateLocation()); }
","             { return parser::make_COMMA(updateLocation()); }
":"             { return parser::make_COLON(updateLocation()); }
"="             { return parser::make_ASSIGN(updateLocation()); }
"["             { return parser::make_LSQUARE(updateLocation()); }
"]"             { return parser::make_RSQUARE(updateLocation()); }
"("             { return parser::make_LPAREN(updateLocation()); }
")"             { return parser::make_RPAREN(updateLocation()); }
"{"             { return parser::make_LCURLY(updateLocation()); }
"}"             { return parser::make_RCURLY(updateLocation()); }
"!"             { return parser::make_NOT_OP(updateLocation()); }
"?"             { return parser::make_SCAN(updateLocation()); }
"+"             { return parser::make_ADD_OP(updateLocation()); }
"-"             { return parser::make_SUB_OP(updateLocation()); }
"*"             { return parser::make_MUL_OP(updateLocation()); }
"/"             { return parser::make_DIV_OP(updateLocation()); }
"%"             { return parser::make_MOD_OP(updateLocation()); }

{identifier}    { auto newloc = updateLocation(); return parser::make_IDENTIFIER(yytext, newloc); }
{number}        { auto newloc = updateLocation(); return parser::make_CONSTANT(std::atoi(yytext), newloc); }
{comment}       { updateLocation(); }

[\n|\r\n]+      { curPos_.lines(yyleng); }
[\t]+           { curPos_.columns(yyleng); }

. {
  auto loc = updateLocation();
  driver_.reportError<UnknownToken>(yytext, loc); 
}

<<EOF>>         { return paracl::frontend::parser::make_EOF(updateLocation()); }

%%
