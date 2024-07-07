class Cuboid:
    def __init__(self,a,b,c):
        self.a = a
        self.b = b
        self.c = c
    def compute_surface(self):
        plocha = self.a * self.b * 2 + self.b * self.c * 2 + self.a * self.c * 2
        return plocha
    def make_enlarged_copy(self,a1,b1,c1):
         return Cuboid(self.a + a1, self.b + b1, self.c + c1)