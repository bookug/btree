/*=============================================================================
# Filename: Storage.h
# Author: syzz
# Mail: 1181955272@qq.com
# Last Modified: 2015-04-26 16:43
# Description: swap between memory and disk, achieving system-like method
=============================================================================*/

#ifndef _STORAGE_H
#define _STORAGE_H

#include "../util/Util.h"
#include "../node/Node.h"
#include "../node/IntlNode.h"
#include "../node/LeafNode.h"
#include "../heap/Heap.h"
#include "../bstr/TBstr.h"
#include "file.h"

//It controls read, write, swap
class Storage
{                    
public:
	static const unsigned BLOCK_SIZE = 1 << 16;	//fixed size of disk-block
	static const unsigned MAX_BUFFER_SIZE = 1 << 31;		//max buffer size 
	static const unsigned HEAP_SIZE = MAX_BUFFER_SIZE/Node::INTL_SIZE;
	static const unsigned MAX_BLOCK_NUM = 1 << 24;		//max block-num
	//below two constants: must can be exactly divided by 8
	static const unsigned SET_BLOCK_NUM = 1 << 8;		//initial blocks num
	static const unsigned SET_BLOCK_INC = SET_BLOCK_NUM;	//base of blocks-num inc
	static const unsigned SuperNum = MAX_BLOCK_NUM/(8*BLOCK_SIZE)+1;
	//static const unsigned TRANSFER_CAPACITY = BLOCK_SIZE;
	//enum ReadType { OVER = 0, EXPAND, NORMAL };
private:
	unsigned cur_block_num;
	std::string filepath;
	unsigned* treeheight;
	BlockInfo* freelist;
	FILE* treefp;				//file: tree nodes
	Heap* minheap;				//heap of Nodes's pointer, sorted in NF_RK
	unsigned freemem;  			//free memory to use, non-negative
	//is transfer and capacity necessary?!
	//TBstr* transfer;				//reduce new-operation
	//unsigned long long time; 		//QUERY(achieving an old-swap startegy?)
	long Address(unsigned _blocknum) const;
	unsigned Blocknum(long address) const;
	unsigned AllocBlock();
	void FreeBlock(unsigned _blocknum);
	void ReadAlign(unsigned* _next);
	void WriteAlign(unsigned* _next, bool& _SpecialBlock);

public:
	Storage();
	Storage(std::string& _filepath, std::string& _mode, unsigned* _height);//create a fixed-size file or open an existence
	bool preRead(Node*& _root, Node*& _leaves_head, Node*& _leaves_tail);		//read and build all nodes, only root in memory
	bool readNode(Node* _np, int* _request);	//read, if virtual 
	bool createNode(Node*& _np);		//use fp to create a new node
	//NOTICE(if children and child not exist, build children's Nodes)
	bool writeNode(Node* _np);
	bool readTBstr(TBstr* _bp, unsigned* _next);
	bool writeTBstr(const TBstr* _bp, unsigned* _curnum, bool& _SpecialBlock);
	bool writeTree(Node* _np);
	void updateHeap(Node* _np, unsigned _rank, bool _inheap) const;
	void request(int _needmem);	//deal with memory request
	bool handler(unsigned _needmem);	//swap some nodes out
	//Node* select();                         //select a node to release
	//bool update();				//update InMem Node's rank, with clock
	~Storage();	
	void print(std::string s);				//DEBUG
};

#endif
