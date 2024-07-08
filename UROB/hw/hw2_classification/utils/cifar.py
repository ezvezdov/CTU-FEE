import os
import pickle
import subprocess
from typing import Tuple

import numpy as np
import matplotlib.pyplot as plt


def download_cifar10(directory: str) -> None:
    """Downloads and extracts the CIFAR-10 dataset.

    Args:
        directory (str): The directory where the dataset will be downloaded and extracted.

    Returns:
        None

    Note:
        If the dataset already exists in the specified directory, this function does nothing.
    """

    if os.path.exists(directory):
        print("CIFAR-10 dataset already exists")
        return

    # Download CIFAR-10 dataset
    url = "https://www.cs.toronto.edu/~kriz/cifar-10-python.tar.gz"
    tar = "cifar-10-python.tar.gz"
    subprocess.call(["wget", url])

    # Extract and move to the specified directory
    subprocess.call(["tar", "-xvzf", tar])
    subprocess.call(["mv", "cifar-10-batches-py", directory])
    subprocess.call(["rm", tar])


def load_cifar10_bach_file(file: str) -> Tuple[np.ndarray, np.ndarray]:
    """Loads a batch file of the CIFAR-10 dataset.

    Args:
        file (str): The path to the batch file to be loaded.

    Returns:
        X (numpy.ndarray): An array containing the image data.
        Y (numpy.ndarray): An array containing the corresponding labels.
    """

    with open(file, "rb") as f:
        datadict = pickle.load(f, encoding="latin1")
        X = datadict["data"]
        Y = datadict["labels"]
        X = X.reshape(10000, 3, 32, 32).transpose(0, 2, 3, 1).astype("float")
        Y = np.array(Y)
        return X, Y


def load_cifar10(directory: str) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
    """Loads the CIFAR-10 dataset from the specified directory.

    Args:
        directory (str): The directory where the CIFAR-10 dataset is located.

    Returns:
        X_train (numpy.ndarray): An array containing training images.
        y_train (numpy.ndarray): An array containing training labels.
        X_test (numpy.ndarray): An array containing test images.
        y_test (numpy.ndarray): An array containing test labels.
    """

    X_train, y_train = [], []
    for i in range(1, 6):
        file = os.path.join(directory, f"data_batch_{i}")
        X, y = load_cifar10_bach_file(file)
        X_train.append(X)
        y_train.append(y)
    X_train = np.concatenate(X_train)
    y_train = np.concatenate(y_train)
    X_test, y_test = load_cifar10_bach_file(os.path.join(directory, "test_batch"))
    return X_train, y_train, X_test, y_test


def visualize_cifar10(X: np.ndarray, y: np.ndarray) -> None:
    """Visualizes a random selection of CIFAR-10 dataset images.

    Args:
        X (numpy.ndarray): An array of images to be visualized.
        y (numpy.ndarray): An array of corresponding labels.

    Returns:
        None

    Note:
        This function displays a grid of images with their respective class labels.
    """

    classes = ['plane', 'car', 'bird', 'cat',
               'deer', 'dog', 'frog',
               'horse', 'ship', 'truck']

    num_classes = len(classes)
    samples_per_class = 7

    plt.figure(figsize=(num_classes, samples_per_class))

    for label, class_name in enumerate(classes):

        # Find indices of images for the current class.
        indices = np.flatnonzero(y == label)
        indices = np.random.choice(indices, samples_per_class, replace=False)

        for i, idx in enumerate(indices):
            plt_idx = i * num_classes + label + 1
            plt.subplot(samples_per_class, num_classes, plt_idx)
            plt.imshow(X[idx].astype('uint8'))
            plt.axis('off')
            if i == 0:
                plt.title(class_name)
    plt.show()


if __name__ == "__main__":
    download_cifar10("data/datasets/CIFAR10")
