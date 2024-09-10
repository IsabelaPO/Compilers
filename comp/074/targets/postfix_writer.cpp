#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/frame_size_calculator.h"
#include ".auto/all_nodes.h"  // all_nodes.h is automatically generated

//---------------------------------------------------------------------------

void til::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void til::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void til::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.DOUBLE(node->value());
  } else {
    _pf.SDOUBLE(node->value());
  }
}
void til::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl + 2);
  _pf.INT(0);
  _pf.EQ();
}
void til::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}
void til::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if(_inFunctionBody){
    _pf.INT(node->value()); // push an integer
  } else {
    _pf.SINT(node->value());
  }
}

void til::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters

  if(_inFunctionBody){
    // local variable initializer
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl1));
  } else {
    // global variable initializer
    _pf.DATA();
    _pf.SADDR(mklbl(lbl1));
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_unary_minus_node(cdk::unary_minus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if(node->is_typed(cdk::TYPE_DOUBLE)){
    _pf.DNEG();
  }else{
    _pf.NEG(); // 2-complement
  }
  
}

void til::postfix_writer::do_unary_plus_node(cdk::unary_plus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.ADD();
}
void til::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.SUB();
}
void til::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MUL();
}
void til::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}
void til::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void til::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void til::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void til::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void til::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void til::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void til::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  const std::string &id = node->name();
  auto symbol = _symtab.find(id);
  if (symbol->global()) {
    _pf.ADDR(symbol->name());
  } else {
    _pf.LOCAL(symbol->offset());
  }
}

void til::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.LDDOUBLE();
  } else {
    // integers, pointers, and strings
    _pf.LDINT();
  }
}

void til::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // determine the new value
  if(node->is_typed(cdk::TYPE_DOUBLE)){
    _pf.DUP64();
  } else {
    _pf.DUP32();
  }
  
  node->lvalue()->accept(this, lvl); // where to store the value
  if(node->is_typed(cdk::TYPE_DOUBLE)){
    _pf.STDOUBLE();
  } else {
    _pf.STINT();
  }
  
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_program_node(til::program_node * const node, int lvl) {
  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");
  
  _whileLoopLabels = new std::vector<std::pair<std::string, std::string>>();

  frame_size_calculator fsc(_compiler, _symtab);
  node->statements()->accept(&fsc, lvl);
  _pf.ENTER(fsc.localsize());
  
  _inFunctionBody = true;
  
  node->statements()->accept(this, lvl);

  _inFunctionBody = false;

  // end the main function
  _pf.INT(0);
  _pf.STFVAL32();
  _pf.LEAVE();
  _pf.RET();

  delete _whileLoopLabels;

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("printd");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_evaluation_node(til::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
    _pf.TRASH(4); // delete the evaluated value
  } else if (node->argument()->is_typed(cdk::TYPE_STRING)) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void til::postfix_writer::do_print_node(til::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
    auto child = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ix));

    child->accept(this, lvl); // expression to print

    if (child->is_typed(cdk::TYPE_STRING)){
      _functions_to_declare.insert("prints");
      _pf.CALL("prints");
      _pf.TRASH(4);
    } else if (child->is_typed(cdk::TYPE_INT)){
      _functions_to_declare.insert("printi");
      _pf.CALL("printi");
      _pf.TRASH(4);
    }

  }

  if(node->newline()){
      _functions_to_declare.insert("println");
      _pf.CALL("println");
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_read_node(til::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  /*_pf.CALL("readi");
  _pf.LDFVAL32();
  node->argument()->accept(this, lvl);
  _pf.STINT();*/
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_while_node(til::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int _whilecond, _whileend;
  
  _pf.ALIGN();
  _pf.LABEL(mklbl(_whilecond = ++_lbl));
  node->condition()->accept(this, lvl+2);
  _pf.JZ(mklbl(_whileend = ++_lbl));

  _whileLoopLabels->push_back(std::make_pair(mklbl(_whilecond), mklbl(_whileend)));
  node->block()->accept(this, lvl + 2);
  _whileLoopLabels->pop_back();

  _pf.JMP(mklbl(_whilecond));
  _pf.ALIGN();
  _pf.LABEL(mklbl(_whileend));

}

//---------------------------------------------------------------------------

void til::postfix_writer::do_if_node(til::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_if_else_node(til::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_address_of_node(til::address_of_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  //TODO
}

void til::postfix_writer::do_block_node(til::block_node * const node, int lvl) {
   ASSERT_SAFE_EXPRESSIONS;
    _symtab.push(); // for block-local vars
    if (node->declaration()) node->declaration()->accept(this, lvl + 2);
    if (node->instructions()) node->instructions()->accept(this, lvl + 2);
    _symtab.pop();
}

void til::postfix_writer::do_alloc_node(til::alloc_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  //TODO
}

void til::postfix_writer::do_declaration_node(til::declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  auto symbol = new_symbol();
  reset_new_symbol();

  int offset = 0;
  int typesize = node->type()->size(); 
  
  if(_inFunctionArgs){
    offset = _offset;
    _offset += typesize;
  }
  else if (_inFunctionBody) {
    _offset -= typesize;
    offset = _offset;
  } else {
    offset = 0;
  }
  
  symbol->set_offset(offset);
  
  auto id = node->identifier();
  
  if (_inFunctionBody){
    if(node->initializer() == nullptr || _inFunctionArgs){
      return;
    }
    node->initializer()->accept(this, lvl);
    if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_STRING)){
      _pf.LOCAL(symbol->offset());
      _pf.STINT();
    } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
      if (node->initializer()->is_typed(cdk::TYPE_INT)){
        _pf.I2D();
      } 
      _pf.LOCAL(symbol->offset());
      _pf.STDOUBLE();
    } else {
      std::cerr << "cannot initialize" << std::endl;
    }

  } else {
    if (node->initializer() == nullptr) {
        _pf.BSS();
        _pf.ALIGN();
        /*if (symbol->qualifier() == tPUBLIC) {
          
        }*/
        _pf.GLOBAL(symbol->name(), _pf.OBJ());
        _pf.LABEL(symbol->name());
        _pf.SALLOC(typesize);
    } else {
      if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE) || node->is_typed(cdk::TYPE_STRING)){
        
        _pf.DATA();
        _pf.ALIGN();
        _pf.GLOBAL(symbol->name(), _pf.OBJ());
        _pf.LABEL(symbol->name());
        
        if (node->is_typed(cdk::TYPE_DOUBLE) && node->initializer()->is_typed(cdk::TYPE_INT)){
          cdk::integer_node *itd = dynamic_cast<cdk::integer_node*>(node->initializer());
          cdk::double_node idt(itd->lineno(), itd->value());
          idt.accept(this, lvl);
        } else {
          node->initializer()->accept(this, lvl);
        }

      } else if(node->is_typed(cdk::TYPE_FUNCTIONAL)){
        node->initializer()->accept(this, lvl);
        _pf.DATA();
        _pf.ALIGN();
        _pf.GLOBAL(symbol->name(), _pf.OBJ());
        _pf.LABEL(symbol->name());
      }
      else {
        std::cerr << node->lineno() << "unexpected initializer\n" << std::endl;
        exit(1);
      }
    }
  }
}

void til::postfix_writer::do_index_pointer_node(til::index_pointer_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  //TODO
}

void til::postfix_writer::do_next_node(til::next_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto loopIndex = static_cast<size_t>(node->loopIndex());
  auto index = _whileLoopLabels->size() - loopIndex;
  auto label = std::get<0>(_whileLoopLabels->at(index));

  _pf.JMP(label);

}

void til::postfix_writer::do_nullptr_node(til::nullptr_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
   if (_inFunctionBody) {
    _pf.INT(0);
  } else {
    _pf.SINT(0);
  }
}

void til::postfix_writer::do_return_node(til::return_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  //TODO
}

void til::postfix_writer::do_sizeof_node(til::sizeof_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (_inFunctionBody)
    _pf.INT(node->argument()->type()->size());
  else
    _pf.SINT(node->argument()->type()->size());
}


void til::postfix_writer::do_stop_node(til::stop_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  auto loopIndex = static_cast<size_t>(node->loopIndex());
  auto index = _whileLoopLabels->size() - loopIndex;
  auto label = std::get<1>(_whileLoopLabels->at(index));

  _pf.JMP(label);

}

void til::postfix_writer::do_function_call_node(til::function_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  std::shared_ptr<cdk::functional_type> func_type;
  func_type = cdk::functional_type::cast(node->functions()->type());
  int args_size = 0;
  
  for (size_t i = node->arguments()->size(); i > 0; i--) {
    auto arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i - 1));
    arg->accept(this, lvl + 2);
    
    args_size += arg->type()->size();
  }

  node->functions()->accept(this,lvl);
  _pf.BRANCH();
  
  if (args_size > 0) {
    _pf.TRASH(args_size);
  }
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.LDFVAL64();
  } else if (!node->is_typed(cdk::TYPE_VOID)) {
    _pf.LDFVAL32();
  }

}

void til::postfix_writer::do_function_definition_node(til::function_definition_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  const auto functionLabel = mklbl(++_lbl);

  _pf.TEXT();
  _pf.ALIGN();
  _pf.LABEL(functionLabel);

  const auto previous_offset = _offset;
  _offset = 8;

  _symtab.push();

  if(node->arguments()){
    _inFunctionArgs = true;
    node->arguments()->accept(this, lvl);
    _inFunctionArgs = false;
  }

  frame_size_calculator fsc(_compiler, _symtab);
  node->accept(&fsc, lvl);
  _pf.ENTER(fsc.localsize());

  _offset = 0; 
  _inFunctionBody = true;
  if (node->block())
    node->block()->accept(this, lvl + 2);
  _inFunctionBody = false;

  _offset = previous_offset;
  _symtab.pop(); 
  _pf.LEAVE();
  _pf.RET();  
}
