//  test_parse.cpp
//    1999 - 2023 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include <fstream>
#include <iostream>

// include "ParseProblem.h"
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
  } catch (std::exception e) {
    std::cerr << "Error: Rule file error " << e.what() << std::endl;
    exit(0);
  }
  ifs.close();

  // create a parser class
  ParseRegistration rs(grammar);

  // read an input string
  std::ifstream strfs(INPUTFILENAME);
  if (!strfs.good()) {
    std::cerr << "Error: Cannot open file : " << INPUTFILENAME << std::endl;
    exit(0);
  }
  std::string input;
  getline(strfs, input);

  // set the upper number of parse trees (default is 10) 
  rs.set_limit(10);
  try {
    // parse the input string.
    auto results = rs.solve(input);

    // get the parse tree
    std::cout << "** call BackTraceAll" << std::endl;
    rs.print_all_results(results); // get all trees

    int num = results->get_prob_list().size(); // get a tree
    for (int i = 0; i < num; i++) {
      std::cout << "** call BackTrace" << std::endl;
      rs.print_result_at(results, i);
    }

  } catch (std::runtime_error e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
};
