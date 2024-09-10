#include <string>
#include "targets/type_checker.h"
#include ".auto/all_nodes.h"  // automatically generated
#include <cdk/types/primitive_type.h>

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//---------------------------------------------------------------------------

void til::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void til::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}
void til::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}
void til::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
}
void til::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  if(node->argument()->is_typed(cdk::TYPE_INT)){ //int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->argument()->is_typed(cdk::TYPE_UNSPEC)){ //unspec 
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->argument()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in unary logic expression");
  }
}

void til::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){ //int int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_INT)){ //unspec int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //int unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary logic expression");
  }
  
}
void til::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){ //int int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_INT)){ //unspec int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //int unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary logic expression");
  }
}

//---------------------------------------------------------------------------

void til::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void til::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
}

//---------------------------------------------------------------------------

void til::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (!node->argument()->is_typed(cdk::TYPE_INT) && !node->argument()->is_typed(cdk::TYPE_DOUBLE)){
    throw std::string("wrong type in argument of unary expression");
  } else if (node->argument()->is_typed(cdk::TYPE_UNSPEC)){
    node->argument()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }

  // in Simple, expressions are always int
  node->type(node->argument()->type());
}

void til::type_checker::do_unary_minus_node(cdk::unary_minus_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void til::type_checker::do_unary_plus_node(cdk::unary_plus_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void til::type_checker::processBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of binary expression");

  // in Simple, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void til::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){ //int int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //double double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)){ //double int
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //int double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)){ //pointer int
    node->type(node->left()->type());
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_POINTER)){ //int pointer
    node->type(node->right()->type());
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_INT)){ //unspec int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //int unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //unspec double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->left()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //double unspec
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->right()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_POINTER)){ //unspec pointer
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->type(node->right()->type());
  } else if(node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //pointer unspec
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->type(node->left()->type());
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //unspec unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}

void til::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){ //int int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //double double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)){ //double int
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //int double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)){ //pointer int
    node->type(node->left()->type());
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_POINTER)){ //int pointer
    node->type(node->right()->type());
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_INT)){ //unspec int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //int unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //unspec double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->left()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //double unspec
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->right()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_POINTER)){ //unspec pointer
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->type(node->right()->type());
  } else if(node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //pointer unspec
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->type(node->left()->type());
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //unspec unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_POINTER)) { //pointer pointer
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
  else {
    throw std::string("wrong types in binary expression");
  }
}

void til::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){ //int int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //double double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)){ //double int
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //int double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_INT)){ //unspec int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //int unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //unspec double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->left()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //double unspec
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->right()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //unspec unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}

void til::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){ //int int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //double double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)){ //double int
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //int double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_INT)){ //unspec int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //int unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //unspec double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->left()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //double unspec
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->right()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //unspec unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}

void til::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){ //int int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_INT)){ //unspec int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //int unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //unspec unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}

void til::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){ //int int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //double double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)){ //double int
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //int double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_INT)){ //unspec int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //int unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //unspec double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->left()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //double unspec
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->right()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //unspec unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}

void til::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){ //int int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //double double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)){ //double int
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //int double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_INT)){ //unspec int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //int unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //unspec double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->left()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //double unspec
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->right()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //unspec unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}

void til::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
 ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){ //int int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //double double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)){ //double int
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //int double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_INT)){ //unspec int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //int unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //unspec double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->left()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //double unspec
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->right()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //unspec unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}

void til::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){ //int int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //double double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)){ //double int
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //int double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_INT)){ //unspec int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //int unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //unspec double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->left()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //double unspec
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->right()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //unspec unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}

void til::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){ //int int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //double double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)){ //double int
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //int double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_POINTER)) { //pointer pointer
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_INT)){ //unspec int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //int unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //unspec double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->left()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //double unspec
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->right()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_POINTER)){ //unspec pointer
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->type(node->right()->type());
  } else if(node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //pointer unspec
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->type(node->left()->type());
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //unspec unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}

void til::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  
  if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)){ //int int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //double double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)){ //double int
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //int double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_POINTER)) { //pointer pointer
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_INT)){ //unspec int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //int unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_DOUBLE)){ //unspec double
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->left()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //double unspec
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    node->right()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if(node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_POINTER)){ //unspec pointer
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->type(node->right()->type());
  } else if(node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //pointer unspec
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->type(node->left()->type());
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_UNSPEC)){ //unspec unspec
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}

//---------------------------------------------------------------------------

void til::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  auto symbol = _symtab.find(id);
  if (symbol == nullptr) {
    throw std::string("undeclared variable '" + id + "'");
  } 
  node->type(symbol->type());
}


void til::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());
  
}

void til::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl);
  node->rvalue()->accept(this, lvl);

  /*if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
    node->rvalue()->type(node->lvalue()->type());
  }*/
  if(node->lvalue()->is_typed(cdk::TYPE_INT) && node->rvalue()->is_typed(cdk::TYPE_INT)){
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if((node->lvalue()->is_typed(cdk::TYPE_DOUBLE) && node->rvalue()->is_typed(cdk::TYPE_DOUBLE))){
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if((node->lvalue()->is_typed(cdk::TYPE_STRING) && node->rvalue()->is_typed(cdk::TYPE_STRING))){
    node->rvalue()->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
  } else {
    throw std::string("wrong type in right argument");
  }

  /*
  
  try {
    node->lvalue()->accept(this, lvl);
  } catch (const std::string &id) {
    auto symbol = std::make_shared<til::symbol>(cdk::primitive_type::create(4, cdk::TYPE_INT), id, 0);
    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }

  if (!node->lvalue()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of assignment expression");

  node->rvalue()->accept(this, lvl + 2);
  if (!node->rvalue()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of assignment expression");

  // in Simple, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));*/
}

//---------------------------------------------------------------------------

void til::type_checker::do_program_node(til::program_node *const node, int lvl) {

}

void til::type_checker::do_evaluation_node(til::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void til::type_checker::do_print_node(til::print_node *const node, int lvl) {

  for (size_t i = 0; i < node->arguments()->size(); i++) {
    auto child = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i));
    
    child->accept(this, lvl);

    if(!child->is_typed(cdk::TYPE_STRING) && !child->is_typed(cdk::TYPE_INT) && !child->is_typed(cdk::TYPE_DOUBLE)){
    throw std::string("wrong type for argument for print instruction");
    } else if(child->is_typed(cdk::TYPE_UNSPEC)){
      child->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    }
   
  }
}

//---------------------------------------------------------------------------

void til::type_checker::do_read_node(til::read_node *const node, int lvl) {
  /*try {
    node->argument()->accept(this, lvl);
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }*/
}

//---------------------------------------------------------------------------

void til::type_checker::do_while_node(til::while_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (node->condition()->is_typed(cdk::TYPE_UNSPEC)) {
    node->condition()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (!node->condition()->is_typed(cdk::TYPE_INT)) {
    throw std::string("wrong type in condition of loop");
  }
}

//---------------------------------------------------------------------------

void til::type_checker::do_if_node(til::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (node->condition()->is_typed(cdk::TYPE_UNSPEC)) {
    node->condition()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (!node->condition()->is_typed(cdk::TYPE_INT)) {
    throw std::string("wrong type in condition of if");
  }
}

void til::type_checker::do_if_else_node(til::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (node->condition()->is_typed(cdk::TYPE_UNSPEC)) {
    node->condition()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (!node->condition()->is_typed(cdk::TYPE_INT)) {
    throw std::string("wrong type in condition of if-else");
  }
}


//---------------------------------------------------------------------------
void til::type_checker::do_address_of_node(til::address_of_node *const node, int lvl) {
  // TODO 
}

void til::type_checker::do_block_node(til::block_node *const node, int lvl) {
  //TODO
}

void til::type_checker::do_alloc_node(til::alloc_node * const node, int lvl) {
  //TODO
}

void til::type_checker::do_declaration_node(til::declaration_node * const node, int lvl) {
  if(node->type()!=nullptr){
    if(node->initializer()!=nullptr){
      node->initializer()->accept(this, lvl + 2);
      if(node->is_typed(cdk::TYPE_STRING)){
        if(!node->initializer()->is_typed(cdk::TYPE_STRING)){
          throw std::string("String expected");
        }
      } else if(node->is_typed(cdk::TYPE_INT)){
        if (!node->initializer()->is_typed(cdk::TYPE_INT)){
          throw std::string("Integer expected");
        }
      } else if(!node->is_typed(cdk::TYPE_DOUBLE)){
        if (!node->initializer()->is_typed(cdk::TYPE_INT) && !node->initializer()->is_typed(cdk::TYPE_DOUBLE)){
          throw std::string("Double or integer expected");
        }
      }
    } 
  } else { // no type, tVAR
    node->initializer()->accept(this, lvl + 2);
    if (node->initializer()->is_typed(cdk::TYPE_UNSPEC)) {
      node->initializer()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    } else if (node->initializer()->is_typed(cdk::TYPE_VOID)) {
      throw std::string("Cannot declare variable of type void");
    }

    node->type(node->initializer()->type());
  }

  auto symbol = make_symbol(node->identifier(), node->type(), node->qualifier());
  if(_symtab.insert(node->identifier(), symbol)){  
    _parent->set_new_symbol(symbol);
    return;
  }
}

void til::type_checker::do_index_pointer_node(til::index_pointer_node * const node, int lvl) {
  //TODO
}

void til::type_checker::do_next_node(til::next_node * const node, int lvl) {
  //TODO
}

void til::type_checker::do_nullptr_node(til::nullptr_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::reference_type::create(4, cdk::primitive_type::create(0, cdk::TYPE_UNSPEC)));
}

void til::type_checker::do_return_node(til::return_node * const node, int lvl) {
  //TODO
}

void til::type_checker::do_sizeof_node(til::sizeof_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->is_typed(cdk::TYPE_UNSPEC)) {
    node->argument()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void til::type_checker::do_stop_node(til::stop_node * const node, int lvl) {
  //TODO
}

void til::type_checker::do_function_call_node(til::function_call_node * const node, int lvl) {
  ASSERT_UNSPEC;

  std::shared_ptr<cdk::functional_type> functype;

  if(node->functions()){ //regular call
    node->functions()->accept(this, lvl);
    if(!(node->functions()->is_typed(cdk::TYPE_FUNCTIONAL))){
      throw std::string("wrong type in function call");
    }

    functype = cdk::functional_type::cast(node->functions()->type());
    if (functype->input()->length() != node->arguments()->size()) {
      throw std::string("wrong number of arguments in function call");
    }

    for (size_t i = 0; i < node->arguments()->size(); i++) {
      auto arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i));
      arg->accept(this, lvl);

      auto param_type = functype->input(i);
      
      if(!(param_type->name() == cdk::TYPE_INT && arg->is_typed(cdk::TYPE_INT))){
        throw std::string("wrong type for argument in function call");
      }
      else if(!(param_type->name() == cdk::TYPE_DOUBLE && arg->is_typed(cdk::TYPE_DOUBLE))){
        throw std::string("wrong type for argument in function call");
      }
      else if(!(param_type->name() == cdk::TYPE_STRING && arg->is_typed(cdk::TYPE_STRING))){
        throw std::string("wrong type for argument in function call");
      }

      if (arg->is_typed(cdk::TYPE_UNSPEC)) {
        if (param_type->name() == cdk::TYPE_DOUBLE) {
          arg->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
        } else {
          arg->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
        }
      } else if (arg->is_typed(cdk::TYPE_POINTER) && param_type->name() == cdk::TYPE_POINTER) {
        if (cdk::reference_type::cast(arg->type())->referenced()->name() == cdk::TYPE_UNSPEC
              || cdk::reference_type::cast(arg->type())->referenced()->name() == cdk::TYPE_VOID
              || cdk::reference_type::cast(param_type)->referenced()->name() == cdk::TYPE_VOID) {
          arg->type(param_type);
        }
      }
    }
  }
  node->type(functype->output(0));
}

void til::type_checker::do_function_definition_node(til::function_definition_node * const node, int lvl) {
  ASSERT_UNSPEC;
}