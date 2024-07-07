class ComplexNumber:
    def __init__(self,im,re):
        self.im = im
        self.re = re
    def __add__(self,other):
        return ComplexNumber(self.re + other.re, self.im + self.im) 

    def to_string(self):
        return: str(self.re),"+", str(self.im) + "i")
    

    
        


a = ComplexNumber(5,6)
b = ComplexNumber(7,8)
c = a + b # __add__()