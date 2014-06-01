#include "Huffman.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void HuffmanCompressor::compressFile (string fileIn, string fileOut) {
	string text = readFile (fileIn);

	/* Build a frequency table */
	int frequencies[UNIQUE_SYMBOLS] = {0};

	const char* ptr = text.c_str();

	while (*ptr != '\0')
		++frequencies[*ptr ++];

	/* Build the Huffman's tree */
	INode* root = BuildTree (frequencies);

	/* Instantiate the new Table of codes */
	HuffCodeMap codes;
	/* Generate the codes for the Table */
	GenerateCodes(root, HuffCode(), codes);


	ofstream fout = ofstream(fileOut, ios::out);

	for (HuffCodeMap::const_iterator it = codes.begin(); it != codes.end(); ++it)
	{
		/* Print the table to the console */
		cout << it->first << " ";
		copy(it->second.begin(), it->second.end(),
			ostream_iterator<bool>(cout));
		cout << endl;

		/* Print the table to the file */

		fout << it->first << "\0";
		copy(it->second.begin(), it->second.end(),
			ostream_iterator<bool>(fout));
		fout << "\1";

	}


	fout << "\2";

	//	fout << "\nINICIO DE TEXTO COMPRIMIDO\n";
	
	vector<HuffCode> translatedText = translateFromText(text, codes);

	/* Count the number of bits needed for the compressed text */

	unsigned long count = 0;

	for (unsigned int i = 0; i < translatedText.size(); i++) {
		count += translatedText[i].size();
		//fout << HuffCodeToString(translatedText[i]) << "\n";
	}

	//fout << "Count = " << count << "\n";

	bitbuffer.initialize(count);

	writeFile (translatedText, fout);
	delete root;
	return;
}

void HuffmanCompressor::decompressFile (std::string fileIn, std::string fileOut) {

	ifstream fin = ifstream(fileIn, ios::in);

	DecodingHuffCodeMap codes;
	char currentChar;

	while (fin.good()) {
		currentChar = fin.get();

		// Check if the table has ended
		if (currentChar == '\2')
			break;

		// Create a new Code
		HuffCode currentCode;
		char currentInput = currentChar;
		while (currentInput != '\1') {
			currentInput = fin.get();

			if (currentInput == '0') {
				currentCode.push_back(false);
			} else if (currentInput == '1') {
				currentCode.push_back(true);
			}

		}

		codes.insert(std::pair<HuffCode, char> (currentCode, currentChar));
	}

	cout << "Reading done!" << endl;

	for (DecodingHuffCodeMap::const_iterator it = codes.begin(); it != codes.end(); ++it)
	{
		/* Print the table to the console */
		cout << it->second << " ";
		copy(it->first.begin(), it->first.end(),
			ostream_iterator<bool>(cout));
		cout << endl;
	}

	BitBuffer* bitBuffer = new BitBuffer();
	bool bit;

	// Create a new Code
	HuffCode currentCode;
	string text;

	DecodingHuffCodeMap::iterator it;

	while(!bitbuffer.eof())
	{
		bit = (bool) bitbuffer.get(1);
		currentCode.push_back(bit);

		it = codes.find(currentCode);
		if (it != codes.end()) {
			//currentChar = codes.at(currentCode);
			text.append(1,it->second);
		}
	}

	cout << text;
}

string HuffmanCompressor::readFile (string fileIn) {
	ifstream in (fileIn, ios::in | ios::binary);

	if (in) {
		string contents;
		in.seekg(0,ios::end);
		contents.resize(in.tellg());
		in.seekg(0,ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw (errno);
}

INode * HuffmanCompressor::BuildTree (const int (&frequencies) [UNIQUE_SYMBOLS]) {

	/* Creates a priority queue of Nodes, using NodeCmp to compare */
	priority_queue <INode*, vector<INode*>, NodeCmp> trees;

	/* Initializes the forest of trees with the frequencies given */
	for (int i = 0; i < UNIQUE_SYMBOLS; ++i) {
		if (frequencies[i] != 0)
			trees.push(new LeafNode (frequencies[i], (char) i));
	}

	/* While there is more than one tree */
	while (trees.size() > 1) {

		/* Select the two nodes with smallest frequencies wich are located at the top of the priority queue */
		INode * rightChild = trees.top();
		trees.pop();

		INode* leftChild = trees.top();
		trees.pop();

		/* Creates a new Node wich is the parent of the two smallest nodes */
		INode* parent = new InternalNode(rightChild, leftChild);

		/* Add the new Node to the forest */
		trees.push(parent);
	}

	/* Returns the complete tree */
	return trees.top();
}

void HuffmanCompressor::GenerateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap& outCodes) {

	/* Prefix starts null */
	if (const LeafNode* lf = dynamic_cast <const LeafNode*> (node))
		/* Sets the code of the actual node to the actual prefix */
		outCodes[lf->c] = prefix;

	else if (const InternalNode* in = dynamic_cast <const InternalNode*> (node)) {
		/* Create a new prefix to treat the left Node, equal to the actual prefix */
		HuffCode leftPrefix = prefix;
		/* Add a 0 to the prefix */
		leftPrefix.push_back(false);
		/* Recursively call this function for the left Node */
		GenerateCodes (in->left, leftPrefix, outCodes);

		/* Create a new prefix to treat the right Node, equal to the actual prefix */
		HuffCode rightPrefix = prefix;
		/* Add a 1 to the prefix */
		rightPrefix.push_back(true);
		/* Recursively call this function for the right Node */
		GenerateCodes (in->right, rightPrefix, outCodes);
	}
}

std::vector<HuffmanCompressor::HuffCode> HuffmanCompressor::translateFromText(std::string text, HuffCodeMap code) {

	vector<HuffCode> translatedText;

	const char* c = text.c_str();

	for (unsigned int i = 0; i < text.size(); i++) {
		translatedText.push_back(code.at(c[i]));
	}

	return translatedText;
}

std::string HuffmanCompressor::HuffCodeToString(HuffCode code) {
	stringstream ss;

	for (unsigned int i = 0; i < code.size(); i++) {
		if (code[i])
			ss << "1";
		else
			ss << "0";
	}

	return ss.str();
}

void HuffmanCompressor::writeFile (std::vector<HuffCode> text, std::ofstream& out) {


	for (unsigned int i = 0; i < text.size(); i++) {

		for (unsigned int j = 0; j < text[i].size(); j++) {

			bitbuffer.add(text[i][j], 1);

		}
	}

	bitbuffer.write(out);

	out.close();

}