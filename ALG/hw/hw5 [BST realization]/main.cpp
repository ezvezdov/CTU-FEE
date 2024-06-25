// https://www.youtube.com/watch?v=mtvbVLK5xDQ
#include<bits/stdc++.h>

#define INSERT_CMD 'i'
#define DELETE_CMD 'd'

using namespace std;

typedef struct
{
    int max_h;
    int vertex_count;
    int intervals_count;
}BST_data;

typedef struct Node
{
    int a;
    int b;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;

BST_data get_data(Node *node, int h){
    BST_data data({INT_MIN,0,0});
    if(node == NULL){
        return (BST_data){h-1,0,0};
    }

    BST_data left_data = get_data(node->left,h+1);
    BST_data right_data = get_data(node->right,h+1);


    data.max_h = max(max(left_data.max_h,data.max_h),right_data.max_h);
    data.intervals_count = node->b - node->a + 1 + left_data.intervals_count + right_data.intervals_count;
    data.vertex_count = left_data.vertex_count + right_data.vertex_count + 1;

    return data;
}

bool intervals_have_union(int a1, int b1, int a2, int b2){
    if(a2 < a1){
        swap(a1,a2);
        swap(b1,b2);
    }
    return (a1 <= a2 && a2 <= b1 && b1 <= b2) || (a1 <= a2 && a2 <= b2 && b2 <= b1);
}

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

Node *create_node(int a, int b){
    Node *node = new Node;
    node->a = a;
    node->b = b;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

Node *get_min(Node *root){
    Node *cur_node = root;

    while (cur_node->left != NULL){
        cur_node = cur_node->left;
    }

    return cur_node;
}

Node *BST_delete(Node *root, int c){
    if(root == NULL){
        return NULL;
    }
    
    Node *cur_node = root;
    Node *min_node = NULL;
    Node *parent = NULL;

    while (cur_node != NULL)
    {   
        
        parent = cur_node->parent;

        if(cur_node->a <= c && cur_node->b >= c){

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
                
                min_node = get_min(cur_node->right);
                cur_node->a = min_node->a;
                cur_node->b = min_node->b;

                BST_delete(min_node,min_node->a);
            }
            break;
        }
        else if(cur_node->a <= c && cur_node->b <= c){
            cur_node = cur_node->right;
        }
        else if(cur_node->a >= c && cur_node->b >= c){
            cur_node = cur_node->left;
        }
    }
    return root;
}

Node *iBST_insert(Node *root, int a, int b){
    queue<Node*> q;

    a = min(a,root->a);
    b = max(b,root->b);
    q.push(root->left);
    q.push(root->right);

    while (!q.empty())
    {
        Node *node = q.front();
        q.pop();
        
        while (node != NULL && intervals_have_union(a,b,node->a,node->b)){
            a = min(a,node->a);
            b = max(b,node->b);
            node = BST_delete(node,node->a);
        }
        if(node == NULL){
            continue;
        }
        
        q.push(node->left);
        q.push(node->right);
    }

    root->a = a;
    root->b = b;

    return root;
}

Node *BST_insert(Node *root, int a, int b){
    Node *new_node = create_node(a,b);
    if(root == NULL){
        return new_node;
    }

    Node *cur_node = root;

    while (cur_node != NULL)
    {
        
        if(new_node->a < cur_node->a && new_node->b < cur_node->a){
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
        else if(new_node->a > cur_node->b && new_node->b > cur_node->b){
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
            // iBST_insert

            cur_node = iBST_insert(cur_node,a,b);
            root = cur_node->parent == NULL ? cur_node : root;

            delete new_node;
            break;
        }
        
    }
    return root;
}

int main(){
    Node *root = NULL;

    int cmd_count;
    cin >> cmd_count;
    char cmd;
    int a,b,c;
    for(int i = 0; i < cmd_count; i++){
        cin >> cmd;

        if(cmd == INSERT_CMD){
            cin >> a >> b;
            root = BST_insert(root,a,b);
        }
        else if(cmd == DELETE_CMD){
            cin >> c;
            root = BST_delete(root,c);
        }
    }

    BST_data data = get_data(root,0);

    cout << data.vertex_count << " " << data.max_h<< " " <<  data.intervals_count << endl;

    free_tree(root);
}