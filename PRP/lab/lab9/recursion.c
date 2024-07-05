#include<stdio.h>

void hello(int n){
    if(n == 0){
        return;
    }
    printf("Hello world!\n");
    hello(n-1);
}

int string_len(char* str, int n){
    printf("%d\n",n);
    if(str[n] != '\0'){
        return string_len(str, n+1);
    }
    return n-1;
    
}

int is_queen_can_be_here(int n, int row, int col, int board[n][n]){
    for(int i = 1; row-i >= 0; i++){
        if(board[row-i][col] == 1){
            return 0;
        }
    }
    
    for(int i = 1; row+i < n && col+i < n; i++){
        if(board[row+i][col+i] == 1){
            return 0;
        }
    }
    for(int i = 1; row-i >= 0 && col+i >= 0; i++){
        if(board[row-i][col-i] == 1){
            return 0;
        }
    }
    for(int i = 1; row+i < n && col-i >= 0; i++){
        if(board[row+i][col-i] == 1){
            return 0;
        }
    }
    for(int i = 1; row-i >= 0 && col+i < n; i++){
        if(board[row-i][col+i] == 1){
            return 0;
        }
    }
    return 1;
        
}

int queens(int n, int row, int board[n][n]){
    if(n == 5){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                printf("%d", )
            }
        }
    }
    for(int col = 0 ; col < n; col++){
        if(is_queen_can_be_here(n,row, col,board) == 1){
            board[row][col] = 1;
            queens(n, row+1, board)
        }
    }
    return 0;

}

}

int main(){
    int n;
    int board[n][n] = {0};
}