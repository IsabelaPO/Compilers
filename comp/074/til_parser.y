%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include ".auto/all_nodes.h"
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                   i;          /* integer value */
  double                d;          /* double value */
  std::string          *s;          /* symbol name or string literal */
  cdk::basic_node      *node;       /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  til::block_node      *blk;
  std::vector<std::shared_ptr<cdk::basic_type>> *type_vec;
};

%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING
%token tTYPE_INT tTYPE_DOUBLE tTYPE_STRING tTYPE_VOID
%token tLOOP tIF tPRINT tPRINTLN tREAD tAND tOR tGE tLE tEQ tNE
%token tBLOCK
%token tVAR tPROGRAM tSET tSIZEOF tFUNCTION
%token tSTOP tNEXT tRETURN tOBJECTS tINDEX tNULL
%token tEXTERNAL tPUBLIC tFORWARD tPRIVATE

%nonassoc tIFX
%nonassoc tELSE

/*%right '='
%right tAND tOR
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-'
%left '*' '/' '%'*/
%nonassoc tUNARY

%type <node> program instruction declaration func_declaration func_argument
%type <sequence> declarations instructions expressions func_declarations func_arguments
%type <expression> expr expr_element func_def
%type <lvalue> lval
%type <blk> block block_content
%type <type_vec> types 
%type <type> type func_type pointer_type void_pointer_type func_return_type

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file : func_declarations program   { compiler->ast(new cdk::sequence_node(LINE, $2, $1)); }
     | func_declarations           { compiler->ast($1); }
     | program                         { compiler->ast(new cdk::sequence_node(LINE, $1)); }
     | /* EMPTY */                     { compiler->ast(new cdk::sequence_node(LINE)); }
     ;

func_declarations : func_declaration                       { $$ = new cdk::sequence_node(LINE, $1); }
                  | func_declarations func_declaration     { $$ = new cdk::sequence_node(LINE, $2, $1); }
                  ;

func_declaration : '('tEXTERNAL type tIDENTIFIER ')'        { $$ = new til::declaration_node(LINE, tEXTERNAL, $3, *$4, nullptr); delete $4;}
                 | '('tFORWARD type tIDENTIFIER ')'         { $$ = new til::declaration_node(LINE, tFORWARD, $3, *$4, nullptr); delete $4;}
                 | '('tPUBLIC type tIDENTIFIER expr ')'     { $$ = new til::declaration_node(LINE, tPUBLIC, $3, *$4, $5); delete $4;}
                 | '('tPUBLIC type tIDENTIFIER ')'          { $$ = new til::declaration_node(LINE, tPUBLIC, $3, *$4, nullptr); delete $4;}
                 | '('tPUBLIC tVAR tIDENTIFIER expr ')'     { $$ = new til::declaration_node(LINE, tPUBLIC, nullptr, *$4, $5); delete $4;}
                 | '('tPUBLIC tIDENTIFIER expr ')'          { $$ = new til::declaration_node(LINE, tPUBLIC, nullptr, *$3, $4); delete $3;}
                 | declaration                              { $$ = $1; }
                 ;

program : '(' tPROGRAM block_content ')'                    { $$ = new til::program_node(LINE, $3); }
        ;

block_content : declarations instructions                  { $$ = new til::block_node(LINE, $1, $2); }
              | declarations                               { $$ = new til::block_node(LINE, $1,  new cdk::sequence_node(LINE)); }       
              | instructions                               { $$ = new til::block_node(LINE,  new cdk::sequence_node(LINE), $1); }       
              | /*EMPTY*/                                  { $$ = new til::block_node(LINE,  new cdk::sequence_node(LINE),  new cdk::sequence_node(LINE)); }       
              ;

declarations : declaration                   { $$ = new cdk::sequence_node(LINE, $1); }
     	   | declarations declaration      { $$ = new cdk::sequence_node(LINE, $2, $1); }
             ;

declaration : '(' type tIDENTIFIER expr ')'   { $$ = new til::declaration_node(LINE, tPRIVATE, $2, *$3, $4); delete $3; }
            | '(' type tIDENTIFIER ')'        { $$ = new til::declaration_node(LINE, tPRIVATE, $2, *$3, nullptr); delete $3; }
            | '(' tVAR tIDENTIFIER expr ')'   { $$ = new til::declaration_node(LINE, tPRIVATE, nullptr, *$3, $4); delete $3; } 
            ;

types : type               { $$ = new std::vector<std::shared_ptr<cdk::basic_type>>(1, $1); }
      | types type         { $$ = $1; $$->push_back($2);}
      ;

type : pointer_type              { $$ = $1; }
     | void_pointer_type         { $$ = $1; }
     ;

pointer_type: tTYPE_INT          { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT); }
            | tTYPE_DOUBLE       { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE); }
            | tTYPE_STRING       { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING); }
            | func_type          { $$ = $1; }
            | pointer_type '!'   { $$ = cdk::reference_type::create(4, $1); }
            ;

void_pointer_type: tTYPE_VOID '!'              { $$ = cdk::reference_type::create(4, cdk::primitive_type::create(0, cdk::TYPE_VOID)); }
                 | void_pointer_type '!'       { $$ = $1; }
                 ;

func_type: '(' func_return_type ')'                    { $$ = cdk::functional_type::create($2); }
         | '(' func_return_type '(' types ')' ')'      { $$ = cdk::functional_type::create(*$4, $2); delete $4; } 
         ;

func_return_type: type                     { $$ = $1; }
                | tTYPE_VOID               { $$ = cdk::primitive_type::create(4, cdk::TYPE_VOID); }
                ;

instructions : instruction                    { $$ = new cdk::sequence_node(LINE, $1);}  
             | instructions instruction        { $$ = new cdk::sequence_node(LINE, $2, $1);}
             ;

instruction : expr                           { $$ = new til::evaluation_node(LINE, $1); }
            | '(' tPRINT expressions ')'     { $$ = new til::print_node(LINE, $3, false); }
            | '(' tPRINTLN expressions ')'   { $$ = new til::print_node(LINE, $3, true); }
            | '(' tSTOP tINTEGER ')'         { $$ = new til::stop_node(LINE, $3); }  
            | '(' tSTOP ')'                  { $$ = new til::stop_node(LINE, 1); }
            | '(' tNEXT tINTEGER ')'         { $$ = new til::next_node(LINE, $3); }  
            | '(' tNEXT ')'                  { $$ = new til::next_node(LINE, 1); } 
            | '(' tRETURN expr ')'           { $$ = new til::return_node(LINE, $3); }
            | '(' tRETURN ')'                { $$ = new til::return_node(LINE, nullptr); }
            | '(' tLOOP expr instruction ')'              { $$ = new til::while_node(LINE, $3, $4); }
            | '(' tIF  expr instruction ')'               { $$ = new til::if_node(LINE, $3, $4); }
            | '(' tIF expr instruction instruction ')'    { $$ = new til::if_else_node(LINE, $3, $4, $5); }
            | block   {$$ = $1;}
            ;

block : '(' tBLOCK block_content ')'                     { $$ = $3; } 
      ;

expressions : expr                    { $$ = new cdk::sequence_node(LINE, $1);}  
            | expressions expr        { $$ = new cdk::sequence_node(LINE, $2, $1);}
            ;

expr : expr_element                 { $$ = $1; }
     | '(' tREAD ')'                { $$ = new til::read_node(LINE); }
     | '(' '-' expr ')' { $$ = new cdk::unary_minus_node(LINE, $3); }
     | '(' '+' expr ')' { $$ = new cdk::unary_plus_node(LINE, $3); }
     | '(' '+' expr expr ')'        { $$ = new cdk::add_node(LINE, $3, $4); }
     | '(' '-' expr expr ')'        { $$ = new cdk::sub_node(LINE, $3, $4); }
     | '(' '*' expr expr ')'        { $$ = new cdk::mul_node(LINE, $3, $4); }
     | '(' '/' expr expr ')'        { $$ = new cdk::div_node(LINE, $3, $4); }
     | '(' '%' expr expr ')'        { $$ = new cdk::mod_node(LINE, $3, $4); }
     | '(' '<' expr expr ')'        { $$ = new cdk::lt_node(LINE, $3, $4); }
     | '(' '>' expr expr ')'        { $$ = new cdk::gt_node(LINE, $3, $4); }
     | '(' tGE expr expr ')'        { $$ = new cdk::ge_node(LINE, $3, $4); }
     | '(' tLE expr expr ')'        { $$ = new cdk::le_node(LINE, $3, $4); }
     | '(' tNE expr expr ')'        { $$ = new cdk::ne_node(LINE, $3, $4); }
     | '(' tEQ expr expr ')'        { $$ = new cdk::eq_node(LINE, $3, $4); }
     | '(' '~' expr ')'             { $$ = new cdk::not_node(LINE, $3); }
     | '(' tAND expr expr ')'       { $$ = new cdk::and_node(LINE, $3, $4); }
     | '(' tOR expr expr ')'        { $$ = new cdk::or_node(LINE, $3, $4); }
     | '(' tSIZEOF expr ')'         { $$ = new til::sizeof_node(LINE, $3); }
     | '(' tOBJECTS expr ')'        { $$ = new til::alloc_node(LINE, $3); }
     | '(' '?' lval ')'             { $$ = new til::address_of_node(LINE, $3);}
     | func_def                     { $$ = $1; }
     | lval                         { $$ = new cdk::rvalue_node(LINE, $1); }
     | '(' tSET lval expr')'        { $$ = new cdk::assignment_node(LINE, $3, $4); }
     | '(' expr expressions ')'     { $$ = new til::function_call_node(LINE, $2, $3); }
     | '(' expr ')'                 { $$ = new til::function_call_node(LINE, $2, new cdk::sequence_node(LINE)); }
     | '(' '@' expressions ')'      { $$ = new til::function_call_node(LINE, nullptr, $3); }
     | '(' '@' ')'                  { $$ = new til::function_call_node(LINE, nullptr, new cdk::sequence_node(LINE)); }
     ;

func_def : '(' tFUNCTION '(' func_return_type func_arguments ')' block_content ')'        { $$ = new til::function_definition_node(LINE, $4, $5, $7); }
         | '(' tFUNCTION '(' func_return_type ')' block_content ')'                       { $$ = new til::function_definition_node(LINE, $4 ,new cdk::sequence_node(LINE), $6); }
         ; 

func_arguments : func_argument                              { $$ = new cdk::sequence_node(LINE, $1); }
               | func_arguments func_argument               { $$ = new cdk::sequence_node(LINE, $2, $1); }
               ;

func_argument : '(' type tIDENTIFIER ')'                    { $$ = new til::declaration_node(LINE, tPRIVATE, $2, *$3, nullptr); delete $3;}
              ;

expr_element : tINTEGER              { $$ = new cdk::integer_node(LINE, $1); }
             | tDOUBLE               { $$ = new cdk::double_node(LINE, $1); }
             | tSTRING               { $$ = new cdk::string_node(LINE, $1); }
             | tNULL                 { $$ = new til::nullptr_node(LINE); }
             ;

lval : tIDENTIFIER                  { $$ = new cdk::variable_node(LINE, $1); }
     | '(' tINDEX expr expr')'      { $$ = new til::index_pointer_node(LINE, $3 , $4); }   
     ;

%%
