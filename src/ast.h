#ifndef __AST_H__
#define __AST_H__

#include "common.h"
#include <boost/fusion/include/adapt_struct.hpp>

namespace uscheme {
namespace ast {

  struct Expression { protected: Expression() {} };
  struct Literal : public Expression { protected: Literal() {} };

  struct Integer : public Literal {
    long long value;
  };
  
  struct Boolean : public Literal {
    bool value;
  };

  struct String : public Literal {
    std::string value;
  };

  struct Variable : public Literal {
    std::string name;
  };

  struct Binding {
    Variable variable;
    PTR<Expression> expression;
  };

  struct Body {
    std::vector<Binding> bindings;
    std::vector<PTR<Expression> > expressions;
  };
  
  struct Conditional : public Expression {
    PTR<Expression> predicate;
    PTR<Expression> consequent;
    PTR<Expression> alternate;
  };
  
  struct Conjunction : public Expression {
    std::vector<PTR<Expression> > expressions;
  };
  
  struct Disjunction : public Expression {
    std::vector<PTR<Expression> > expressions;
  };

  struct Mutation : public Expression {
    Binding binding;
  };

  struct Lambda : public Expression {
    std::vector<Variable> arguments;
    Body body;
  };

  struct Application : public Expression {
    PTR<Expression> applicator;
    std::vector<PTR<Expression> > arguments;
  };

}}

#endif
