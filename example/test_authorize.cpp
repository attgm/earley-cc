//  test_authorize.cpp
//    1999 - 2023 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include <fstream>
#include <iostream>

// include "AuthorizeProblem.h"
#include "AuthorizeProblem.h"

const char *const RULE_FILENAME = "rule.txt";   // production rule file
const char *const INPUT_FILENAME = "input.txt"; // input string file

int main() {
  // open the production rule file
  std::ifstream ifs(RULE_FILENAME);
  if (!ifs.good()) {
    std::cerr << "Error: Cannot open file : " << RULE_FILENAME << std::endl;
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
  AuthorizeRegistration rs(grammar);

  // read an input string
  std::ifstream strfs(INPUT_FILENAME);
  if (!strfs.good()) {
    std::cerr << "Error: Cannot open file : " << INPUT_FILENAME << std::endl;
    exit(0);
  }
  std::string input;
  getline(strfs, input);

  try {
    // parse the input string
    auto result = rs.solve(input);

    // get the probability
    std::cout << result->get_probability() << std::endl;

  } catch (std::runtime_error e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
};
