// test_authorize
//  認定問題解析プログラム
// $Id: test_authorize.cc,v 1.1 2000/02/01 23:41:19 atsushi Exp $
#include <iostream>
#include <fstream>

// 1.AuthorizeProblem.hをincludeすること
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
	
	// 3. パーザクラスを生成する.
	AuthorizeRegistration rs(&grammar);
	
	// 4. 入力文字列を読み込む
	std::ifstream strfs(INPUTFILENAME);
	if(!strfs.good()){
		std::cerr << "Error: Cannot open file : " << INPUTFILENAME << std::endl;
		exit(0);
	}
	std::string input;
	getline(strfs, input);
	
	// 5. パージングを行う.
	rs.Regist(input);
	
	// 6.バックトレースを行う 
	std::cout << rs.CalcProbability() << std::endl;
	
	// 他にもパージングをしたい文字列がある場合, 
	//5に戻る.
	
	return 0;
};
