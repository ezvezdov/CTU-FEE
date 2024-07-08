from typing import List, Union

import yaml
import numpy as np
from jinja2 import Template
import matplotlib.pyplot as plt


def load_config(config_path: str, args: dict) -> dict:
    with open(config_path) as file:
        template = Template(file.read())
    config_text = template.render(**args)
    config = yaml.load(config_text, Loader=yaml.FullLoader)
    return config


def plot(ax: plt.Axes, X: Union[np.ndarray, List[np.ndarray]], Y: Union[np.ndarray, List[np.ndarray]],
         label_names: list = None, x_label: str = '',
         y_label: str = '', title: str = ''):
    assert len(X) == len(Y)
    legend = False if (label_names is None or len(X) == 1) else True

    for i in range(len(X)):
        label = label_names[i] if legend else None
        if X[i].size < 20:
            ax.plot(X[i], Y[i], linewidth=1.5, marker='o', label=label)
        else:
            ax.plot(X[i], Y[i], linewidth=1.5, label=label)
    adjust_plot(ax, X, x_label, y_label, title, legend=legend)


def error_bar(ax: plt.Axes, x: np.ndarray, y: np.ndarray, stds: np.ndarray, x_label: str = '',
              y_label: str = '', title: str = ''):
    ax.errorbar(x, y, yerr=stds, linestyle='dotted', linewidth=1.5, marker='o', capsize=4)
    adjust_plot(ax, x, x_label, y_label, title, legend=False)


def adjust_plot(ax: plt.Axes, X: Union[np.ndarray, List[np.ndarray]], x_label: str, y_label: str, title: str,
                legend: bool):
    unique_x = list(set(np.concatenate(X))) if isinstance(X, list) else X
    min_k, max_k = np.min(unique_x), np.max(unique_x)

    if legend:
        ax.legend()

    if len(unique_x) < 20:
        ax.set_xticks(unique_x)
        ax.set_xlim(min_k - 1, max_k + 0.35 * (max_k - min_k))

    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    ax.set_title(title)
    ax.grid(True)


def exponential_moving_average(data, alpha):
    ema = np.zeros(data.size)
    ema[0] = data[0]
    for i in range(1, data.size):
        weights = np.flip((1 - alpha) ** np.arange(i + 1))
        weighted_sum = np.sum(weights * data[:i + 1])
        ema[i] = weighted_sum / np.sum(weights)
    return ema


def dynamic_ema(data):
    """ Calculate the exponential moving average with alpha based on the standard deviation.
    of the data over a moving window.
    """
    mean_std = mean_standard_deviation(data, 100)
    alpha = 2 / (mean_std + 1)
    print(f"alpha = {alpha}")
    return exponential_moving_average(data, alpha)


def mean_standard_deviation(data, window):
    """ Compute the standard deviation over a moving window.
    Then compute the mean of the standard deviation over a moving window.
    """
    stds = np.zeros(data.size)
    for i in range(data.size):
        stds[i] = np.std(data[max(0, i - window):i + 1])
    return np.max(stds)


def plot_nn_training(loss_history: dict, accuracy_history: dict, ema: bool = False, alpha: float = 0.1):
    """Show the training history of a neural network.

    Args:
        loss_history (dict): A dictionary containing the training loss history.
        accuracy_history (dict): A dictionary containing the training accuracy history.
        ema (bool, optional): Whether to use exponential moving average. Defaults to False.
        alpha (float, optional): The alpha value for exponential moving average. Defaults to 0.1.

    Returns:
        None
    """
    train_loss_iters = np.array(list(loss_history["train"].keys()))
    train_losses = np.array(list(loss_history["train"].values()))

    val_loss_iters = np.array(list(loss_history["val"].keys()))
    val_losses = np.array(list(loss_history["val"].values()))

    train_acc_iters = np.array(list(accuracy_history["train"].keys()))
    train_accs = np.array(list(accuracy_history["train"].values()))

    val_acc_iters = np.array(list(accuracy_history["val"].keys()))
    val_accs = np.array(list(accuracy_history["val"].values()))

    if ema:
        train_losses = exponential_moving_average(train_losses, alpha)
        val_losses = exponential_moving_average(val_losses, alpha)
        train_accs = exponential_moving_average(train_accs, alpha)
        val_accs = exponential_moving_average(val_accs, alpha)

    X_acc = [train_acc_iters, val_acc_iters]
    Y_acc = [train_accs, val_accs]

    X_loss = [train_loss_iters, val_loss_iters]
    Y_loss = [train_losses, val_losses]

    fig, axes = plt.subplots(1, 2, figsize=(12, 4))

    plot(axes[0], X_loss, Y_loss, ['Training', 'Validation'],
         'Iterations', 'Loss', 'Loss History')
    plot(axes[1], X_acc, Y_acc, ['Training', 'Validation'],
         'Iterations', 'Accuracy', 'Accuracy History')

    plt.show()


def show_cross_validation_knn(k_to_metrics: dict, label_names: list = None):
    """Show the results of cross-validation on k.

    Args:
        k_to_metrics (dict): A dictionary containing the results of cross-validation.
        label_names (list): A list of labels for each class (optional).

    Returns:
        None
    """

    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 7))
    fig.suptitle('Cross-validation on k')

    # Plot accuracy on the first subplot
    k_acc = np.array([int(k) for k in k_to_metrics['accuracy'].keys()])
    acc_means = np.array([v for v in k_to_metrics['accuracy'].values()])
    plot(ax1, [k_acc], [acc_means], x_label='k', y_label='Accuracy')

    # Plot precision on the second subplot
    k_pre = np.array([int(k) for k in k_to_metrics['precision'].keys()])
    k_pre = [k_pre] * len(k_to_metrics['precision'][k_pre[0]])
    precision_means = np.hstack([v[..., np.newaxis] for v in k_to_metrics['precision'].values()])
    plot(ax2, k_pre, precision_means, label_names, x_label='k', y_label='Precision')

    # Plot recall on the third subplot
    k_rec = np.array([int(k) for k in k_to_metrics['recall'].keys()])
    k_rec = [k_rec] * len(k_to_metrics['recall'][k_rec[0]])
    recall_means = np.hstack([v[..., np.newaxis] for v in k_to_metrics['recall'].values()])
    plot(ax3, k_rec, recall_means, label_names, x_label='k', y_label='Recall')

    # Plot f1 on the fourth subplot
    k_f1 = np.array([int(k) for k in k_to_metrics['f1'].keys()])
    k_f1 = [k_f1] * len(k_to_metrics['f1'][k_f1[0]])
    f1_means = np.hstack([v[..., np.newaxis] for v in k_to_metrics['f1'].values()])
    plot(ax4, k_f1, f1_means, label_names, x_label='k', y_label='F1')

    plt.tight_layout()
    plt.show()
