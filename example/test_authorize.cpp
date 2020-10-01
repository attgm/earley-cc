//  test_authorize.cpp
//    1999 - 2020 Atsushi Tagami
//
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
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
	if(!grammar.good()){
		std::cerr << "Error: Rule file error " 
				<< RULEFILENAME << ":" << grammar.error_line() << std::endl;
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
	rs.regist(input);
	
	// 6.バックトレースを行う 
	std::cout << rs.calc_probability() << std::endl;
	
	// 他にもパージングをしたい文字列がある場合, 
	//5に戻る.
	
	return 0;
};
