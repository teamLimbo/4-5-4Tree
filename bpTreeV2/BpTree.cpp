//
//  BpTree.cpp
//  bpTreeV2
//
//  Created by Archit Sood on 2015-07-30.
//  Copyright (c) 2015 Archit Sood. All rights reserved.
//

#include "BpTree.h"


//=========================================TREENODE========================================

treeNode::treeNode(int numKeys_, treeNode* parent_, bool isLeaf_)
{
    numKeys = numKeys_;
    parent = parent_;
    isLeaf = isLeaf_;
    keys = new int[numKeys_];
    pointers = new treeNode*[numKeys_ + 1];
    counter = 0;
    
}

void treeNode::makeLeaf(treeNode* treeNode_){
    
    if (treeNode_->isLeaf == false){
        treeNode_->isLeaf = true;
    }
    
}

bool treeNode::isFull(treeNode* treeNode_){
    
    if ((treeNode_->numKeys) <= (treeNode_->counter)){
        return true;
    }
    
    return false;
}

//=========================================DATABASE========================================


dataBase::dataBase(int n){}

void dataBase::dataBaseInsert(int key_, string value_, dataBase* record){
    
    record->recordDatabase[key_] = value_;
}

//=========================================BPTREE========================================


BpTree::BpTree(int n){
    
    this->root = new treeNode(n, NULL, true);
    this->records = new dataBase(n);
    this->keys_per_node = n;
}


void BpTree::insert(int n, string s){
    
    //Simple insertion into a leaf thats not full=========================
    
    if ((root->isFull(root) == false) && (root->isLeaf == true)){
        
        root->keys[root->counter] = n;
        root->counter++;
        records->dataBaseInsert(n, s, records);
    }
    
    sort(root->keys,root->keys+root->counter); //This fucntion sorts the node upto inserted elements everytime when the new value is inserted
    
    //Insertion when the leaf node is full=====================================
    if ((root->isFull(root) == true)){

        splitNode(root);
    
    }

}

void BpTree::remove(int n){}

void BpTree::find(int n){
   
    if (records->recordDatabase.find(n) == records->recordDatabase.end()){
        cout<<"This key does not exist"<<endl;
    }
    else{
        cout<<records->recordDatabase[n]<<endl;
    }
}

void BpTree::printKeys(){}

void BpTree::printValues(){
    
    for (map<int,string>::iterator it = records->recordDatabase.begin(); it!=records->recordDatabase.end(); ++it){
        cout << it->first << " => " << it->second << '\n';
    }
}

void BpTree::splitNode(treeNode *node){
    
    //Splitling up a leaf
    
    if (node->isLeaf == true){
    
        treeNode* newNode = new treeNode(keys_per_node, node, true);
    
        int i;
        if (keys_per_node%2 == 0){
            i = keys_per_node/2;
        }
        else{
            i = keys_per_node/2+1;
        }
    
        //Put the split values in the new node.
        for (int y = i; y <= keys_per_node; y++) {
            newNode->keys[y-i] = node->keys[y];
        }
    
        newNode->counter = i-1;             //Keep track of the new insertion point for the new node

        //The orginal node keeps the first i values intact but the counter
        //(place where new value is inserted) moves.
        node->counter = i;
        
        //Linking up the pointers
        node->pointers[i] = newNode;
        node->parent->pointers[node->parent->counter]=newNode;
        newNode->parent = node->parent;
        
        //update the values in parent node
        node->parent->keys[node->parent->counter - 1] = newNode->keys[0];

    }
    
   // Splitting up an interior node (non-leaf)
    
    if(node->isLeaf == false){
    
    
    }

    
    
}


