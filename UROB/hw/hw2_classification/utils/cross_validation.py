import numpy as np
import matplotlib.pyplot as plt


def plot_metric(ax: plt.Axes, k_choices: np.ndarray, metric_means: np.ndarray,
                label_names: list, metric_name: str):
    """Plot a metric over different values of k.

    Args:
        ax (matplotlib.pyplot.Axes): The subplot where the metric will be plotted.
        k_choices (numpy.ndarray): An array of k values.
        metric_means (numpy.ndarray): A 2D array of metric values for different classes.
        label_names (list): A list of labels for each class (optional).
        metric_name (str): The name of the metric to be plotted.

    Returns:
        None
    """

    for i in range(metric_means.shape[0]):
        label = label_names[i] if label_names is not None else f'Class {i}'
        ax.plot(k_choices, metric_means[i], linewidth=1.5, marker='o', label=label)
    adjust_plot(ax, k_choices, metric_name)


def plot_error_bar(ax: plt.Axes, k_choices: np.ndarray, means: np.ndarray,
                   stds: np.ndarray, metric_name: str):
    """Plot a metric with error bars over different values of k.

    Args:
        ax (matplotlib.pyplot.Axes): The subplot where the metric will be plotted.
        k_choices (numpy.ndarray): An array of k values.
        means (numpy.ndarray): A 1D array of metric means.
        stds (numpy.ndarray): A 1D array of metric standard deviations.
        metric_name (str): The name of the metric to be plotted.

    Returns:
        None
    """
    ax.errorbar(k_choices, means, yerr=stds, linestyle='dotted', linewidth=1.5, marker='o', capsize=4)
    adjust_plot(ax, k_choices, metric_name, legend=False)


def adjust_plot(ax: plt.Axes, k_choices: np.ndarray, value_name: str, legend: bool = True):
    """Adjust the properties of a subplot.

    Args:
        ax (matplotlib.pyplot.Axes): The subplot to be adjusted.
        k_choices (numpy.ndarray): An array of k values.
        value_name (str): The name of the metric to be plotted.
        legend (bool): Whether to show the legend. Default is True.

    Returns:
        None
    """

    min_k, max_k = np.min(k_choices), np.max(k_choices)

    ax.set_xlim(min_k - 1, max_k + 0.35 * (max_k - min_k))
    ax.set_xticks(k_choices)
    ax.set_ylabel(value_name)
    ax.set_title(value_name)
    ax.set_xlabel('k')
    ax.grid(True)

    if legend:
        ax.legend()


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
    acc_means = np.array([np.mean(v) for v in k_to_metrics['accuracy'].values()])
    acc_stds = np.array([np.std(v) for v in k_to_metrics['accuracy'].values()])
    plot_error_bar(ax1, k_acc, acc_means, acc_stds, 'Accuracy')

    # Plot precision on the second subplot
    k_pre = np.array([int(k) for k in k_to_metrics['precision'].keys()])
    precision_means = np.hstack([v[..., np.newaxis] for v in k_to_metrics['precision'].values()])
    plot_metric(ax2, k_pre, precision_means, label_names, 'Precision')

    # Plot recall on the third subplot
    k_rec = np.array([int(k) for k in k_to_metrics['recall'].keys()])
    recall_means = np.hstack([v[..., np.newaxis] for v in k_to_metrics['recall'].values()])
    plot_metric(ax3, k_rec, recall_means, label_names, 'Recall')

    # Plot f1 on the fourth subplot
    k_f1 = np.array([int(k) for k in k_to_metrics['f1'].keys()])
    f1_means = np.hstack([v[..., np.newaxis] for v in k_to_metrics['f1'].values()])
    plot_metric(ax4, k_f1, f1_means, label_names, 'F1')

    plt.tight_layout()
    plt.show()
