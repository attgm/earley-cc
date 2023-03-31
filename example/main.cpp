//  main.cpp
//    1999 - 2023 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

// include "ParseProblem.h" and "AuthorizeProblem.h"
#include "AuthorizeProblem.h"
#include "ParseProblem.h"

const char *const RULEFILENAME = "rule.txt";   // production rule file
const char *const INPUTFILENAME = "input.txt"; // input string file

int main() {
  // open the production rule file
  std::ifstream ifs(RULEFILENAME);
  if (!ifs.good()) {
    std::cerr << "Error: Cannot open file : " << RULEFILENAME << std::endl;
    exit(0);
  }
  // crate a production rule database from the stream
  auto grammar = std::make_shared<Grammar>();
  try {
    grammar->load_rule(ifs);
  } catch (std::runtime_error e) {
    std::cerr << "Error: " << e.what() << std::endl;
    exit(0);
  }
  ifs.close();

  // read an input string
  std::ifstream strfs(INPUTFILENAME);
  if (!strfs.good()) {
    std::cerr << "Error: Cannot open file : " << INPUTFILENAME << std::endl;
    exit(0);
  }
  std::string input;
  getline(strfs, input);

  try {
    //== Parse Problem
    // create a parser class for a parse problem
    ParseRegistration preg(grammar);
    // set the upper number of parse trees (default is 10) 
    preg.set_limit(10);
    // parse the input string.
    auto presult = preg.solve(input);
    // get the parse tree
    std::cout << "-- Parse Problem --" << std::endl;
    preg.print_all_results(presult);

    //== Authorize Problem
    // create a parser class for an authorize problem
    AuthorizeRegistration areg(grammar);
    // parse the input string.
    auto aresult = areg.solve(input);
    // get the probability
    std::cout << "-- Authorize Problem --" << std::endl;
    std::cout << aresult->get_probability() << std::endl;
  } catch (std::runtime_error e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
};
