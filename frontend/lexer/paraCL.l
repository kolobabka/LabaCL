%option c++

%option yylineno
%option yyclass="ParaCLexer"

%{
    #include <stdio.h>
    #include "grammar.tab.hh"
    
    #include "../frontend/customLexer/customLexer.hpp"
    #define YY_USER_ACTION setLocation ();
    #define yyterminate() return yy::parser::token_type::END

%}

TRASH_SYMBS             [ \t\r\n\v]+
DIGIT                   [0-9]

ALPHA                   [a-zA-Z]

WORD                    ({ALPHA}|"_")({ALPHA}|{DIGIT}|"_")*

ONE_LINE_COMMENT        "//".*
MULTILINE_COMMENT       "/*"(.|\n)*"*/"

STRING                  \"(\\.|[^\"])*\"

%%

{TRASH_SYMBS}           {   /*skip*/    }
{ONE_LINE_COMMENT}      {   /*skip*/    }
{MULTILINE_COMMENT}     {   /*skip*/    }

{DIGIT}*                {   return yy::parser::token_type::NUMBER;      }
{DIGIT}+"."{DIGIT}+     {   return yy::parser::token_type::NUMBER;      }

"if"                    {   return yy::parser::token_type::IF;          }
"while"                 {   return yy::parser::token_type::WHILE;       }

"print"                 {   return yy::parser::token_type::PRINT;       }
"get"                   {   return yy::parser::token_type::GET;         }
"sizeof"                {   return yy::parser::token_type::SIZEOF;      }
"sqrt"                  {   return yy::parser::token_type::SQRT;        }

"?"                     {   return yy::parser::token_type::SCAN;        }

","                     {   return yy::parser::token_type::COMMA;       }
";"                     {   return yy::parser::token_type::SEMICOLON;   }

"("                     {   return yy::parser::token_type::OPCIRCBRACK; }
")"                     {   return yy::parser::token_type::CLCIRCBRACK; }

"["                     {   return yy::parser::token_type::OPSQBRACK;   }
"]"                     {   return yy::parser::token_type::CLSQBRACK;   }

"{"                     {   return yy::parser::token_type::OPCURVBRACK; }
"}"                     {   return yy::parser::token_type::CLCURVBRACK; }

"+"                     {   return yy::parser::token_type::ADD;         }
"-"                     {   return yy::parser::token_type::SUB;         }
"*"                     {   return yy::parser::token_type::MUL;         }
"/"                     {   return yy::parser::token_type::DIV;         }
"%"                     {   return yy::parser::token_type::MOD;         }

"=="                    {   return yy::parser::token_type::EQ;          }
"!="                    {   return yy::parser::token_type::NEQ;         }
">"                     {   return yy::parser::token_type::MORE;        }
"<"                     {   return yy::parser::token_type::LESS;        }
">="                    {   return yy::parser::token_type::GTE;         }
"<="                    {   return yy::parser::token_type::LTE;         }

"&&"                    {   return yy::parser::token_type::AND;         }
"||"                    {   return yy::parser::token_type::OR;          }

"="                     {   return yy::parser::token_type::ASSIGN;      }
":"                     {   return yy::parser::token_type::COLON;       }

"func"                  {   return yy::parser::token_type::FUNC_DECL;   }
"return"                {   return yy::parser::token_type::RET;         }

"tex_add_section"       {   return yy::parser::token_type::TEX_ADD_SECTION; }
"tex_add_text"          {   return yy::parser::token_type::TEX_ADD_TEXT;    }

"tex_add_std_head"      {   return yy::parser::token_type::TEX_ADD_STD_HEAD;}
"tex_add_end"           {   return yy::parser::token_type::TEX_ADD_END;     }

"tex_add_content"       {   return yy::parser::token_type::TEX_ADD_CONTENT;     }

"tex_compile"           {   return yy::parser::token_type::TEX_COMPILE; }

"create_graph"          {   return yy::parser::token_type::CREATE_GRAPH;}

{WORD}                  {   return yy::parser::token_type::ID;          }
{STRING}                {   return yy::parser::token_type::TEXT;        }

.                       {   return yy::parser::token_type::LEXERR;      }

%%
