// https://www.youtube.com/watch?v=vRwi_UcZGjU
// https://www.youtube.com/watch?v=haZU_8w_OTE
// https://www.youtube.com/watch?v=CdGVkgMuq_4
// https://www.youtube.com/watch?v=rbg7Qf8GkQ4

#include<bits/stdc++.h>

#define DEFAULT_AVL_D 1

using namespace std;

typedef struct Node
{
    int value;
    int h;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;

void free_tree(Node *root){
    queue<Node*> q;
    q.push(root);

    while (!q.empty())
    {
        Node *cur_node = q.front();
        q.pop();

        if(cur_node == NULL){
            continue;
        }

        q.push(cur_node->left);
        q.push(cur_node->right);

        delete cur_node;
    }   
}

Node *create_node(int value){
    Node *node = new Node;
    node->value = value;
    node->h = 0;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

int get_height(Node *node){
    if(node == NULL){
        return -1;
    }

    return node->h;
}

Node *AVL_right_rotation(Node *root){
    Node *prev_root = root;
    Node *prev_subtree = prev_root->left == NULL ? NULL : prev_root->left->right;

    root = root->left;
    root->parent = prev_root->parent;
    root->right = prev_root;

    prev_root->parent = root;
    prev_root->left = prev_subtree;

    if(prev_subtree != NULL){
        prev_subtree->parent = prev_root;
    }

    root->h = 1 + max(get_height(root->left),get_height(root->right));
    prev_root->h = 1 + max(get_height(prev_root->left),get_height(prev_root->right));

    return root;
}

Node *AVL_left_rotation(Node *root){
    Node *prev_root = root;
    Node *prev_subtree =  prev_root->right == NULL ? NULL : prev_root->right->left;

    root = root->right;
    root->parent = prev_root->parent;
    root->left = prev_root;

    prev_root->parent = root;
    prev_root->right = prev_subtree;
    if(prev_subtree != NULL){
        prev_subtree->parent = prev_root;
    }

    root->h = 1 + max(get_height(root->left),get_height(root->right));
    prev_root->h = 1 + max(get_height(prev_root->left),get_height(prev_root->right));

    return root;
}

int check_balance(Node *node){
    if(node == NULL){
        return 0;
    }
    return get_height(node->left) - get_height(node->right);
}

Node *AVL_rebalance(Node *node, int D, int *rotations_count){
    while(node != NULL){
        int balance = check_balance(node);

        if(abs(balance) > D ){
            //Current node isn't balanced

            Node *cur_parent = node->parent;

            // Check is current node right child of parent
            bool is_right = cur_parent == NULL ? false : cur_parent->right == node;

            // Increment count of rotations
            *rotations_count = *rotations_count + 1; 

            if(balance > 0){
                //Left-heavy
                if(check_balance(node->left) >= 0 ){
                    // Right rotation
                    node = AVL_right_rotation(node);
                }
                else{
                    // Left-Right rotation
                    node->left = AVL_left_rotation(node->left);
                    node = AVL_right_rotation(node);
                }
            }
            else{
                //Right-heavy
                if(check_balance(node->right) <= 0){
                    // Left rotation
                    node = AVL_left_rotation(node);    
                }
                else{
                    // Right-Left rotation
                    node->right = AVL_right_rotation(node->right);
                    node = AVL_left_rotation(node);   
                }
            }

            // Setting pointer of new "root" to parent
            if(cur_parent != NULL){
                if(is_right){
                    cur_parent->right = node;
                }
                else{
                    cur_parent->left = node;
                }
            }

        }

        // Changing the heights
        node->h = 1 + max(get_height(node->left),get_height(node->right));

        if(node->parent == NULL){
            break;
        }
        node = node->parent;
    }
    return node;
}

Node *get_max(Node *root){
    Node *cur_node = root;

    while (cur_node->right != NULL){
        cur_node = cur_node->right;
    }

    return cur_node;
}

Node * AVL_insert(Node *root, int value, int D, int *rotations_count){
    Node *new_node = create_node(value);
    if(root == NULL){
        return new_node;
    }

    Node *cur_node = root;

    while (cur_node != NULL)
    {
        if(new_node->value < cur_node->value){
            // node goes to left 

            if(cur_node->left == NULL){
                new_node->parent = cur_node;
                cur_node->left = new_node;
                break;
            }
            else{
                cur_node = cur_node->left;
            }
        }
        else if(new_node->value > cur_node->value){
            // node goes to right

            if(cur_node->right == NULL){
                new_node->parent = cur_node;
                cur_node->right = new_node;
                break;
            }
            else{
                cur_node = cur_node->right;
            }
        }
        else{
            delete new_node;
            return root;
        }
        
    }

    // Balance AVL tree
    root = AVL_rebalance(cur_node,D,rotations_count);

    return root;
}

Node *AVL_delete(Node *root, int value,int D,int *rotations_count){
    if(root == NULL){
        return NULL;
    }
    
    Node *cur_node = root;
    Node *max_node = NULL;
    Node *parent = NULL;

    while (cur_node != NULL)
    {   
        parent = cur_node->parent;

        if(cur_node->value == value){
            if( !(cur_node->left != NULL && cur_node->right != NULL)){
                // 1. LEAF => NULL
                // 2. Have right children => right children
                // 3. Have left children => left children

                Node *cur_node_next = cur_node->left != NULL ? cur_node->left : cur_node->right;
                
                if(parent != NULL && parent->left == cur_node){
                    parent->left = cur_node_next;
                }
                else if(parent != NULL && parent->right == cur_node){
                    parent->right = cur_node_next;
                }   

                if(cur_node_next != NULL){
                    cur_node_next->parent = cur_node->parent;
                }

                // return new node if current node is root or hasn't parrent 
                root = (cur_node == root || parent == NULL) ? cur_node_next: root;

                delete cur_node;
            }
            else{
                // left and right childern
                
                max_node = get_max(cur_node->left);
                cur_node->value = max_node->value;
                parent = max_node->parent;
                AVL_delete(max_node,max_node->value,D,rotations_count);
            }
            break;
        }
        else if(cur_node->value < value){
            cur_node = cur_node->right;
        }
        else if(cur_node->value > value){
            cur_node = cur_node->left;
        }
    }

    // Balance AVL tree
    root = AVL_rebalance(parent,D,rotations_count);

    return root;
}

int main(){
    int D, A, B, M, N;
    cin >> D >> A >> B >> M >> N;

    Node * AVL_root = NULL;
    Node * rAVL_root = NULL;

    int AVL_rotations = 0, rAVL_rotations = 0;

    int s[1000001] = {0};
    s[1] = B;
    for(int k = 1; k < N; k++){
        if(k == 1){
            AVL_root = AVL_insert(AVL_root, s[k],DEFAULT_AVL_D,&AVL_rotations);
            rAVL_root = AVL_insert(rAVL_root, s[k],D,&rAVL_rotations);
        }

        s[k+1] = (A * s[k] + B) % M;

        AVL_root = AVL_insert(AVL_root, s[k+1],DEFAULT_AVL_D,&AVL_rotations);
        rAVL_root = AVL_insert(rAVL_root, s[k+1],D,&rAVL_rotations);
    }

    for(int i = 3; i <= N; i+=3){
        AVL_root = AVL_delete(AVL_root, s[i],DEFAULT_AVL_D,&AVL_rotations);
        rAVL_root = AVL_delete(rAVL_root, s[i],D, &rAVL_rotations);
    }

    cout << get_height(AVL_root) << " " << AVL_rotations << endl;
    cout << get_height(rAVL_root) << " " << rAVL_rotations << endl;

    free_tree(AVL_root);
    free_tree(rAVL_root);
}