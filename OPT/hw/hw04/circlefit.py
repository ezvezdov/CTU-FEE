import scipy.io as sio
import numpy as np
from math import pi
from matplotlib import pyplot as plt
import random

def quad_to_center(d,e,f):
    x0 = -d/2
    y0 = -e/2
    r = np.abs(np.sqrt(x0**2 + y0**2 - f))
    return x0, y0, r

def fit_circle_nhom(X): 
    A = np.insert(X,2,1,axis=1)
    b = -( X[:,0]**2 + X[:,1]**2)
    x, residuals, rank, s = np.linalg.lstsq(A, b, rcond=None)
    return x

def fit_circle_hom(X):
    # Skripta: kapitola 7.3 (zacatek kapitoly, NE priklady)
    A = np.ones((X.shape[0],4))
    A[:,0] = X[:,0]**2 + X[:,1]**2
    A[:,1:3] = X

    L,V = np.linalg.eig(A.T @ A)

    # sort eigenvalues
    idx = L.argsort()
    L = L[idx]
    V = V[:,idx]

    x = V[:,0]

    return x[1:] / x[0]

def dist(X, x0, y0, r):
    return abs(r - np.linalg.norm(X-[x0,y0],axis=1))

def fit_circle_ransac(X, num_iter, threshold):
    # https://www.youtube.com/watch?v=9D5rrtCC_E0

    d,e,f = [0,0,0]
    inliers_amount = 0

    for _ in range(0,num_iter):
        points = np.array(random.choices(X,k=3))

        d_cur,e_cur,f_cur = fit_circle_hom(points)
        x0, y0, r = quad_to_center(d_cur,e_cur,f_cur)
    
        cur_inliers_amount = sum(dist(X,x0,y0,r) < threshold)

        if inliers_amount < cur_inliers_amount:
            inliers_amount = cur_inliers_amount
            d,e,f = [d_cur,e_cur,f_cur]

    x0, y0, r = quad_to_center(d,e,f)
    return x0, y0, r

def plot_circle(x0,y0,r, color, label):
    t = np.arange(0,2*pi,0.01)
    X = x0 + r*np.cos(t)
    Y = y0 + r*np.sin(t)
    plt.plot(X,Y, color=color, label=label)

if(__name__ == '__main__'):
    data = sio.loadmat('data.mat')
    X = data['X'] # only inliers
    A = data['A'] # X + outliers

    def_nh = fit_circle_nhom(X)
    x0y0r_nh = quad_to_center(*def_nh)
    dnh = dist(X, *x0y0r_nh)

    def_h = fit_circle_hom(X)
    x0y0r_h = quad_to_center(*def_h)
    dh = dist(X, *x0y0r_h)

    results = {'def_nh':def_nh, 'def_h':def_h, 
               'x0y0r_nh' : x0y0r_nh, 'x0y0r_h': x0y0r_nh,
               'dnh': dnh, 'dh':dh}
    
    GT = sio.loadmat('GT.mat')
    for key in results:
        print('max difference',  np.amax(np.abs(results[key] - GT[key])), 'in', key)


    x = fit_circle_ransac(A, 2000, 0.1)

    plt.figure(1)
    plt.subplot(121)
    plt.scatter(X[:,0], X[:,1], marker='.', s=3)
    plot_circle(*x0y0r_h, 'r', 'hom')
    plot_circle(*x0y0r_nh, 'b', 'nhom')
    plt.legend()
    plt.axis('equal')    
    plt.subplot(122)
    plt.scatter(A[:,0], A[:,1], marker='.', s=2)
    plot_circle(*x, 'y', 'ransac')
    plt.legend()
    plt.axis('equal')
    plt.show()
