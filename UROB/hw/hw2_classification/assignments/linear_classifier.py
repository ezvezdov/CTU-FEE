import numpy as np
from utils.engine import Tensor


class LinearClassifier:
    def __init__(self, num_features: int, num_classes: int, learning_rate: float = 1e-3,
                 weight_scale: float = 1e-4, batch_size: int = 100,
                 num_iters: int = 1000, verbose: bool = True, reg: float = 1e-3):

        self.num_classes = num_classes
        self.num_features = num_features

        self.verbose = verbose
        self.num_iters = num_iters
        self.batch_size = batch_size
        self.weight_scale = weight_scale
        self.learning_rate = learning_rate
        self.reg = reg

        self.W = Tensor(np.random.randn(num_features, num_classes) * weight_scale, req_grad=True)
        self.b = Tensor(np.zeros(num_classes), req_grad=True)

    def load_weights(self, W: np.ndarray, b: np.ndarray) -> None:
        """ Load the weights and biases into the model.

        Args:
            W: The weights of shape (D, C)
            b: The biases of shape (C,)

        Returns:
            None
        """

        self.W = Tensor(W, req_grad=True)
        self.b = Tensor(b, req_grad=True)

    def predict(self, X: np.ndarray) -> np.ndarray:
        """ Predict the labels of the data.

        Args:
            X: Input data of shape (N, D)

        Returns:
            y_pred: The predicted labels of the data. Array of shape (N,)
        """

        scores = self.compute_scores(X)
        return np.argmax(scores.data, axis=1)

    def train(self, X: np.ndarray, y: np.ndarray) -> list:
        num_train = X.shape[0]

        loss_history = []
        for i in range(self.num_iters):
            batch_indices = np.random.choice(num_train, self.batch_size, replace=False)
            X_batch = X[batch_indices]
            y_batch = y[batch_indices]

            scores = self.compute_scores(X_batch)
            loss = Tensor(0)

            # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱ Assignment 3.2 ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
            # TODO:                                                             #
            # Implement one iteration of the training loop. Use the computed    #
            # scores to compute the Cross Entropy Loss, add the regularization  #
            # loss of all parameters and store it to the variable `loss`.       #
            # Then, compute the backward pass and update the weights and biases #
            # of the model. After that zero out the gradients of the weights    #
            # and biases.                                                       #
            #                                                                   #
            # HINT: - Use only already implemented functions of the Tensor      #
            #         class.                                                    #
            #       - Do not forget to add the regularization loss              #
            #         (defined in Tensor class) of ALL parameters. Use self.reg #
            #         as the regularization strength.                           #
            #       - Call step() on the `loss` variable to update              #
            #         the parameters.                                           #
            #                                                                   #
            # Good luck!                                                        #
            # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
            # 🌀 INCEPTION 🌀 (Your code begins its journey here. 🚀 Do not delete this line.)

            loss = loss + scores.cross_entropy_loss(y_batch) + self.b.regularization_loss(self.reg) + self.W.regularization_loss(self.reg)
        
        

            loss.backward()
            loss.step(learning_rate=self.learning_rate)            
            loss.zero_grad()
            
            # 🌀 TERMINATION 🌀 (Your code reaches its end. 🏁 Do not delete this line.)

            loss_history.append(loss.data)
            if self.verbose and i % 100 == 0:
                print(f"iteration {i} / {self.num_iters}: {loss.data}")

        return loss_history

    def compute_scores(self, X: np.ndarray) -> Tensor:
        """ Compute the scores of the model.

        Args:
            X: Input data of shape (N, D)

        Returns:
            scores: The scores of the model. Tensor of shape (N, C)

        """

        scores = Tensor(np.zeros((X.shape[0], self.num_classes)), req_grad=False)
        X = Tensor(X)

        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱ Assignment 3.1 ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # TODO:                                                             #
        # Implement computation of the scores of the model.                 #
        #                                                                   #
        # Good luck!                                                        #
        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # 🌀 INCEPTION 🌀 (Your code begins its journey here. 🚀 Do not delete this line.)
        
        scores = X.matmul(self.W) + self.b

        # 🌀 TERMINATION 🌀 (Your code reaches its end. 🏁 Do not delete this line.)

        return scores
