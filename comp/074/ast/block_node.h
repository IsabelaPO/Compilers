#ifndef __SIMPLE_AST_BLOCK_NODE_H__
#define __SIMPLE_AST_BLOCK_NODE_H__

#include <cdk/ast/sequence_node.h>
#include <cdk/ast/basic_node.h>

namespace til{
    class block_node : public cdk::basic_node {
        cdk::sequence_node *_declaration;
        cdk::sequence_node *_instructions;


    public: 
        block_node(int lineno, cdk::sequence_node *declaration, cdk::sequence_node *instructions ) :
            basic_node(lineno), _declaration(declaration), _instructions(instructions) {}

    public: 
        inline cdk::sequence_node *declaration(){
            return _declaration;
        }

        inline cdk::sequence_node *instructions(){
            return _instructions;
        }

        void accept(basic_ast_visitor *sp, int level){
            sp->do_block_node(this, level);
        }
    };
} //til

#endif