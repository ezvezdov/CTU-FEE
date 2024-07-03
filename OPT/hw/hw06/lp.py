from scipy.optimize import linprog
import numpy as np
from matplotlib import pyplot as plt


def vyhra(c,k):
    # x1 | x10 | x0 |x02 | x2 | z
    lp_c = [0,0,0,0,0,-1]
    A_ub = [[-c[0],-c[1],0,0,0,1],
            [0,-c[1],-c[2],-c[3],0,1],
            [0,0,0,-c[3],-c[4],1],
            [-1,0,0,0,0,0],
            [0,-1,0,0,0,0],
            [0,0,-1,0,0,0],
            [0,0,0,-1,0,0],
            [0,0,0,0,-1,0]]
    b_ub = [0,0,0,0,0,0,0,0]
    A_eq = [[1,1,1,1,1,0]]
    b_eq = [k]
    x = linprog(lp_c,A_ub,b_ub,A_eq,b_eq)
    return x.x[:-1]

def vyhra2(c,k,m):
    # x1 | x0 | x2 | z
    lp_c = [0,0,0,-1]
    A_ub = [[-c[0],0,0,1],
            [0,-c[1],0,1],
            [0,0,-c[2],1]]
    b_ub = [0,0,0]
    A_eq = [[1,1,1,0]]
    b_eq = [k]
    bounds = [[m,None],[m,None],[m,None],[None,None]]
    x = linprog(lp_c,A_ub,b_ub,A_eq,b_eq,bounds)
    return x.x[:-1]

def minimaxfit(x,y):

    # a0 .. an b w r
    x = x.T
    A_ub = -np.ones((x.shape[0],x.shape[1]+2))
    A_ub[:,0:x.shape[1]] = x
    A_ub[:,-2] = np.ones(x.shape[0])
    
    A_ub2 = -np.ones((x.shape[0],x.shape[1]+2))
    A_ub2[:,0:x.shape[1]] = -x
    
    A_ub = np.append(A_ub,A_ub2,axis=0)

    b_ub = np.append(y[0],-y[0])

    c = np.zeros(x.shape[1]+2)
    c[-1] = 1

    bounds = [[None, None]] * (x.shape[1]+1)
    bounds.append([0,None])

    print("A",A_ub)
    print("b",b_ub)
    print("c",c)

    res = linprog(c,A_ub,b_ub,bounds=bounds)
    print(res)
    
    a = res.x[0: x.shape[1]]
    b = res.x[-2]
    r = res.fun
    
    return [a,b,r]

# def plotline(X,Y,a,b,r):
#     print("a"  ,a)
#     print("b",b)
    
#     print("r",r)
#     x = np.linspace(min(X[0])+5, max(X[0])+5, 1000)
#     y = a[0] * x + b

#     plt.plot(x,y)
#     plt.scatter(X[0],Y[0])
    
#     plt.show()

if __name__ == "__main__":
    ###### TEST 1 ######
    c = np.array([1.27, 1.02, 4.70, 3.09, 9.00])
    k = 3000
    x = vyhra(c,k)
    print("Vyhra result:",x)
    # x ~ [0, 2694, 0, 0, 305]

    ###### TEST 2 ######
    c = np.array([1.27, 4.70, 9.00])
    k = 3000
    m = 400
    x = vyhra2(c,k,m)
    print("Vyhra2 result:",x)
    # x ~ [2046,  553,  400]

    ###### TEST 3 ######
    x = np.array([[1, 2, 3, 3, 2], [4, 1, 2, 5, 6], [7,8,9, -5,7]])
    y = np.array([[7,4,1,2,5]])
    a, b, r = minimaxfit(x,y)

    ###### TEST 4 ######
    x = np.array([[4,-1,7,4,8]])
    y = np.array([[5,4,1,2,5]])
    a,b,r = minimaxfit(x,y)
    print(a,b,r)
    # plotline(x,y,a,b,r)
