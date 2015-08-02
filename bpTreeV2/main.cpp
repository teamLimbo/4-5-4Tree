#include "BpTree.h"


int main(){
    
    cout<<"Please enter the number of keys you want per node:\n";
    
    int numKeys;
    
    cin>>numKeys;
    
    BpTree* tree = new BpTree(numKeys);
    
    
    tree->insert(7, "daniel");
    tree->insert(1, "Rachel");
    tree->insert(5, "Maria");
    tree->insert(21, "Archit");
    
    cout<<tree->root->keys[0]<<endl;
    cout<<tree->root->keys[1]<<endl;
    cout<<tree->root->keys[2]<<endl;
    cout<<tree->root->keys[3]<<endl;

   
    tree->printValues();
    cout<<"\n";
    tree->find(5);

//    cout<<tree->records->lastKey;

    
    return 0;
}