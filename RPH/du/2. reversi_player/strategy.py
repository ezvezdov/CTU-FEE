'''
        Board in my strategy:

        P D X G G X D P
        D D X G G X D D
        X X X G G X X X
        G G G S S G G G
        G G G S S G G G
        X X X G G X X X
        D D X G G X D D
        P D X G G X D P

        P is Perfect
        D is Dangerous
        X is Excelent
        G is Good
        S is Start (start positions)

        Priority of this names:
            P > X > G > D
'''

#TODO: check is my_move makes opponent stone to my_stone on my dangerous place
#TODO: check is avilable my_move that, opponent isnt in perfect position
#TODO: клетки могут терять свой приоритет


'''
P > new_dangerous > X > G > D
'''