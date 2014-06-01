#pragma once

#include "BitBuffer.h"

#include <stdio.h>
#include <math.h>
#include <string>
#include <queue>
#include <map>
#include <iostream>
#include <iterator>
#include <vector>

#define len(x) ((int) log10((float) x) + 1)
#define UNIQUE_SYMBOLS 255

typedef struct node Node;

/* Node with Frequency */
class INode {
public:
	const int freq;

	virtual ~INode() {}

protected:
	INode(int freq) : freq(freq) {}
};

/* Node with Pointers to Left and Right Nodes */
class InternalNode : public INode {
public:
	INode * const left;
	INode * const right;

	InternalNode (INode * c0, INode * c1) : INode (c0->freq + c1->freq), left(c0), right(c1) {}
	
	~InternalNode () {
		delete left;
		delete right;
	}
};

/* Node with Char */
class LeafNode : public INode
{
public:
	const char c;

	LeafNode (int freq, char c) : INode (freq), c(c) {}
};

/* Struct to compare two Nodes, Node1.freq > Node2.freq */
struct NodeCmp
{
	bool operator() (const INode* lhs, const INode* rhs) const { return lhs->freq > rhs->freq; }
};

class HuffmanCompressor {

	/* A code is a vector of bools */
	typedef std:: vector<bool> HuffCode;
	/* The table of codes is a map with the corresponding char and HuffCode */
	typedef std:: map<char, HuffCode> HuffCodeMap;

	typedef std:: map<HuffCode, char> DecodingHuffCodeMap;

public:
	/* Function to return the smallest sub-tree */
	int findSmaller (Node *array[], int differentFrom);

	/* Create the conversion table */
	void fillTable (int codeTable[], Node* tree, int Code);

	void compressFile (const std::string& fileIn, const std::string& fileOut);

	void decompressFile (const std::string& fileIn, const std::string& fileOut);

	/* Generate the table of codes based on the Huffman Tree */
	void GenerateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap& outCodes);
	

private:	
	
	/* Based on the frequencies of the characters builds the Huffman tree */
	INode * BuildTree (const int (&frequencies) [UNIQUE_SYMBOLS]);
	
	/* Reads a file and returns a string with it's content */
	std::string readFile (std::string fileIn);

	std::string HuffCodeToString(HuffCode code);
	
	std:: vector<HuffCode> translateFromText(std::string text, HuffCodeMap code);

	/* Reads a vector of HuffCodes and writes it bit by bit to File */
	void writeFile (std::vector<HuffCode> text, std::ofstream& out, BitBuffer* bitbuffer);
};