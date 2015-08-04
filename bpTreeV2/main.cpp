#include "BpTree.h"


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
    
//    cout<<tree->root->keys[0]<<endl;
//    cout<<tree->root->keys[1]<<endl;
//    
//    cout<<tree->root->parent->keys[0]<<endl;
//    cout<<tree->root->pointers[numKeys]->keys[0]<<endl;
//    cout<<tree->root->pointers[numKeys]->keys[1]<<endl;
//    cout<<tree->root->parent->pointers[tree->root->parent->counter]->keys[0]<<endl;
//    cout<<tree->root->parent->pointers[tree->root->parent->counter]->keys[1]<<endl;
    
    tree->printKeys();
//    
//    cout<<"Hello";

   
//   tree->printValues();
//    cout<<"\n";
//    tree->find(5);

//    cout<<tree->records->lastKey;

    
    return 0;
}