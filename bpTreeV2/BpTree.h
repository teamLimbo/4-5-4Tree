//
//  BpTree.h
//  bpTreeV2
//
//  Created by Archit Sood on 2015-07-30.
//  Copyright (c) 2015 Archit Sood. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <array>
#include <map>
#include <algorithm>
using namespace std;

//=========================================TREENODE========================================
class treeNode{
    
public:
    
    treeNode(int numKeys_, treeNode* parent_, bool isLeaf_);
    void makeLeaf(treeNode* treeNode_);
    bool isFull(treeNode* treeNode_);
    
    
    int numKeys;            // Size of the node i.e number of possible keys
    treeNode** pointers;	// Poniter to the array of pointer
    int* keys;              // Points to the array of keys
//  string* value;          // Points the value associated with the key
    treeNode* parent;		// Pointer to the parent node (null if root)
    bool isLeaf;            // Checks if the current node is a leaf
    int counter;            // Keeps count of number of keys inserted in the node
    
};

//=========================================DATABASE========================================


class dataBase{
    
public:
    
    dataBase(int n);
    void dataBaseInsert(int key_, string value_, dataBase* record);
    
    map <int, string> recordDatabase;
    
};

//=========================================BPTREE========================================

class BpTree{

public:

    //Main functions
    BpTree(int n);
	void insert(int n, string s);
	void remove(int n);
	void find(int n);
	void printKeys();
	void printValues();
    
    //Helper fuctions
    void splitNode(treeNode* node);
    treeNode* findRoot(treeNode* node);
    treeNode* findLeaf(treeNode* node);
    
    
    //Class variables
	treeNode* root;
    dataBase* records;
	int keys_per_node;


	//TODO: put recursive helper functions
};