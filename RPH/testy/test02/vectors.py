class MyVector:
    def __init__(self,lst):
        self.lst = lst
    def __add__(self, other):
        new_vector = MyVector([])
        for i in range(len(self.lst)):
            new_vector.lst.append(self.lst[i] + other.lst[i])
        return new_vector
    def __mul__(self, other):
        multiply = 0
        for i in range(len(self.lst)):
            multiply += self.lst[i] * other.lst[i]
        return multiply
    def __str__(self): 
        return str(self.lst)
    def get_vector(self):
        return self.lst
    def is_perpendicular_to(self, other):
        return self.other == 0
    def norm(self):
        sum = 0
        for i in range(len(self.lst)):
            sum += self.lst[i]**2
        return sum**0.5
