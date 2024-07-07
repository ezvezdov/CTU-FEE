import random

class Game_of_life():
    def __init__(self,board):
        self.board = board
    def check_borders(self,i,j):
        if i >= 0 and i < len(self.board) and j >= 0 and j < len(self.board[i]):
            return True
        return False

    def update(self):
        new_board = []
        for i in range(0,len(self.board)):
            new_board.append([])
            for j in range(0,len(self.board[i])):
                near_counter = 0
                if self.check_borders(i-1, j-1) and self.board[i-1][j-1] == 1: near_counter+=1
                if self.check_borders(i-1, j  ) and self.board[i-1][j] == 1: near_counter+=1
                if self.check_borders(i-1, j+1) and self.board[i-1][j+1] == 1: near_counter+=1
                if self.check_borders(i  , j-1) and self.board[i][j-1] == 1: near_counter+=1
                if self.check_borders(i  , j+1) and self.board[i][j+1] == 1: near_counter+=1
                if self.check_borders(i+1, j-1) and self.board[i+1][j-1] == 1: near_counter+=1
                if self.check_borders(i+1, j  ) and self.board[i+1][j] == 1: near_counter+=1
                if self.check_borders(i+1, j+1) and self.board[i+1][j+1] == 1: near_counter+=1

                if near_counter < 2 or near_counter > 3:
                    new_board[i].append(0)
                elif near_counter == 3 or near_counter == 2  and self.board[i][j] == 1:
                    new_board[i].append(1)
                else:
                    new_board[i].append(0)
        self.board = new_board
    def print_board(self):
        for i in range(0,len(self.board)):
            for j in range(0,len(self.board[i])):
                print(self.board[i][j], end=" ")
            print()
        print()

board = [ [random.randint(0,1) for j in range(5)] for i in range(5)]
game = Game_of_life(board)
for i in range(5):
    game.print_board()
    game.update()
