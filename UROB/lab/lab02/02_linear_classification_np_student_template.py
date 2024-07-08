import matplotlib.pyplot as plt
import numpy as np

#######################################################
# 2D classification by gradient descent in numpy      #
# zimmerk@fel.cvut.cz                                 #
# Robot learning (UROB)                               #
#######################################################


# Optionally, you can define your own points
# plt.clf()
# plt.axis([-1., 1., 0, 1.])
# x = np.array(plt.ginput(10))
# y = np.array([1, 1, 1, 1, 1, 0, 0, 0, 0, 0])
# np.save('data_x', x)
# np.save('data_y', y)

x, y = np.load('data_x.npy'), np.load('data_y.npy')

def sigmoid(z):
    return 1 / (1 + np.exp(-z))

def loss_function(x, y, w):
    loss, grad = 0, np.zeros(3)
    for i in range(len(x)):
        u = w[0] * x[i, 0] + w[1] * x[i, 1] + w[2]
        p = sigmoid(u)
        # loss +=  .... <- FILL THIS IN
        # grad +=  .... <- FILL THIS IN
    return loss, grad


w = np.array([-1.0, 1.0, 0.3])

for i in range(50):

    loss, grad = loss_function(x, y, w)
    print(i, 'w=', w, 'loss=', loss)
    w = w - 0.1 * grad

    # visualize
    eps = 1e-4
    T = np.linspace(-1, 1, 50)
    plt.figure(1), plt.clf()
    plt.plot(x[y==0, 0], x[y==0, 1], markersize=10, markeredgewidth = 3, marker='x', color='b', linestyle='None')
    plt.plot(x[y==1, 0], x[y==1, 1], markersize=10, markeredgewidth = 3, marker='o', color='r', linestyle='None')
    plt.plot(T, -(w[0]/(w[1]+eps) * T + w[2]/(w[1]+eps)), color='green', linewidth = 3)
    plt.xlabel('x_1')
    plt.ylabel('x_2')
    plt.xlim(0, 1)
    plt.ylim(0, 1)
    plt.grid(True)
    plt.pause(0.01)
    plt.draw()
