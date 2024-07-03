import numpy as np

###################################################################################
# Predikce prumerne hrube mzdy
###################################################################################
def fit_wages(t,M):
    # Convert 1. -> 2.
    # 1. x1 + x2 * t = phm
    # 2. [1 t] * [x1 x2] = phm
    A = np.ones((len(M),2))
    A[:,1] = t

    # MNC
    x, residuals, rank, s = np.linalg.lstsq(A, M, rcond=None)
    return x

def quarter2_2009(x):
    t = 2009 + (2-1)/4
    return x[1] * t + x[0]

def test_mzda():
    # Load data
    data = np.loadtxt("materials/mzdy.txt")
    t = data[:,0]
    phm = data[:,1]

    (x1,x2) = fit_wages(t,phm)

    # Visualize
    import matplotlib.pyplot as plt
    _ = plt.plot(t, phm, 'o', label='Vstupni data', markersize=5)
    _ = plt.plot(t, x2*t + x1, 'r', label='Odhadnuta linearni funkce')
    _ = plt.legend()
    plt.show()

###################################################################################
# Interpolace denni teploty ve Svatonovicich
###################################################################################

def fit_temps(t,T,omega):
    A = np.ones((len(t),4))
    A[:,1] = t
    A[:,2] = np.sin(omega * t)
    A[:,3] = np.cos(omega * t)

    x, residuals, rank, s = np.linalg.lstsq(A, T, rcond=None)

    return x

def test_teplota():

    data = np.loadtxt("materials/teplota.txt")
    time = data[:,0]
    temp = data[:,1]

    omega = (2 * np.pi/365)
    (x1,x2,x3,x4) = fit_temps(time,temp, omega)

    import matplotlib.pyplot as plt
    _ = plt.plot(time, temp, 'o', label='Vstupni data', markersize=5)
    _ = plt.plot(time, x4 * np.cos(omega * time) + x3 * np.sin(omega * time) + x2*time + x1, 'r', label='Odhadnuta linearni funkce')
    _ = plt.legend()
    plt.show()