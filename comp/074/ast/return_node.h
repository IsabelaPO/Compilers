#ifndef __SIMPLE_AST_RETURN_NODE_H__
#define __SIMPLE_AST_RETURN_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>

namespace til {
    class return_node : public cdk::basic_node {
        cdk::expression_node *_lastIndex;
    

    public: 
        return_node(int lineno, cdk::expression_node *lastIndex) : 
            basic_node(lineno), _lastIndex(lastIndex) {}

    public:
        inline cdk::expression_node *lastIndex() {
            return _lastIndex;
        }   

        void accept(basic_ast_visitor  *sp, int level) {
            sp->do_return_node(this, level);
        }
    };

} // til

#endif