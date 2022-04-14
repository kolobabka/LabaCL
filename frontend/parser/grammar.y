%language "c++"

%skeleton "lalr1.cc"

%defines
%define lr.type ielr
%define api.value.type variant
%param {yy::FrontendDriver* driver}

%code requires
{
#include <iostream>
#include <string>
#include "frontend/AST/ast.hpp"


namespace yy {class FrontendDriver;};

}

%code
{
extern int yylineno;
#include "frontend/driver/driver.hpp"
#include <string>

namespace yy {

    parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* location, FrontendDriver *driver);

}

namespace {

    AST::OperNode* makeUnaryOperNode (AST::OperNode::OperType type, AST::Node* child, yy::location loc) {

        AST::OperNode* newOperNode = new AST::OperNode (type, loc);
        newOperNode->addChild (child);
        return newOperNode;
    }

    AST::OperNode* makeBinOperNode (AST::OperNode::OperType type, 
                                    AST::Node* firstChild, AST::Node* secondChild, yy::location loc) {

        AST::OperNode* newOperNode = new AST::OperNode (type, loc);
        newOperNode->addChild (firstChild);
        newOperNode->addChild (secondChild);
        return newOperNode;
    }

    AST::CondNode* makeCondNode (AST::CondNode::ConditionType type, AST::Node* expr, 
                                 AST::Node* body, yy::location loc) {

        AST::CondNode* newCondNode = new AST::CondNode (type, loc);
        newCondNode->addChild (expr);
        newCondNode->addChild (body);
        return newCondNode;
    }

    AST::OperNode* makeAssign (const std::string &firstChild, AST::Node* secondChild, 
                               yy::location assignLoc, yy::location varLoc) {
        AST::OperNode* newNode  = new AST::OperNode (AST::OperNode::OperType::ASSIGN, assignLoc);
        newNode->addChild (new AST::VarNode (firstChild, varLoc));
        newNode->addChild (secondChild);
        return newNode;
    }
}

}

%locations

%token <int>                NUMBER
%token <std::string>        ID

%token                      ASSIGN      "="

%token                      ADD         "+"
%token                      SUB         "-"

%token                      MUL         "*"
%token                      DIV         "/"
%token                      MOD         "%"

%token                      EQ          "=="
%token                      NEQ         "!="
%token                      MORE        ">"
%token                      LESS        "<"
%token                      GTE         ">="
%token                      LTE         "<="

%token                      AND         "&&"
%token                      OR          "||"

%token                      IF          "if"
%token                      ELSE        "else"

%token                      WHILE       "while"
%token                      PRINT       "print"

%token                      OPCIRCBRACK "("
%token                      CLCIRCBRACK ")"

%token                      OPCURVBRACK "{"
%token                      CLCURVBRACK "}"

%token                      COMMA       ","
%token                      SEMICOLON   ";"

%token                      COLON       ":"

%token                      SCAN        "?"

%token                      END         0   "end of file"

%token                      FUNC_DECL   "func"
%token                      RET         "return"

%token                      LEXERR

/*PRIORITIES*/
%left SHIFT_THERE
%right ELSE THEN
%right ASSIGN
%left OR
%left AND
%left EQ NEQ
%left MORE LESS GTE LTE
%left ADD SUB
%left MUL DIV MOD
%right UNARY_OP
%right REDUCE_THERE

/* AST TREE */
%type <std::vector<AST::Node*>*>    statementHandler
%type <AST::Node*>                  statement
%type <AST::Node*>                  statementNoUnaryOpers
%type <AST::Node*>                  pseudoSt

%type <AST::Node*>                  conditionStmt
%type <AST::Node*>                  body

%type <AST::Node*>                  func
%type <std::vector<AST::Node*>*>    argsList
%type <std::vector<AST::Node*>*>    args

%type <std::vector<AST::Node*>*>    exprList
%type <std::vector<AST::Node*>*>    expA

%type <AST::Node*>                  returnStatement
%type <AST::Node*>                  printStatement

%type <AST::Node*>                  condition

%type <AST::Node*>                  expression
%type <AST::Node*>                  expressionNoUnaryOpers
%type <AST::Node*>                  opStatement
%type <AST::Node*>                  opNotUnaryStatement

%start translationStart

%%

translationStart            :   statementHandler                {
                                                                    AST::ScopeNode* globalScope = new AST::ScopeNode (@1, nullptr);
                                                                    if ($1) {

                                                                        for (auto curStmtNode: *($1))             
                                                                        {
                                                                            if (!curStmtNode)
                                                                                continue;
                                                                            
                                                                            globalScope->addChild (curStmtNode);
                                                                        }
                                                                        
                                                                        delete $1;
                                                                    }
                                                                    driver->setRoot (globalScope);
                                                                    #if 1
                                                                        driver->callDump (std::cout);
                                                                        exit (0);
                                                                    #endif
                                                                };

statementHandler            :   statement                       {
                                                                    if ($1) {
                                                                        $$ = new std::vector<AST::Node*>;
                                                                        $$->push_back ($1);
                                                                    } else
                                                                        $$ = nullptr;
                                                                }
                            /* |   ID ASSIGN body pseudoSt         {
                                                                    if ($3) {
                                                                        $$ = new std::vector<AST::Node*>;
                                                                        $$->push_back (makeAssign ($1, $3, @2, @1));
                                                                        $$->push_back ($4);
                                                                    } else
                                                                        $$ = nullptr;
                                                                }
                            |   ID ASSIGN body END              {
                                                                    if ($3) {
                                                                        $$ = new std::vector<AST::Node*>;
                                                                        $$->push_back (makeAssign ($1, $3, @2, @1));
                                                                    } else
                                                                        $$ = nullptr;
                                                                } */
                            |   body statementNoUnaryOpers      {
                                                                    if ($1) {
                                                                        $$ = new std::vector<AST::Node*>;
                                                                        $$->push_back ($1);
                                                                        $$->push_back ($2);
                                                                    } else
                                                                        $$ = nullptr;
                                                                }
                            |   body END                        {   
                                                                    if ($1) {
                                                                        $$ = new std::vector<AST::Node*>;
                                                                        $$->push_back ($1);
                                                                    } else
                                                                        $$ = nullptr;
                                                                }
                            /* |   statementHandler body statementNoUnaryOpers
                                                                {
                                                                    if ($1 && $2) {
                                                                        if ($2->getType () != AST::NodeT::FILLER) {
                                                                            $1->push_back ($2);
                                                                            $1->push_back ($3);
                                                                        }
                                                                        $$ = $1;
                                                                    } else {
                                                                        $$ = nullptr;
                                                                        if ($1) {
                                                                            for (auto v: *($1))
                                                                                delete v;
                                                                        }
                                                                        delete $1;
                                                                        delete $2;
                                                                    }

                                                                } */
                            |   statementHandler statement      {
                                                                    if ($1 && $2) {
                                                                        if ($2->getType () != AST::NodeT::FILLER)
                                                                            $1->push_back ($2);
                                                                        $$ = $1;
                                                                    } else {
                                                                        $$ = nullptr;
                                                                        if ($1) {
                                                                            for (auto v: *($1))
                                                                                delete v;
                                                                        }
                                                                        delete $1;
                                                                        delete $2;
                                                                    }
                                                                };

statement                   :   expression                      {   $$ = $1;                    }
                            |   pseudoSt                        {   $$ = $1;                    }
                            |   SEMICOLON                       {   $$ = new AST::Filler;       };

pseudoSt                    :   printStatement                  {   $$ = $1;                    }
                            |   returnStatement                 {   $$ = $1;                    }
                            |   conditionStmt                   {   $$ = $1;                    };

statementNoUnaryOpers       :   expressionNoUnaryOpers          {   $$ = $1;                    }
                            |   pseudoSt                        {   $$ = $1;                    };
                            /* |   SEMICOLON                       {   $$ = new AST::Filler;       }; */

expressionNoUnaryOpers      :   ID ASSIGN func                  {   $$ = makeAssign ($1, $3, @2, @1);   }
                            |   ID error SEMICOLON              {   driver->pushError (@1, "Undefined statement");  $$ = nullptr;   }
                            |   error ASSIGN opStatement SEMICOLON 
                                                                {   driver->pushError (@1, "Undefined statement");  $$ = nullptr;   }
                            |   opNotUnaryStatement SEMICOLON   {   $$ = $1;                            };

returnStatement             :   RET opStatement SEMICOLON       {   $$ = makeUnaryOperNode (AST::OperNode::OperType::RETURN, $2, @1);   };

printStatement              :   PRINT opStatement SEMICOLON     {   $$ = makeUnaryOperNode (AST::OperNode::OperType::PRINT, $2, @1);     }
                            |   PRINT error END                 {   driver->pushError (@2, "Undefined expression in print");    $$ = nullptr;   };

conditionStmt               :   IF condition statement ELSE statement
                                                                {
                                                                    if ($2 && $3) {
                                                                        $$ = makeCondNode (AST::CondNode::ConditionType::IF, $2, $3, @1);
                                                                        $$->addChild ($5);
                                                                    } else {
                                                                        $$ = nullptr;
                                                                        delete $2;
                                                                        delete $3;
                                                                    }

                                                                }
                            |   IF condition statement %prec THEN
                                                                {
                                                                    if ($2 && $3) {
                                                                        $$ = makeCondNode (AST::CondNode::ConditionType::IF, $2, $3, @1);
                                                                    } else {
                                                                        $$ = nullptr;
                                                                        delete $2;
                                                                        delete $3;
                                                                    }
                                                                }
                            |   WHILE condition statement     
                                                                {
                                                                    if ($2 && $3) {
                                                                        $$ = makeCondNode (AST::CondNode::ConditionType::WHILE, $2, $3, @1);
                                                                    } else {
                                                                        $$ = nullptr;
                                                                        delete $2;
                                                                        delete $3;
                                                                    }
                                                                };

condition                   :   OPCIRCBRACK opStatement CLCIRCBRACK     
                                                                {   $$ = $2; };

expression                  :   ID ASSIGN func                  {   $$ = makeAssign ($1, $3, @2, @1);   }
                            |   ID error SEMICOLON              {   driver->pushError (@1, "Undefined statement");  $$ = nullptr;   }
                            |   error ASSIGN opStatement SEMICOLON 
                                                                {   driver->pushError (@1, "Undefined statement");  $$ = nullptr;   }
                            |   opStatement SEMICOLON           {   $$ = $1;                            };

func                        :   FUNC_DECL argsList body         {
                                                                    AST::FuncNode* funcDecl = new AST::FuncNode (AST::FuncNode::FuncComponents::FUNC_DECL, @1);
                                                                    AST::FuncNode* funcArgs = new AST::FuncNode (AST::FuncNode::FuncComponents::FUNC_ARGS, @2);
                                                                    if ($2) {
                                                                        for (auto v: *($2))
                                                                            funcArgs->addChild (v);
                                                                        delete $2;
                                                                    }
                                                                    funcDecl->addChild (funcArgs);
                                                                    funcDecl->addChild ($3);
                                                                    $$ = funcDecl;
                                                                }
                            |   FUNC_DECL argsList COLON ID body
                                                                {
                                                                    AST::FuncNode* funcDecl = new AST::FuncNode (AST::FuncNode::FuncComponents::FUNC_DECL, @1);
                                                                    AST::FuncNode* funcArgs = new AST::FuncNode (AST::FuncNode::FuncComponents::FUNC_ARGS, @2);
                                                                    if ($2) {
                                                                        for (auto v: *($2))
                                                                            funcArgs->addChild (v);
                                                                        delete $2;
                                                                    }

                                                                    AST::FuncNode* funcName = new AST::FuncNode (AST::FuncNode::FuncComponents::FUNC_NAME, @4);
                                                                    AST::VarNode*  funcID   = new AST::VarNode ($4, @4);
                                                                    funcName->addChild (funcID);

                                                                    funcDecl->addChild (funcName);
                                                                    funcDecl->addChild (funcArgs);
                                                                    funcDecl->addChild ($5);
                                                                    $$ = funcDecl;
                                                                };

argsList                    :   OPCIRCBRACK args CLCIRCBRACK    {   $$ = $2;        }
                            |   OPCIRCBRACK CLCIRCBRACK         {   $$ = nullptr;   };

args                        :   ID                              {
                                                                    $$ = new std::vector<AST::Node*>;
                                                                    AST::VarNode* newParam = new AST::VarNode ($1, @1);
                                                                    $$->push_back (newParam);
                                                                }
                            |   args COMMA ID                   {
                                                                    AST::VarNode* newParam = new AST::VarNode ($3, @3);
                                                                    $1->push_back (newParam);
                                                                    $$ = $1;
                                                                };

exprList                    :   OPCIRCBRACK expA CLCIRCBRACK    {   $$ = $2;        }
                            |   OPCIRCBRACK CLCIRCBRACK         {   $$ = nullptr;   };

expA                        :   opStatement                     {
                                                                    $$ = new std::vector<AST::Node*>;
                                                                    $$->push_back ($1);
                                                                }
                            |   expA COMMA opStatement          {   $1->push_back ($3); $$ = $1;    };

/*OPERATORS*/
opNotUnaryStatement         :   NUMBER                          {   $$ = new AST::NumNode   ($1);                                       }
                            |   body                            {   $$ = $1;                                                            }
                            |   ID                              {   $$ = new AST::VarNode   ($1, @1);                                   }
                            |   error                           {   driver->pushError (@1, "Undefined statement");  $$ = nullptr;       }
                            |   SCAN                            {   $$ = new AST::OperNode  (AST::OperNode::OperType::SCAN, @1);    }
                            |   ID exprList                     {
                                                                    $$ = new AST::OperNode  (AST::OperNode::OperType::CALL, @1);
                                                                    AST::VarNode* funcName  = new AST::VarNode ($1, @1);

                                                                    AST::FuncNode* funcArgs = new AST::FuncNode (AST::FuncNode::FuncComponents::FUNC_ARGS, @2);
                                                                    if ($2) {
                                                                        for (auto v: *($2))
                                                                            funcArgs->addChild (v);
                                                                        delete $2;
                                                                    }

                                                                    $$->addChild (funcName);
                                                                    $$->addChild (funcArgs);
                                                                }
                            |   opStatement OR opStatement      {   $$ = makeBinOperNode (AST::OperNode::OperType::OR, $1, $3, @2);     }
                            |   opStatement AND opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::AND, $1, $3, @2);    }
                            |   opStatement EQ opStatement      {   $$ = makeBinOperNode (AST::OperNode::OperType::EQ, $1, $3, @2);     }
                            |   opStatement NEQ opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::NEQ, $1, $3, @2);    }
                            |   opStatement MORE opStatement    {   $$ = makeBinOperNode (AST::OperNode::OperType::MORE, $1, $3, @2);   }
                            |   opStatement LESS opStatement    {   $$ = makeBinOperNode (AST::OperNode::OperType::LESS, $1, $3, @2);   }
                            |   opStatement GTE opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::GTE, $1, $3, @2);    }
                            |   opStatement LTE opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::LTE, $1, $3, @2);    }
                            |   opStatement ADD opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::ADD, $1, $3, @2);    }
                            |   opStatement SUB opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::SUB, $1, $3, @2);    }
                            |   opStatement MUL opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::MUL, $1, $3, @2);    }
                            |   opStatement DIV opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::DIV, $1, $3, @2);    }
                            |   opStatement MOD opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::MOD, $1, $3, @2);    }
                            |   ID ASSIGN opStatement           {   $$ = makeAssign ($1, $3, @2, @1);                                   }
                            |   OPCIRCBRACK opStatement CLCIRCBRACK 
                                                                {   $$ = $2;                                                            };

opStatement                 :   NUMBER                          {   $$ = new AST::NumNode   ($1);                                       }
                            |   body                            {   $$ = $1;                                                            }
                            |   ID                              {   $$ = new AST::VarNode   ($1, @1);                                   }
                            |   error                           {   driver->pushError (@1, "Undefined statement");  $$ = nullptr;       }
                            |   SCAN                            {   $$ = new AST::OperNode  (AST::OperNode::OperType::SCAN, @1);    }
                            |   ID exprList                     {
                                                                    $$ = new AST::OperNode  (AST::OperNode::OperType::CALL, @1);
                                                                    AST::VarNode* funcName  = new AST::VarNode ($1, @1);

                                                                    AST::FuncNode* funcArgs = new AST::FuncNode (AST::FuncNode::FuncComponents::FUNC_ARGS, @2);
                                                                    if ($2) {
                                                                        for (auto v: *($2))
                                                                            funcArgs->addChild (v);
                                                                        delete $2;
                                                                    }

                                                                    $$->addChild (funcName);
                                                                    $$->addChild (funcArgs);
                                                                }
                            |   SUB opStatement %prec UNARY_OP  {   $$ = makeUnaryOperNode (AST::OperNode::OperType::UNARY_M, $2, @1);  }
                            |   ADD opStatement %prec UNARY_OP  {   $$ = makeUnaryOperNode (AST::OperNode::OperType::UNARY_P, $2, @1);  }
                            |   opStatement OR opStatement      {   $$ = makeBinOperNode (AST::OperNode::OperType::OR, $1, $3, @2);     }
                            |   opStatement AND opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::AND, $1, $3, @2);    }
                            |   opStatement EQ opStatement      {   $$ = makeBinOperNode (AST::OperNode::OperType::EQ, $1, $3, @2);     }
                            |   opStatement NEQ opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::NEQ, $1, $3, @2);    }
                            |   opStatement MORE opStatement    {   $$ = makeBinOperNode (AST::OperNode::OperType::MORE, $1, $3, @2);   }
                            |   opStatement LESS opStatement    {   $$ = makeBinOperNode (AST::OperNode::OperType::LESS, $1, $3, @2);   }
                            |   opStatement GTE opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::GTE, $1, $3, @2);    }
                            |   opStatement LTE opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::LTE, $1, $3, @2);    }
                            |   opStatement ADD opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::ADD, $1, $3, @2);    }
                            |   opStatement SUB opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::SUB, $1, $3, @2);    }
                            |   opStatement MUL opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::MUL, $1, $3, @2);    }
                            |   opStatement DIV opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::DIV, $1, $3, @2);    }
                            |   opStatement MOD opStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::MOD, $1, $3, @2);    }
                            |   ID ASSIGN opStatement           {   $$ = makeAssign ($1, $3, @2, @1);                                   }
                            |   OPCIRCBRACK opStatement CLCIRCBRACK 
                                                                {   $$ = $2;                                                            };

body                        :   OPCURVBRACK statementHandler CLCURVBRACK
                                                                {
                                                                    AST::ScopeNode* newScope = new AST::ScopeNode (@1);
                                                                    if ($2) {
                                                                        for (auto curStmtNode: *($2))
                                                                           newScope->addChild (curStmtNode);
                                                                    }
                                                                    delete $2;
                                                                    $$ = newScope;
                                                                }
                            |   OPCURVBRACK statementHandler body CLCURVBRACK 
                                                                {
                                                                    AST::ScopeNode* newScope = new AST::ScopeNode (@1);
                                                                    if ($2) {
                                                                        for (auto curStmtNode: *($2))
                                                                           newScope->addChild (curStmtNode);
                                                                    }
                                                                    delete $2;
                                                                    newScope->addChild ($3);
                                                                    $$ = newScope;
                                                                }
                            |   OPCURVBRACK CLCURVBRACK         {   $$ = new AST::ScopeNode (@1);   };

%%

namespace yy {

parser::token_type yylex (parser::semantic_type* yylval, parser::location_type* location, FrontendDriver* driver) {
    
    try {

        return driver->yylex (yylval, location);
    
    } catch (std::runtime_error& err) {

        std::cout << err.what () << std::endl;
        throw err; 

    }

}

void parser::error (const parser::location_type& location, const std::string& what) {

}

}
