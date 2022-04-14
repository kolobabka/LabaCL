// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "grammar.tab.hh"


// Unqualified %code blocks.
#line 21 "frontend/parser/grammar.y"

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


#line 95 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 187 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"

  /// Build a parser object.
  parser::parser (yy::FrontendDriver* driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      driver (driver_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_returnStatement: // returnStatement
      case symbol_kind::S_printStatement: // printStatement
      case symbol_kind::S_ifStatement: // ifStatement
      case symbol_kind::S_whileStatement: // whileStatement
      case symbol_kind::S_conditionExpression: // conditionExpression
      case symbol_kind::S_body: // body
      case symbol_kind::S_assignment: // assignment
      case symbol_kind::S_func: // func
      case symbol_kind::S_assignStatement: // assignStatement
      case symbol_kind::S_orStatement: // orStatement
      case symbol_kind::S_andStatement: // andStatement
      case symbol_kind::S_eqStatement: // eqStatement
      case symbol_kind::S_cmpStatement: // cmpStatement
      case symbol_kind::S_addStatement: // addStatement
      case symbol_kind::S_mulStatement: // mulStatement
      case symbol_kind::S_unaryStatement: // unaryStatement
      case symbol_kind::S_term: // term
      case symbol_kind::S_atomic: // atomic
        value.copy< AST::Node* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // NUMBER
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statementHandler: // statementHandler
      case symbol_kind::S_argsList: // argsList
      case symbol_kind::S_args: // args
      case symbol_kind::S_exprList: // exprList
      case symbol_kind::S_expA: // expA
        value.copy< std::vector<AST::Node*>* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_returnStatement: // returnStatement
      case symbol_kind::S_printStatement: // printStatement
      case symbol_kind::S_ifStatement: // ifStatement
      case symbol_kind::S_whileStatement: // whileStatement
      case symbol_kind::S_conditionExpression: // conditionExpression
      case symbol_kind::S_body: // body
      case symbol_kind::S_assignment: // assignment
      case symbol_kind::S_func: // func
      case symbol_kind::S_assignStatement: // assignStatement
      case symbol_kind::S_orStatement: // orStatement
      case symbol_kind::S_andStatement: // andStatement
      case symbol_kind::S_eqStatement: // eqStatement
      case symbol_kind::S_cmpStatement: // cmpStatement
      case symbol_kind::S_addStatement: // addStatement
      case symbol_kind::S_mulStatement: // mulStatement
      case symbol_kind::S_unaryStatement: // unaryStatement
      case symbol_kind::S_term: // term
      case symbol_kind::S_atomic: // atomic
        value.move< AST::Node* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NUMBER: // NUMBER
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ID: // ID
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statementHandler: // statementHandler
      case symbol_kind::S_argsList: // argsList
      case symbol_kind::S_args: // args
      case symbol_kind::S_exprList: // exprList
      case symbol_kind::S_expA: // expA
        value.move< std::vector<AST::Node*>* > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_returnStatement: // returnStatement
      case symbol_kind::S_printStatement: // printStatement
      case symbol_kind::S_ifStatement: // ifStatement
      case symbol_kind::S_whileStatement: // whileStatement
      case symbol_kind::S_conditionExpression: // conditionExpression
      case symbol_kind::S_body: // body
      case symbol_kind::S_assignment: // assignment
      case symbol_kind::S_func: // func
      case symbol_kind::S_assignStatement: // assignStatement
      case symbol_kind::S_orStatement: // orStatement
      case symbol_kind::S_andStatement: // andStatement
      case symbol_kind::S_eqStatement: // eqStatement
      case symbol_kind::S_cmpStatement: // cmpStatement
      case symbol_kind::S_addStatement: // addStatement
      case symbol_kind::S_mulStatement: // mulStatement
      case symbol_kind::S_unaryStatement: // unaryStatement
      case symbol_kind::S_term: // term
      case symbol_kind::S_atomic: // atomic
        value.YY_MOVE_OR_COPY< AST::Node* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // NUMBER
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statementHandler: // statementHandler
      case symbol_kind::S_argsList: // argsList
      case symbol_kind::S_args: // args
      case symbol_kind::S_exprList: // exprList
      case symbol_kind::S_expA: // expA
        value.YY_MOVE_OR_COPY< std::vector<AST::Node*>* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_returnStatement: // returnStatement
      case symbol_kind::S_printStatement: // printStatement
      case symbol_kind::S_ifStatement: // ifStatement
      case symbol_kind::S_whileStatement: // whileStatement
      case symbol_kind::S_conditionExpression: // conditionExpression
      case symbol_kind::S_body: // body
      case symbol_kind::S_assignment: // assignment
      case symbol_kind::S_func: // func
      case symbol_kind::S_assignStatement: // assignStatement
      case symbol_kind::S_orStatement: // orStatement
      case symbol_kind::S_andStatement: // andStatement
      case symbol_kind::S_eqStatement: // eqStatement
      case symbol_kind::S_cmpStatement: // cmpStatement
      case symbol_kind::S_addStatement: // addStatement
      case symbol_kind::S_mulStatement: // mulStatement
      case symbol_kind::S_unaryStatement: // unaryStatement
      case symbol_kind::S_term: // term
      case symbol_kind::S_atomic: // atomic
        value.move< AST::Node* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // NUMBER
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statementHandler: // statementHandler
      case symbol_kind::S_argsList: // argsList
      case symbol_kind::S_args: // args
      case symbol_kind::S_exprList: // exprList
      case symbol_kind::S_expA: // expA
        value.move< std::vector<AST::Node*>* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_returnStatement: // returnStatement
      case symbol_kind::S_printStatement: // printStatement
      case symbol_kind::S_ifStatement: // ifStatement
      case symbol_kind::S_whileStatement: // whileStatement
      case symbol_kind::S_conditionExpression: // conditionExpression
      case symbol_kind::S_body: // body
      case symbol_kind::S_assignment: // assignment
      case symbol_kind::S_func: // func
      case symbol_kind::S_assignStatement: // assignStatement
      case symbol_kind::S_orStatement: // orStatement
      case symbol_kind::S_andStatement: // andStatement
      case symbol_kind::S_eqStatement: // eqStatement
      case symbol_kind::S_cmpStatement: // cmpStatement
      case symbol_kind::S_addStatement: // addStatement
      case symbol_kind::S_mulStatement: // mulStatement
      case symbol_kind::S_unaryStatement: // unaryStatement
      case symbol_kind::S_term: // term
      case symbol_kind::S_atomic: // atomic
        value.copy< AST::Node* > (that.value);
        break;

      case symbol_kind::S_NUMBER: // NUMBER
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_ID: // ID
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_statementHandler: // statementHandler
      case symbol_kind::S_argsList: // argsList
      case symbol_kind::S_args: // args
      case symbol_kind::S_exprList: // exprList
      case symbol_kind::S_expA: // expA
        value.copy< std::vector<AST::Node*>* > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_returnStatement: // returnStatement
      case symbol_kind::S_printStatement: // printStatement
      case symbol_kind::S_ifStatement: // ifStatement
      case symbol_kind::S_whileStatement: // whileStatement
      case symbol_kind::S_conditionExpression: // conditionExpression
      case symbol_kind::S_body: // body
      case symbol_kind::S_assignment: // assignment
      case symbol_kind::S_func: // func
      case symbol_kind::S_assignStatement: // assignStatement
      case symbol_kind::S_orStatement: // orStatement
      case symbol_kind::S_andStatement: // andStatement
      case symbol_kind::S_eqStatement: // eqStatement
      case symbol_kind::S_cmpStatement: // cmpStatement
      case symbol_kind::S_addStatement: // addStatement
      case symbol_kind::S_mulStatement: // mulStatement
      case symbol_kind::S_unaryStatement: // unaryStatement
      case symbol_kind::S_term: // term
      case symbol_kind::S_atomic: // atomic
        value.move< AST::Node* > (that.value);
        break;

      case symbol_kind::S_NUMBER: // NUMBER
        value.move< int > (that.value);
        break;

      case symbol_kind::S_ID: // ID
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_statementHandler: // statementHandler
      case symbol_kind::S_argsList: // argsList
      case symbol_kind::S_args: // args
      case symbol_kind::S_exprList: // exprList
      case symbol_kind::S_expA: // expA
        value.move< std::vector<AST::Node*>* > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location, driver));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_returnStatement: // returnStatement
      case symbol_kind::S_printStatement: // printStatement
      case symbol_kind::S_ifStatement: // ifStatement
      case symbol_kind::S_whileStatement: // whileStatement
      case symbol_kind::S_conditionExpression: // conditionExpression
      case symbol_kind::S_body: // body
      case symbol_kind::S_assignment: // assignment
      case symbol_kind::S_func: // func
      case symbol_kind::S_assignStatement: // assignStatement
      case symbol_kind::S_orStatement: // orStatement
      case symbol_kind::S_andStatement: // andStatement
      case symbol_kind::S_eqStatement: // eqStatement
      case symbol_kind::S_cmpStatement: // cmpStatement
      case symbol_kind::S_addStatement: // addStatement
      case symbol_kind::S_mulStatement: // mulStatement
      case symbol_kind::S_unaryStatement: // unaryStatement
      case symbol_kind::S_term: // term
      case symbol_kind::S_atomic: // atomic
        yylhs.value.emplace< AST::Node* > ();
        break;

      case symbol_kind::S_NUMBER: // NUMBER
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_ID: // ID
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_statementHandler: // statementHandler
      case symbol_kind::S_argsList: // argsList
      case symbol_kind::S_args: // args
      case symbol_kind::S_exprList: // exprList
      case symbol_kind::S_expA: // expA
        yylhs.value.emplace< std::vector<AST::Node*>* > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // translationStart: statementHandler
#line 163 "frontend/parser/grammar.y"
                                                                {
                                                                    AST::ScopeNode* globalScope = new AST::ScopeNode ();
                                                                    if (yystack_[0].value.as < std::vector<AST::Node*>* > ()) {
                                                                        for (auto curStmtNode: *(yystack_[0].value.as < std::vector<AST::Node*>* > ()))             
                                                                        {   
                                                                            if (!curStmtNode)
                                                                                continue;
                                                                            
                                                                            globalScope->addChild (curStmtNode);
                                                                        }
                                                                        
                                                                        delete yystack_[0].value.as < std::vector<AST::Node*>* > ();
                                                                    }
                                                                    driver->setRoot (globalScope);
                                                                    #if 0
                                                                        driver->callDump (std::cout);
                                                                    #endif
                                                                }
#line 959 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 3: // statementHandler: statement
#line 182 "frontend/parser/grammar.y"
                                                                {
                                                                    if (yystack_[0].value.as < AST::Node* > ()) {
                                                                        yylhs.value.as < std::vector<AST::Node*>* > () = new std::vector<AST::Node*>;
                                                                        yylhs.value.as < std::vector<AST::Node*>* > ()->push_back (yystack_[0].value.as < AST::Node* > ());
                                                                    } else
                                                                        yylhs.value.as < std::vector<AST::Node*>* > () = nullptr;
                                                                }
#line 971 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 4: // statementHandler: statementHandler statement
#line 189 "frontend/parser/grammar.y"
                                                                {
                                                                    if (yystack_[1].value.as < std::vector<AST::Node*>* > () && yystack_[0].value.as < AST::Node* > ()) {
                                                                        yystack_[1].value.as < std::vector<AST::Node*>* > ()->push_back (yystack_[0].value.as < AST::Node* > ());
                                                                        yylhs.value.as < std::vector<AST::Node*>* > () = yystack_[1].value.as < std::vector<AST::Node*>* > ();
                                                                    } else {
                                                                        yylhs.value.as < std::vector<AST::Node*>* > () = nullptr;
                                                                        if (yystack_[1].value.as < std::vector<AST::Node*>* > ()) {
                                                                            for (auto v: *(yystack_[1].value.as < std::vector<AST::Node*>* > ()))
                                                                                delete v;
                                                                        }
                                                                        delete yystack_[1].value.as < std::vector<AST::Node*>* > ();
                                                                        delete yystack_[0].value.as < AST::Node* > ();
                                                                    }
                                                                }
#line 990 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 5: // statement: assignment
#line 205 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();    }
#line 996 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 6: // statement: ifStatement
#line 206 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();    }
#line 1002 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 7: // statement: whileStatement
#line 207 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();    }
#line 1008 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 8: // statement: orStatement ";"
#line 208 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[1].value.as < AST::Node* > ();    }
#line 1014 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 9: // statement: printStatement
#line 209 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();    }
#line 1020 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 10: // statement: returnStatement
#line 210 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();    }
#line 1026 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 11: // statement: error ";"
#line 211 "frontend/parser/grammar.y"
                                                                {   driver->pushError (yystack_[1].location, "Undefined statement");  yylhs.value.as < AST::Node* > () = nullptr;   }
#line 1032 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 12: // statement: error "end of file"
#line 212 "frontend/parser/grammar.y"
                                                                {   driver->pushError (yystack_[1].location, "Undefined statement");  yylhs.value.as < AST::Node* > () = nullptr;   }
#line 1038 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 13: // returnStatement: "return" assignStatement ";"
#line 214 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeUnaryOperNode (AST::OperNode::OperType::RETURN, yystack_[1].value.as < AST::Node* > (), yystack_[2].location);   }
#line 1044 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 14: // printStatement: "print" assignStatement ";"
#line 216 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeUnaryOperNode (AST::OperNode::OperType::PRINT, yystack_[1].value.as < AST::Node* > (), yystack_[2].location);     }
#line 1050 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 15: // printStatement: "print" error ";"
#line 217 "frontend/parser/grammar.y"
                                                                {   driver->pushError (yystack_[1].location, "Undefined expression in print");    yylhs.value.as < AST::Node* > () = nullptr;   }
#line 1056 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 16: // printStatement: "print" error "end of file"
#line 218 "frontend/parser/grammar.y"
                                                                {   driver->pushError (yystack_[1].location, "Undefined expression in print");    yylhs.value.as < AST::Node* > () = nullptr;   }
#line 1062 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 17: // argsList: "(" args ")"
#line 220 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < std::vector<AST::Node*>* > () = yystack_[1].value.as < std::vector<AST::Node*>* > ();        }
#line 1068 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 18: // argsList: "(" ")"
#line 221 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < std::vector<AST::Node*>* > () = nullptr;   }
#line 1074 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 19: // args: ID
#line 223 "frontend/parser/grammar.y"
                                                                {
                                                                    yylhs.value.as < std::vector<AST::Node*>* > () = new std::vector<AST::Node*>;
                                                                    AST::VarNode* newParam = new AST::VarNode (yystack_[0].value.as < std::string > (), yystack_[0].location);
                                                                    yylhs.value.as < std::vector<AST::Node*>* > ()->push_back (newParam);
                                                                }
#line 1084 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 20: // args: args "," ID
#line 228 "frontend/parser/grammar.y"
                                                                {
                                                                    AST::VarNode* newParam = new AST::VarNode (yystack_[0].value.as < std::string > (), yystack_[0].location);
                                                                    yystack_[2].value.as < std::vector<AST::Node*>* > ()->push_back (newParam);
                                                                    yylhs.value.as < std::vector<AST::Node*>* > () = yystack_[2].value.as < std::vector<AST::Node*>* > ();
                                                                }
#line 1094 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 21: // exprList: "(" expA ")"
#line 234 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < std::vector<AST::Node*>* > () = yystack_[1].value.as < std::vector<AST::Node*>* > ();        }
#line 1100 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 22: // exprList: "(" ")"
#line 235 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < std::vector<AST::Node*>* > () = nullptr;   }
#line 1106 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 23: // expA: assignStatement
#line 237 "frontend/parser/grammar.y"
                                                                {
                                                                    yylhs.value.as < std::vector<AST::Node*>* > () = new std::vector<AST::Node*>;
                                                                    yylhs.value.as < std::vector<AST::Node*>* > ()->push_back (yystack_[0].value.as < AST::Node* > ());
                                                                }
#line 1115 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 24: // expA: expA "," assignStatement
#line 241 "frontend/parser/grammar.y"
                                                                {   yystack_[2].value.as < std::vector<AST::Node*>* > ()->push_back (yystack_[0].value.as < AST::Node* > ()); yylhs.value.as < std::vector<AST::Node*>* > () = yystack_[2].value.as < std::vector<AST::Node*>* > ();    }
#line 1121 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 25: // ifStatement: "if" conditionExpression body
#line 243 "frontend/parser/grammar.y"
                                                                {
                                                                    if (yystack_[1].value.as < AST::Node* > () && yystack_[0].value.as < AST::Node* > ()) {
                                                                        yylhs.value.as < AST::Node* > () = makeCondNode (AST::CondNode::ConditionType::IF, yystack_[1].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[2].location);
                                                                    } else {
                                                                        yylhs.value.as < AST::Node* > () = nullptr;
                                                                        delete yystack_[1].value.as < AST::Node* > ();
                                                                        delete yystack_[0].value.as < AST::Node* > ();
                                                                    }
                                                                }
#line 1135 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 26: // ifStatement: "if" conditionExpression statement
#line 252 "frontend/parser/grammar.y"
                                                                 {
                                                                    if (yystack_[1].value.as < AST::Node* > () && yystack_[0].value.as < AST::Node* > ()) {
                                                                        AST::ScopeNode* newScope = new AST::ScopeNode ();
                                                                        newScope->addChild (yystack_[0].value.as < AST::Node* > ());
                                                                        yylhs.value.as < AST::Node* > () = makeCondNode (AST::CondNode::ConditionType::IF, yystack_[1].value.as < AST::Node* > (), newScope, yystack_[2].location);
                                                                    } else {
                                                                        yylhs.value.as < AST::Node* > () = nullptr;
                                                                        delete yystack_[1].value.as < AST::Node* > ();
                                                                        delete yystack_[0].value.as < AST::Node* > ();
                                                                    }
                                                                }
#line 1151 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 27: // whileStatement: "while" conditionExpression body
#line 264 "frontend/parser/grammar.y"
                                                                {
                                                                    if (yystack_[1].value.as < AST::Node* > () && yystack_[0].value.as < AST::Node* > ()) {
                                                                        yylhs.value.as < AST::Node* > () = makeCondNode (AST::CondNode::ConditionType::WHILE, yystack_[1].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[2].location);
                                                                    } else {
                                                                        yylhs.value.as < AST::Node* > () = nullptr;
                                                                        delete yystack_[1].value.as < AST::Node* > ();
                                                                        delete yystack_[0].value.as < AST::Node* > ();
                                                                    }
                                                                }
#line 1165 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 28: // whileStatement: "while" conditionExpression statement
#line 274 "frontend/parser/grammar.y"
                                                                {
                                                                   if (yystack_[1].value.as < AST::Node* > () && yystack_[0].value.as < AST::Node* > ()) {
                                                                        AST::ScopeNode* newScope = new AST::ScopeNode ();
                                                                        newScope->addChild (yystack_[0].value.as < AST::Node* > ());
                                                                        yylhs.value.as < AST::Node* > () = makeCondNode (AST::CondNode::ConditionType::WHILE, yystack_[1].value.as < AST::Node* > (), newScope, yystack_[2].location);
                                                                    } else {
                                                                        yylhs.value.as < AST::Node* > () = nullptr;
                                                                        delete yystack_[1].value.as < AST::Node* > ();
                                                                        delete yystack_[0].value.as < AST::Node* > ();
                                                                    } 
                                                                }
#line 1181 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 29: // conditionExpression: "(" assignStatement ")"
#line 287 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[1].value.as < AST::Node* > ();    }
#line 1187 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 30: // conditionExpression: "(" error ")"
#line 288 "frontend/parser/grammar.y"
                                                                {   driver->pushError (yystack_[1].location, "Bad expression for condition"); yylhs.value.as < AST::Node* > () = nullptr;   }
#line 1193 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 31: // body: "{" statementHandler "}"
#line 292 "frontend/parser/grammar.y"
                                                                {
                                                                    AST::ScopeNode* newScope = new AST::ScopeNode ();
                                                                    if (yystack_[1].value.as < std::vector<AST::Node*>* > ()) {
                                                                        for (auto curStmtNode: *(yystack_[1].value.as < std::vector<AST::Node*>* > ()))
                                                                           newScope->addChild (curStmtNode);
                                                                    }
                                                                    delete yystack_[1].value.as < std::vector<AST::Node*>* > ();
                                                                    yylhs.value.as < AST::Node* > () = newScope;
                                                                }
#line 1207 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 32: // body: "{" "}"
#line 301 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = new AST::ScopeNode (yystack_[1].location);   }
#line 1213 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 33: // assignment: ID "=" assignStatement ";"
#line 305 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeAssign (yystack_[3].value.as < std::string > (), yystack_[1].value.as < AST::Node* > (), yystack_[2].location, yystack_[3].location);   }
#line 1219 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 34: // assignment: ID "=" func ";"
#line 306 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeAssign (yystack_[3].value.as < std::string > (), yystack_[1].value.as < AST::Node* > (), yystack_[2].location, yystack_[3].location);   }
#line 1225 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 35: // assignment: ID "=" func
#line 307 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeAssign (yystack_[2].value.as < std::string > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location, yystack_[2].location);   }
#line 1231 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 36: // assignment: ID "=" error ";"
#line 308 "frontend/parser/grammar.y"
                                                                {   driver->pushError (yystack_[1].location, "Bad expression after assignment");  
                                                                    yylhs.value.as < AST::Node* > () = nullptr;   
                                                                }
#line 1239 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 37: // assignment: ID "=" body
#line 311 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeAssign (yystack_[2].value.as < std::string > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location, yystack_[2].location);  }
#line 1245 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 38: // assignment: ID "=" body ";"
#line 312 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeAssign (yystack_[3].value.as < std::string > (), yystack_[1].value.as < AST::Node* > (), yystack_[2].location, yystack_[3].location);  }
#line 1251 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 39: // assignment: ID error ";"
#line 313 "frontend/parser/grammar.y"
                                                                {   driver->pushError (yystack_[2].location, "Unexpected operation with variable");   
                                                                    yylhs.value.as < AST::Node* > () = nullptr;   
                                                                }
#line 1259 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 40: // assignment: error "=" assignStatement ";"
#line 317 "frontend/parser/grammar.y"
                                                                {   
                                                                    driver->pushError (yystack_[3].location, "rvalue can't become lvalue"); 
                                                                    yylhs.value.as < AST::Node* > () = nullptr; 
                                                                }
#line 1268 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 41: // func: "func" argsList body
#line 322 "frontend/parser/grammar.y"
                                                                {
                                                                    AST::FuncNode* funcDecl = new AST::FuncNode (AST::FuncNode::FuncComponents::FUNC_DECL, yystack_[2].location);
                                                                    AST::FuncNode* funcArgs = new AST::FuncNode (AST::FuncNode::FuncComponents::FUNC_ARGS, yystack_[1].location);
                                                                    if (yystack_[1].value.as < std::vector<AST::Node*>* > ()) {
                                                                        for (auto v: *(yystack_[1].value.as < std::vector<AST::Node*>* > ()))
                                                                            funcArgs->addChild (v);
                                                                        delete yystack_[1].value.as < std::vector<AST::Node*>* > ();
                                                                    }
                                                                    funcDecl->addChild (funcArgs);
                                                                    funcDecl->addChild (yystack_[0].value.as < AST::Node* > ());
                                                                    yylhs.value.as < AST::Node* > () = funcDecl;
                                                                }
#line 1285 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 42: // func: "func" argsList ":" ID body
#line 335 "frontend/parser/grammar.y"
                                                                {
                                                                    AST::FuncNode* funcDecl = new AST::FuncNode (AST::FuncNode::FuncComponents::FUNC_DECL, yystack_[4].location);
                                                                    AST::FuncNode* funcArgs = new AST::FuncNode (AST::FuncNode::FuncComponents::FUNC_ARGS, yystack_[3].location);
                                                                    if (yystack_[3].value.as < std::vector<AST::Node*>* > ()) {
                                                                        for (auto v: *(yystack_[3].value.as < std::vector<AST::Node*>* > ()))
                                                                            funcArgs->addChild (v);
                                                                        delete yystack_[3].value.as < std::vector<AST::Node*>* > ();
                                                                    }

                                                                    AST::FuncNode* funcName = new AST::FuncNode (AST::FuncNode::FuncComponents::FUNC_NAME, yystack_[1].location);
                                                                    AST::VarNode*  funcID   = new AST::VarNode (yystack_[1].value.as < std::string > (), yystack_[1].location);
                                                                    funcName->addChild (funcID);

                                                                    funcDecl->addChild (funcName);
                                                                    funcDecl->addChild (funcArgs);
                                                                    funcDecl->addChild (yystack_[0].value.as < AST::Node* > ());
                                                                    yylhs.value.as < AST::Node* > () = funcDecl;
                                                                }
#line 1308 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 43: // assignStatement: orStatement
#line 354 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();        }
#line 1314 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 44: // assignStatement: ID "=" assignStatement
#line 355 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeAssign (yystack_[2].value.as < std::string > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location, yystack_[2].location);  }
#line 1320 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 45: // orStatement: andStatement
#line 357 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();        }
#line 1326 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 46: // orStatement: orStatement "||" andStatement
#line 358 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeBinOperNode (AST::OperNode::OperType::OR, yystack_[2].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location);     }
#line 1332 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 47: // andStatement: eqStatement
#line 361 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();        }
#line 1338 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 48: // andStatement: andStatement "&&" eqStatement
#line 362 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeBinOperNode (AST::OperNode::OperType::AND, yystack_[2].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location);    }
#line 1344 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 49: // eqStatement: cmpStatement
#line 364 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();        }
#line 1350 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 50: // eqStatement: eqStatement "==" cmpStatement
#line 365 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeBinOperNode (AST::OperNode::OperType::EQ, yystack_[2].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location);     }
#line 1356 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 51: // eqStatement: eqStatement "!=" cmpStatement
#line 366 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeBinOperNode (AST::OperNode::OperType::NEQ, yystack_[2].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location);    }
#line 1362 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 52: // cmpStatement: addStatement
#line 368 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();        }
#line 1368 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 53: // cmpStatement: cmpStatement ">" addStatement
#line 369 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeBinOperNode (AST::OperNode::OperType::MORE, yystack_[2].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location);  }
#line 1374 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 54: // cmpStatement: cmpStatement "<" addStatement
#line 370 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeBinOperNode (AST::OperNode::OperType::LESS, yystack_[2].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location);  }
#line 1380 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 55: // cmpStatement: cmpStatement ">=" addStatement
#line 371 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeBinOperNode (AST::OperNode::OperType::GTE, yystack_[2].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location);   }
#line 1386 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 56: // cmpStatement: cmpStatement "<=" addStatement
#line 372 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeBinOperNode (AST::OperNode::OperType::LTE, yystack_[2].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location);   }
#line 1392 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 57: // addStatement: mulStatement
#line 374 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();        }
#line 1398 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 58: // addStatement: addStatement "+" mulStatement
#line 375 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeBinOperNode (AST::OperNode::OperType::ADD, yystack_[2].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location);   }
#line 1404 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 59: // addStatement: addStatement "-" mulStatement
#line 376 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeBinOperNode (AST::OperNode::OperType::SUB, yystack_[2].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location);   }
#line 1410 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 60: // mulStatement: unaryStatement
#line 378 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();        }
#line 1416 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 61: // mulStatement: mulStatement "*" unaryStatement
#line 379 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeBinOperNode (AST::OperNode::OperType::MUL, yystack_[2].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location);   }
#line 1422 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 62: // mulStatement: mulStatement "/" unaryStatement
#line 380 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeBinOperNode (AST::OperNode::OperType::DIV, yystack_[2].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location);   }
#line 1428 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 63: // mulStatement: mulStatement "%" unaryStatement
#line 381 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeBinOperNode (AST::OperNode::OperType::MOD, yystack_[2].value.as < AST::Node* > (), yystack_[0].value.as < AST::Node* > (), yystack_[1].location);   }
#line 1434 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 64: // unaryStatement: term
#line 383 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();        }
#line 1440 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 65: // unaryStatement: "-" unaryStatement
#line 384 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeUnaryOperNode (AST::OperNode::OperType::UNARY_M, yystack_[0].value.as < AST::Node* > (), yystack_[1].location); }
#line 1446 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 66: // unaryStatement: "+" unaryStatement
#line 385 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = makeUnaryOperNode (AST::OperNode::OperType::UNARY_P, yystack_[0].value.as < AST::Node* > (), yystack_[1].location); }
#line 1452 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 67: // term: atomic
#line 387 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[0].value.as < AST::Node* > ();    }
#line 1458 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 68: // term: "(" assignStatement ")"
#line 389 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = yystack_[1].value.as < AST::Node* > ();    }
#line 1464 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 69: // atomic: NUMBER
#line 391 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = new AST::NumNode   (yystack_[0].value.as < int > ());                                   }
#line 1470 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 70: // atomic: "?"
#line 392 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = new AST::OperNode  (AST::OperNode::OperType::SCAN, yystack_[0].location);    }
#line 1476 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 71: // atomic: ID exprList
#line 393 "frontend/parser/grammar.y"
                                                                {
                                                                    yylhs.value.as < AST::Node* > () = new AST::OperNode  (AST::OperNode::OperType::CALL, yystack_[1].location);
                                                                    AST::VarNode* funcName  = new AST::VarNode (yystack_[1].value.as < std::string > (), yystack_[1].location);

                                                                    AST::FuncNode* funcArgs = new AST::FuncNode (AST::FuncNode::FuncComponents::FUNC_ARGS, yystack_[0].location);
                                                                    if (yystack_[0].value.as < std::vector<AST::Node*>* > ()) {
                                                                        for (auto v: *(yystack_[0].value.as < std::vector<AST::Node*>* > ()))
                                                                            funcArgs->addChild (v);
                                                                        delete yystack_[0].value.as < std::vector<AST::Node*>* > ();
                                                                    }

                                                                    yylhs.value.as < AST::Node* > ()->addChild (funcName);
                                                                    yylhs.value.as < AST::Node* > ()->addChild (funcArgs);

                                                                }
#line 1496 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;

  case 72: // atomic: ID
#line 408 "frontend/parser/grammar.y"
                                                                {   yylhs.value.as < AST::Node* > () = new AST::VarNode   (yystack_[0].value.as < std::string > (), yystack_[0].location);                               }
#line 1502 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"
    break;


#line 1506 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const signed char parser::yypact_ninf_ = -37;

  const signed char parser::yytable_ninf_ = -73;

  const short
  parser::yypact_[] =
  {
     145,    11,   -37,   216,   121,   121,    17,    17,   183,   233,
     -37,   233,    48,    14,   -37,   -37,   -37,   -37,   -37,   -37,
       1,    37,    12,    64,    54,    42,   -37,   -37,   -37,   -37,
     233,   -37,    30,   153,    92,   -37,    40,   -37,   -37,   191,
      52,    52,    10,    27,    39,    50,    47,    55,   -37,   -37,
     121,   -37,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,    59,   -37,    62,    87,    75,    77,
      78,    83,   -37,   -14,   -37,    88,    90,   -37,   -37,   -37,
     -37,   -37,   -37,   233,   -37,   -37,   -37,    37,    12,    64,
      64,    54,    54,    54,    54,    42,    42,   -37,   -37,   -37,
     -37,   -37,   -37,   116,     4,    18,   -37,   -37,   -37,   -37,
     233,   -37,   -37,   -37,   -37,   -37,   -37,    21,   122,   -37,
     -37,   -37,   129,   110,   -37,   -37
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,    69,     0,     0,     0,     0,     0,     0,     0,
      70,     0,     0,     0,     3,    10,     9,     6,     7,     5,
       0,    45,    47,    49,    52,    57,    60,    64,    67,    12,
       0,    11,     0,     0,     0,    71,    72,    66,    65,     0,
       0,     0,     0,    72,     0,    43,     0,     0,     1,     4,
       0,     8,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    39,     0,     0,     0,    37,
      35,     0,    22,     0,    23,     0,     0,    26,    25,    28,
      27,    16,    15,     0,    14,    68,    13,    46,    48,    50,
      51,    53,    54,    55,    56,    58,    59,    61,    62,    63,
      40,    36,    32,     0,     0,     0,    38,    34,    33,    21,
       0,    30,    29,    44,    31,    19,    18,     0,     0,    41,
      24,    17,     0,     0,    20,    42
  };

  const short
  parser::yypgoto_[] =
  {
     -37,   -37,    72,   -11,   -37,   -37,   -37,   -37,   -37,   -37,
     -37,   -37,   133,   -36,   -37,   -37,    -8,     0,    94,   101,
      31,    74,    41,     2,   -37,   -37
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,    12,    13,    14,    15,    16,   105,   117,    35,    73,
      17,    18,    40,    69,    19,    70,    44,    45,    21,    22,
      23,    24,    25,    26,    27,    28
  };

  const signed char
  parser::yytable_[] =
  {
      20,    46,    49,    47,    78,    80,    37,    38,   115,   109,
      81,    29,   110,    20,    -2,     1,    30,     2,     3,    50,
       4,     5,    64,    53,    54,    71,    74,   116,    51,    77,
      79,    76,    83,     6,     7,     8,     9,    82,    31,    39,
      20,    20,    67,    10,   121,    11,   118,   122,    48,    34,
      61,    62,    63,     1,    52,     2,     3,    65,     4,     5,
      59,    60,    34,    97,    98,    99,    84,    20,    50,   119,
      85,     6,     7,     8,     9,   113,    67,    55,    56,    57,
      58,    10,    86,    11,    89,    90,   100,   125,     1,   101,
       2,     3,    49,     4,     5,     2,    43,   104,     4,     5,
      95,    96,   120,    20,   106,   107,     6,     7,     8,     9,
     108,   111,   102,   112,     9,    72,    10,     1,    11,     2,
       3,    10,     4,     5,     2,    36,   123,     4,     5,    91,
      92,    93,    94,   124,    67,     6,     7,     8,     9,   103,
      41,   114,     0,     9,    87,    10,     1,    11,     2,     3,
      10,     4,     5,    88,    66,     0,     2,    43,     0,     4,
       5,     0,     0,     0,     6,     7,     8,     9,     0,     0,
       0,     0,     0,     0,    10,     9,    11,    67,     0,     0,
       0,     0,    10,    68,    42,     0,     2,    43,     0,     4,
       5,     0,    75,     0,     2,    43,     0,     4,     5,     0,
       0,     0,     0,     0,     0,     9,     0,     0,     0,     0,
       0,     0,    10,     9,     0,     0,     0,    32,     0,     0,
      10,    33,   -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,
     -72,   -72,   -72,   -72,   -72,     0,     2,    43,    34,     4,
       5,     0,     0,   -72,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     9,     0,     0,     0,     0,
       0,     0,    10
  };

  const signed char
  parser::yycheck_[] =
  {
       0,     9,    13,    11,    40,    41,     4,     5,     4,    23,
       0,     0,    26,    13,     0,     1,     5,     3,     4,    18,
       6,     7,    30,    11,    12,    33,    34,    23,    27,    40,
      41,    39,     5,    19,    20,    21,    22,    27,    27,    22,
      40,    41,    24,    29,    23,    31,    28,    26,     0,    22,
       8,     9,    10,     1,    17,     3,     4,    27,     6,     7,
       6,     7,    22,    61,    62,    63,    27,    67,    18,   105,
      23,    19,    20,    21,    22,    83,    24,    13,    14,    15,
      16,    29,    27,    31,    53,    54,    27,   123,     1,    27,
       3,     4,   103,     6,     7,     3,     4,    22,     6,     7,
      59,    60,   110,   103,    27,    27,    19,    20,    21,    22,
      27,    23,    25,    23,    22,    23,    29,     1,    31,     3,
       4,    29,     6,     7,     3,     4,     4,     6,     7,    55,
      56,    57,    58,     4,    24,    19,    20,    21,    22,    67,
       7,    25,    -1,    22,    50,    29,     1,    31,     3,     4,
      29,     6,     7,    52,     1,    -1,     3,     4,    -1,     6,
       7,    -1,    -1,    -1,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    22,    31,    24,    -1,    -1,
      -1,    -1,    29,    30,     1,    -1,     3,     4,    -1,     6,
       7,    -1,     1,    -1,     3,     4,    -1,     6,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    22,    -1,    -1,    -1,     1,    -1,    -1,
      29,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,     3,     4,    22,     6,
       7,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    29
  };

  const signed char
  parser::yystos_[] =
  {
       0,     1,     3,     4,     6,     7,    19,    20,    21,    22,
      29,    31,    40,    41,    42,    43,    44,    49,    50,    53,
      56,    57,    58,    59,    60,    61,    62,    63,    64,     0,
       5,    27,     1,     5,    22,    47,     4,    62,    62,    22,
      51,    51,     1,     4,    55,    56,    55,    55,     0,    42,
      18,    27,    17,    11,    12,    13,    14,    15,    16,     6,
       7,     8,     9,    10,    55,    27,     1,    24,    30,    52,
      54,    55,    23,    48,    55,     1,    55,    42,    52,    42,
      52,     0,    27,     5,    27,    23,    27,    57,    58,    59,
      59,    60,    60,    60,    60,    61,    61,    62,    62,    62,
      27,    27,    25,    41,    22,    45,    27,    27,    27,    23,
      26,    23,    23,    55,    25,     4,    23,    46,    28,    52,
      55,    23,    26,     4,     4,    52
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    39,    40,    41,    41,    42,    42,    42,    42,    42,
      42,    42,    42,    43,    44,    44,    44,    45,    45,    46,
      46,    47,    47,    48,    48,    49,    49,    50,    50,    51,
      51,    52,    52,    53,    53,    53,    53,    53,    53,    53,
      53,    54,    54,    55,    55,    56,    56,    57,    57,    58,
      58,    58,    59,    59,    59,    59,    59,    60,    60,    60,
      61,    61,    61,    61,    62,    62,    62,    63,    63,    64,
      64,    64,    64
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     2,     1,     1,     1,     2,     1,
       1,     2,     2,     3,     3,     3,     3,     3,     2,     1,
       3,     3,     2,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     4,     4,     3,     4,     3,     4,     3,
       4,     3,     5,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     1,     2,     2,     1,     3,     1,
       1,     2,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER", "ID",
  "\"=\"", "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"%\"", "\"==\"", "\"!=\"",
  "\">\"", "\"<\"", "\">=\"", "\"<=\"", "\"&&\"", "\"||\"", "\"if\"",
  "\"while\"", "\"print\"", "\"(\"", "\")\"", "\"{\"", "\"}\"", "\",\"",
  "\";\"", "\":\"", "\"?\"", "\"func\"", "\"return\"", "LEXERR", "'+'",
  "'-'", "'*'", "'/'", "'%'", "'='", "$accept", "translationStart",
  "statementHandler", "statement", "returnStatement", "printStatement",
  "argsList", "args", "exprList", "expA", "ifStatement", "whileStatement",
  "conditionExpression", "body", "assignment", "func", "assignStatement",
  "orStatement", "andStatement", "eqStatement", "cmpStatement",
  "addStatement", "mulStatement", "unaryStatement", "term", "atomic", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   163,   163,   182,   189,   205,   206,   207,   208,   209,
     210,   211,   212,   214,   216,   217,   218,   220,   221,   223,
     228,   234,   235,   237,   241,   243,   252,   264,   273,   286,
     288,   291,   301,   304,   306,   307,   308,   311,   312,   313,
     316,   322,   334,   354,   355,   357,   358,   361,   362,   364,
     365,   366,   368,   369,   370,   371,   372,   374,   375,   376,
     378,   379,   380,   381,   383,   384,   385,   387,   388,   391,
     392,   393,   408
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    37,     2,     2,
       2,     2,    35,    33,     2,    34,     2,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    38,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32
    };
    // Last valid token kind.
    const int code_max = 287;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // yy
#line 1977 "/home/kolobabka/Desktop/LabaCL/build/parser.cc"

#line 410 "frontend/parser/grammar.y"


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
