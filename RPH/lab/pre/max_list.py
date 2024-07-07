def max_in_list(A): 
    max_A = A[0]
    for i in range(len(A)):
        if(max_A < A[i]):
            max_A  = A[i]
    return max_A

#A = [1,5,6,3,8,3,9]
A = list(map(int, input().split()))

print(max_in_list(A))


        

 