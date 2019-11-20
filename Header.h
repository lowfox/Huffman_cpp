#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

enum class program {
	ENCODE = 0,//���k
	DECODE	//��
};

class node {
public:
	char character = '\0';	//�����B�����̂Ȃ��m�[�h��'\0'
	int nodeNum = 0;	//���̃m�[�h�̗v�f���B
	node* next = nullptr;		//�m�[�h���X�g�́A���̃m�[�h�|�C���^
	node* prev= nullptr;		//�m�[�h���X�g�́A�O�̃m�[�h�|�C���^
	node* parent= nullptr;	//�n�t�}���؂̐e�̃m�[�h�|�C���^
	node* left= nullptr;		//�n�t�}���؂̍��̎q�̃m�[�h�|�C���^
	node* right= nullptr;	//�n�t�}���؂̉E�̎q�̃m�[�h�|�C���^

};

//�v���g�^�C�v�錾
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
