#ifndef INTERPRET_H__
#define INTERPRET_H__

#include <algorithm>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fstream>
#include <cmath>

#include "ast.hpp"
#include "location.hh"
#include "nAryTree.hpp"

#include <matplot/matplot.h>

namespace interpret {

    //=======================================================================
    //===================== SCOPE TABLE IMPLEMENTATION ======================
    //=======================================================================

    struct ScopeTblWrapper {
        enum class WrapperType {

            NUM,
            FUNC,
            ARR_LIST,
            INLINE,
            TEXT

        };

        WrapperType type_;
        ScopeTblWrapper (WrapperType type) : type_ (type) {}
        virtual ~ScopeTblWrapper () = 0;
    };

    struct NumScope final : public ScopeTblWrapper {
        double val_;

        NumScope (const double val) : ScopeTblWrapper (ScopeTblWrapper::WrapperType::NUM), val_ (val) {}
    };

    struct TextScope final : public ScopeTblWrapper {
        std::string text_;

        TextScope (const std::string str) : ScopeTblWrapper (ScopeTblWrapper::WrapperType::TEXT), text_ (str) {}
    };

    struct ArrListScope final : public ScopeTblWrapper {

        std::vector<double> list_;

        ArrListScope (const std::vector<double>& list) : 
            ScopeTblWrapper (ScopeTblWrapper::WrapperType::ARR_LIST), list_ (list) {};

    };

    struct FuncScope final : public ScopeTblWrapper {
        const AST::FuncNode *name_ = nullptr;
        const AST::FuncNode *args_ = nullptr;
        const AST::ScopeNode *execScope_ = nullptr;

        FuncScope (const AST::FuncNode *funcDecl) : ScopeTblWrapper (ScopeTblWrapper::WrapperType::FUNC)
        {
            const AST::FuncNode *leftChild = static_cast<const AST::FuncNode *> ((*funcDecl)[0]);
            if (leftChild->getFuncCompType () == AST::FuncNode::FuncComponents::FUNC_NAME) {
                name_ = leftChild;
                leftChild = static_cast<const AST::FuncNode *> ((*funcDecl)[1]);
            }
            args_ = leftChild;
            execScope_ = static_cast<const AST::ScopeNode *> (funcDecl->getRightChild ());
        }
    };

    struct InlineScope final : public ScopeTblWrapper {
        InlineScope () : ScopeTblWrapper (ScopeTblWrapper::WrapperType::INLINE) {}
    };

    struct Scope final {
        using tblSource = std::pair<std::string, ScopeTblWrapper *>;
        using tblVec = std::vector<tblSource>;
        using tblIt = tblVec::const_iterator;

    public:
        tblVec tbl_;
        tblIt endOfVisibleInParent_;
        Scope *parent_;

    public:
        Scope (Scope *parent = nullptr, tblIt it = tblIt (nullptr)) : parent_ (parent), endOfVisibleInParent_ (it) {}

        void push (tblSource obj) { tbl_.push_back (obj); }

        Scope* getParent () const {return parent_;}

        tblIt tblBegin () const { return tbl_.cbegin (); }
        tblIt tblEnd () const { return tbl_.cend (); }
        bool tbl_empty () const { return tbl_.empty (); }

        std::pair<Scope *, tblIt> lookup (const std::string &name)
        {
            for (tblIt curIt = tbl_.begin (), endIt = tbl_.end (); curIt != endIt; ++curIt)
                if ((*curIt).first == name)
                    return {this, curIt};

            Scope *curScope = this;

            while (curScope->parent_ != nullptr) {
                for (tblIt curIt = curScope->parent_->tbl_.begin (), endIt = (curScope->endOfVisibleInParent_) + 1;
                     curIt != endIt;
                     ++curIt)
                    if ((*curIt).first == name)
                        return {curScope->parent_, curIt};

                curScope = curScope->parent_;
            }

            return {nullptr, tbl_.end ()};
        }
    };

    //=======================================================================

    struct Context;

    struct EvalApplyNode {
        const AST::Node *node_ = nullptr;
        EvalApplyNode *parent_ = nullptr;

    public:
        EvalApplyNode (const AST::Node *node, EvalApplyNode *parent) : node_ (node), parent_ (parent) {}

        const AST::Node *getNode () const { return node_; }
        virtual std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) = 0;

        virtual ~EvalApplyNode () {}
    };

    EvalApplyNode *buildApplyNode (const AST::Node *node, EvalApplyNode *parent);

    template <typename T>
    class StackWrapper {
        std::vector<T> stack_;

    public:
        T push_back (T *node)
        {
            stack_.push_back (node);
            return node;
        }

        void pop_back () { stack_.pop_back (); }

        bool empty () const { return stack_.empty (); }
        size_t size () const { return stack_.size (); }

        T back () const { return stack_.back (); }

        T operator[] (const size_t pos) { return stack_[pos]; }
        const T operator[] (const size_t pos) const { return stack_[pos]; }
    };

    class EAScope final : public EvalApplyNode {
        int curChildrenToExec_ = 0;
        std::vector<EvalApplyNode *> children_;

    public:
        EAScope (const AST::ScopeNode *astScope, EvalApplyNode *parent, Context &context);

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override;

        EvalApplyNode *getLastChildren () const { return children_.back (); }
    };

    class EAAssign final : public EvalApplyNode {
        std::string lhs_;
        EvalApplyNode *rhs_;

    public:
        EAAssign (const AST::OperNode *astAssign, EvalApplyNode *parent, Context &context);

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override;

        std::string getLhs () const { return lhs_; }
        EvalApplyNode *getRhs () const { return rhs_; }
    };

    class EAInlineScope final : public EvalApplyNode {
    public:
        EAInlineScope (const AST::InlineScopeNode *astNode, EvalApplyNode *parent) : EvalApplyNode (astNode, parent) {}

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override;
    };

    class EANum final : public EvalApplyNode {
        double val_;

    public:
        EANum (const AST::NumNode *astNum, EvalApplyNode *parent)
            : EvalApplyNode (astNum, parent), val_ (astNum->getValue ())
        {
        }

        double getVal () const { return val_; }

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override;
    };

    class EAText final : public EvalApplyNode {
        std::string text_;

    public:
        EAText (const AST::TextNode *textNode, EvalApplyNode *parent)
            : EvalApplyNode (textNode, parent), text_ (textNode->getText ())
        {}

        std::string getText () const { return text_; }

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override;
    };

    class EAVar final : public EvalApplyNode {
        std::string name_;

    public:
        EAVar (const AST::VarNode *astVar, EvalApplyNode *parent)
            : EvalApplyNode (astVar, parent), name_ (astVar->getName ())
        {
        }

        std::string getName () const { return name_; }

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override;
    };

    class EAArrList final : public EvalApplyNode {

        std::vector<AST::Node *> list_;
        std::vector<double> evaluatedList_;
        int curIndToExec_ = 0;

    public:
        EAArrList (const AST::ArrList *arrList, EvalApplyNode *parent,
                   const std::vector<AST::Node *> &list):
            EvalApplyNode (arrList, parent), list_ (list) {} 

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override;

    };

    class EAFunc final : public EvalApplyNode {
        std::string name_;

    public:
        EAFunc (const AST::FuncNode *astVar, EvalApplyNode *parent)
            : EvalApplyNode (astVar, parent),
              name_ (static_cast<AST::VarNode *> (astVar->getParent ()->getLeftChild ())->getName ())
        {
        }

        std::string getName () const { return name_; }

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override;
    };

    class EAIf final : public EvalApplyNode {
        const AST::Node *expr_;
        const AST::ScopeNode *scope_;

    public:
        EAIf (const AST::CondNode *astCond, EvalApplyNode *parent) : EvalApplyNode (astCond, parent)
        {
            expr_ = (*astCond)[0];
            scope_ = static_cast<const AST::ScopeNode *> ((*astCond)[1]);
        }

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override;
    };

    class EAWhile final : public EvalApplyNode {
        const AST::Node *expr_;
        const AST::ScopeNode *scope_;

    public:
        EAWhile (const AST::CondNode *astCond, EvalApplyNode *parent) : EvalApplyNode (astCond, parent)
        {
            expr_ = (*astCond)[0];
            scope_ = static_cast<const AST::ScopeNode *> ((*astCond)[1]);
        }

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override;
    };

    //=======================================================================
    //==================== CONTEXT STRUCT IMPLEMENTATION ====================
    //=======================================================================
    struct Context final {
        std::vector<Scope *> scopeStack_;
        std::vector<ScopeTblWrapper *> calcStack_;
        std::vector<std::pair<double, EvalApplyNode *>> retStack_;
        std::vector<EvalApplyNode *> rubbishEANodeStack_;
        std::vector<Scope *> rubbishScopeStack_;
        std::vector<ScopeTblWrapper *> rubbishCalcStack_;

        EvalApplyNode *prev_ = nullptr;

        Context () {}

        Context (const Context &rhs) = delete;
        Context (Context &&rhs) = delete;
        Context &operator= (const Context &other) = delete;
        Context &operator= (Context &&other) = delete;

        ~Context ()
        {
            for (auto v : rubbishScopeStack_)
                delete v;
            for (auto v : rubbishEANodeStack_)
                delete v;
            for (auto v : rubbishCalcStack_)
                delete v;
        }

        using tblSource = std::pair<std::string, ScopeTblWrapper *>;
        using tblVec = std::vector<tblSource>;
        using tblIt = tblVec::const_iterator;

        void addScope ();
        void removeCurScope ();

        InlineScope *buildScopeWrapper ()
        {
            rubbishCalcStack_.push_back (new InlineScope);
            return static_cast<InlineScope *> (rubbishCalcStack_.back ());
        }

        NumScope *buildScopeWrapper (const double val)
        {
            rubbishCalcStack_.push_back (new NumScope (val));
            return static_cast<NumScope *> (rubbishCalcStack_.back ());
        }

        TextScope *buildScopeWrapper (const std::string text)
        {
            rubbishCalcStack_.push_back (new TextScope (text));
            return static_cast<TextScope *> (rubbishCalcStack_.back ());
        }

        FuncScope *buildScopeWrapper (const AST::FuncNode *funcDecl)
        {
            rubbishCalcStack_.push_back (new FuncScope (funcDecl));
            return static_cast<FuncScope *> (rubbishCalcStack_.back ());
        }

        ArrListScope *buildScopeWrapper (const std::vector<double>& list)
        {
            rubbishCalcStack_.push_back (new ArrListScope (list));
            return static_cast<ArrListScope *> (rubbishCalcStack_.back ());
        }

        Scope *buildScope (Scope *parent = nullptr, tblIt it = tblIt (nullptr))
        {
            rubbishScopeStack_.push_back (new Scope (parent, it));
            return rubbishScopeStack_.back ();
        }
        EvalApplyNode *buildApplyNode (const AST::Node *node, EvalApplyNode *parent);
    };
    //=======================================================================

    template <typename operApp>
    class EABinOp final : public EvalApplyNode {
        operApp apply_;

    public:
        EABinOp (const AST::OperNode *astOper, EvalApplyNode *parent) : EvalApplyNode (astOper, parent) {}

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override
        {
            const AST::Node *node = EvalApplyNode::getNode ();
            const AST::Node *rhs = (*node)[1];
            const AST::Node *lhs = (*node)[0];

            const AST::Node *prevExec = context.prev_->getNode ();

            if (prevExec == rhs) {
                apply_ (context);
                return {parent_, this};
            }
            else if (prevExec == lhs) {
                EvalApplyNode *rhsToExec = context.buildApplyNode (rhs, this);
                return {rhsToExec, this};
            }

            EvalApplyNode *lhsToExec = context.buildApplyNode (lhs, this);
            return {lhsToExec, this};
        }
    };

    template <typename operApp>
    class EATernOp final : public EvalApplyNode {
        operApp apply_;

    public:
        EATernOp (const AST::OperNode *astOper, EvalApplyNode *parent) : EvalApplyNode (astOper, parent) {}

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override
        {
            const AST::Node *node = EvalApplyNode::getNode ();
            const AST::Node *rhs = (*node)[2];
            const AST::Node *mhs = (*node)[1];
            const AST::Node *lhs = (*node)[0];

            const AST::Node *prevExec = context.prev_->getNode ();

            if (prevExec == rhs) {
                apply_ (context);
                return {parent_, this};
            }
            else if (prevExec == lhs) {
                EvalApplyNode *mhsToExec = context.buildApplyNode (mhs, this);
                return {mhsToExec, this};
            }
            else if (prevExec == mhs) {
                EvalApplyNode *rhsToExec = context.buildApplyNode (rhs, this);
                return {rhsToExec, this};
            }

            EvalApplyNode *lhsToExec = context.buildApplyNode (lhs, this);
            return {lhsToExec, this};
        }
    };

    //=======================================================================

    template <typename operApp>
    class EAUnOp final : public EvalApplyNode {
        operApp apply_;

    public:
        EAUnOp (const AST::OperNode *astOper, EvalApplyNode *parent) : EvalApplyNode (astOper, parent) {}

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override
        {
            const AST::Node *node = EvalApplyNode::getNode ();
            const AST::Node *lhs = (*node)[0];

            const AST::Node *prevExec = context.prev_->getNode ();

            if (prevExec == lhs) {
                apply_ (context);
                return {parent_, this};
            }

            EvalApplyNode *lhsToExec = context.buildApplyNode (lhs, this);
            return {lhsToExec, this};
        }
    };

    class EAScan final : public EvalApplyNode {
    public:
        EAScan (const AST::OperNode *astOper, EvalApplyNode *parent) : EvalApplyNode (astOper, parent) {}

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override
        {
            double tmp;
            std::cin >> tmp;

            context.calcStack_.push_back (context.buildScopeWrapper (tmp));
            return {parent_, this};
        }
    };

    double getTopAndPopNum (Context &context);
    ScopeTblWrapper *getTopAndPopCalcStack (Context &context);
    
    struct UnOpSizeof {
        void operator() (Context &context) const 
        {
            auto res = getTopAndPopCalcStack (context);
            switch (res->type_)
            {
                case ScopeTblWrapper::WrapperType::NUM:
                    context.calcStack_.push_back (context.buildScopeWrapper (1));
                    break;
                case ScopeTblWrapper::WrapperType::ARR_LIST: {
                    const std::vector<double>& list = static_cast<ArrListScope *> (res)->list_;
                    context.calcStack_.push_back (context.buildScopeWrapper (list.size()));
                    break;
                }
                default:
                    throw std::runtime_error ("Bad wrapper type in Sizeof ()");       
            }
        }
    };

    struct UnOpSqrt {
        void operator() (Context &context) const
        {
            auto res = getTopAndPopCalcStack (context);
            switch (res->type_)
            {
                case ScopeTblWrapper::WrapperType::NUM:
                    context.calcStack_.push_back (context.buildScopeWrapper (std::sqrt (static_cast<NumScope *> (res)->val_)));
                    break;
                case ScopeTblWrapper::WrapperType::ARR_LIST: {
                    const std::vector<double>& list = static_cast<ArrListScope *> (res)->list_;
                    std::vector<double> res (list.size ());
                    for (int i = 0; i < list.size (); ++i)
                        res [i] = std::sqrt (list [i]);
                    context.calcStack_.push_back (context.buildScopeWrapper (res));
                    break;
                }
                default:
                    throw std::runtime_error ("Bad data type for sqrt ()");
            }
        }
    };

    struct UnOpPrint {
        void operator() (Context &context) const 
        {
            auto res = getTopAndPopCalcStack (context);
            switch (res->type_)
            {
                case ScopeTblWrapper::WrapperType::NUM:
                    std::cout << static_cast<NumScope *> (res)->val_ << std::endl;
                    break;
                case ScopeTblWrapper::WrapperType::ARR_LIST: {
                    const std::vector<double>& list = 
                        static_cast<ArrListScope *> (res)->list_;
                    std::cout << "[ ";
                    for (auto v: list)
                        std::cout << v << " ";
                    std::cout << "]" << std::endl;
                    break;
                }
                default:
                    throw std::runtime_error ("Bad wrapper type in UnOpPrint ()");       
            }
        }
    };

    struct UnOpMinus {
        void operator() (Context &context) const
        {
            context.calcStack_.push_back (context.buildScopeWrapper (-getTopAndPopNum (context)));  // WAT????
        }
    };

    struct UnOpPlus {
        void operator() (Context &context) const
        {
            context.calcStack_.push_back (context.buildScopeWrapper (+getTopAndPopNum (context)));
        }
    };

    namespace {
        void addNums (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {
            double firstVal    = static_cast<NumScope *> (first)->val_;
            double secondVal   = static_cast<NumScope *> (second)->val_;
            context.calcStack_.push_back (context.buildScopeWrapper (firstVal + secondVal));
        }
        void addArrs (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {
            const std::vector<double>& firstList = static_cast<ArrListScope *> (first)->list_;
            const std::vector<double>& secondList = static_cast<ArrListScope *> (second)->list_;
            int maxSize = std::max (firstList.size (), secondList.size ());
            std::vector<double> res (maxSize);
            for (int i = 0; i < maxSize; ++i)
                res [i] = (i < firstList.size () ? firstList [i] : 0) +
                            (i < secondList.size () ? secondList [i] : 0);
            context.calcStack_.push_back (context.buildScopeWrapper (res));
        }
    }

    namespace {

        void getElemFromArray (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {

            const std::vector<double>& list = static_cast<ArrListScope *> (first)->list_;
            const int index = static_cast<NumScope *> (second)->val_;

            if (index >= list.size())
                throw std::runtime_error ("Attempt to get non-existent element from array");

            context.calcStack_.push_back (context.buildScopeWrapper (list[index]));
        }
    }

    struct BinOpGet {

        void operator () (Context &context) const 
        {
            auto first = getTopAndPopCalcStack (context);
            auto second = getTopAndPopCalcStack (context);

            if (second->type_ != ScopeTblWrapper::WrapperType::ARR_LIST)
                throw std::runtime_error ("Attempt to apply operator get to non-array entity");

            if (first->type_ != ScopeTblWrapper::WrapperType::NUM)  
                throw std::runtime_error ("Non-number index");

            try {
                getElemFromArray (context, second, first);
            }
            catch (std::runtime_error &err) {                    
                throw err;
            }
        } 
    };

    struct BinOpTexAddText {

        void operator () (Context &context) const 
        {
            auto first = getTopAndPopCalcStack (context);
            auto second = getTopAndPopCalcStack (context);

            if (first->type_ != ScopeTblWrapper::WrapperType::TEXT)
                throw std::runtime_error ("Expected text");

            if (second->type_ != ScopeTblWrapper::WrapperType::TEXT)  
                throw std::runtime_error ("Expected file name");

            std::ofstream opFile;
            opFile.open (static_cast<TextScope *> (second)->text_, std::ios::app);
            if (!opFile.is_open ())
                throw std::runtime_error ("Can't open tex file to write in");
            
            opFile << std::endl << static_cast<TextScope *> (first)->text_ << std::endl;

            opFile.close ();
        }

    };

    struct BinOpTexAddSection {

        void operator () (Context &context) const 
        {
            auto first = getTopAndPopCalcStack (context);
            auto second = getTopAndPopCalcStack (context);

            if (first->type_ != ScopeTblWrapper::WrapperType::TEXT)
                throw std::runtime_error ("Expected section name");

            if (second->type_ != ScopeTblWrapper::WrapperType::TEXT)  
                throw std::runtime_error ("Expected file name");

            std::ofstream opFile;
            opFile.open (static_cast<TextScope *> (second)->text_, std::ios::app);
            if (!opFile.is_open ())
                throw std::runtime_error ("Can't open tex file to write in");

            opFile << "\\section{" << (static_cast<TextScope *> (first)->text_) << "}" << std::endl;

            opFile.close ();
        }

    };

    struct UnOpTexAddContent {

        void operator () (Context &context) const 
        {
            auto first = getTopAndPopCalcStack (context);

            if (first->type_ != ScopeTblWrapper::WrapperType::TEXT)  
                throw std::runtime_error ("Expected file name");

            std::ofstream opFile;
            opFile.open (static_cast<TextScope *> (first)->text_, std::ios::app);
            if (!opFile.is_open ())
                throw std::runtime_error ("Can't open tex file to write in");

            opFile << "\\tableofcontents\n"
                      "\\newpage\n\n";

            opFile.close ();
        }

    };

    struct UnOpTexAddEnd {

        void operator () (Context &context) const 
        {
            auto first = getTopAndPopCalcStack (context);

            if (first->type_ != ScopeTblWrapper::WrapperType::TEXT)  
                throw std::runtime_error ("Expected file name");

            std::ofstream opFile;
            opFile.open (static_cast<TextScope *> (first)->text_, std::ios::app);
            if (!opFile.is_open ())
                throw std::runtime_error ("Can't open tex file to write in");

            opFile << std::endl << "\\end{document}" << std::endl;

            opFile.close ();
        }

    };

    struct UnOpTexCompile {

        void operator () (Context &context) const 
        {
            auto first = getTopAndPopCalcStack (context);

            if (first->type_ != ScopeTblWrapper::WrapperType::TEXT)  
                throw std::runtime_error ("Expected file name");

            std::string compileCommand = std::string ("latex ") + static_cast<TextScope *> (first)->text_;
            system (compileCommand.c_str ());
        }

    };

    struct TernOpTexAddHead {

        void operator () (Context &context) const 
        {
            auto first = getTopAndPopCalcStack (context);
            auto second = getTopAndPopCalcStack (context);
            auto third = getTopAndPopCalcStack (context);

            if (first->type_ != ScopeTblWrapper::WrapperType::TEXT)
                throw std::runtime_error ("Expected author name");

            if (second->type_ != ScopeTblWrapper::WrapperType::TEXT)  
                throw std::runtime_error ("Expected title name");

            if (third->type_ != ScopeTblWrapper::WrapperType::TEXT)
                throw std::runtime_error ("Expected file name");

            std::ofstream opFile;
            opFile.open (static_cast<TextScope *> (third)->text_, std::ios::app);
            if (!opFile.is_open ())
                throw std::runtime_error ("Can't open tex file to write in");

            opFile <<  "\\documentclass{article} \n"
                       "\\usepackage[utf8]{inputenc} \n"
                       "\\usepackage{hyperref}\n"
                       "\\usepackage{indentfirst}\n"
                       "\\title{" << static_cast<TextScope *> (second)->text_ << 
                       "}\n"
                       "\\author{" << static_cast<TextScope *> (first)->text_ <<
                       "}\n"
                       "\\date{}\n"
                       "\\begin{document}\n"
                       "\\maketitle\n" << std::endl;

            opFile.close ();
        }

    };

    namespace {

        std::pair<double, double> Approx
        (std::vector<double>::iterator xSt, std::vector<double>::iterator xFin,
         std::vector<double>::iterator ySt, std::vector<double>::iterator yFin)
        {
            double sumX     = 0;
            double sumX2    = 0;
            double sumY     = 0;
            double sumXY    = 0;

            int n = 0;
            while (xSt != xFin && ySt != yFin) {
                
                sumX    += *xSt;
                sumX2   += (*xSt) * (*xSt);
                sumY    += *ySt;
                sumXY   += (*xSt) * (*ySt);

                ySt++;
                xSt++;
                n++;

            }

            double k = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
            double b = (sumY - k * sumX) / n;

            return std::pair<double, double> (k, b);
        }

    };

    struct TernOpCreateGraph {
    
        void operator() (Context &context) const
        {
            auto first = getTopAndPopCalcStack (context);
            auto second = getTopAndPopCalcStack (context);
            auto third = getTopAndPopCalcStack (context);

            if (first->type_ != ScopeTblWrapper::WrapperType::ARR_LIST ||
                second->type_ != ScopeTblWrapper::WrapperType::ARR_LIST)
                throw std::runtime_error ("Attempt to apply operator get to non-array entity");

            if (third->type_ != ScopeTblWrapper::WrapperType::TEXT)
                throw std::runtime_error ("Expected output picture name");

            std::vector<double>& x = static_cast<ArrListScope *> (second)->list_;
            std::vector<double>& y = static_cast<ArrListScope *> (first)->list_;
            
            auto [k, b] = Approx (x.begin (), x.end (), y.begin (), y.end ());

            double maxCoordX = *(std::max_element (x.begin (), x.end ()));
            double minCoordX = *(std::min_element (x.begin (), x.end ()));

            std::vector<double> approximatedX;
            approximatedX.push_back (minCoordX);
            approximatedX.push_back (maxCoordX);

            std::vector<double> approximatedY;
            approximatedY.push_back (minCoordX * k + b);
            approximatedY.push_back (maxCoordX * k + b);

            auto fig = matplot::figure ();

            auto plSet = matplot::plot (approximatedX, approximatedY);
            plSet->line_width (2);

            matplot::hold (matplot::on);
            
            auto pSet = matplot::scatter (x, y);
            pSet->marker_face(true);
            pSet->marker_color({0, .5, .5});
            pSet->marker_size (7);
            pSet->marker_style (matplot::line_spec::marker_style::circle);

            fig->draw ();

            matplot::save (static_cast<TextScope *> (third)->text_);

        }

    };

    struct BinOpAdd {

        void operator() (Context &context) const
        {
            auto first = getTopAndPopCalcStack (context);
            auto second = getTopAndPopCalcStack (context);

            if (first->type_ != second->type_)
                throw std::runtime_error ("Wrong types for arithmetic operation");
            else {
                switch (first->type_) {
                    case ScopeTblWrapper::WrapperType::NUM: {
                        addNums (context, first, second);
                        break;
                    }
                    case ScopeTblWrapper::WrapperType::ARR_LIST: {
                        addArrs (context, first, second);
                        break;
                    }
                    default:
                        throw std::runtime_error ("Unexpected data type in add");
                }
            }
        }
    };

    namespace {
        void subNums (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {
            double firstVal    = static_cast<NumScope *> (first)->val_;
            double secondVal   = static_cast<NumScope *> (second)->val_;
            context.calcStack_.push_back (context.buildScopeWrapper (secondVal - firstVal));
        }
        void subArrs (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {
            const std::vector<double>& firstList = static_cast<ArrListScope *> (first)->list_;
            const std::vector<double>& secondList = static_cast<ArrListScope *> (second)->list_;
            int maxSize = std::max (firstList.size (), secondList.size ());
            std::vector<double> res (maxSize);
            for (int i = 0; i < maxSize; ++i)
                res [i] = - (i < firstList.size () ? firstList [i] : 0) +
                            (i < secondList.size () ? secondList [i] : 0);
            context.calcStack_.push_back (context.buildScopeWrapper (res));
        }
    }

    struct BinOpSub {
        void operator() (Context &context) const
        {
            auto first = getTopAndPopCalcStack (context);
            auto second = getTopAndPopCalcStack (context);

            if (first->type_ != second->type_)
                throw std::runtime_error ("Wrong types for arithmetic operation");
            else {
                switch (first->type_) {
                    case ScopeTblWrapper::WrapperType::NUM: {
                        subNums (context, first, second);
                        break;
                    }
                    case ScopeTblWrapper::WrapperType::ARR_LIST: {
                        subArrs (context, first, second);
                        break;
                    }
                    default:
                        throw std::runtime_error ("Unexpected data type in sub");
                }
            }
        }
    };

    namespace {
        void mulNums (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {
            double firstVal    = static_cast<NumScope *> (first)->val_;
            double secondVal   = static_cast<NumScope *> (second)->val_;
            context.calcStack_.push_back (context.buildScopeWrapper (secondVal * firstVal));
        }
        void mulArrs (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {
            const std::vector<double>& firstList = static_cast<ArrListScope *> (first)->list_;
            const std::vector<double>& secondList = static_cast<ArrListScope *> (second)->list_;
            int maxSize = std::max (firstList.size (), secondList.size ());
            std::vector<double> res (maxSize);
            for (int i = 0; i < maxSize; ++i)
                res [i] =   (i < firstList.size () ? firstList [i] : 0) *
                            (i < secondList.size () ? secondList [i] : 0);
            context.calcStack_.push_back (context.buildScopeWrapper (res));
        }
    }

    struct BinOpMul {
        void operator() (Context &context) const
        {
            auto first = getTopAndPopCalcStack (context);
            auto second = getTopAndPopCalcStack (context);

            if (first->type_ != second->type_)
                throw std::runtime_error ("Wrong types for arithmetic operation");
            else {
                switch (first->type_) {
                    case ScopeTblWrapper::WrapperType::NUM: {
                        mulNums (context, first, second);
                        break;
                    }
                    case ScopeTblWrapper::WrapperType::ARR_LIST: {
                        mulArrs (context, first, second);
                        break;
                    }
                    default:
                        throw std::runtime_error ("Unexpected data type in mul");
                }
            }
        }
    };

    namespace {
        void divNums (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {
            double firstVal    = static_cast<NumScope *> (first)->val_;
            double secondVal   = static_cast<NumScope *> (second)->val_;
            context.calcStack_.push_back (context.buildScopeWrapper (secondVal / firstVal));
        }
        void divArrs (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {
            const std::vector<double>& firstList = static_cast<ArrListScope *> (first)->list_;
            const std::vector<double>& secondList = static_cast<ArrListScope *> (second)->list_;
            int maxSize = std::max (firstList.size (), secondList.size ());
            std::vector<double> res (maxSize);
            for (int i = 0; i < maxSize; ++i)
                res [i] =   (i < secondList.size () ? secondList [i] : 0) /
                            (i < firstList.size () ? firstList [i] : 1);
            context.calcStack_.push_back (context.buildScopeWrapper (res));
        }
    }

    struct BinOpDiv {
        void operator() (Context &context) const
        {
            auto first = getTopAndPopCalcStack (context);
            auto second = getTopAndPopCalcStack (context);

            if (first->type_ != second->type_)
                throw std::runtime_error ("Wrong types for arithmetic operation");
            else {
                switch (first->type_) {
                    case ScopeTblWrapper::WrapperType::NUM: {
                        divNums (context, first, second);
                        break;
                    }
                    case ScopeTblWrapper::WrapperType::ARR_LIST: {
                        divArrs (context, first, second);
                        break;
                    }
                    default:
                        throw std::runtime_error ("Unexpected data type in mul");
                }
            }
        }
    };

    namespace {
    
        const double Epsilon = 1e-5;

        bool DoubleCmp (const double first, const double second) {
            return std::abs (first - second) < Epsilon;
        }

    }

    struct BinOpMore {
        void operator() (Context &context) const
        {
            const double first = getTopAndPopNum (context);
            const double second = getTopAndPopNum (context);

            context.calcStack_.push_back ((context.buildScopeWrapper (second > first)));
        }
    };

    struct BinOpLess {
        void operator() (Context &context) const
        {
            const double first = getTopAndPopNum (context);
            const double second = getTopAndPopNum (context);

            context.calcStack_.push_back (((context.buildScopeWrapper (second < first))));
        }
    };

    struct BinOpLTE {
        void operator() (Context &context) const
        {
            const double first = getTopAndPopNum (context);
            const double second = getTopAndPopNum (context);

            context.calcStack_.push_back (context.buildScopeWrapper (second < first || DoubleCmp (first, second)));
        }
    };

    struct BinOpGTE {
        void operator() (Context &context) const
        {
            const double first = getTopAndPopNum (context);
            const double second = getTopAndPopNum (context);

            context.calcStack_.push_back (context.buildScopeWrapper (second > first || DoubleCmp (first, second)));
        }
    };

    struct BinOpEQ {
        void operator() (Context &context) const
        {
            const double first = getTopAndPopNum (context);
            const double second = getTopAndPopNum (context);

            context.calcStack_.push_back (context.buildScopeWrapper (DoubleCmp (first, second)));
        }
    };

    struct BinOpNEQ {
        void operator() (Context &context) const
        {
            const double first = getTopAndPopNum (context);
            const double second = getTopAndPopNum (context);

            context.calcStack_.push_back (context.buildScopeWrapper (!DoubleCmp (first, second)));
        }
    };

    struct BinOpOr {
        void operator() (Context &context) const
        {
            const int first = getTopAndPopNum (context);
            const int second = getTopAndPopNum (context);

            context.calcStack_.push_back (((context.buildScopeWrapper (second || first))));
        }
    };

    struct BinOpAnd {
        void operator() (Context &context) const
        {
            const int first = getTopAndPopNum (context);
            const int second = getTopAndPopNum (context);

            context.calcStack_.push_back (((context.buildScopeWrapper (second && first))));
        }
    };

    struct BinOpMod {
        void operator() (Context &context) const
        {
            const int first = getTopAndPopNum (context);
            const int second = getTopAndPopNum (context);

            context.calcStack_.push_back (((context.buildScopeWrapper (second % first))));
        }
    };

    struct EACall final : public EvalApplyNode {
        int curArgsToCalc_ = 0;

    public:
        EACall (const AST::OperNode *astOper, EvalApplyNode *parent) : EvalApplyNode (astOper, parent) {}

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override;
    };

    struct EAReturn final : public EvalApplyNode {
    public:
        EAReturn (const AST::OperNode *astOper, EvalApplyNode *parent) : EvalApplyNode (astOper, parent) {}

        std::pair<EvalApplyNode *, EvalApplyNode *> eval (Context &context) override;
    };

    class Interpreter final {
        const AST::ScopeNode *root_;

    public:
        Interpreter (const AST::ScopeNode *root) : root_ (root) {}

        void run ();
    };

}  // namespace interpret

#endif
