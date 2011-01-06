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
  struct grammar : qi::grammar<Iterator, Variable(), ascii::space_type>{
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

    grammar() : grammar::base_type(variable) {
      integer = qi::long_long[phx::bind(&Integer::value, qi::_val) = qi::_1];
      string_value = qi::lexeme['"' >> +(qi::char_ - '"') >> '"'];
      string = string_value[phx::bind(&String::value, qi::_val) = qi::_1];
      boolean = qi::lit("true")[phx::bind(&Boolean::value, qi::_val) = true] 
              | qi::lit("false")[phx::bind(&Boolean::value, qi::_val) = false];
      identifier = qi::alpha >> *qi::alnum;
      variable = identifier[phx::bind(&Variable::name, qi::_val) = qi::_1];
    }
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
  
  uscheme::parser::grammar<std::string::const_iterator> g;
  uscheme::ast::Variable b;
  b.name = "junk";
  std::string::const_iterator iter = str.begin();
  std::string::const_iterator end = str.end();
  bool r = boost::spirit::qi::phrase_parse(iter, end, g,
      boost::spirit::ascii::space, b);
  if(r && iter == str.end()) { std::cout << "yay " << b.name << std::endl; }
  // do stuff
  return 0;
}
