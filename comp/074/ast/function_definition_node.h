#ifndef __SIMPLE_AST_FUNCTION_DEFINITION_NODE_H__
#define __SIMPLE_AST_FUNCTION_DEFINITION_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/typed_node.h>
#include <vector>
#include "block_node.h"

namespace til {

  /**
   * Class for describing function definition nodes.
   */
  class function_definition_node : public cdk::expression_node {
    cdk::sequence_node *_arguments;
    til::block_node *_block;

    public:
        function_definition_node(int lineno,std::shared_ptr<cdk::basic_type> return_type, cdk::sequence_node *arguments, til::block_node *block): 
         cdk::expression_node(lineno) , _arguments(arguments), _block(block) {
          std::vector<std::shared_ptr<cdk::basic_type>> args;
          for (size_t i = 0; i < arguments->size(); i++) {
            args.push_back(dynamic_cast<cdk::typed_node*>(arguments->node(i))->type());
          }
          this->type(cdk::functional_type::create(args, return_type));
        }

        cdk::sequence_node *arguments() {return _arguments;} 

        til::block_node *block() {return _block;}
  

        void accept(basic_ast_visitor *sp, int level) { sp->do_function_definition_node(this, level); }

  };

} // til

#endif