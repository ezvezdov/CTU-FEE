import numpy as np

# configure numpy to render floats with 3 decimal places
np.set_printoptions(formatter={'float': "{: 0.3f}".format})


# +++++++++++++++++ Assignment +++++++++++++++++
# In this file your task is to complete the functions
# marked with ellipses (...) and text cues. You should
# not change any other code in this file. You should 
# also not import any other modules here.
#
# This task is to implement a custom tensor class that
# supports basic operations and automatic backpropagation.
# +++++++++++++++++++++++++++++++++++++++++++++++


def reshape_gradient(gradient: np.ndarray, target_shape: tuple) -> np.ndarray:
    """Reshape the gradient to match the shape of the target Tensor.

    Args:
        gradient: The gradient to reshape.
        target_shape: The shape of the target Tensor.

    Returns:
        The reshaped gradient.
    """

    # if the gradient has the same shape as the target shape, return the gradient
    if gradient.shape == target_shape:
        return gradient

    # if the target shape is scalar, return the sum of the gradient
    if target_shape == ():
        return np.sum(gradient)

    # if the target shape is a vector, expand the dimension
    keepdims = True
    while len(target_shape) != len(gradient.shape):
        target_shape = (1, *target_shape)
        keepdims = False

    # otherwise, we need reduce the gradient along axes that were broadcast
    broadcast_axes = []
    for i, (grad_axis, tar_axis) in enumerate(zip(gradient.shape, target_shape)):

        # if the target axis is 1 and the gradient is larger, then
        # the Tensor was broadcast along this axis
        if tar_axis == 1 and grad_axis != 1:
            broadcast_axes.append(i)

    return np.sum(gradient, axis=tuple(broadcast_axes), keepdims=keepdims)

def back_none():
    return None


class Tensor:
    """
    A custom tensor class that supports basic operations and automatic differentiation.

    Args:
        data (array-like): Input data to create the tensor.
        _parent (tuple, optional): Tuple of parent tensors in the computation graph. Defaults to ().
        _op (str, optional): Operation associated with this tensor. Defaults to ''.
        label (str, optional): Label or name for the tensor. Defaults to ''.
        req_grad (bool, optional): Whether gradient updates should be performed for this tensor. Defaults to False.

    Attributes:
        data (numpy.ndarray): The underlying data stored in the tensor.
        label (str): A label for the tensor.
        grad (numpy.ndarray): Gradient of the tensor with respect to some loss.
        req_grad (bool): Indicates if gradient updates are to be performed for this tensor.
    """

    def __init__(self, data, _parent=(), _op='', label='', req_grad=False):
        self.data = np.array(data)
        self.label = label
        self.grad = np.zeros(self.data.shape)
        self.req_grad = req_grad

        self._backward = back_none
        self._prev = set(_parent)
        self._op = _op

    # +++++++++++++++++ Basic Operations +++++++++++++++++

    def __add__(self, other) -> 'Tensor':
        other = other if isinstance(other, Tensor) else Tensor(other)
        out = Tensor(self.data + other.data,(self, other), '+') # your code here

        def _backward():
            self.grad += reshape_gradient(np.ones_like(self.data) * out.grad, self.data.shape) # your code here
            other.grad += reshape_gradient(np.ones_like(other.data) * out.grad, other.data.shape) # your code here

        out._backward = _backward
        return out

    def __mul__(self, other) -> 'Tensor':
        other = other if isinstance(other, Tensor) else Tensor(other)
        out = Tensor(self.data * other.data,(self, other), '*') # your code here

        def _backward():
            self.grad += reshape_gradient( other.data * out.grad, self.data.shape) # your code here
            other.grad += reshape_gradient(self.data * out.grad, other.data.shape) # your code here

        out._backward = _backward
        return out

    def matmul(self, other) -> 'Tensor':
        if type(self) == type(other):
            pass
        elif isinstance(other, Tensor):
            pass
        else:
            #print(f"The other is not a Tensor! {type(other)}")
            other = Tensor(other)
        out = Tensor(self.data @ other.data,(self, other), '@') # your code here

        def _backward():
            self.grad += out.grad @ other.data.T # your code here
            other.grad += self.data.T  @ out.grad # your code here

        out._backward = _backward
        return out
    
    def __pow__(self, other) -> 'Tensor':
        assert isinstance(other, (int, float))
        out = Tensor(self.data ** other, (self, ), f'**{other}') # your code here
        
        def _backward():
            self.grad += other * (self.data)**(other-1) * out.grad # your code here
        out._backward = _backward

        return out

    def __sub__(self, other) -> 'Tensor':
        return self + (-other)

    def __matmul__(self, other) -> 'Tensor':
        return self.matmul(other)

    def __neg__(self) -> 'Tensor':
        return self * -1

    def __truediv__(self, other) -> 'Tensor':
        return self * (other ** -1)

    def __radd__(self, other) -> 'Tensor':
        return self + other

    def __rsub__(self, other) -> 'Tensor':
        return (-self) + other

    def __rmul__(self, other) -> 'Tensor':
        return self * other
    
    def __rtruediv__(self, other) -> 'Tensor':
        return other * (self ** -1)

    def __rpow__(self, other) -> 'Tensor':
        return other ** self

    # +++++++++++++++++ Basic Functions +++++++++++++++++

    def sin(self) -> 'Tensor':
        out = Tensor(np.sin(self.data), (self, )) # your code here

        def _backward():
            self.grad += np.cos(self.data) * out.grad # your code here
        out._backward = _backward

        return out
    
    def cos(self) -> 'Tensor':
        out = Tensor(np.cos(self.data), (self, )) # your code here

        def _backward():
            self.grad -= np.sin(self.data) * out.grad # your code here
        out._backward = _backward

        return out

    def exp(self) -> 'Tensor':
        out = Tensor(np.exp(self.data), (self, )) # your code here # your code here
            
        def _backward():
            self.grad += np.exp(self.data) * out.grad # your code here
        out._backward = _backward

        return out

    def log(self) -> 'Tensor':
        out = Tensor(np.log(self.data), (self, )) # your code here
            
        def _backward():
            self.grad += (1 / self.data) * out.grad # your code here
        out._backward = _backward

        return out

    # +++++++++++++++++ Other Functions +++++++++++++++++

    def sum(self, axis=None) -> 'Tensor':
        out = Tensor(np.sum(self.data),(self,)) # your code here
            
        def _backward():
            self.grad += np.ones_like(self.data) * out.grad # your code here
        out._backward = _backward

        return out

    def stack(self, other, axis=0) -> 'Tensor':
        other = other if isinstance(other, Tensor) else Tensor(other)
        out = Tensor(np.stack((self.data, other.data), axis=axis), (self, other), 'stack')

        def _backward():
            self.grad += out.grad[0]
            other.grad += out.grad[1]
        out._backward = _backward

        return out

    def T(self) -> 'Tensor':
        out = Tensor(self.data.T, (self,), 'T')

        def _backward():
            self.grad += out.grad.T
        out._backward = _backward

        return out

    # +++++++++++++++++ Activation Functions +++++++++++++++++

    def relu(self) -> 'Tensor':
        out = Tensor(np.maximum(0, self.data), (self, ), 'relu') # your code here

        def _backward():
            self.grad += (self.data > 0) * out.grad # your code here

        out._backward = _backward
        return out

    def sigmoid(self) -> 'Tensor':
        out = Tensor(1 / (1 + np.exp(-self.data)), (self, ), 'sigmoid') # your code here

        def _backward():
            self.grad += (out.data * (1 - out.data)) * out.grad # your code here

        out._backward = _backward
        return out

    def tanh(self) -> 'Tensor':
        out = Tensor(np.tanh(self.data), (self, ), 'tanh') # your code here

        def _backward():
            self.grad += (1 - (out.data ** 2)) * out.grad # your code here

        out._backward = _backward
        return out

    # +++++++++++++++++ Loss Functions +++++++++++++++++

    def cross_entropy_loss(self, target: np.ndarray) -> 'Tensor':
        assert isinstance(target, np.ndarray) and len(target.shape) == 1, 'target must be a 1D numpy array'

        N = target.shape[0]
        max_vector = np.max(self.data, axis=1, keepdims=True)  # (N, 1)
        exp = np.exp(self.data - max_vector)  # (N, C)
        softmax = exp / np.sum(exp, axis=1, keepdims=True)  # (N, C)

        one_hot_target = np.zeros_like(softmax)  # (N, C)
        one_hot_target[np.arange(target.shape[0]), target] = 1  # (N, C)

        out = Tensor(-np.sum(one_hot_target * np.log(softmax) / N), (self,), f"CE Loss \n(target={target})")

        def _backward():
            self.grad += (softmax-one_hot_target)/ N * out.grad # your code here

        out._backward = _backward
        return out

    def regularization_loss(self, reg: float) -> 'Tensor':
        out = Tensor(reg * np.sum(self.data**2),(self,)) # your code here

        def _backward():
            self.grad += (2 * reg * self.data) * out.grad # your code here

        out._backward = _backward
        return out

    # +++++++++++++++++ Backward Pass and Optimization +++++++++++++++++

    def backward(self) -> None:
        topo = self._traverse_children()

        self.grad = np.ones(self.data.shape)
        for node in reversed(topo):
            node._backward()

    def zero_grad(self) -> None:
        topo = [self]
        topo.extend(self._traverse_children())

        for node in reversed(topo):
            node.grad = np.zeros(node.data.shape)

    def step(self, learning_rate: float) -> None:
        topo = [self]
        topo.extend(self._traverse_children())

        for node in reversed(topo):
            if node.req_grad:
                node.data -= node.grad * learning_rate

    def _traverse_children(self) -> list:
        topo, visited = [], set()

        def build_topo(node):
            if node not in visited:
                visited.add(node)
                for child in node._prev:
                    build_topo(child)
                topo.append(node)

        build_topo(self)
        return topo

    def __repr__(self) -> str:
        return f'Tensor(data={self.data}, grad={self.grad}, label={self.label})'
