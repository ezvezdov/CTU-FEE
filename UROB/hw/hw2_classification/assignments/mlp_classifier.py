from typing import Union
from copy import deepcopy

import numpy as np
from tqdm import tqdm

from utils.engine import Tensor


class MLPClassifier:
    def __init__(self, input_size: int, hidden_dim_1: int, hidden_dim_2: int,
                 output_size: int, activation: str = 'relu', weight_scale: float = 1e-3,
                 learning_rate: float = 1e-3, reg: float = 1e-6, batch_size: int = 100,
                 num_iters: int = 1000, verbose: bool = False):

        self.params = dict(
            W1=Tensor(np.random.randn(input_size, hidden_dim_1) * weight_scale, req_grad=True),
            b1=Tensor(np.random.randn(hidden_dim_1), req_grad=True),
            W2=Tensor(np.random.randn(hidden_dim_1, hidden_dim_2) * weight_scale, req_grad=True),
            b2=Tensor(np.random.randn(hidden_dim_2), req_grad=True),
            W3=Tensor(np.random.randn(hidden_dim_2, output_size) * weight_scale, req_grad=True),
            b3=Tensor(np.random.randn(output_size), req_grad=True)
        )

        self.reg = reg
        self.verbose = verbose
        self.num_iters = num_iters
        self.activation = activation
        self.batch_size = batch_size
        self.num_classes = output_size
        self.learning_rate = learning_rate

    def _activation_function(self,data: Tensor) -> Tensor:
        if self.activation == 'relu':
            return data.relu()
        elif self.activation == 'sigmoid':
            return data.sigmoid()
        elif self.activation == 'tanh':
            return data.tanh()

    def _first_layer(self, X: Tensor) -> Tensor:
        """Forward pass of the first layer of the MLP.

        Args:
            X: Input data of shape (N, D)

        Returns:
            out: Output data of shape (N, H1)
        """

        out = None

        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱ Assignment 5.1 ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # TODO:                                                             #
        # Implement the first layer of the MLP.                             #
        #                                                                   #
        # Hint: You may want to use the `self.activation` attribute to      #
        #       determine which activation function to use.                 #
        #       (It can be either 'relu', 'sigmoid', or 'tanh'. All of them #
        #       are implemented in the Tensor class.)                       #
        # Good luck!                                                        #
        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # 🌀 INCEPTION 🌀 (Your code begins its journey here. 🚀 Do not delete this line.)

        W1 = self.params['W1']
        b1 = self.params['b1']
        val = X.matmul(W1) + b1
        out = self._activation_function(val)

        # 🌀 TERMINATION 🌀 (Your code reaches its end. 🏁 Do not delete this line.)

        return out

    def _second_layer(self, X: Tensor) -> Tensor:
        """Forward pass of the second layer of the MLP.

        Args:
            X: Input data of shape (N, H1)

        Returns:
            out: Output data of shape (N, H2)

        """

        out = None

        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱ Assignment 5.2 ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # TODO:                                                             #
        # Implement the second layer of the MLP.                            #
        #                                                                   #
        # Good luck!                                                        #
        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # 🌀 INCEPTION 🌀 (Your code begins its journey here. 🚀 Do not delete this line.)
        
        W2 = self.params['W2']
        b2 = self.params['b2']
        val = X.matmul(W2) + b2
        out = self._activation_function(val)

        # 🌀 TERMINATION 🌀 (Your code reaches its end. 🏁 Do not delete this line.)

        return out

    def _third_layer(self, X: Tensor) -> Tensor:
        """Forward pass of the third layer of the MLP.

        Args:
            X: Input data of shape (N, H2)

        Returns:
            out: Output data of shape (N, C)

        """

        out = None

        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱ Assignment 5.3 ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # TODO:                                                             #
        # Implement the third layer of the MLP.                             #
        #                                                                   #
        # Good luck!                                                        #
        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # 🌀 INCEPTION 🌀 (Your code begins its journey here. 🚀 Do not delete this line.)
        
        W3 = self.params['W3']
        b3 = self.params['b3']
        out = X.matmul(W3) + b3

        # 🌀 TERMINATION 🌀 (Your code reaches its end. 🏁 Do not delete this line.)

        return out

    def forward(self, X: Union[np.ndarray, Tensor]) -> Tensor:
        """Forward pass of the neural network.

        Args:
            X (Union[np.ndarray, Tensor]): Input data of shape (N, D)

        Returns:
            Tensor: Output data of shape (N, C)
        """

        out = None
        X = Tensor(X) if isinstance(X, np.ndarray) else X
        scores = Tensor(np.zeros((X.data.shape[0], self.num_classes)), req_grad=False)

        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱ Assignment 5.4 ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # TODO:                                                             #
        # Implement the forward pass of the neural network.                 #
        #                                                                   #
        # Hint: You should use the functions you implemented above.         #
        #                                                                   #
        # Good luck!                                                        #
        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # 🌀 INCEPTION 🌀 (Your code begins its journey here. 🚀 Do not delete this line.)
        
        scores = self._third_layer(self._second_layer(self._first_layer(X)))

        # 🌀 TERMINATION 🌀 (Your code reaches its end. 🏁 Do not delete this line.)

        return scores

    def predict(self, X: np.ndarray, zero_grad: bool = False) -> np.ndarray:
        """Predict the class labels for the provided data.

        Args:
            X (np.ndarray): Input data of shape (N, D)
            zero_grad (bool, optional): Whether to zero the gradients after
                prediction. Defaults to False.

        Returns:
            np.ndarray: Predicted class labels of shape (N,)
        """
        scores = Tensor(np.zeros((X.shape[0], self.num_classes), dtype=np.float32))
        y_pred = np.zeros(X.shape[0], dtype=np.int32)
        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱ Assignment 5.5 ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # TODO:                                                             #
        # Implement the predict function.                                   #
        #                                                                   #
        # Hint: Remember that the prediction is the class with the highest  #
        # score - argmax of the score vector.                               #
        #                                                                   #
        # Good luck!                                                        #
        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # 🌀 INCEPTION 🌀 (Your code begins its journey here. 🚀 Do not delete this line.)
        
        scores = self.forward(X)
        y_pred = np.argmax(scores.data,axis = 1)
        

        # 🌀 TERMINATION 🌀 (Your code reaches its end. 🏁 Do not delete this line.)

        if zero_grad:
            scores.zero_grad()

        return y_pred

    def loss(self, X: np.ndarray, y: np.ndarray, zero_grad: bool = False) -> Tensor:
        """Compute the loss of the neural network.

        Args:
            X (np.ndarray): Input data of shape (N, D)
            y (np.ndarray): Labels of shape (N,)
            zero_grad (bool, optional): Whether to zero the gradients after
                computing the loss. Defaults to False.

        Returns:
            Tensor: Loss of the neural network
        """

        loss = Tensor(0)

        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱ Assignment 5.6 ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # TODO:                                                             #
        # Compute the loss of the neural network based on the provided data #
        # and labels.                                                       #
        #                                                                   #
        # Hint: The loss function is the cross entropy loss applied to the  #
        # score vector plus the regularization loss of all parameters.      #
        #                                                                   #
        # Good luck!                                                        #
        #                                                                   #
        # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
        # 🌀 INCEPTION 🌀 (Your code begins its journey here. 🚀 Do not delete this line.)
        
        scores = self.forward(X)
        loss = scores.cross_entropy_loss(y) + (self.params['W1']).regularization_loss(self.reg) + (self.params['b1']).regularization_loss(self.reg) + \
            (self.params['W2']).regularization_loss(self.reg) + (self.params['b2']).regularization_loss(self.reg) + \
            (self.params['W3']).regularization_loss(self.reg) + (self.params['b3']).regularization_loss(self.reg)

        # 🌀 TERMINATION 🌀 (Your code reaches its end. 🏁 Do not delete this line.)

        if zero_grad:
            loss.zero_grad()

        return loss

    def train(self, X_train: np.ndarray, y_train: np.ndarray, X_val: np.ndarray, y_val: np.ndarray) -> tuple:
        """Train the neural network.

        Args:
            X_train (np.ndarray): Training data of shape (N, D)
            y_train (np.ndarray): Training labels of shape (N,)
            X_val (np.ndarray): Validation data of shape (N_val, D)
            y_val (np.ndarray): Validation labels of shape (N_val,)

        Returns:
            tuple: Tuple containing:
                loss_history (dict): Dictionary containing the training and
                    validation loss history.
                acc_history (dict): Dictionary containing the training and
                    validation accuracy history.
        """

        best_val_acc = 0
        best_params = dict()
        loss_history = dict(train=dict(), val=dict())
        acc_history = dict(train=dict(), val=dict())

        for i in tqdm(range(self.num_iters), desc="Training"):
            batch_indices = np.random.choice(X_train.shape[0], self.batch_size)
            X_batch = X_train[batch_indices]
            y_batch = y_train[batch_indices]

            train_loss = None

            # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱ Assignment 5.7 ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
            # TODO:                                                             #
            # Compute the loss of the neural network based on the batch data    #
            # and labels and store it to variable 'train_loss'. Then, compute   #
            # the backward pass and update the weights and biases of the model. #
            # After that zero out the gradients of the weights and biases.      #
            #                                                                   #
            # Good luck!                                                        #
            #                                                                   #
            # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
            # 🌀 INCEPTION 🌀 (Your code begins its journey here. 🚀 Do not delete this line.)
            
            train_loss = self.loss(X_batch,y_batch)
            train_loss.backward()
            train_loss.step(learning_rate = self.learning_rate)
            train_loss.zero_grad()



            # 🌀 TERMINATION 🌀 (Your code reaches its end. 🏁 Do not delete this line.)

            loss_history["train"][i] = train_loss.data

            if i % 500 == 0:
                val_loss = self.loss(X_val, y_val, zero_grad=True)
                loss_history["val"][i] = val_loss.data

                y_pred_train = self.predict(X_train, zero_grad=True)
                y_pred_val = self.predict(X_val, zero_grad=True)
                acc_history["train"][i] = np.mean(y_pred_train == y_train)
                acc_history["val"][i] = np.mean(y_pred_val == y_val)

                if acc_history["val"][i] > best_val_acc:
                    best_val_acc = acc_history["val"][i]
                    best_params = deepcopy(self.params)

        self.params = best_params

        return loss_history, acc_history

    # ================ Methods for the animation =================

    def transform(self, X: np.ndarray) -> np.ndarray:
        X = Tensor(X)
        out = self._first_layer(X)
        out = self._second_layer(out)
        return out.data

    def transform_point(self, x: np.ndarray) -> np.ndarray:
        x = x[:2][np.newaxis, :]
        out = self.transform(x)
        return np.array([out[0][0], out[0][1], 0])

    def predict_transformed(self, X: np.ndarray) -> np.ndarray:
        X = Tensor(X)
        scores = self._third_layer(X)
        return np.argmax(scores.data, axis=1)
