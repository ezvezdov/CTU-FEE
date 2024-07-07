class MyMatrix():
    def __init__(self, arr):
        self.arr = arr
    
    def save(self, filename):
        f= open(filename, 'wt', encoding='utf-8')


        for i in range(len(self.arr)):
            for j in range(len(self.arr[i])):
                f.write(str(self.arr[i][j]) + " ")
            f.write("\n")
    
        f.close()

    def load(self,filename):
        f = open(filename, 'r',encoding='utf-8')
        line = list(map(int, f.readline().split()))
        print(line)
        f.close()

filename = 'file.txt'
matrix = MyMatrix([[1,2,3],[4,5,6],[7,8,9]])
matrix.save(filename)
#matrix.load(filename)