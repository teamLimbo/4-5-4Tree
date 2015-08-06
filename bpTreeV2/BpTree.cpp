//
//  BpTree.cpp
//  bpTreeV2
//
//  Created by Archit Sood (301188293) and Daniel Soheili (301163609) on 2015-07-30.
//  Copyright (c) 2015 Archit Sood. All rights reserved.
//

#include "BpTree.h"


//=========================================TREENODE========================================

treeNode::treeNode(int numKeys_, treeNode* parent_, bool isLeaf_)
{
    //Initializing the node Constructor
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

treeNode::~treeNode(){

    cout<<"Node Deleted"<<endl;
}

//=========================================DATABASE========================================


dataBase::dataBase(int n){}

void dataBase::dataBaseInsert(int key_, string value_, dataBase* record){
    
    record->recordDatabase[key_] = value_;
}

dataBase::~dataBase(){

    cout<<"Database Deleted"<<endl;
}

//=========================================BPTREE========================================


BpTree::BpTree(int n){
    
    this->root = new treeNode(n, NULL, true);
    this->records = new dataBase(n);
    this->keys_per_node = n;
}

//*********************************************************************************

void BpTree::insert(int n, string s){
    
    //Find the root of current tree
    treeNode* dynamicRoot = new treeNode(keys_per_node, NULL, true);
    dynamicRoot = findRoot(root);

    
    //Simple insertion into a leaf thats not full
    if ((dynamicRoot->isFull(dynamicRoot) == false) && (dynamicRoot->isLeaf == true)){
        
        dynamicRoot->keys[dynamicRoot->counter] = n;
        dynamicRoot->counter++;
        records->dataBaseInsert(n, s, records);
        //The following fucntion sorts the node upto inserted elements everytime before a new value is inserted
        sort(dynamicRoot->keys,dynamicRoot->keys+dynamicRoot->counter);
    }
    
    
    //Insertion when the leaf node is full
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
    
    //Insertion when current node is not leaf
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

//*********************************************************************************

void BpTree::remove(int n){

    //The following removes the key from the Database
    //Try find after removing an inserted item
    //It will say that the key does not exist
    this->records->recordDatabase.erase(n);

}

//*********************************************************************************

void BpTree::find(int n){
   
    //finds the value of the specified key from database which is just an imitation
    //of B+ tree's leaf level
    if (records->recordDatabase.find(n) == records->recordDatabase.end()){
        cout<<"This key does not exist"<<endl;
    }
    else{
        cout<<records->recordDatabase[n]<<endl;
    }
}

//*********************************************************************************

void BpTree::printKeys(){
    
    treeNode* dynamicRoot = new treeNode(keys_per_node, NULL, true);
    dynamicRoot = findRoot(root);
    
    //Using queue to do a breadth first search of keys and printing them in
    //specified order
    queue<pair<treeNode* , int> > printQueue;
    pair<treeNode*, int> item (dynamicRoot, 0); // this creates a node, level pair
    //Root is at level 0 above
    
    printQueue.push(item);
    int currentLevel = 0;  //Keeps track of current level
    
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

//*********************************************************************************

void BpTree::printValues(){
    
    //Goes to the record database (imitation of b+ trees leaf level and prints the key value pairs)
    for (map<int,string>::iterator it = records->recordDatabase.begin(); it!=records->recordDatabase.end(); ++it){
        cout << it->first << " => " << it->second << '\n';
    }
}

//*********************************************************************************


BpTree::~BpTree(){

    cout<<"Tree Deleted"<<endl;
}

//*********************************************************************************

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
    
        //Keep track of the new insertion point for the new node
        newNode->counter = i-1;

        //The orginal node keeps the first i values intact but the counter
        //(place where new value is inserted) moves.
        node->counter = i;
        
        if (node->parent == NULL){

            //This happens when a new root is created
            treeNode* newRoot = new treeNode(keys_per_node, NULL, false);
            node->parent = newRoot;
        }
        
        newNode->parent = node->parent;
        
        //update the values in parent node
        node->parent->keys[node->parent->counter] = newNode->keys[0];
        node->parent->counter++;
        sort(node->parent->keys,node->parent->keys+node->parent->counter);
        
        //Link the new pointers and move the older ones
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
        
        int pointerIndex;  //keeps track of which pointers need to be distributed
        
        if ((keys_per_node+1)%2 == 0){
            pointerIndex = (keys_per_node+1)/2;
        }
        else{
            pointerIndex = (keys_per_node+1)/2 + 1;
        }
        
        int keyIndex;      //keeps track of new insertion point of current and new node
        
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
        
        //Link the new pointers and move the older ones
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
    
    
}

//*********************************************************************************

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

//*********************************************************************************

treeNode* BpTree::findLeaf(treeNode *node, int n){
    
    /*The objective here is to find the appropriate leaf node for the new 
     key that needs to be inserted. The makes use of split function implemented
     above.
    */
    
    //current node is leaf and not full
    if (node->isLeaf == true && node->isFull(node) == false){
        return node;
    }
    
    //current node is leaf but full
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
    
    //current node is not a leaf
    else if (node->isLeaf == false) {
        for (int i=0; i < node->counter; i++){
            
            //inserted value is less than the current node's key
            if (n < node->keys[i]){
                if(node->pointers[i]->isFull(node->pointers[i]) == false && node->pointers[i]->isLeaf == true){
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
            
            //inserted value is greater than the current nodes key
            else if (n > node->keys[i]){
                
                if(node->pointers[i+1]->isFull(node->pointers[i+1]) == false && node->pointers[i+1]->isLeaf == true){
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



//Sample Test function used

int main(){
    
    cout<<"Please enter the number of keys you want per node:\n";
    
    int numKeys;
    
    cin>>numKeys;
    cout<<endl;
    
    BpTree* tree = new BpTree(numKeys);
    
    
    tree->insert(7, "Ross");
    tree->insert(1, "Rachel");
    tree->insert(5, "Monica");
    tree->insert(21, "Chandler");
    tree->insert(4, "Joey");
    tree->insert(27, "Phoebe");
    tree->insert(50, "Iron-Man");
    tree->insert(6, "Hulk");
    tree->insert(28, "Captain America");
    
    tree->printKeys();
    tree->printValues();
    
    cout<<"\n";
    tree->find(28);
    tree->remove(28);
    tree->find(28);
    
    return 0;
}
 

//Console Output:
//
// Please enter the number of keys you want per node:
// 3
// 
// [5, 7, 27]
// [1, 4][5, 6][7, 21][27, 28, 50]1 => Rachel
// 4 => Joey
// 5 => Monica
// 6 => Hulk
// 7 => Ross
// 21 => Chandler
// 27 => Phoebe
// 28 => Captain America
// 50 => Iron-Man
// 
// Captain America
// This key does not exist



