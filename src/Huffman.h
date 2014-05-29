#pragma once

#include <stdio.h>
#include <math.h>
#include <string>
#include <fstream>
#include <queue>
#include <map>
#include <iostream>
#include <iterator>

#define len(x) ((int) log10((float) x) + 1)


typedef struct node Node;


class INode {
public:
	const int f;

	virtual ~INode() {}

protected:
	INode(int f) : f(f) {}
};

class InternalNode : public INode {
public:
	INode * const left;
	INode * const right;

	InternalNode (INode * c0, INode * c1) : INode (c0->f + c1->f), left(c0), right(c1) {}
	
	~InternalNode () {
		delete left;
		delete right;
	}
};

class LeafNode : public INode
{
public:
	const char c;

	LeafNode (int f, char c) : INode (f), c(c) {}
};

struct NodeCmp
{
	bool operator() (const INode* lhs, const INode* rhs) const { return lhs->f > rhs->f; }
};

class HuffmanCompressor {

	const int UNIQUE_SYMBOLS = 255;
	typedef std:: vector<bool> HuffCode;
	typedef std:: map<char, HuffCode> HuffCodeMap;



public:
	/* Function to return the smallest sub-tree */
	int findSmaller (Node *array[], int differentFrom);

	INode * BuildTree (const int (&frequencies) [UNIQUE_SYMBOLS]);
	/* Create the conversion table */
	void fillTable (int codeTable[], Node* tree, int Code);

	void compressFile (std::string fileIn, std::string fileOut);

	std::string readFile (std::string fileIn);

	void GenerateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap& outCodes);
}