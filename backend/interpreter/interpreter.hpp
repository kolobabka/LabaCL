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

#include "ast.hpp"
#include "location.hh"
#include "nAryTree.hpp"

namespace interpret {

    //=======================================================================
    //===================== SCOPE TABLE IMPLEMENTATION ======================
    //=======================================================================

    struct ScopeTblWrapper {
        enum class WrapperType {

            NUM,
            FUNC,
            ARR_LIST,
            INLINE

        };

        WrapperType type_;
        ScopeTblWrapper (WrapperType type) : type_ (type) {}
        virtual ~ScopeTblWrapper () = 0;
    };

    struct NumScope final : public ScopeTblWrapper {
        int val_;

        NumScope (const int val) : ScopeTblWrapper (ScopeTblWrapper::WrapperType::NUM), val_ (val) {}
    };

    struct ArrListScope final : public ScopeTblWrapper {

        std::vector<int> list_;

        ArrListScope (const std::vector<int>& list) : 
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
        int val_;

    public:
        EANum (const AST::NumNode *astNum, EvalApplyNode *parent)
            : EvalApplyNode (astNum, parent), val_ (astNum->getValue ())
        {
        }

        int getVal () const { return val_; }

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
        std::vector<int> evaluatedList_;
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
        std::vector<std::pair<int, EvalApplyNode *>> retStack_;
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

        NumScope *buildScopeWrapper (const int val)
        {
            rubbishCalcStack_.push_back (new NumScope (val));
            return static_cast<NumScope *> (rubbishCalcStack_.back ());
        }

        FuncScope *buildScopeWrapper (const AST::FuncNode *funcDecl)
        {
            rubbishCalcStack_.push_back (new FuncScope (funcDecl));
            return static_cast<FuncScope *> (rubbishCalcStack_.back ());
        }

        ArrListScope *buildScopeWrapper (const std::vector<int>& list)
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
            int tmp;
            std::cin >> tmp;

            context.calcStack_.push_back (context.buildScopeWrapper (tmp));
            return {parent_, this};
        }
    };

    int getTopAndPopNum (Context &context);
    ScopeTblWrapper *getTopAndPopCalcStack (Context &context);

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
                    const std::vector<int>& list = 
                        static_cast<ArrListScope *> (res)->list_;
                    std::cout << "[ ";
                    for (auto v: list)
                        std::cout << v << " ";
                    std::cout << "]" << std::endl;
                    break;
                }
                default:
                    std::runtime_error ("Bad wrapper type in UnOpPrint ()");       
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
            int firstVal    = static_cast<NumScope *> (first)->val_;
            int secondVal   = static_cast<NumScope *> (second)->val_;
            context.calcStack_.push_back (context.buildScopeWrapper (firstVal + secondVal));
        }
        void addArrs (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {
            const std::vector<int>& firstList = static_cast<ArrListScope *> (first)->list_;
            const std::vector<int>& secondList = static_cast<ArrListScope *> (second)->list_;
            int maxSize = std::max (firstList.size (), secondList.size ());
            std::vector<int> res (maxSize);
            for (int i = 0; i < maxSize; ++i)
                res [i] = (i < firstList.size () ? firstList [i] : 0) +
                            (i < secondList.size () ? secondList [i] : 0);
            context.calcStack_.push_back (context.buildScopeWrapper (res));
        }
    }

    namespace {

        void getElemFromArray (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {

            const std::vector<int>& list = static_cast<ArrListScope *> (first)->list_;
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
                throw std::runtime_error ("Non-integer index");

            try {
                getElemFromArray (context, second, first);
            }
            catch (std::runtime_error &err) {                    
                throw err;
            }
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
            int firstVal    = static_cast<NumScope *> (first)->val_;
            int secondVal   = static_cast<NumScope *> (second)->val_;
            context.calcStack_.push_back (context.buildScopeWrapper (secondVal - firstVal));
        }
        void subArrs (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {
            const std::vector<int>& firstList = static_cast<ArrListScope *> (first)->list_;
            const std::vector<int>& secondList = static_cast<ArrListScope *> (second)->list_;
            int maxSize = std::max (firstList.size (), secondList.size ());
            std::vector<int> res (maxSize);
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
            int firstVal    = static_cast<NumScope *> (first)->val_;
            int secondVal   = static_cast<NumScope *> (second)->val_;
            context.calcStack_.push_back (context.buildScopeWrapper (secondVal * firstVal));
        }
        void mulArrs (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {
            const std::vector<int>& firstList = static_cast<ArrListScope *> (first)->list_;
            const std::vector<int>& secondList = static_cast<ArrListScope *> (second)->list_;
            int maxSize = std::max (firstList.size (), secondList.size ());
            std::vector<int> res (maxSize);
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
            int firstVal    = static_cast<NumScope *> (first)->val_;
            int secondVal   = static_cast<NumScope *> (second)->val_;
            context.calcStack_.push_back (context.buildScopeWrapper (secondVal / firstVal));
        }
        void divArrs (Context &context, ScopeTblWrapper* first, ScopeTblWrapper* second) {
            const std::vector<int>& firstList = static_cast<ArrListScope *> (first)->list_;
            const std::vector<int>& secondList = static_cast<ArrListScope *> (second)->list_;
            int maxSize = std::max (firstList.size (), secondList.size ());
            std::vector<int> res (maxSize);
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

    struct BinOpMore {
        void operator() (Context &context) const
        {
            const int first = getTopAndPopNum (context);
            const int second = getTopAndPopNum (context);

            context.calcStack_.push_back ((context.buildScopeWrapper (second > first)));
        }
    };

    struct BinOpLess {
        void operator() (Context &context) const
        {
            const int first = getTopAndPopNum (context);
            const int second = getTopAndPopNum (context);

            context.calcStack_.push_back (((context.buildScopeWrapper (second < first))));
        }
    };

    struct BinOpLTE {
        void operator() (Context &context) const
        {
            const int first = getTopAndPopNum (context);
            const int second = getTopAndPopNum (context);

            context.calcStack_.push_back (((context.buildScopeWrapper (second <= first))));
        }
    };

    struct BinOpGTE {
        void operator() (Context &context) const
        {
            const int first = getTopAndPopNum (context);
            const int second = getTopAndPopNum (context);

            context.calcStack_.push_back (((context.buildScopeWrapper (second >= first))));
        }
    };

    struct BinOpEQ {
        void operator() (Context &context) const
        {
            const int first = getTopAndPopNum (context);
            const int second = getTopAndPopNum (context);

            context.calcStack_.push_back (((context.buildScopeWrapper (second == first))));
        }
    };

    struct BinOpNEQ {
        void operator() (Context &context) const
        {
            const int first = getTopAndPopNum (context);
            const int second = getTopAndPopNum (context);

            context.calcStack_.push_back (((context.buildScopeWrapper (second != first))));
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
