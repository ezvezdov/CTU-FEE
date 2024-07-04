#include <functional>
#include "bst_tree.h"

void bst_tree::insert(long long data) {
    node * new_node = new node(data);
    node * current = root;
    node* tmp = nullptr;

    while(true){
        if(root == nullptr){
            if(root.compare_exchange_strong(tmp, new_node)){
                break;
            }
        }
        else if(current->data > data){
            if(current->left == nullptr){
                if(current->left.compare_exchange_strong(tmp,new_node)){
                    break;
                }
                else{
                    continue;
                }
            }
            else{
                current = current->left;
            }
        }
        else if(current->data < data){
            if(current->right == nullptr){
                if(current->right.compare_exchange_strong(tmp,new_node)){
                    break;
                }
                else{
                    continue;
                }
            }
            else{
                current = current->right;
            }
        }
    }
    // Naimplementujte zde vlaknove-bezpecne vlozeni do binarniho vyhledavaciho stromu
}

bst_tree::~bst_tree() {
    // Rekurzivni funkce pro pruchod stromu a dealokaci pameti prirazene jednotlivym
    // uzlum
    std::function<void(node*)> cleanup = [&](node * n) {
        if(n != nullptr) {
            cleanup(n->left);
            cleanup(n->right);

            delete n;
        }
    };
    cleanup(root);
}
