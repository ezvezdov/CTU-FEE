import torch
import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np

#######################################################
# gradient descent on quadratic landscape             #
# zimmerk@fel.cvut.cz                                 #
# Vision for Robotics (VIR)                           #
#######################################################

fig, ax = plt.subplots(subplot_kw={"projection": "3d"})

w = torch.tensor([-4, 4], requires_grad=True, dtype=torch.double)
A = torch.tensor([[1, 0],[0, 4]], requires_grad=False, dtype=torch.double)
for i in range(20):
    loss = 0.5 * w.T @ A @ w
    loss.backward()
    with torch.no_grad():
        w -= torch.tensor([1, 0.25]) * w.grad
    w.grad.zero_()
    print(i, w, loss.detach().numpy())

    # visualize result
    W = w.detach().numpy()
    Anp = A.detach().numpy()
    plt.figure(1)
    ax = plt.axes(projection='3d')
    W0,W1 = np.meshgrid(np.arange(-5,5,0.5), np.arange(-5,5,0.5))
    LOSS = np.zeros_like(W0, dtype = np.float64)
    for k in range(LOSS.shape[0]):
        for j in range(LOSS.shape[1]):
            LOSS[k,j] = 0.5 * (W0[k,j].T * Anp[0,0] * W0[k,j] + W0[k,j].T * Anp[0,1] * W1[k,j] + W1[k,j].T * Anp[1,0] * W0[k,j] + W1[k,j].T * Anp[1,1] * W1[k,j])

    surf = ax.plot_surface(W0, W1, LOSS,  cmap=cm.jet, linewidth= 0, antialiased= False, alpha=0.5)

    LOSS_W = (0.5 * w.T @ A @ w).detach().numpy()
    ax.scatter(W[0], W[1], LOSS_W, s=100, linewidth = 5, marker='x', color='b')
    ax.set_xlim3d(-5, 5)
    ax.set_ylim3d(-5, 5)
    ax.set_xlabel('w[0]')
    ax.set_ylabel('w[1]')
    ax.set_zlabel('w.T @ A @ w')
    plt.pause(0.01)
    plt.draw()
