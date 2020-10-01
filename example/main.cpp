// main
//  mainプログラム(の一例)
// $Id: main.cc,v 1.6 2000/03/07 11:49:09 atsushi Exp $
#include <iostream>
#include <fstream>

// 1.ParseProblem.h/AuthorizeProblemをincludeすること
#include "ParseProblem.h"
#include "AuthorizeProblem.h"

const char* const RULEFILENAME  = "rule.txt";   // 生成規則のファイル名
const char* const INPUTFILENAME = "input.txt";  // 入力ファイル名

int main(){
  // 文法ファイルを開く
  std::ifstream ifs(RULEFILENAME);
  if(!ifs.good()){
    std::cerr << "Error: Cannot open file : " << RULEFILENAME << std::endl;
    exit(0);
  }
  // 2. 文法ファイルのstreamから文法のデータベースを作成する            
  Grammar grammar(ifs);
  if(!grammar.Good()){
    std::cerr << "Error: Rule file error " 
	 << RULEFILENAME << ":" << grammar.ErrorLine() << std::endl;
    exit(0);
  }
  ifs.close();

  // 3. 入力文字列を読み込む
  std::ifstream strfs(INPUTFILENAME);
  if(!strfs.good()){
    std::cerr << "Error: Cannot open file : " << INPUTFILENAME << std::endl;
    exit(0);
  }
  std::string input;
  getline(strfs, input);

  //== 構文解析
  // 4. 構文解析用パーザクラスを生成する.
  ParseRegistration     preg(&grammar);
  // 5. 上位いくつの木を導出するかの設定(初期値は10)
  preg.SetLimit(10);
  // 6. 構文解析をおこなう
  preg.Regist(input);
  // 7. バックトレースを行う 
  std::cout << "-- 構文解析 --" << std::endl;
  preg.BackTraceAll();

  //== 認定問題
  // 8. 認定問題用パーザクラスを生成する.
  AuthorizeRegistration areg(&grammar);
  // 9. パージングする
  areg.Regist(input);
  //10. 結果の出力
  std::cout << "-- 認定問題 --" << std::endl;
  std::cout << areg.CalcProbability() << std::endl;
  
  return 0;
};
