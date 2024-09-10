#ifndef __SIMPLE_AST_INDEX_POINTER_NODE_H__
#define __SIMPLE_AST_INDEX_POINTER_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>


namespace til {
    class index_pointer_node : public cdk::lvalue_node {
        cdk::expression_node *_index;
        cdk::expression_node *_base;
   

    public:
    index_pointer_node(int lineno, cdk::expression_node *index, cdk::expression_node *base) : 
        lvalue_node(lineno), _index(index), _base(base) {}

    public: 
    inline cdk::expression_node *index(){
        return _index;
    }

    inline cdk::expression_node *base(){
        return _base;
    }

    void accept(basic_ast_visitor *sp, int level) {
        sp->do_index_pointer_node(this, level);
    }
    };
} // til

#endif