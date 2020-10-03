//  main.cpp
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

// 1.ParseProblem.h/AuthorizeProblemをincludeすること
#include "AuthorizeProblem.h"
#include "ParseProblem.h"

const char *const RULEFILENAME = "rule.txt";   // 生成規則のファイル名
const char *const INPUTFILENAME = "input.txt"; // 入力ファイル名

int main() {
  // 文法ファイルを開く
  std::ifstream ifs(RULEFILENAME);
  if (!ifs.good()) {
    std::cerr << "Error: Cannot open file : " << RULEFILENAME << std::endl;
    exit(0);
  }
  // 2. 文法ファイルのstreamから文法のデータベースを作成する
  auto grammar = std::make_shared<Grammar>();
  try {
    grammar->load_rule(ifs);
  } catch (std::runtime_error e) {
    std::cerr << "Error: " << e.what() << std::endl;
    exit(0);
  }
  ifs.close();

  // 3. 入力文字列を読み込む
  std::ifstream strfs(INPUTFILENAME);
  if (!strfs.good()) {
    std::cerr << "Error: Cannot open file : " << INPUTFILENAME << std::endl;
    exit(0);
  }
  std::string input;
  getline(strfs, input);

  //== 構文解析
  // 4. 構文解析用パーザクラスを生成する.
  ParseRegistration preg(grammar);
  // 5. 上位いくつの木を導出するかの設定(初期値は10)
  preg.set_limit(10);
  // 6. 構文解析をおこなう
  preg.regist(input);
  // 7. バックトレースを行う
  std::cout << "-- 構文解析 --" << std::endl;
  preg.back_trace_all();

  //== 認定問題
  // 8. 認定問題用パーザクラスを生成する.
  AuthorizeRegistration areg(grammar);
  // 9. パージングする
  areg.regist(input);
  // 10. 結果の出力
  std::cout << "-- 認定問題 --" << std::endl;
  std::cout << areg.calc_probability() << std::endl;

  return 0;
};
