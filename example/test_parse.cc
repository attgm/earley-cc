// test_parse.cc
//  構文解析プログラム(の一例)
// $Id: test_parse.cc,v 1.2 2000/03/07 11:49:09 atsushi Exp $
#include <iostream>
#include <fstream>

// 1.ParseProblem.hをincludeすること
#include "ParseProblem.h"

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
	
	// 3. パーザクラスを生成する.
	ParseRegistration rs(&grammar);
	
	// 4. 入力文字列を読み込む
	std::ifstream strfs(INPUTFILENAME);
	if(!strfs.good()){
		std::cerr << "Error: Cannot open file : " << INPUTFILENAME << std::endl;
		exit(0);
	}
	std::string input;
	getline(strfs, input);
	
	// 5. 上位いくつの木を導出するかの設定(初期値は10)
	rs.SetLimit(10);
	
	// 6. パージングを行う.
	rs.Regist(input);
	
	// 7.バックトレースを行う 
	std::cout << "** call BackTraceAll" << std::endl;
	rs.BackTraceAll(); // いっぺんに全部やってしまうやりかた
	
	int num = rs.GetResultNum(); // 1つ1つ呼び出すやりかた
	for(int i=0; i<num; i++){
		std::cout << "** call BackTrace" << std::endl;
		rs.BackTrace(i);
	}
	
	// 他にもパージングをしたい文字列がある場合, 
	//4に戻る.
	
	return 0;
};
