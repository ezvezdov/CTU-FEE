from typing import Tuple

import mpl_toolkits.mplot3d.axes3d as p3
import numpy as np
import scipy.io as sio
from matplotlib import animation
from matplotlib import patches as mpatches
from matplotlib import pyplot as plt


def set_axes_equal(ax):
    '''Make axes of 3D plot have equal scale so that spheres appear as spheres,
    cubes as cubes, etc..  This is one possible solution to Matplotlib's
    ax.set_aspect('equal') and ax.axis('equal') not working for 3D.

    Input
      ax: a matplotlib axis, e.g., as output from plt.gca().
    '''

    x_limits = ax.get_xlim3d()
    y_limits = ax.get_ylim3d()
    z_limits = ax.get_zlim3d()

    x_range = abs(x_limits[1] - x_limits[0])
    x_middle = np.mean(x_limits)
    y_range = abs(y_limits[1] - y_limits[0])
    y_middle = np.mean(y_limits)
    z_range = abs(z_limits[1] - z_limits[0])
    z_middle = np.mean(z_limits)

    plot_radius = 0.5*max([x_range, y_range, z_range])

    ax.set_xlim3d([x_middle - plot_radius, x_middle + plot_radius])
    ax.set_ylim3d([y_middle - plot_radius, y_middle + plot_radius])
    ax.set_zlim3d([z_middle - plot_radius, z_middle + plot_radius])

def playmotion(conn, A, B = None):
    fig = plt.figure()
    ax = p3.Axes3D(fig)
    ax.axis('off')

    conns = [x[x!=41] for x in np.split(conn, np.where(conn==41)[0]) if len(x[x!=41])]

    macAs = []
    macBs = []

    m,n = A.shape

    if(B is not None):
        B = B.reshape(3,m//3,n, order='F')
        for conn in conns:
            macBs.append(ax.plot(B[0,conn,0], B[1,conn,0], B[2,conn,0], marker='o', color='r')[0])

    A = A.reshape(3,m//3,n, order='F')
    for conn in conns:
        macAs.append(ax.plot(A[0,conn,0], A[1,conn,0], A[2,conn,0], marker='o', color='b')[0])

    fig.legend(handles =[mpatches.Patch(color='red', label='approximation'),mpatches.Patch(color='blue', label='GT')])
    set_axes_equal(ax)

    def update_points(i, A, B, macAs, macBs, conn):
        for conn, macA in zip(conns, macAs):
            macA.set_data(np.array(A[:2,conn,i]))
            macA.set_3d_properties(A[2,conn,i], 'z')
        for conn, macB in zip(conns, macBs):
            macB.set_data(np.array(B[:2,conn,i]))
            macB.set_3d_properties(B[2,conn,i], 'z')
        return macAs + macBs
    
    ani = animation.FuncAnimation(fig, update_points, n, fargs=(A, B, macAs, macBs, conns), interval=1)
    plt.show()

def fitlin(A: np.ndarray, k: int) -> Tuple[np.ndarray, np.ndarray]:
    """ computes the optimal linear fit of A
    
    Args:
        A: np.ndarray: matrix with points 
        k: np.ndarray: dimension of linear approx 

    Return:
        np.ndarray: orthonormal basis
        np.ndarray: coordinates w.r.t the basis 

    Shape:
       - Input: (m,n)
       - Output: (m,k), (k,n)
    """

    # spektralni rozklad
    L,V = np.linalg.eig(A @ A.T)
    
    # В ебучем сука нампае властни числа вообще не сержазены никак
    # Авторы настолько долбоебы, что я просто в ахуе с них, приходится
    # Прибегать к каким-то ну просто охуевшим костылям (в виде сортировки)
    # чтобы пофиксить их ебаную лень.
    idx = L.argsort()
    L = L[idx]
    V = V[:,idx]

    # ortonormalni baze a souradnice w.r.t. teto bazi
    U = V[:,-k:]
    C = U.T @ A
    
    return U,C

def fitaff(A: np.ndarray, k: int) -> Tuple[np.ndarray, np.ndarray, np.ndarray]:
    """ computes the optimal affine fit of A
    
    Args:
        A: np.ndarray: matrix with points 
        k: np.ndarray: dimension of affine approx 

    Return:
        np.ndarray: orthonormal basis
        np.ndarray: coordinates w.r.t the basis 
        np.ndarray: point of the affine space

    Shape:
       - Input: (m,n)
       - Output: (m,k), (k,n), (m,)
    """
    # teziste
    b0 = np.mean(A,1)

    # odecitame teziste
    A = A - b0.reshape(-1,1);

    # spektralni rozklad
    L,V = np.linalg.eig(A @ A.T)

    # sorting of eigenvalues 
    idx = L.argsort()
    L = L[idx]
    V = V[:,idx]

    # ortonormalni baze a souradnice w.r.t. teto bazi
    U = V[:,-k:]
    C = U.T @ A
    
    return U, C, b0

def erraff(A: np.ndarray) -> np.ndarray:
    """ computes the errors of affine approximations of A
    
    Args:
        A: np.ndarray: matrix with points 

    Return:
        np.ndarray: vector of errors

    Shape:
       - Input: (m,n)
       - Output: (m,)
    """

    m,n = A.shape

    # teziste
    b0 = np.mean(A,1)

    # odecteme teziste
    A = A - b0.reshape(-1,1)

    L,V = np.linalg.eig(A @ A.T)

    # sorting of eigenvalues 
    idx = L.argsort()
    L = L[idx]
    V = V[:,idx]

    d = np.zeros(m)

    for i in range(1,m):
        d[-i-1] = d[-i] + L[i-1]
        
    return d

def drawfitline(A: np.ndarray) -> None:
    """ draws the optimal line fitting points from A
    
    Args:
        A: np.ndarray: matrix with points 

    Shape:
       - Input: (2,n)
    """

    
    U,C = fitlin(A,1)
    B = U @ C

    min_index = np.argmin(B[0,:])
    max_index = np.argmax(B[0,:])
    point_min = B[:,min_index]
    point_max = B[:,max_index]

    


    plt.subplot(221)
    plt.plot([A[0,:],B[0,:]],[A[1,:],B[1,:]],color="red")
    plt.scatter(A[0,:],A[1,:], marker="x",color ="red")
    # plt.scatter(B[0,:],B[1,:], marker="x",color ="blue")
    plt.plot([point_min[0], point_max[0]], [point_min[1], point_max[1]], marker = 'o',color="green")
    
    plt.title('drawfitline')

def plottraj2(C: np.ndarray) -> None:
    """ draws the optimal line fitting points from A
    
    Args:
        A: np.ndarray: matrix with points 

    Shape:
       - Input: (2,n)
    """

    plt.subplot(222)
    plt.scatter(C[0,:],C[1,:],marker=",")
    plt.title('plottraj2')


if(__name__ == '__main__'):
    A = sio.loadmat('data/line.mat')['A']
    drawfitline(A)

    conn = np.loadtxt('data/connected_points.txt', comments='%', dtype=int)-1
    filename = 'makarena1.txt' # see the data folder and try more examples
    A = np.loadtxt('data/' + filename).T
    k = 2 # dimension of affine approximation

    U, C, b0 = fitaff(A,k)
    B = U@C+b0.reshape(-1,1)

    plottraj2(C[:2])

    plt.subplot(212)
    plt.semilogy(erraff(A))
    plt.xlabel('dimension')
    plt.ylabel('error, log scale')
    plt.title('Error of affine approximation \n for motion capture')

    plt.tight_layout()
    playmotion(conn, A, B)
