from copy import deepcopy


class MyPlayer():
    ''' 
        Player choose position with best priority.
    '''

    def __init__(self, my_color, opponent_color):
        self.name = "zviazyau"
        self.my_color = my_color
        self.opponent_color = opponent_color
        self.board_size = 8
        self.empty_position = -1

        self.__line_doesnt_exist = (-1, -1)
        self.__lines_start = {} # dict[position] = [update_start_position,update_start_position2...]

    # check is index out of range (False - yes, True - no)
    def __check_border(self, i, j):
        return True if i >= 0 and i < self.board_size and j >= 0 and j < self.board_size else False

    # modes: 1 up, 2 down, 3 left, 4 right, 5 up left, 6 right down, 7 down left, 8 up right
    def __check_line(self, board, i, j, opponent_color, mode):
        i_copy = i
        j_copy = j
        while self.__check_border(i, j) and board[i][j] == opponent_color:
            if mode == 1:
                i -= 1
            elif mode == 2:
                i += 1
            elif mode == 3:
                j -= 1
            elif mode == 4:
                j += 1
            elif mode == 5:
                i -= 1
                j -= 1
            elif mode == 6:
                i += 1
                j += 1
            elif mode == 7:
                i += 1
                j -= 1
            elif mode == 8:
                i -= 1
                j += 1

        if self.__check_border(i, j) and board[i][j] == self.empty_position and not(i == i_copy and j == j_copy):
            if opponent_color == self.opponent_color:
                self.__lines_start[(i, j)].append((i_copy, j_copy))
            return (i, j)
        return self.__line_doesnt_exist

    def __is_dangerous_position(self, board, pos):
        dangerous_positions = [
            (0, 1), (1, 1), (1, 0),
            (0, self.board_size-2), (1,
                                     self.board_size-2), (1, self.board_size-1),
            (self.board_size-2, 0), (self.board_size -
                                     2, 1), (self.board_size-1, 1),
            (self.board_size-1, self.board_size-2), (self.board_size-2,
                                                     self.board_size-2), (self.board_size-2, self.board_size-1)
        ]
        dangerous_up_left = [(0, 1), (1, 1), (1, 0)]
        dangerous_up_right = [(0, self.board_size-2),
                              (1, self.board_size-2), (1, self.board_size-1)]
        dangerous_down_left = [(self.board_size-2, 0),
                               (self.board_size-2, 1), (self.board_size-1, 1)]
        dangerous_down_right = [(self.board_size-1, self.board_size-2), (self.board_size-2,
                                                                         self.board_size-2), (self.board_size-2, self.board_size-1)]

        if dangerous_up_left.count(pos) != 0 and board[0][0] == self.empty_position:
            return True
        if dangerous_up_right.count(pos) != 0 and board[0][self.board_size-1] == self.empty_position:
            return True
        if dangerous_down_left.count(pos) != 0 and board[self.board_size-1][0] == self.empty_position:
            return True
        if dangerous_down_right.count(pos) != 0 and board[self.board_size-1][self.board_size-1] == self.empty_position:
            return True

        return False

    def __is_perfect_position(self, pos):
        perfect_positions = [
            (0, 0),
            (0, self.board_size-1),
            (self.board_size-1, 0),
            (self.board_size-1, self.board_size-1)
        ]
        if perfect_positions.count(pos):
            return True
        return False

    def __is_excelent_position(self, board, pos):
        excelent_up = [(0, i) for i in range(2, self.board_size-2)]
        excelent_left = [(i, 0) for i in range(2, self.board_size-2)]
        excelent_right = [(i, self.board_size - 1)
                          for i in range(2, self.board_size-2)]
        excelent_down = [(self.board_size-1, i)
                         for i in range(2, self.board_size-2)]

        previous_dangerous = False
        if board[0][0] != self.empty_position:
            if pos == (0, 1) or pos == (1, 0):
                previous_dangerous = True
        if board[0][self.board_size-1] != self.empty_position:
            if pos == (0, self.board_size-2) or pos == (1, self.board_size-1):
                previous_dangerous = True
        if board[self.board_size-1][0] != self.empty_position:
            if pos == (self.board_size-2, 0) or pos == (self.board_size-1, 1):
                previous_dangerous = True
        if board[self.board_size-1][self.board_size-1] != self.empty_position:
            if pos == (self.board_size-2, self.board_size-1) or pos == (self.board_size-1, self.board_size-2):
                previous_dangerous = True

        return pos in excelent_up or pos in excelent_left or pos in excelent_right or pos in excelent_down or previous_dangerous

    def __sort_positions(self, board, positions):
        P_pos = []
        X_pos = []
        G_pos = []
        D_pos = []

        while len(positions) != 0:
            pos = positions.pop()
            if self.__is_perfect_position(pos):
                P_pos.append(pos)
            elif self.__is_excelent_position(board, pos):
                X_pos.append(pos)
            elif self.__is_dangerous_position(board, pos):
                D_pos.append(pos)
            else:
                G_pos.append(pos)
        P_pos.sort()
        X_pos.sort()
        G_pos.sort()
        D_pos.sort()

        sorted_positions = P_pos + X_pos + G_pos + D_pos
        return sorted_positions, [len(P_pos), len(X_pos), len(G_pos), len(D_pos)]

    def moves_analyse(self, board, moves_data, my_count_of_types):
        #moves_data = [position, count_of_new_dangerous, count_of_types_opponent, opponent_position]
        equal_positions0 = []
        equal_positions = []
        sorted_positions = []
        minimum_dangerous = 12  # ALL DANGEROUS
        min_perfect_opponent = 4  # ALL PERFECT

        for i in range(len(moves_data)):
            if min_perfect_opponent >= moves_data[i][2][0]:
                if min_perfect_opponent > moves_data[i][2][0]:
                    equal_positions0 = []
                equal_positions0.append(moves_data[i])
                min_perfect_opponent = moves_data[i][2][0]

        if my_count_of_types[0] != 0:
            return equal_positions0[0][0]

        for i in range(len(equal_positions0)):
            if equal_positions0[i][1] <= minimum_dangerous:
                if equal_positions0[i][1] < minimum_dangerous:
                    equal_positions = []
                equal_positions.append(equal_positions0[i])
                minimum_dangerous = equal_positions0[i][1]

        main_score = -1000

        for i in range(len(equal_positions)):
            score_now = equal_positions[i][2][3] * 5 - \
                equal_positions[i][2][2] - 2 * equal_positions[i][2][1]
            if (self.__is_excelent_position(board, equal_positions[i][0])):
                score_now += 10
            if score_now >= main_score:
                if score_now > main_score:
                    sorted_positions = []
                sorted_positions.append(equal_positions[i][0])
                main_score = score_now

        sorted_positions, useless_inf = self.__sort_positions(
            board, sorted_positions)

        if len(sorted_positions) != 0:
            return sorted_positions[0]
        return None

    def move(self, board):
        self.__lines_start = dict()
        for i in range(8):
            for j in range(8):
                self.__lines_start[(i, j)] = []

        positions = self.get_posible_positions(
            board, self.my_color, self.opponent_color)
        positions, my_count_of_types = self.__sort_positions(board, positions)

        moves_data = []

        for pos in range(len(positions)):
            board_copy = deepcopy(board)

            updated_board, new_dangerous = self.update_board(
                board_copy, positions[pos][0], positions[pos][1])

            opponent_positions = self.get_posible_positions(
                updated_board, self.opponent_color, self.my_color)
            opponent_positions, count_of_types_opponent = self.__sort_positions(
                updated_board, opponent_positions)

            moves_data.append([positions[pos], new_dangerous,
                              count_of_types_opponent, opponent_positions])

        if len(positions) == 0:
            return None

        my_move = self.moves_analyse(board, moves_data, my_count_of_types)

        if my_move == None or positions.count(my_move) == 0:
            return positions[0]
        return my_move

    def update_board(self, updated_board, end_i, end_j):
        for i in range(len(self.__lines_start[(end_i, end_j)])):
            start_i = self.__lines_start[(end_i, end_j)][i][0]
            start_j = self.__lines_start[(end_i, end_j)][i][1]

            updated_board[end_i][end_j] = self.my_color
            new_dangerous = 0
            if self.__is_dangerous_position(updated_board, (end_i, end_j)):
                new_dangerous += 1

            while start_i != end_i or start_j != end_j:
                updated_board[start_i][start_j] = self.my_color
                if self.__is_dangerous_position(updated_board, (start_i, start_j)):
                    new_dangerous += 1

                if start_j < end_j:
                    start_j += 1
                if start_j > end_j:
                    start_j -= 1
                if start_i < end_i:
                    start_i += 1
                if start_i > end_i:
                    start_i -= 1
        return updated_board, new_dangerous

    def get_posible_positions(self, board, color, opponent_color):
        positions = set()
        for i in range(self.board_size):
            for j in range(self.board_size):
                if(board[i][j] == color):
                    positions.add(self.__check_line(
                        board, i-1, j, opponent_color, 1))
                    positions.add(self.__check_line(
                        board, i+1, j, opponent_color, 2))
                    positions.add(self.__check_line(
                        board, i, j-1, opponent_color, 3))
                    positions.add(self.__check_line(
                        board, i, j+1, opponent_color, 4))
                    positions.add(self.__check_line(
                        board, i-1, j-1, opponent_color, 5))
                    positions.add(self.__check_line(
                        board, i+1, j+1, opponent_color, 6))
                    positions.add(self.__check_line(
                        board, i+1, j-1, opponent_color, 7))
                    positions.add(self.__check_line(
                        board, i-1, j+1, opponent_color, 8))
        positions.add(self.__line_doesnt_exist)
        positions.remove(self.__line_doesnt_exist)
        return positions
