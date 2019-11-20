#pragma once
#include"Header.h"


int main(int argc, char *argv[]) {

	//圧縮もしくは解凍データ入力。
	if (argc != 2) {
		std::cout << "引数に対象ファイルのあるパスを一つ入力してください。\n";
		return false;
	}

	FILE* fp = fopen(argv[1], "rb");
	if (fp == NULL) {
		std::cout << "fopenエラー\n";
	}


	int selectProgram = 0;													//ユーザー選択
	node* listStart = nullptr;												//ノードリストの先頭ポインタ
	node* treeHead = nullptr;													//2分木の根のポインタ
	char* haffmanCodeListPath = nullptr;										//ハフマン符号一覧表のファイルパス
	char* encodeFilePath = nullptr;											//圧縮ファイルのファイルパス
	char* decodeFilePath = nullptr;											//解凍ファイルのファイルパス

	while (1) {
		//圧縮か解凍選択
		std::cout << "圧縮(0)/解凍(1)？ =>";
		std::cin >> selectProgram;
		if (selectProgram == static_cast<int>(program::ENCODE)) {
			/* ----------圧縮処理---------- */
			listStart = createNodeList(fp);									//ファイルデータを読みこみ、ノードリストを作成。
			treeHead = createHaffmanTree(listStart);						//作成したノードリストからハフマン木を作成
			haffmanCodeListPath = createHaffmanCodeList(treeHead);			//作成したハフマン木からハフマン符号一覧表を作成
			encodeFilePath = createEncodeFile(haffmanCodeListPath);			//圧縮を実行。新規ファイルに出力
			break;
		}
		else if (selectProgram == static_cast<int>(program::DECODE)) {
			/* ----------解凍処理---------- */
			if (checkHaffmanCodeList(haffmanCodeListPath)) {				//ハフマン符号一覧表が正しくあるかチェック
				decodeFilePath = createDecodeFile(fp, haffmanCodeListPath);	//解凍を実行。新規ファイルに出力
			}
			else {
				return false;
			}
			break;
		}
		else {
			std::cout << "入力エラーです\n";
			continue;
		}
	}


	return true;
}


/* ----------圧縮---------- */
/**
* ノードリストの作成
* @param fp 圧縮対象ファイルのファイルポインタ
* @return ノードリストの先頭ポインタ
*/
node* createNodeList(FILE* fp) {
	int fileLen = 0;
	node* newNode = nullptr;
	node* findNode = nullptr;
	node* listStart = nullptr;
	node* listEnd = nullptr;

	int character;

	while (character = fgetc(fp) != EOF) {
		//リスト探索
		findNode = searchNode(listStart, static_cast<char>(character));
		if ( findNode == nullptr) {
			//ノードをリストに新規登録
			newNode = createNode(listStart, static_cast<char>(character));
			if (listStart == nullptr) {
				//ノードリストに何も入ってない時（最初の登録）のリスト追加。
				listStart = newNode;
				listEnd = newNode;
				listEnd->next = nullptr;
			}
			else {
				//ノードリストに中身はあるけど、検索に引っかからなかった時（ノードを新規登録時）のリスト追加。
				listEnd = listStart;
				while (listEnd->next != nullptr) {
					listEnd = listEnd->next;
				}
				listEnd->next = newNode;
				newNode->prev = listEnd;
			}
		}
		else {
			//検索に引っかかったのでそのノード個数を＋1
			findNode->nodeNum++;
		}
	}
	fclose(fp);
	return listStart;
}

/**
* ノードの探索
* @param character 探索する文字
* @return 見つけたノードのポインタ
*/
node* searchNode(node* listStart ,char character) {
	if (listStart == nullptr) {
		return nullptr;
	}
	while (listStart->character != character) {
		listStart = listStart->next;
		if (listStart == nullptr) {
			return nullptr;
		}
	}
	return listStart;
}

/**
* ノードの作成
* @param character ノードに登録する文字
* @return ノードのポインタ
*/
node* createNode(node* listStart, char character) {
	node* newNode = new node;
	newNode->character = character;
	newNode->nodeNum++;
	return newNode;

}


/**
* ハフマン木の作成
* @param listStart ノードリストの先頭ポインタ
* @return ハフマン木の根ポインタ 
*/
node* createHaffmanTree(node* listStart) {

	listStart = sortNodeList(listStart);	//ノードリストのソート（小さい順）
	node* haffman = listStart;//ハフマン木を作っていくために、少ないノードから大きいノードへ移動していくポインタ
	node* noCharNode;//文字ありノードの親になるノード
	node* firstNode=nullptr;
	node* secondNode = nullptr;
	int charNodesNum=0;//ノードリストのノードの個数

	//ここにノード個数を数える処理を入れる




	for (int i = 1; i <= (charNodesNum-1)*2; charNodesNum++) {
		noCharNode = createNode(listStart, '\0');
		if (i % 2 == 1) {
			//パートナー（仮の左）を選択
			//奇数
			firstNode = selectNode(listStart, i);
		}
		else {
			//偶数
			//パートナー（仮の右）を選択
			secondNode = selectNode(listStart, i);
			
			//両パートナーの親にnoCharNodeを入れる
			firstNode->parent = noCharNode;
			secondNode->parent = noCharNode;

			//両パートナーを比較。親の左右に入れる。
			if (firstNode->nodeNum < secondNode->nodeNum) {
				noCharNode->left = firstNode;
				noCharNode->right = secondNode;
			}
			else {
				noCharNode->right = firstNode;
				noCharNode->left = secondNode;
			}
			//createNodeで+1してるので-1
			noCharNode->nodeNum = (noCharNode->left->nodeNum) + (noCharNode->right->nodeNum) - 1;
		}
	}

	return nullptr;
}

/**
* ノードリストのソート。出現回数の小さい順にソートする。
* @param listStart ノードリストの先頭ポインタ
* @return ノードリストの先頭ポインタ
*/
node* sortNodeList(node* listStart) {
	
	return nullptr;
}

/**
* 2分木を作成するための候補となるノードを選択してくれる。
* @param listStart ノードリストの先頭ポインタ
* @param freq 文字ありノードを小さい順に並べたときの順番号。
* @return 選択されたノードのポインタ
*/
node* selectNode(node* listStart, int freq) {
	static node* moveList_ch = listStart;//文字のあるノードを動く
	node* retNode;
	if (freq % 2 == 1 && freq != 1) {
		//freqが奇数で1以外の時は文字のないノードを選択//3,5,7...
		retNode = moveList_ch->prev->parent;
	}
	else {
		//freqが１と偶数のときは、文字のあるノードを小さい順に選択//1,2,4,6...
		retNode = moveList_ch;
	}
	moveList_ch = moveList_ch->next;
	return retNode;
}



/**
* 文字ありノードのハフマン符号一覧表を作成
* @param treeHead ハフマン木の根ポインタ
* @return 一覧表のファイルパス
*/
char* createHaffmanCodeList(node* treeHead) {
	return nullptr;
}

/**
* 圧縮ファイルの作成
* @param fp 圧縮対象ファイルのファイルポインタ
* @param path 一覧表のファイルパス 
* @return 圧縮ファイルのファイルパス
*/
char* createEncodeFile(char* path) {
	return nullptr;
}


/* ----------解凍----------- */
/**
* ハフマン符号一覧表が正しくあるかチェック
* @param path 一覧表のファイルパス
* @return true 正しい
* @return false 正しくない
*/
bool checkHaffmanCodeList(char* path) {

	return true;
	std::cout << "ハフマン符号一覧表が無いもしくは内容が正しくありません。\n";
	std::cout << "FILEPATH：%s\n" << path;
	
}

/**
* 解凍ファイルの作成
* @param fp 解凍対象ファイルのファイルポインタ
* @param path 一覧表のファイルパス
* @return 解凍ファイルのファイルパス
*/
char* createDecodeFile(FILE* fp, char* path) {
	return nullptr;
}



