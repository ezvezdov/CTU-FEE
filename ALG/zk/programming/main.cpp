// BST implementation from hw5/hw6

#include<bits/stdc++.h>

using namespace std;

typedef struct Node
{
    int right_speed;
    int left_speed;
    int right_path;
    int left_path;

    int value;
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
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    node->left_speed = 0;
    node->right_speed = 0;
    node->left_path = 0;
    node->right_path = 0;
    return node;
}

Node *BST_insert(Node *root, int value){
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
                cur_node->left_path++;
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
                cur_node->right_path++;
                cur_node = cur_node->right;
            }
        }
        else{
            delete new_node;
            return root;
        }
    }

    return root;
}

void compute_values(Node *root){
    if(root->left == NULL){
        root->left_path = 1;
        root->left_speed = 1;
    }
    else{
        compute_values(root->left);
        root->left_path = max(root->left->left_path, root->left->right_path) + 1;
    }
    
    if(root->right== NULL){
        root->right_path = 1;
        root->right_speed = 1;
    }
    else{
        compute_values(root->right);
        root->right_path = max(root->right->left_path, root->right->right_path) +1;
        root->right_speed = root->left_path + max(root->right->right_speed,root->right->left_speed);
    }

    if(root->left == NULL){
        root->left_speed = root->right_path;
    }
    else{
        root->left_speed = root->right_path + max(root->left->right_speed,root->left->left_speed);
    }
}

int main(){
    Node *root = NULL;

    int n,value;
    cin >> n;
    for(int i = 0; i < n; i++){
        cin >> value;
        root = BST_insert(root,value);
    }

    compute_values(root);

    printf("%d\n",max(root->left_speed,root->right_speed));

    free_tree(root);
}