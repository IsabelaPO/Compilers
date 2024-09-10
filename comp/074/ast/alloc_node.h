#ifndef __SIMPLE_AST_ALLOC_NODE_H__
#define __SIMPLE_AST_ALLOC_NODE_H__

#include <cdk/ast/unary_operation_node.h>
#include <cdk/ast/expression_node.h>

namespace til {
  class alloc_node: public cdk::unary_operation_node {
  public:
    inline alloc_node(int lineno, cdk::expression_node *argument) :
        cdk::unary_operation_node(lineno, argument) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_alloc_node(this, level);
    }

  };

} // til

#endif