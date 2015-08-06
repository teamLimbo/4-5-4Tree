//
//  BpTree.h
//  bpTreeV2
//
//  Created by Archit Sood (301188293) and Daniel Soheili (301163609) on 2015-07-30.
//  Copyright (c) 2015 Archit Sood. All rights reserved.
//

/*
==========================================READ-ME==========================================

Instructions to compile:
1. Go the desired folder where the current header and cpp files are stored
2. Type : "g++ BpTree.cpp -std=c++11" to compile if the main is inserted in .cpp file
3. Or Type: "g++ *.cpp -std=c++11" if a different main file is used.
4. Run "./a.out" to execute.                                    

===========================================================================================
*/


#ifndef BPTREE_H
#define BPTREE_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <array>
#include <map>
#include <algorithm>
#include <queue>

using namespace std;

//=========================================TREENODE========================================
class treeNode{
    
public:
    
    treeNode(int numKeys_, treeNode* parent_, bool isLeaf_);    //constructor
    void makeLeaf(treeNode* treeNode_);                         //makes node object leaf
    bool isFull(treeNode* treeNode_);                           //Checks if the node is full
    ~treeNode();                                                //destructor
    
    int numKeys;            // Size of the node i.e number of possible keys
    treeNode** pointers;	// Poniter to the array of pointer
    int* keys;              // Points to the array of keys
    treeNode* parent;		// Pointer to the parent node (null if root)
    bool isLeaf;            // Checks if the current node is a leaf
    int counter;            // Keeps count of number of keys inserted in the node
    
};

//=========================================DATABASE========================================


class dataBase{
    
public:
    
    dataBase(int n);
    void dataBaseInsert(int key_, string value_, dataBase* record);
    ~dataBase();
    
    map <int, string> recordDatabase;  //Keeps track of key-value pairs
    
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
    ~BpTree();
    
    //Helper fuctions
    void splitNode(treeNode* node);                 //Splits the given node
    treeNode* findRoot(treeNode* node);             //Finds the root of the tree
    treeNode* findLeaf(treeNode* node, int n);      //Finds the desirable leaf node to insert key
    
    //Class variables
	treeNode* root;     //Node type
    dataBase* records;  //dataBase type, Keeps track of key-value pairs
	int keys_per_node;  //Maximum number of keys per node


};
#endif
