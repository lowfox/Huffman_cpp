#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

enum class program {
	ENCODE = 0,//圧縮
	DECODE	//解凍
};

class node {
public:
	char character = '\0';	//文字。文字のないノードは'\0'
	int nodeNum = 0;	//このノードの要素数。
	node* next = nullptr;		//ノードリストの、次のノードポインタ
	node* prev= nullptr;		//ノードリストの、前のノードポインタ
	node* parent= nullptr;	//ハフマン木の親のノードポインタ
	node* left= nullptr;		//ハフマン木の左の子のノードポインタ
	node* right= nullptr;	//ハフマン木の右の子のノードポインタ

};

//プロトタイプ宣言
node* createNodeList(FILE* fp);
node* searchNode(node* listStart, char character);
node* createNode(node* listStart, char character);
node* createHaffmanTree(node* listStart);
node* sortNodeList(node* listStart);
node* selectNode(node* listStart, int freq);
char* createHaffmanCodeList(node* treeHead);
char* createEncodeFile(char* path);
bool checkHaffmanCodeList(char* path);
char* createDecodeFile(FILE* fp, char* path);
