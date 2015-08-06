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
    tree->insert(28, "Captain America");
    
    tree->printKeys();
    tree->printValues();
    
    cout<<"\n";
    tree->find(28);
    tree->remove(28);
    tree->find(28);
    
    return 0;
}