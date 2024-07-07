class MyPlayer():
    '''
        Hrac se diva na zisk pri CD a DC a voli to, kde je ten zisk vetsi.
    '''
    def __init__(self,payoff_matrix,number_of_iterations = 1):
        self.payoff_matrix = payoff_matrix
        self.number_of_iterations = number_of_iterations
        self.current_iteration = 0
        self.answer = False
        self.cooperate = True

        self.matrix_CDdivDC = self.payoff_matrix[0][1][0] / self.payoff_matrix[1][0][0]
        self.matrix_CCdivDD = self.payoff_matrix[0][0][0] / self.payoff_matrix[1][1][0]

    def move(self):
        if self.cooperate:
            if self.matrix_CCdivDD > 1 and self.matrix_CDdivDC >= 0.1:
                self.answer = False
            if self.matrix_CCdivDD > 1 and self.matrix_CDdivDC <= 0.1:
                self.answer = True
            if self.matrix_CCdivDD <= 1 and self.matrix_CDdivDC >= 0.1:
                self.answer = False
            if self.matrix_CCdivDD <= 1 and self.matrix_CDdivDC <= 0.1:
                self.answer = True
        else:
            if self.matrix_CDdivDC > 1:
                self.answer = False
            else:
                self.answer = True
        self.current_iteration += 1
        return self.answer

    def record_last_moves(self, my_last_move, opponent_last_move):
        if not my_last_move and opponent_last_move:
            self.cooperate = False
        
        