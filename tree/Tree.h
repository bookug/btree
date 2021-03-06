/*=============================================================================
# Filename: Tree.h
# Author: syzz
# Mail: 1181955272@qq.com
# Last Modified: 2015-04-26 16:44
# Description: struct and interface of the B+ tree
=============================================================================*/

#ifndef _TREE_H
#define _TREE_H

#include "../util/Util.h"
#include "../node/Node.h"
#include "../node/IntlNode.h"
#include "../node/LeafNode.h"
#include "../storage/Storage.h"
#include "../rangevalue/RangeValue.h"

class Tree
{					
private:
	unsigned int height;		//0 indicates an empty tree
	Node* root;
	Node* leaves_head;			//the head of LeafNode-list
	Node* leaves_tail;			//the tail of LeafNode-list
	std::string mode;           //BETTER(to use enum)
	Storage* TSM;           	//Tree-Storage-Manage
	RangeValue VALUES;			//just for range query
	//always alloc one more byte than length, then user can add a '\0'
	//to get a real string, instead of new and copy
	//other operations will be harmful to search, so store value in
	//transfer temporally, while length adjusted.
	//TODO: in multi-user case, multiple-search will cause problem,
	//so lock is a must. Add lock to transfer is better than to add 
	//lock to every key/value. However, modify requires a lock for a
	//key/value, and multiple search for different keys are ok!!!
	TBstr transfer[3];	//0:transfer value searched; 1:copy key-data from const char*; 2:copy val-data from const char*
	unsigned transfer_size[3];
	std::string storepath;
	std::string filename;      	//ok for user to change
	/* some private functions */
	std::string getFilePath();	//in UNIX system
	void CopyToTransfer(const char* _str, unsigned _len, unsigned _index);
	void release(Node* _np) const;
	void prepare(Node* _np) const;
public:
	Tree();				//always need to initial transfer
	Tree(const std::string& _storepath, const std::string& _filename, const char* _mode);
	unsigned int getHeight() const;
	void setHeight(unsigned _h);
	Node* getRoot() const;
	//void setRoot(Node* _root);
	//insert, search, remove, set
	bool search(const char* _str1, unsigned _len1, char*& _str2, int& _len2);
	bool search(const TBstr* _key1, const TBstr*& _value);
	bool insert(const TBstr* _key, const TBstr* _value);
	bool insert(const char* _str1, unsigned _len1, const char* _str2, unsigned _len2);
	bool modify(const TBstr* _key, const TBstr* _value);
	bool modify(const char* _str1, unsigned _len1, const char* _str2, unsigned _len2);
	Node* find(const TBstr* _key, int* store, bool ifmodify) const;
	//Node* find(unsigned _len, const char* _str, int* store) const;
	bool remove(const TBstr* _key);
	bool remove(const char* _str, unsigned _len);
	const TBstr* getRangeValue();
	bool range_query(const TBstr* _key1, const TBstr* _key2);
	bool save(); 			
	~Tree();
	void print(std::string s);			//DEBUG(print the tree)
};
//NOTICE: need to save tree manually before delete, otherwise will cause problem. 
//(problem range between two extremes: not-modified, totally-modified)
//After saved, it's ok to continue operations on tree!

#endif

