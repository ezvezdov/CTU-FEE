import numpy as np
import scipy.io.wavfile as wav
from matplotlib import pyplot as plt
import warnings
warnings.filterwarnings("ignore")

def ar_fit_model(y: np.ndarray, p: int) -> np.ndarray:
    """Computes the parameters of the autogression model

    Args:
        y: np.ndarray: sound signal
        p: int: required order of AR model

    Return:
        np.ndarray: estimated parameters of AR model 

    Shape:
       - Input: (N,)
       - Output: (p+1,)
    """
    a = np.zeros(p+1)
    b = y[p:]
    
    M = np.ones((len(y)-p,p+1))

    # filling matrix M
    for t in range(p,len(y)):
        start = t-1
        end = t-p-1 if t-p-1 != -1 else None

        M[t-p][1::] = y[start:end:-1]
    

    # print("y shape:", np.shape(y))
    # print("M shape:",np.shape(M))
    # print("b shape:",np.shape(b))

    a, residuals, rank, s = np.linalg.lstsq(M, b.T, rcond=None)

    return a

def ar_predict(a: np.ndarray, y0: np.ndarray, N:int) -> np.ndarray:
    """ computes the rest of elements of y, starting from (p+1)-th 
        one up to N-th one. 

    Args:
        a: np.ndarray: estimated parameters of AR model
        y0: np.ndarray: beginning of sequence to be predicted
        N: int:  required length of predicted sequence, including the 
                 beginning represented by y0. 
    Return:
        np.ndarray: the predicted sequence 

    Shape:
       - Input: (p+1,), (p,)
       - Output: (N,)
    """
    y_pred = np.zeros(N)
    p = y0.shape[0]
    y_pred[:p] = y0

    for t in range(p,N):
        start = t-1
        end = t-p-1 if t-p-1 != -1 else None

        y_pred[t] = a[0] + y_pred[start:end:-1] @ (a[1::]).T
    
    return y_pred

if(__name__ == '__main__'):

    fs,y=wav.read('gong.wav')
    y = y.copy()/32767
    p = 300      # size of history considered for prediction
    N = len(y)   # length of the sequence
    K = 10000    # visualize just K first elements

    a = ar_fit_model(y, p)
    print(a)


    y0 = y[:p]
    y_pred = ar_predict(a, y0, N)

    wav.write('gong_predicted.wav', fs, y_pred)

    plt.plot(y[:K], 'b', label = 'original')
    plt.plot(y_pred[:K], 'r', label = 'AR model')
    plt.legend()
    plt.show()

