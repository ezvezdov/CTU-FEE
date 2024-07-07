class MyVector:
    def __init__(self,lst):
        self.lst = lst
    def get_vector(self):
        return self.lst
    def __mul__(self, other):
        multiply = 0
        for i in range(len(self.lst)):
            multiply += self.lst[i] * other.lst[i]
        return multiply

