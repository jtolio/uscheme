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
  struct grammar : qi::grammar<Iterator, PTR<Expression>(), ascii::space_type>{
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
    qi::rule<Iterator, Body(), ascii::space_type> begin;
    qi::rule<Iterator, Application(), ascii::space_type> application;

    grammar() : grammar::base_type(expression) {
      integer = qi::long_long[phx::bind(&Integer::value, qi::_val) = qi::_1];
      string_value = qi::lexeme['"' >> +(qi::char_ - '"') >> '"'];
      string = string_value[phx::bind(&String::value, qi::_val) = qi::_1];
      boolean = qi::lit("true")[phx::bind(&Boolean::value, qi::_val) = true] 
              | qi::lit("false")[phx::bind(&Boolean::value, qi::_val) = false];
      identifier = qi::alpha >> *qi::alnum;
      variable = identifier[phx::bind(&Variable::name, qi::_val) = qi::_1];
      expression = variable[boost::bind(&make_expression_Variable, ::_1, ::_2)]|
                   integer[boost::bind(&make_expression_Integer, ::_1, ::_2)] |
                   boolean[boost::bind(&make_expression_Boolean, ::_1, ::_2)] |
                   string[boost::bind(&make_expression_String, ::_1, ::_2)];
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
#undef MAKE_EXPRESSION
  };
  
/*
    body = def* exp*
    
    def = (define var exp)
    
    exp = integer
        | boolean
        | string
        | var
        | (if exp exp exp)
        | (and exp*)
        | (or exp*)
        | (set! var exp)
        | (lambda (var*) body)
        | (begin body)
        | (exp exp*)
*/

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
  PTR<ast::Expression> b;
  std::string::const_iterator iter = str.begin();
  std::string::const_iterator end = str.end();
  bool r = boost::spirit::qi::phrase_parse(iter, end, g,
      boost::spirit::ascii::space, b);
  if(r && iter == str.end()) {
    if(b.get()) {
      if(typeid(*(b.get())) == typeid(ast::Variable))
        std::cout << "variable " << ((ast::Variable*)b.get())->name;
      else if(typeid(*(b.get())) == typeid(ast::Integer))
        std::cout << "integer " << ((ast::Integer*)b.get())->value;
      else if(typeid(*(b.get())) == typeid(ast::String))
        std::cout << "string " << ((ast::String*)b.get())->value;
      else if(typeid(*(b.get())) == typeid(ast::Boolean))
        std::cout << "boolean " << ((ast::Boolean*)b.get())->value;
      else
        std::cout << "unknown";
    }
    std::cout << std::endl;
  }
  // do stuff
  return 0;
}
