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

BOOST_FUSION_ADAPT_STRUCT(uscheme::ast::Integer,
                     (long long, value))
BOOST_FUSION_ADAPT_STRUCT(uscheme::ast::Boolean,
                     (bool, value))
BOOST_FUSION_ADAPT_STRUCT(uscheme::ast::String,
                     (std::string, value))
BOOST_FUSION_ADAPT_STRUCT(uscheme::ast::Variable,
                     (std::string, name))
BOOST_FUSION_ADAPT_STRUCT(uscheme::ast::Binding,
                     (uscheme::ast::Variable, variable)
                     (PTR<uscheme::ast::Expression>, expression))
BOOST_FUSION_ADAPT_STRUCT(uscheme::ast::Body,
                     (std::vector<uscheme::ast::Binding>, bindings)
                     (std::vector<PTR<uscheme::ast::Expression> >, expressions))
BOOST_FUSION_ADAPT_STRUCT(uscheme::ast::Conditional,
                     (PTR<uscheme::ast::Expression>, predicate)
                     (PTR<uscheme::ast::Expression>, consequent)
                     (PTR<uscheme::ast::Expression>, alternate))
BOOST_FUSION_ADAPT_STRUCT(uscheme::ast::Conjunction,
                     (std::vector<PTR<uscheme::ast::Expression> >, expressions))
BOOST_FUSION_ADAPT_STRUCT(uscheme::ast::Disjunction,
                     (std::vector<PTR<uscheme::ast::Expression> >, expressions))
BOOST_FUSION_ADAPT_STRUCT(uscheme::ast::Mutation,
                     (uscheme::ast::Binding, binding))
BOOST_FUSION_ADAPT_STRUCT(uscheme::ast::Lambda,
                     (std::vector<uscheme::ast::Variable>, arguments)
                     (uscheme::ast::Body, body))
BOOST_FUSION_ADAPT_STRUCT(uscheme::ast::Application,
                     (PTR<uscheme::ast::Expression>, applicator)
                     (std::vector<PTR<uscheme::ast::Expression> >, arguments))

#endif
