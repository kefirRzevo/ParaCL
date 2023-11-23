
%{

#include "parser.tab.hh"
#include "frontend/Lexer.hpp"

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

"break"         { return Parser::make_BREAK(updateLocation()); }
"continue"      { return Parser::make_CONTINUE(updateLocation()); }
"do"            { return Parser::make_DO(updateLocation()); }
"else"          { return Parser::make_ELSE(updateLocation()); }
"for"           { return Parser::make_FOR(updateLocation()); }
"if"            { return Parser::make_IF(updateLocation()); }
"int"           { return Parser::make_INT(updateLocation()); }
"print"         { return Parser::make_PRINT(updateLocation()); }
"while"         { return Parser::make_WHILE(updateLocation()); }

"++"            { return Parser::make_INC_OP(updateLocation()); }
"--"            { return Parser::make_DEC_OP(updateLocation()); }
"&&"            { return Parser::make_AND_OP(updateLocation()); }
"||"            { return Parser::make_OR_OP(updateLocation()); }
"<="            { return Parser::make_LE_OP(updateLocation()); }
">="            { return Parser::make_GE_OP(updateLocation()); }
">"             { return Parser::make_G_OP(updateLocation()); }
"<"             { return Parser::make_L_OP(updateLocation()); }
"=="            { return Parser::make_EQ_OP(updateLocation()); }
"!="            { return Parser::make_NE_OP(updateLocation()); }
";"             { return Parser::make_SEMICOL(updateLocation()); }
","             { return Parser::make_COMMA(updateLocation()); }
":"             { return Parser::make_COLON(updateLocation()); }
"="             { return Parser::make_ASSIGN(updateLocation()); }
"["             { return Parser::make_LSQUARE(updateLocation()); }
"]"             { return Parser::make_RSQUARE(updateLocation()); }
"("             { return Parser::make_LPAREN(updateLocation()); }
")"             { return Parser::make_RPAREN(updateLocation()); }
"{"             { return Parser::make_LCURLY(updateLocation()); }
"}"             { return Parser::make_RCURLY(updateLocation()); }
"!"             { return Parser::make_NOT_OP(updateLocation()); }
"?"             { return Parser::make_QMARK(updateLocation()); }
"+"             { return Parser::make_ADD_OP(updateLocation()); }
"-"             { return Parser::make_SUB_OP(updateLocation()); }
"*"             { return Parser::make_MUL_OP(updateLocation()); }
"/"             { return Parser::make_DIV_OP(updateLocation()); }
"%"             { return Parser::make_MOD_OP(updateLocation()); }

{identifier}    { auto newloc = updateLocation(); return Parser::make_IDENTIFIER(yytext, newloc); }
{number}        { auto newloc = updateLocation(); return Parser::make_CONSTANT(std::atoi(yytext), newloc); }
{comment}       { updateLocation(); }

[\n|\r\n]+      { curPos_.lines(yyleng); }
[\t]+           { curPos_.columns(yyleng); }

. {
  auto loc = updateLocation();
  driver_->reportError<Lexical>(yytext, loc); 
}

<<EOF>>         { return Parser::make_EOF(updateLocation()); }

%%
