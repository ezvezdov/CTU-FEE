import numpy as np


def reshape_to_vectors(data: np.ndarray) -> np.ndarray:
    """
    Reshape the data to a 2D array of shape (N, D) where N is the number
    of data points and D is the dimensionality of each data point.

    Args:
        data (numpy.ndarray): The data to be reshaped of shape (N, D1, D2, ..., Dk).

    Returns:
        numpy.ndarray: The reshaped data of shape (N, D1 * D2 * ... * Dk).
    """

    reshaped_data = np.zeros_like(data)

    # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱ Assignment 2.1 ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
    # TODO:                                                             #
    # Implement the function that reshapes the data to a 2D array of    #
    # shape (N, D) where N is the number of data points and D is the    #
    # dimensionality of each data point.                                #
    #                                                                   #
    # Hint: Use the reshape function from numpy.                        #
    # Good luck!                                                        #
    # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
    # 🌀 INCEPTION 🌀 (Your code begins its journey here. 🚀 Do not delete this line.)

    reshaped_data = data.reshape((data.shape[0], np.prod(data.shape[1:])))
    
    # 🌀 TERMINATION 🌀 (Your code reaches its end. 🏁 Do not delete this line.)

    return reshaped_data


def normalize_data(data: np.ndarray) -> np.ndarray:
    """
    Normalize the data to have zero mean and unit variance.
    The normalization is done over the first dimension.

    Args:
        data (numpy.ndarray): The data to be normalized of shape (N, D).

    Returns:
        numpy.ndarray: The normalized data of shape (N, D).
    """

    normalized_data = np.zeros_like(data)

    # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱ Assignment 4.1 ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
    # TODO:                                                             #
    # Implement the function that normalizes the data to have zero mean #
    # and unit variance. The normalization is done over the first       #
    # dimension.                                                        #
    #                                                                   #
    # Good luck!                                                        #
    # ▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰▱▰ #
    # 🌀 INCEPTION 🌀 (Your code begins its journey here. 🚀 Do not delete this line.)
    
    normalized_data = (data - np.mean(data,axis=0)) / np.std(data,axis=0)

    # 🌀 TERMINATION 🌀 (Your code reaches its end. 🏁 Do not delete this line.)

    return normalized_data

