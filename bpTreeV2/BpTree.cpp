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
    
    treeNode* dynamicRoot = new treeNode(keys_per_node, NULL, true);
    dynamicRoot = findRoot(root);

    
    //Simple insertion into a leaf thats not full=========================
    
    if ((dynamicRoot->isFull(dynamicRoot) == false) && (dynamicRoot->isLeaf == true)){
        
        dynamicRoot->keys[dynamicRoot->counter] = n;
        dynamicRoot->counter++;
        records->dataBaseInsert(n, s, records);
        sort(dynamicRoot->keys,dynamicRoot->keys+dynamicRoot->counter); //This fucntion sorts the node upto inserted elements everytime before a new value is inserted

    }
    
    
    //Insertion when the leaf node is full=================================
    else if ((dynamicRoot->isFull(dynamicRoot) == true) && (dynamicRoot->isLeaf == true)){

        splitNode(dynamicRoot);
        
        if(dynamicRoot->parent->keys[dynamicRoot->parent->counter - 1] < n){
            
            //This is the new node thats created during the split
            dynamicRoot->parent->pointers[dynamicRoot->parent->counter]->keys[dynamicRoot->parent->pointers[dynamicRoot->parent->counter]->counter] = n;
            dynamicRoot->parent->pointers[dynamicRoot->parent->counter]->counter++;
            records->dataBaseInsert(n, s, records);

        }
        else if (dynamicRoot->parent->keys[dynamicRoot->parent->counter - 1] == n){
            
            cout<<"Duplicate Key"<<endl;
            
        }else{
        
            dynamicRoot->keys[root->counter]=n;
            dynamicRoot->counter++;
            records->dataBaseInsert(n, s, records);
        }

    }
    
    if (dynamicRoot->isLeaf == false){
    
        for (int i=0; i < dynamicRoot->counter; i++){
            if (n < dynamicRoot->keys[i]){
                if(dynamicRoot->pointers[i]->isFull(dynamicRoot->pointers[i]) == false && dynamicRoot->pointers[i]->isLeaf == true){
                    dynamicRoot->pointers[i]->keys[dynamicRoot->pointers[i]->counter] = n;
                    dynamicRoot->pointers[i]->counter++;
                    sort(dynamicRoot->pointers[i]->keys,dynamicRoot->pointers[i]->keys+dynamicRoot->pointers[i]->counter);
                }
                
                if (dynamicRoot->pointers[i]->isLeaf == false){
                    
                    treeNode* currentNode = new treeNode(keys_per_node, NULL, false);
                    currentNode = dynamicRoot->pointers[i];
                    
                }
            }
            
            if (n > dynamicRoot->keys[i]){
                
                if(dynamicRoot->pointers[i+1]->isFull(dynamicRoot->pointers[i+1]) == false && dynamicRoot->pointers[i+1]->isLeaf == true){
                    dynamicRoot->pointers[i+1]->keys[dynamicRoot->pointers[i+1]->counter] = n;
                    dynamicRoot->pointers[i+1]->counter++;
                    sort(dynamicRoot->pointers[i+1]->keys,dynamicRoot->pointers[i+1]->keys+dynamicRoot->pointers[i+1]->counter);
                }

            }
        }
    
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

void BpTree::printKeys(){
    
    treeNode* dynamicRoot = new treeNode(keys_per_node, NULL, true);
    
    dynamicRoot = findRoot(root);
    
    treeNode* currentLevel = new treeNode(keys_per_node, NULL, false);
    currentLevel = dynamicRoot;
    
    for (int i=0; i < currentLevel->counter; i++){
        cout<<currentLevel->keys[i]<<", ";
    }
    cout<<endl;
    

    for (int i=0; i < currentLevel->pointers[0]->counter; i++){
        cout<<currentLevel->pointers[0]->keys[i]<<", ";
    }
    cout<<"   ";
    
    for (int i=0; i < currentLevel->pointers[1]->counter; i++){
        cout<<currentLevel->pointers[1]->keys[i]<<", ";
    }
    
    


        
    

}

void BpTree::printValues(){
    
    for (map<int,string>::iterator it = records->recordDatabase.begin(); it!=records->recordDatabase.end(); ++it){
        cout << it->first << " => " << it->second << '\n';
    }
}

void BpTree::splitNode(treeNode *node){
    
    
    //Splitling up a leaf
    treeNode* newNode = new treeNode(keys_per_node, NULL, true);

    if (node->isLeaf == true){

        int i;
        
        if (keys_per_node%2 == 0){
            i = keys_per_node/2;
        }
        else{
            i = keys_per_node/2 + 1;
        }
    
        //Distributing the split values in the new node.
        for (int y = i; y < keys_per_node; y++) {
            newNode->keys[y-i] = node->keys[y];
        }
    
        newNode->counter = i-1;     //Keep track of the new insertion point for the new node

        //The orginal node keeps the first i values intact but the counter
        //(place where new value is inserted) moves.
        node->counter = i;
        
        if (node->parent == NULL){

            //This happens when a new root is created
            treeNode* newRoot = new treeNode(keys_per_node, NULL, false);
            node->parent = newRoot;
        }
        
        //Linking up the pointers
        node->pointers[keys_per_node] = newNode;
        node->parent->pointers[node->parent->counter] = node;
        node->parent->pointers[node->parent->counter+1]=newNode;
        newNode->parent = node->parent;
        
        //update the values in parent node
        node->parent->keys[node->parent->counter] = newNode->keys[0];
        node->parent->counter++;
        

    }
    
   // Splitting up an interior node (non-leaf)
    
    if(node->isLeaf == false){
        
        newNode->isLeaf = false;
        
        int pointerIndex;
        
        if ((keys_per_node+1)%2 == 0){
            pointerIndex = (keys_per_node+1)/2;
        }
        else{
            pointerIndex = (keys_per_node+1)/2 + 1;
        }
        
        int keyIndex;
        
        if ((keys_per_node)%2 == 0){
            keyIndex = (keys_per_node)/2;
        }
        else{
            keyIndex = (keys_per_node)/2 + 1;
        }
        
        //Distributing the split values in the new node
        for (int m = keyIndex; m < keys_per_node; m++) {
            newNode->keys[m-keyIndex] = node->keys[m];
        }
        
        //Distributing the split pointers in the new node
        for (int j = pointerIndex; j < keys_per_node ; j++) {
            newNode->pointers[j-pointerIndex] = node->pointers[j];
        }
       
        
        newNode->counter = keyIndex-1;
        node->counter = keyIndex;
        
        if (node->parent == NULL){
            
            //This happens when a new root is created
            treeNode* newRoot = new treeNode(keys_per_node, NULL, false);
            node->parent = newRoot;
        }
        
        newNode->parent = node->parent;
        node->parent->pointers[node->parent->counter] = node;
        node->parent->pointers[node->parent->counter+1]=newNode;

        //TODO:
        //May have to free up the shifted pointers from orignal node
        
        /*
         The new key's value falls between the values in the orignal
         node and the new node. The left over key is inserted into the 
         parent of the node along with a pointer to the new interior 
         node.
         This will be taken care of in insert function
        */
    
    }
    
    
}

treeNode*  BpTree::findRoot(treeNode* node){
    
    treeNode* dynamicRoot = new treeNode(keys_per_node, NULL, true);
    dynamicRoot = node;
    
    if (node->parent != NULL){
        
        while (dynamicRoot->parent != NULL){
            dynamicRoot = dynamicRoot->parent;
        }
    }
    
    return dynamicRoot;
    
}

treeNode* BpTree::findLeaf(treeNode *node){
    
    treeNode* dynamicLeaf = new treeNode(keys_per_node, NULL, true);


    return dynamicLeaf;
}


