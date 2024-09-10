%option c++ prefix="til_scanner_" outfile="til_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "til_parser.tab.h"

// don't change this
#define yyerror LexerError

#define CHECKED_INT(base) { \
  try{ \
    yylval.i = std::stoi(yytext, nullptr, base); \
    return tINTEGER; \
  } catch (const std::out_of_range&) { \
    yyerror("integer overflow"); \
  } \
}

#define CHECKED_DOUBLE { \
  try{ \
    yylval.d = std::stod(yytext); \
    return tDOUBLE; \
  } catch (const std::out_of_range&) {\
    yyerror("double overflow"); \
  } \
}

%}

EXP [Ee][+-]?[0-9]+

%x X_STRING X_COMMENT X_STRING_ZERO
%%
  yydebug=1; set_debug(1);


">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE;
"&&"                   return tAND;
"||"                   return tOR;

"int"                  return tTYPE_INT;
"double"               return tTYPE_DOUBLE;
"string"               return tTYPE_STRING;
"void"                 return tTYPE_VOID;

"if"                   return tIF; 
"else"                 return tELSE; 
"print"                return tPRINT; 
"println"              return tPRINTLN;
"stop"                 return tSTOP;
"next"                 return tNEXT;
"return"               return tRETURN;
"read"                 return tREAD; 
"program"              return tPROGRAM;
"set"                  return tSET; 
"external"             return tEXTERNAL;
"forward"              return tFORWARD;
"public"               return tPUBLIC;
"var"                  return tVAR;
"loop"                 return tLOOP;
"sizeof"               return tSIZEOF;
"block"                return tBLOCK;
"objects"              return tOBJECTS;
"index"                return tINDEX;
"null"                 return tNULL;
"function"             return tFUNCTION;

[A-Za-z][A-Za-z0-9]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

";".*$                 ; /* ignore comments */

"/*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"*/"        yy_pop_state();
<X_COMMENT>"/*"        yy_push_state(X_COMMENT);
<X_COMMENT>.|\n        ; /* ignore everything in comment */

\"                     yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"           yy_pop_state(); return tSTRING;
<X_STRING>\\\"         *yylval.s += yytext + 1;
<X_STRING>\\\\         *yylval.s += yytext + 1;
<X_STRING>\\0          yy_push_state(X_STRING_ZERO);
<X_STRING>\\[0-7]{1,3}  {
                          int i = std::stoi(yytext + 1, nullptr, 8);
                          if (i > 255) yyerror("sequence out of range");
                          *yylval.s += (char) i;
                        }
<X_STRING>\\t          *yylval.s += '\t';
<X_STRING>\\n          *yylval.s += '\n';
<X_STRING>\\r          *yylval.s += '\r';
<X_STRING>\n           yyerror("newline in string");
<X_STRING>\0           yyerror("null in string");
<X_STRING>.            *yylval.s += yytext;

<X_STRING_ZERO>\"      yy_pop_state(); yy_pop_state(); return tSTRING;
<X_STRING_ZERO>\\\"    ;
<X_STRING_ZERO>\\\\    ;
<X_STRING_ZERO>\n      yyerror("newline in string");
<X_STRING_ZERO>\0      yyerror("null in string");
<X_STRING_ZERO>.       ;

0|[1-9][0-9]*          CHECKED_INT(10);
0x[0-9a-fA-F]+         CHECKED_INT(16);
0x[0-9a-zA-Z]+         yyerror("invalid base 16 integer literal");

[0-9]*\.[0-9]+{EXP}?   CHECKED_DOUBLE;
[0-9]+\.[0-9]*{EXP}?   CHECKED_DOUBLE;
[0-9]+{EXP}            CHECKED_DOUBLE;

[-()<>~=+*/%;@?!.]       return *yytext;

[ \t\n]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
