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

    
    //Simple insertion into a leaf thats not full==========================================
    
    if ((dynamicRoot->isFull(dynamicRoot) == false) && (dynamicRoot->isLeaf == true)){
        
        dynamicRoot->keys[dynamicRoot->counter] = n;
        dynamicRoot->counter++;
        records->dataBaseInsert(n, s, records);
        sort(dynamicRoot->keys,dynamicRoot->keys+dynamicRoot->counter); //This fucntion sorts the node upto inserted elements everytime before a new value is inserted

    }
    
    
    //Insertion when the leaf node is full=================================================
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
        
        treeNode* leafNode = new treeNode(keys_per_node, NULL, true);
        leafNode = findLeaf(dynamicRoot, n);
      //  cout<<"The dynamic root counter"<<dynamicRoot->counter<<endl;
        sort(dynamicRoot->keys,dynamicRoot->keys+dynamicRoot->counter);

        
        if (leafNode->parent->keys[leafNode->parent->counter-1] < n){
            leafNode = leafNode->parent->pointers[leafNode->parent->counter];
        }
        
        leafNode->keys[leafNode->counter] = n;
        leafNode->counter++;
        records->dataBaseInsert(n, s, records);
        sort(leafNode->keys,leafNode->keys+leafNode->counter);

    
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
    queue<pair<treeNode* , int>> printQueue;
    pair<treeNode*, int> item (dynamicRoot, 0);
    printQueue.push(item);
    int currentLevel = 0;
    
    while (printQueue.empty()==false){
       
        item = printQueue.front();
        
        if (currentLevel < item.second){
            cout<<endl;
            currentLevel++;
        }
        
        printQueue.pop();
        cout<<"[";
        for (int i=0; i < item.first->counter; i++){
            cout<<item.first->keys[i];
            
            if (i==item.first->counter-1){
                //do nothing
            }
            else{
                cout<<", ";
            }
        }
        cout<<"]";
        
        if (item.first->isLeaf == false){
            for (int i=0; i <= item.first->counter; i++){
                pair<treeNode*, int> item2 (item.first->pointers[i], item.second+1);
                printQueue.push(item2);
            }
        }
    
    }


}

void BpTree::printValues(){
    
    for (map<int,string>::iterator it = records->recordDatabase.begin(); it!=records->recordDatabase.end(); ++it){
        cout << it->first << " => " << it->second << '\n';
    }
}

void BpTree::splitNode(treeNode *node){
    
    treeNode* newNode = new treeNode(keys_per_node, NULL, true);
    
    //Splitling up a leaf

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
//        int position=8;
//        for (int i=0; node->parent->counter; i++){
//            if (&node== &(node->parent->pointers[i])){
//                position = i;
//                cout<<"The position: "<<position<<endl;
//            }
//        }
//        
//        
//        node->parent->pointers[position] = node;
//        node->parent->pointers[position+1]=newNode;
//        newNode->parent = node->parent;
//        
//        //update the values in parent node
//        node->parent->keys[position+1] = newNode->keys[0];
//        node->parent->counter++;
        
        
//        node->parent->pointers[node->parent->counter] = node;
//        node->parent->pointers[node->parent->counter+1]=newNode;
        
        newNode->parent = node->parent;
        
        //update the values in parent node
       
        node->parent->keys[node->parent->counter] = newNode->keys[0];
        node->parent->counter++;
        sort(node->parent->keys,node->parent->keys+node->parent->counter);
        
        for (int i =0; i <= node->parent->counter; i++){
            if (node->parent->keys[i] == newNode->keys[0]){
                for (int j=node->parent->counter; j >= i+1;j--){
                    node->parent->pointers[j]=node->parent->pointers[j-1];
                }
                node->parent->pointers[i] = node;
                node->parent->pointers[i+1] = newNode;
                
            
            }
        
        }
        

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

treeNode* BpTree::findLeaf(treeNode *node, int n){
    
    
    if (node->isLeaf == true && node->isFull(node) == false){
        return node;
    }
    
    else if (node->isLeaf == true && node->isFull(node) == true){
        splitNode(node);
        for (int i=0; i < node->parent->counter; i++){
            if (n < node->parent->keys[i]){
                return node->parent->pointers[i];
            }
            
            
            else if(n > node->parent->keys[i]){
                return node->parent->pointers[i+1];
            }
        }
        
    }
    
    else if (node->isLeaf == false) {
        for (int i=0; i < node->counter; i++){
            if (n < node->keys[i]){
                if(node->pointers[i]->isFull(node->pointers[i]) == false && node->pointers[i]->isLeaf == true){
                    //  cout<<"Are we here"<<endl;
                    return node->pointers[i];
                }
                
                else if (node->pointers[i]->isFull(node->pointers[i]) == true && node->pointers[i]->isLeaf == true){
                    splitNode(node->pointers[i]);
                    for (int i=0; i < node->pointers[i]->parent->counter; i++){
                        if (n < node->pointers[i]->parent->keys[i]){
                            return node->pointers[i]->parent->pointers[i];
                        }
                        
                        else if(n > node->pointers[i]->parent->keys[i]){
                            return node->pointers[i]->parent->pointers[i+1];
                        }
                    }
                }
                
                else if(node->pointers[i]->isLeaf == false){
                    findLeaf(node->pointers[i], n);
                }
            }
            
            else if (n > node->keys[i]){
                
                if(node->pointers[i+1]->isFull(node->pointers[i+1]) == false && node->pointers[i+1]->isLeaf == true){
                    //   cout<<" or Are we here"<<endl;
                    return node->pointers[i+1];
                }
                else if (node->pointers[i+1]->isFull(node->pointers[i+1]) == true && node->pointers[i+1]->isLeaf == true){
                    splitNode(node->pointers[i+1]);
                    for (int i=0; i < node->pointers[i+1]->parent->counter; i++){
                        if (n < node->pointers[i+1]->parent->keys[i]){
                            return node->pointers[i+1]->parent->pointers[i];
                        }
                        
                        else if(n > node->pointers[i+1]->parent->keys[i]){
                            return node->pointers[i+1]->parent->pointers[i+1];
                        }
                    }
                }
                
                else if(node->pointers[i+1]->isLeaf == false){
                    findLeaf(node->pointers[i+1], n);
                }
            }
        }
    }
    
    return node;
}

void BpTree::traversePrintKeys(treeNode* node){
//    int currentLevel = 1;
    for (int i = 0; i <= node->counter; i++){
        if (node->isLeaf == false){
 //           currentLevel++;
            cout << "[";
            for (int j = 0; j < node->pointers[i]->counter; j++){
                cout<< node->pointers[i]->keys[j] << ", ";
            }
            cout << "]";
//            cout<<currentLevel<<endl;
            traversePrintKeys(node->pointers[i]);
        }
    }
}

