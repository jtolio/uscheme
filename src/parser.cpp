#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <iostream>
#include <string>
#include <complex>

#include "ast.h"

namespace picturesque {
namespace parser {

  namespace qi = boost::spirit::qi;
  void print(int const& i) {
    std::cout << i << std::endl;
  }
  
  template <typename Iterator>
  bool parse_program(Iterator first, Iterator last, std::complex<double>& c) {
    using qi::double_;
    using qi::_1;
    using qi::phrase_parse;
    using boost::spirit::ascii::space;
    using boost::phoenix::ref;
    
    double rN = 0.0;
    double iN = 0.0;
    bool r = phrase_parse(first, last, (
      '(' >> double_[ref(rN) = _1]
          >> -(',' >> double_[ref(iN) = _1]) >> ')'
      | double_[ref(rN) = _1] ), space) && first == last;
    if(r) c = std::complex<double>(rN, iN);
    return r;
  }

}}

int main(int argc, char** argv) {
  using namespace picturesque;
  std::string str;
  while(getline(std::cin, str)) {
    if(str.empty() || str[0] == 'q' || str[0] == 'Q') break;
    std::complex<double> c;
    if(parser::parse_complex(str.begin(), str.end(), c)) {
      std::cout << "got: " << c << std::endl;
    } else {
      std::cout << "failed" << std::endl;
    }
  }  
  std::cout << "bye" << std::endl;
  return 0;
}
