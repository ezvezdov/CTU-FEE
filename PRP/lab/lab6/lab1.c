#include<stdio.h>

void fill_array(int *arr, int n){
    for(int i = 0; i < n; i++){
        scanf("%d",&arr[i]);
    }
    return;
}

int main(){
    int n;
    scanf("%d",&n);

    int arr[n];
    fill_array(arr,n);
    

    for(int i = 0; i < n; i++){
        printf("%d ",arr[i]);
    }

}