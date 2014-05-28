#pragma once

#include <stdio.h>
#include <math.h>
#include <string>
#include <fstream>

#define len(x) ((int) log10((float) x) + 1)

/* Structure of the node */
struct node {
	int value;
	char letter;
	struct node *left, *right;
};

typedef struct node Node;

int englishLetterFrequencies [128] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, 15, 28, 43, 128, 23, 20, 61, 71, 2, 1, 40, 24, 69, 76, 20, 1, 61, 64, 91, 28, 10, 24, 1, 20, 1, 130, 0, 0, 0, 0, 0, 0, 81, 15, 28, 43, 128, 23, 20, 61, 71, 2, 1, 40, 24, 69, 76, 20, 1, 61, 64, 91, 28, 10, 24, 1, 20, 1, 130, 0, 0, 0, 0, 0};

int frequencies [128];

class HuffmanCompressor {
public:
	/* Function to return the smallest sub-tree */
	int findSmaller (Node *array[], int differentFrom);

	void buildHuffmanTree (Node **tree);
	
	/* Create the conversion table */
	void fillTable (int codeTable[], Node* tree, int Code);

	void compressFile (std::string fileIn, std::string fileOut, int codeTable[]);

}