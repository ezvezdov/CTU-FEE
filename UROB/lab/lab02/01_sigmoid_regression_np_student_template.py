import matplotlib.pyplot as plt
import numpy as np

#######################################################
# sigmoid regression by gradient descent in numpy     #
# zimmerk@fel.cvut.cz                                 #
# Robot learning (UROB)                               #
#######################################################


# Optionally, you can define your own points
# plt.clf()
# plt.axis([-1., 1., 0, 1.])
# pts = np.array(plt.ginput(5))

pts = np.load('pts.npy')
x = pts[:, 0]
y = pts[:, 1]


def sigmoid(z):
    return 1 / (1 + np.exp(-z))


def loss_function(x, y, w):
    loss, grad = 0, np.zeros(2)
    for i in range(len(x)):
        u = w[0] * x[i] + w[1]
        z = sigmoid(u)
        loss += np.power(z - y[i],2) # .... <- FILL THIS IN (compute loss from i-th sample x[i], y[i])
        grad += 2 * (z - y[i]) * z * (np.ones_like(z) - z) * np.array([x[i],1]) # .... <- FILL THIS IN (compute grad of loss wrt w from i-th sample x[i], y[i])
    return loss, grad


w = np.array([-5, 0.3])

for i in range(50):
    loss, grad = loss_function(x, y, w)
    print(i, 'w=', w, 'loss=', loss)
    # w = .... <- FILL THIS IN (subtract gradient from w)
    w -= grad

    # visualize
    T = np.linspace(-1, 1, 50)
    plt.figure(1), plt.clf()
    plt.plot(x, y, markersize=10, marker='x', color='r', linestyle='None')
    plt.plot(T, 1 / (1 + np.exp(-(w[0] * T + w[1]))), color='green')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.pause(0.01)
    plt.draw()
