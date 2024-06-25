#include<bits/stdc++.h>

using namespace std;

typedef struct{
    int left_index = -1;
    int right_index = -1;

    int left_cost = 0;
    int right_cost = 0;

    int max_sum = 0;
    int max_with_empty_slot = 0;
} Node;

void search_max_sum(map<int, Node> &nodes, int current_index){
    Node current_node = nodes[current_index];

    if(current_node.left_index == -1 || current_node.right_index == -1){
        return;
    }
    
    search_max_sum(nodes, current_node.left_index);
    search_max_sum(nodes, current_node.right_index);
    
    int max_sum_without_union = nodes[current_node.left_index].max_sum + nodes[current_node.right_index].max_sum;
    int max_sum_with_union = current_node.left_cost + nodes[current_node.left_index].max_with_empty_slot + current_node.right_cost + nodes[current_node.right_index].max_with_empty_slot;
    current_node.max_with_empty_slot = max(current_node.left_cost + nodes[current_node.left_index].max_with_empty_slot + nodes[current_node.right_index].max_sum,current_node.right_cost + nodes[current_node.right_index].max_with_empty_slot + nodes[current_node.left_index].max_sum);

    current_node.max_sum = max(max_sum_without_union,max_sum_with_union);
    
    nodes[current_index] = current_node;
}

int main(){
    int P,S;
    cin >> P >> S;

    vector<int> indexes;
    map<int, Node> nodes;

    for(int i = 0; i< S; i++){
        int I,I1,D1,I2,D2;
        cin >> I >> I1 >> D1 >> I2 >> D2;

        indexes.push_back(I);

        nodes[I].left_index = I1;
        nodes[I].right_index = I2;
        nodes[I].left_cost = D1;
        nodes[I].right_cost = D2;
    }

    search_max_sum(nodes,indexes[0]);
    printf("%d\n",nodes[indexes[0]].max_sum);

    return 0;
}

