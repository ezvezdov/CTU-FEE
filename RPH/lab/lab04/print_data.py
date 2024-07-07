def print_data(lst):
    for i in range(len(lst)):
        for j in range(len(lst[i])):
            print(lst[i][j],end=" ")
        print()
def line_size(r,c,lst):
    start = lst[r][c]
    count = 1
    for i in range(c,len(lst[r])):
        if lst[r][i] == start:
            count+=1
    return count

r=5
c=5
data = [ \
[0, 1, 1, 1, 1, 0, 0, 0 ] , \
[1, 1, 0, 1, 0, 1, 1, 1 ] , \
[0, 1, 1, 0, 0, 1, 0, 1 ] , \
[1, 1, 1, 0, 1, 1, 0, 1 ] , \
[0, 1, 0, 0, 0, 0, 1, 1 ] , \
[1, 0, 1, 1, 0, 0, 0, 0 ] , \
[0, 1, 1, 1, 0, 1, 1, 1 ] , \
[1, 1, 0, 1, 0, 1, 1, 1 ]]
reg_size = line_size(r , c, data)
print(reg_size)