#ifndef __AST_H__
#define __AST_H__

#include <vector>
#include <string>

namespace picturesque {
namespace ast {

  struct Expression { protected: Expression() {} };
  struct Literal : public Expression { protected: Literal() {} };

  //struct Symbol : public Literal {};
  struct Integer : public Literal {
    int64_t value;
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

  struct Definition {
    Variable variable;
    Expression expression;
  };

  struct Body {
    std::vector<Definition> definitions;
    std::vector<Expression> expressions;
  };

  struct Conditional : public Expression {
    Expression predicate;
    Expression consequent;
    Expression alternative;
  };
  struct Conjunction : public Expression {
    std::vector<Expression> expressions;
  };
  struct Disjunction : public Expression {
    std::vector<Expression> expressions;
  };
  struct Binding {
    Variable variable;
    Expression expression;
  };
  struct Mutation : public Expression {
    Binding binding;
  };
  struct Lambda : public Expression {
    std::vector<Variable> arguments;
    Body body;
  };
  struct Let : public Expression {
    std::vector<Binding> bindings;  
    Body body;
  };
  struct Application : public Expression {
    Expression applicator;
    std::vector<Expression> arguments;
  };

}}
#endif
