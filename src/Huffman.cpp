#include "Huffman.h"

void HuffmanCompressor::compressFile (std::string fileIn, std::string fileOut) {
	std::string text = readFile (fileIn);

	/* Construir a tabela de frequências */

	int frequencies[UNIQUE_SYMBOLS] = {0};

	
	const char* ptr = text.c_str();

	while (*ptr != '\0')
		++frequencies[*ptr ++];

	INode* root = BuildTree (frequencies);

	HuffCodeMap codes;
	GenerateCodes(root, HuffCode(), codes);
	delete root;

	for (HuffCodeMap::const_iterator it = codes.begin(); it != codes.end(); ++it)
    {
        std::cout << it->first << " ";
        std::copy(it->second.begin(), it->second.end(),
                  std::ostream_iterator<bool>(std::cout));
        std::cout << std::endl;
    }
    return;
}

std::string HuffmanCompressor::readFile (std::string fileIn) {
	std::ifstream in (fileIn, std::ios::in | std::ios::binary);

	if (in) {
		std::string contents;
		in.seekg(0,std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0,std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw (errno);
}

INode * HuffmanCompressor::BuildTree (const int (&frequencies) [UNIQUE_SYMBOLS]) {

	std::priority_queue <INode*, std::vector<INode*>, NodeCmp> trees;

	for (int i = 0; i < UNIQUE_SYMBOLS; ++i) {
		if (frequencies[i] != 0)
			trees.push(new LeafNode (frequencies[i], (char) i));
	}

	while (trees.size() > 1) {
		INode * rightChild = trees.top();
		trees.pop();

		INode* leftChild = trees.top();
		trees.pop();

		INode* parent = new InternalNode(rightChild, leftChild);
		trees.push(parent);
	}

	return trees.top();
}

void HuffmanCompressor::GenerateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap& outCodes) {
	if (const LeafNode* lf = dynamic_cast <const LeafNode*> (node))
		outCodes[lf->c] = prefix;

	else if (const InternalNode* in = dynamic_cast <const InternalNode*> (node)) {
		HuffCode leftPrefix = prefix;
		leftPrefix.push_back(false);
		GenerateCodes (in->left, leftPrefix, outCodes);

		HuffCode rightPrefix = prefix;
		rightPrefix.push_back(true);
		GenerateCodes (in->right, rightPrefix, outCodes);
	}
}