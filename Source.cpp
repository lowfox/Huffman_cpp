#pragma once
#include"Header.h"


int main(int argc, char *argv[]) {

	//���k�������͉𓀃f�[�^���́B
	if (argc != 2) {
		std::cout << "�����ɑΏۃt�@�C���̂���p�X������͂��Ă��������B\n";
		return false;
	}

	FILE* fp = fopen(argv[1], "rb");
	if (fp == NULL) {
		std::cout << "fopen�G���[\n";
	}


	int selectProgram = 0;													//���[�U�[�I��
	node* listStart = nullptr;												//�m�[�h���X�g�̐擪�|�C���^
	node* treeHead = nullptr;													//2���؂̍��̃|�C���^
	char* haffmanCodeListPath = nullptr;										//�n�t�}�������ꗗ�\�̃t�@�C���p�X
	char* encodeFilePath = nullptr;											//���k�t�@�C���̃t�@�C���p�X
	char* decodeFilePath = nullptr;											//�𓀃t�@�C���̃t�@�C���p�X

	while (1) {
		//���k���𓀑I��
		std::cout << "���k(0)/��(1)�H =>";
		std::cin >> selectProgram;
		if (selectProgram == static_cast<int>(program::ENCODE)) {
			/* ----------���k����---------- */
			listStart = createNodeList(fp);									//�t�@�C���f�[�^��ǂ݂��݁A�m�[�h���X�g���쐬�B
			treeHead = createHaffmanTree(listStart);						//�쐬�����m�[�h���X�g����n�t�}���؂��쐬
			haffmanCodeListPath = createHaffmanCodeList(treeHead);			//�쐬�����n�t�}���؂���n�t�}�������ꗗ�\���쐬
			encodeFilePath = createEncodeFile(haffmanCodeListPath);			//���k�����s�B�V�K�t�@�C���ɏo��
			break;
		}
		else if (selectProgram == static_cast<int>(program::DECODE)) {
			/* ----------�𓀏���---------- */
			if (checkHaffmanCodeList(haffmanCodeListPath)) {				//�n�t�}�������ꗗ�\�����������邩�`�F�b�N
				decodeFilePath = createDecodeFile(fp, haffmanCodeListPath);	//�𓀂����s�B�V�K�t�@�C���ɏo��
			}
			else {
				return false;
			}
			break;
		}
		else {
			std::cout << "���̓G���[�ł�\n";
			continue;
		}
	}


	return true;
}


/* ----------���k---------- */
/**
* �m�[�h���X�g�̍쐬
* @param fp ���k�Ώۃt�@�C���̃t�@�C���|�C���^
* @return �m�[�h���X�g�̐擪�|�C���^
*/
node* createNodeList(FILE* fp) {
	int fileLen = 0;
	node* newNode = nullptr;
	node* findNode = nullptr;
	node* listStart = nullptr;
	node* listEnd = nullptr;

	int character;

	while (character = fgetc(fp) != EOF) {
		//���X�g�T��
		findNode = searchNode(listStart, static_cast<char>(character));
		if ( findNode == nullptr) {
			//�m�[�h�����X�g�ɐV�K�o�^
			newNode = createNode(listStart, static_cast<char>(character));
			if (listStart == nullptr) {
				//�m�[�h���X�g�ɉ��������ĂȂ����i�ŏ��̓o�^�j�̃��X�g�ǉ��B
				listStart = newNode;
				listEnd = newNode;
				listEnd->next = nullptr;
			}
			else {
				//�m�[�h���X�g�ɒ��g�͂��邯�ǁA�����Ɉ���������Ȃ��������i�m�[�h��V�K�o�^���j�̃��X�g�ǉ��B
				listEnd = listStart;
				while (listEnd->next != nullptr) {
					listEnd = listEnd->next;
				}
				listEnd->next = newNode;
				newNode->prev = listEnd;
			}
		}
		else {
			//�����Ɉ������������̂ł��̃m�[�h�����{1
			findNode->nodeNum++;
		}
	}
	fclose(fp);
	return listStart;
}

/**
* �m�[�h�̒T��
* @param character �T�����镶��
* @return �������m�[�h�̃|�C���^
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
* �m�[�h�̍쐬
* @param character �m�[�h�ɓo�^���镶��
* @return �m�[�h�̃|�C���^
*/
node* createNode(node* listStart, char character) {
	node* newNode = new node;
	newNode->character = character;
	newNode->nodeNum++;
	return newNode;

}


/**
* �n�t�}���؂̍쐬
* @param listStart �m�[�h���X�g�̐擪�|�C���^
* @return �n�t�}���؂̍��|�C���^ 
*/
node* createHaffmanTree(node* listStart) {

	listStart = sortNodeList(listStart);	//�m�[�h���X�g�̃\�[�g�i���������j
	node* haffman = listStart;//�n�t�}���؂�����Ă������߂ɁA���Ȃ��m�[�h����傫���m�[�h�ֈړ����Ă����|�C���^
	node* noCharNode;//��������m�[�h�̐e�ɂȂ�m�[�h
	node* firstNode=nullptr;
	node* secondNode = nullptr;
	int charNodesNum=0;//�m�[�h���X�g�̃m�[�h�̌�

	//�����Ƀm�[�h���𐔂��鏈��������




	for (int i = 1; i <= (charNodesNum-1)*2; charNodesNum++) {
		noCharNode = createNode(listStart, '\0');
		if (i % 2 == 1) {
			//�p�[�g�i�[�i���̍��j��I��
			//�
			firstNode = selectNode(listStart, i);
		}
		else {
			//����
			//�p�[�g�i�[�i���̉E�j��I��
			secondNode = selectNode(listStart, i);
			
			//���p�[�g�i�[�̐e��noCharNode������
			firstNode->parent = noCharNode;
			secondNode->parent = noCharNode;

			//���p�[�g�i�[���r�B�e�̍��E�ɓ����B
			if (firstNode->nodeNum < secondNode->nodeNum) {
				noCharNode->left = firstNode;
				noCharNode->right = secondNode;
			}
			else {
				noCharNode->right = firstNode;
				noCharNode->left = secondNode;
			}
			//createNode��+1���Ă�̂�-1
			noCharNode->nodeNum = (noCharNode->left->nodeNum) + (noCharNode->right->nodeNum) - 1;
		}
	}

	return nullptr;
}

/**
* �m�[�h���X�g�̃\�[�g�B�o���񐔂̏��������Ƀ\�[�g����B
* @param listStart �m�[�h���X�g�̐擪�|�C���^
* @return �m�[�h���X�g�̐擪�|�C���^
*/
node* sortNodeList(node* listStart) {
	
	return nullptr;
}

/**
* 2���؂��쐬���邽�߂̌��ƂȂ�m�[�h��I�����Ă����B
* @param listStart �m�[�h���X�g�̐擪�|�C���^
* @param freq ��������m�[�h�����������ɕ��ׂ��Ƃ��̏��ԍ��B
* @return �I�����ꂽ�m�[�h�̃|�C���^
*/
node* selectNode(node* listStart, int freq) {
	static node* moveList_ch = listStart;//�����̂���m�[�h�𓮂�
	node* retNode;
	if (freq % 2 == 1 && freq != 1) {
		//freq�����1�ȊO�̎��͕����̂Ȃ��m�[�h��I��//3,5,7...
		retNode = moveList_ch->prev->parent;
	}
	else {
		//freq���P�Ƌ����̂Ƃ��́A�����̂���m�[�h�����������ɑI��//1,2,4,6...
		retNode = moveList_ch;
	}
	moveList_ch = moveList_ch->next;
	return retNode;
}



/**
* ��������m�[�h�̃n�t�}�������ꗗ�\���쐬
* @param treeHead �n�t�}���؂̍��|�C���^
* @return �ꗗ�\�̃t�@�C���p�X
*/
char* createHaffmanCodeList(node* treeHead) {
	return nullptr;
}

/**
* ���k�t�@�C���̍쐬
* @param fp ���k�Ώۃt�@�C���̃t�@�C���|�C���^
* @param path �ꗗ�\�̃t�@�C���p�X 
* @return ���k�t�@�C���̃t�@�C���p�X
*/
char* createEncodeFile(char* path) {
	return nullptr;
}


/* ----------��----------- */
/**
* �n�t�}�������ꗗ�\�����������邩�`�F�b�N
* @param path �ꗗ�\�̃t�@�C���p�X
* @return true ������
* @return false �������Ȃ�
*/
bool checkHaffmanCodeList(char* path) {

	return true;
	std::cout << "�n�t�}�������ꗗ�\�������������͓��e������������܂���B\n";
	std::cout << "FILEPATH�F%s\n" << path;
	
}

/**
* �𓀃t�@�C���̍쐬
* @param fp �𓀑Ώۃt�@�C���̃t�@�C���|�C���^
* @param path �ꗗ�\�̃t�@�C���p�X
* @return �𓀃t�@�C���̃t�@�C���p�X
*/
char* createDecodeFile(FILE* fp, char* path) {
	return nullptr;
}



