#ifndef __SIMPLE_AST_STOP_NODE_H__
#define __SIMPLE_AST_STOP_NODE_H__

#include <cdk/ast/basic_node.h>

namespace til {
    class stop_node : public cdk::basic_node {
        int _loopIndex;
    

    public: 
        stop_node(int lineno, int loopIndex) : 
            basic_node(lineno), _loopIndex(loopIndex) {}

    public:
        inline int loopIndex() {
            return _loopIndex;
        }   

        void accept(basic_ast_visitor  *sp, int level) {
            sp->do_stop_node(this, level);
        }
    };

} // til

#endif