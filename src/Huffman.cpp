#include "Huffman.h"

using namespace std;

int HuffmanCompressor::findSmaller (Node *array[], int differentFrom) {
	int smaller;
	int i = 0;

	while (array[i]->value == -1)
		i++;
	smaller = i;

	if (i == differentFrom) {
		i++;

		while (array[i]->value == -1)
			i++;
		smaller = i;
	}

	for (i=0; i < 128; i++) {
		if (array[i]->value == -1)
			continue;
		if (i==differentFrom)
			continue;
		if (array[i]->value < array[smaller]->value)
			smaller = i;
	}

	return smaller;
}

void HuffmanCompressor::buildHuffmanTree (Node **tree) {
	Node *temp;
	Node *array[128];

	int i, subTrees = 255;
	int treeOne, treeTwo;

	/* Inicializar o array de Nós */
	for (i=0; i<128; i++) {
		array[i] = new Node;
		array[i]->value = englishLetterFrequencies[i];
		array[i]->letter = (char) i;
		array[i]->left = NULL;
		array[i]->right = NULL;
	}

	/* Enquanto houver várias àrvores */
	while (subTrees > 1) {
		/* Encontrar */
		treeOne = findSmaller(array, -1);
		treeTwo = findSmaller(array, treeOne);

		temp = array[treeOne];

		array[treeOne] = new Node;
		array[treeOne]->value = temp->value + array[treeTwo]->value;
		array[treeOne]->letter = (char) 127;
		array[treeOne]->left = array[treeTwo];
		array[treeTwo]->value = -1;
		subTrees--;
	}

}

void HuffmanCompressor::fillTable (int codeTable[], Node* tree, int Code) {

	if (tree->letter < 27)
		codeTable[(int)tree->letter] = Code;
	else {
		fillTable(codeTable, tree->left, Code*10 +1);
		fillTable(codeTable, tree->right, Code*10 +2);
	}

	return;
}

void HuffmanCompressor::compressFile (std::string inFile, std::string outFile, int codeTable[]) {
		char bit, c, x = 0;

		std::ifstream fin = std::ifstream(inFile, ios::in);

		int n, length, bitsLeft = 8;
		int originalBits = 0, compressedBits = 0;

		while ((c = fin.get())!=10) {
			originalBits++;

			if (c==32) {
				length = len(codeTable[26]);
				n = codeTable[26];
			}
			else {
				length = len(codeTable[c-97]);
				n = codeTable[c-97];
			}

			while (length > 0) {
				compressedBits ++;
				bit = n % 10 - 1;
				n /= 10;
				x = x | bit;
				bitsLeft--;
				length--;

				if (bitsLeft == 0) {
					fputc(x,output);
					x = 0;
					bitsLeft = 8;
				}

				x = x << 1;
			}
		}

		if (bitsLeft != 8) {
			x = x << (bitsLeft - 1);
			fputc(x,output);
		}

		/* PRINT */
		cout << "Original bits = " << originalBits * 8 << endl;
		cout << "Compressed bits = " << compressedBits << endl;
		cout << "Compression = " << originalBits * 100 / compressedBits << endl;



	}

