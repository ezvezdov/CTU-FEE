#!/usr/bin/python
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
from scipy import ndimage


def perceptron(X, y, max_iterations):
    """
    w, b = perceptron(X, y, max_iterations)

    Perceptron algorithm.
    Implements the perceptron algorithm
    (http://en.wikipedia.org/wiki/Perceptron)

    :param X:               d-dimensional observations, (d, number_of_observations) np array
    :param y:               labels of the observations (0 or 1), (n,) np array
    :param max_iterations:  number of algorithm iterations (scalar)
    :return w:              w - weights, (d,) np array
    :return b:              b - bias, python float
    """

    # https://youtu.be/4Gac5I64LM4?si=Ur4E-cvSn3WVLIjL
    z = np.vstack((X,np.ones((1,X.shape[1]))))
    z[:,y == 1] = -lz[:,y == 1]

    v = np.zeros(X.shape[0] + 1)
    w = np.nan
    b = np.nan
    
    for i in range(max_iterations):
        z_p = v.T @ z

        not_sat_zt = z[:,z_p <= 0]

        if not_sat_zt.shape[1] == 0:
            w = v[:-1]
            b = v[-1]
            break

        upd_zt = not_sat_zt[:,0]
        v+= upd_zt
        print(v)

    return w, b


def lift_dimension(X):
    """
    Z = lift_dimension(X)

    Lifts the dimensionality of the feature space from 2 to 5 dimensions

    :param X:   observations in the original space
                2-dimensional observations, (2, number_of_observations) np array
    :return Z:  observations in the lifted feature space, (5, number_of_observations) np array
    """

    Z = np.zeros((X.shape[0]+3,X.shape[1]))
    Z[:2,:] = X
    Z[2,:]  = X[0,:] ** 2
    Z[3,:] = X[0,:] * X[1,:]
    Z[4,:] = X[1,:] ** 2

    return Z


def classif_quadrat_perc(tst, model):
    """
    K = classif_quadrat_perc(tst, model)

    Classifies test samples using the quadratic discriminative function

    :param tst:     2-dimensional observations, (2, n) np array
    :param model:   dictionary with the trained perceptron classifier (parameters of the discriminative function)
                        model['w'] - weights vector, np array (d, )
                        model['b'] - bias term, python float
    :return:        Y - classification result (contains either 0 or 1), (n,) np array
    """

    w = model['w']
    b = model['b']

    X = lift_dimension(tst)

    Q = w.T @ X + b
    Y = np.where(Q < 0, 1, 0)

    return Y



################################################################################
#####                                                                      #####
#####             Below this line are already prepared methods             #####
#####                                                                      #####
################################################################################

def pboundary(X, y, model, figsize=None, style_0='bx', style_1='r+'):
    """
    pboundary(X, y, model)

    Plot boundaries for perceptron decision strategy

    :param X:       d-dimensional observations, (d, number_of_observations) np array
    :param y:       labels of the observations (0 or 1), (n,) np array
    :param model:   dictionary with the trained perceptron classifier (parameters of the discriminative function)
                        model['w'] - weights vector, np array (d, )
                        model['b'] - bias term, python float
    """

    plt.figure(figsize=figsize)
    plt.plot(X[0, y == 0], X[1, y == 0], style_0, ms=10)
    plt.plot(X[0, y == 1], X[1, y == 1], style_1, ms=10)

    minx, maxx = plt.xlim()
    miny, maxy = plt.ylim()

    epsilon = 0.1 * np.maximum(np.abs(maxx - minx), np.abs(maxy - miny))

    x_space = np.linspace(minx - epsilon, maxx + epsilon, 1000)
    y_space = np.linspace(miny - epsilon, maxy + epsilon, 1000)
    x_grid, y_grid = np.meshgrid(x_space, y_space)

    x_grid_fl = x_grid.reshape([1, -1])
    y_grid_fl = y_grid.reshape([1, -1])

    X_grid = np.concatenate([x_grid_fl, y_grid_fl], axis=0)
    Y_grid = classif_quadrat_perc(X_grid, model)
    Y_grid = Y_grid.reshape([1000, 1000])

    blurred_Y_grid = ndimage.gaussian_filter(Y_grid, sigma=0)

    plt.contour(x_grid, y_grid, blurred_Y_grid, colors=['black'])
    plt.xlim(minx, maxx)
    plt.ylim(miny, maxy)


################################################################################
#####                                                                      #####
#####             Below this line you may insert debugging code            #####
#####                                                                      #####
################################################################################

def main():
    # HERE IT IS POSSIBLE TO ADD YOUR TESTING OR DEBUGGING CODE
    pass

if __name__ == "__main__":
    main()
