#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include "common.h"
#include "ast.h"

namespace uscheme {
namespace parser {

  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  namespace phx = boost::phoenix;
  using namespace uscheme::ast;
  
  template <typename Iterator>
  struct grammar : qi::grammar<Iterator, Body(), ascii::space_type>{
    qi::rule<Iterator, Body(), ascii::space_type> body;
    qi::rule<Iterator, Binding(), ascii::space_type> definition;
    qi::rule<Iterator, PTR<Expression>(), ascii::space_type> expression;
    qi::rule<Iterator, Integer(), ascii::space_type> integer;
    qi::rule<Iterator, Boolean(), ascii::space_type> boolean;
    qi::rule<Iterator, std::string(), ascii::space_type> string_value;
    qi::rule<Iterator, std::string(), ascii::space_type> identifier;
    qi::rule<Iterator, String(), ascii::space_type> string;
    qi::rule<Iterator, Variable(), ascii::space_type> variable;
    qi::rule<Iterator, Conditional(), ascii::space_type> conditional;
    qi::rule<Iterator, Conjunction(), ascii::space_type> conjunction;
    qi::rule<Iterator, Disjunction(), ascii::space_type> disjunction;
    qi::rule<Iterator, Mutation(), ascii::space_type> mutation;
    qi::rule<Iterator, Lambda(), ascii::space_type> lambda;
    qi::rule<Iterator, Scope(), ascii::space_type> begin;
    qi::rule<Iterator, Application(), ascii::space_type> application;

    grammar() : grammar::base_type(body) {
      integer = qi::long_long[phx::bind(&Integer::value, qi::_val) = qi::_1];
      string_value = qi::lexeme['"' > +(qi::char_ - '"') > '"'];
      string = string_value[phx::bind(&String::value, qi::_val) = qi::_1];
      boolean = qi::lit("true")[phx::bind(&Boolean::value, qi::_val) = true] 
              | qi::lit("false")[phx::bind(&Boolean::value, qi::_val) = false];
      identifier = qi::lexeme[qi::alpha >> *qi::alnum];
      variable = identifier[phx::bind(&Variable::name, qi::_val) = qi::_1];
      body = (*definition >> *expression)[
          phx::bind(&Body::bindings, qi::_val) = qi::_1,
          phx::bind(&Body::expressions, qi::_val) = qi::_2];
      definition = (qi::lit("(define") > variable > expression > ')')[
          phx::bind(&Binding::variable, qi::_val) = qi::_1,
          phx::bind(&Binding::expression, qi::_val) = qi::_2];
      conditional = (qi::lit("(if") > expression > expression >
          expression > ')')[
              phx::bind(&Conditional::predicate, qi::_val) = qi::_1,
              phx::bind(&Conditional::consequent, qi::_val) = qi::_2,
              phx::bind(&Conditional::alternate, qi::_val) = qi::_3];
      conjunction = (qi::lit("(and") > +expression > ')')[
          phx::bind(&Conjunction::expressions, qi::_val) = qi::_1];
      disjunction = (qi::lit("(or") > +expression > ')')[
          phx::bind(&Disjunction::expressions, qi::_val) = qi::_1];
      mutation = (qi::lit("(set!") > variable > expression > ')')[
          phx::bind(&Binding::variable,
              phx::bind(&Mutation::binding, qi::_val)) = qi::_1,
          phx::bind(&Binding::expression,
              phx::bind(&Mutation::binding, qi::_val)) = qi::_2];
      lambda = (qi::lit("(lambda") > '(' > *variable > ')' > body > ')')[
          phx::bind(&Lambda::arguments, qi::_val) = qi::_1,
          phx::bind(&Lambda::body, qi::_val) = qi::_2];
      begin = (qi::lit("(begin") > body > ')')[
          phx::bind(&Scope::body, qi::_val) = qi::_1];
      application = (qi::lit("(") >> expression >> *expression >> ')')[
          phx::bind(&Application::applicator, qi::_val) = qi::_1,
          phx::bind(&Application::arguments, qi::_val) = qi::_2];
      expression = variable[boost::bind(&make_expression_Variable, ::_1, ::_2)]|
                   integer[boost::bind(&make_expression_Integer, ::_1, ::_2)] |
                   boolean[boost::bind(&make_expression_Boolean, ::_1, ::_2)] |
                   string[boost::bind(&make_expression_String, ::_1, ::_2)] |
                   conditional[boost::bind(
                      &make_expression_Conditional, ::_1, ::_2)] |
                   conjunction[boost::bind(
                      &make_expression_Conjunction, ::_1, ::_2)] |
                   disjunction[boost::bind(
                      &make_expression_Disjunction, ::_1, ::_2)] |
                   mutation[boost::bind(&make_expression_Mutation, ::_1, ::_2)]|
                   lambda[boost::bind(&make_expression_Lambda, ::_1, ::_2)] |
                   begin[boost::bind(&make_expression_Scope, ::_1, ::_2)] |
                   application[boost::bind(&make_expression_Application, ::_1,
                      ::_2)];
      
      body.name("body");
      definition.name("definition");
      expression.name("expression");
      integer.name("integer");
      boolean.name("boolean");
      string_value.name("string");
      string.name("string");
      identifier.name("variable");
      variable.name("variable");
      conditional.name("conditional");
      conjunction.name("conjunction");
      disjunction.name("disjunction");
      mutation.name("mutation");
      lambda.name("lambda");
      begin.name("begin");
      application.name("application");

      qi::on_error<qi::fail>(body,
        std::cout << phx::val("Error! Expecting ") << qi::_4
                  << phx::val(" here: \"")
                  << phx::construct<std::string>(qi::_3, qi::_2)
                  << phx::val("\"") << std::endl
      );

    }
    
    private:
#define MAKE_EXPRESSION(exp_type) \
      static void make_expression_##exp_type(exp_type const& x, \
          boost::spirit::context<boost::fusion::cons<PTR<Expression>&, \
          boost::fusion::nil>, boost::fusion::vector0<void> >& context) { \
        boost::fusion::at_c<0>(context.attributes).reset(new exp_type(x)); \
      }
      MAKE_EXPRESSION(Variable)
      MAKE_EXPRESSION(Integer)
      MAKE_EXPRESSION(Boolean)
      MAKE_EXPRESSION(String)    
      MAKE_EXPRESSION(Conditional)
      MAKE_EXPRESSION(Conjunction)
      MAKE_EXPRESSION(Disjunction)
      MAKE_EXPRESSION(Application)
      MAKE_EXPRESSION(Mutation)
      MAKE_EXPRESSION(Lambda)
      MAKE_EXPRESSION(Scope)
#undef MAKE_EXPRESSION
  };

}}

int main(int argc, char** argv) {
  std::cout << "started" << std::endl;

  using namespace uscheme;
  std::string str;
  std::ostringstream os;
  while(getline(std::cin, str)) {
    if(str.empty()) break;
    os << str;
  }
  str = os.str();
  
  parser::grammar<std::string::const_iterator> g;
  ast::Body b;
  std::string::const_iterator iter = str.begin();
  std::string::const_iterator end = str.end();
  bool r = boost::spirit::qi::phrase_parse(iter, end, g,
      boost::spirit::ascii::space, b);
  if(r && iter == str.end()) {
    std::cout << "yay" << std::endl;
  } else {
    std::cout << "boo" << std::endl;
  }
  // do stuff
  return 0;
}
