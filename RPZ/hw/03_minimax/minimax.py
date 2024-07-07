#!/usr/bin/python
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
import matplotlib.patches as mpatches
from scipy.stats import norm
import copy

import scipy.optimize as opt
# importing bayes doesn't work in BRUTE :(, please copy the functions into this file


def minimax_strategy_discrete(distribution1, distribution2):
    """
    q = minimax_strategy_discrete(distribution1, distribution2)

    Find the optimal Minimax strategy for 2 discrete distributions.

    :param distribution1:           pXk(x|class1) given as a (n, n) np array
    :param distribution2:           pXk(x|class2) given as a (n, n) np array
    :return q:                      optimal strategy, (n, n) np array, values 0 (class 1) or 1 (class 2)
    :return: opt_i:                 index of the optimal solution found during the search, Python int in [0, n*n + 1) range
    :return: eps1:                  cumulative error on the first class for all thresholds, (n * n + 1,) numpy array
    :return: eps2:                  cumulative error on the second class for all thresholds, (n * n + 1,) numpy array
    """
    # Rank matrix
    r = np.divide(distribution1,distribution2)

    # Sort indexes
    indexes = np.argsort(r, axis=None)
    indexes2d = np.array(np.unravel_index(indexes, r.shape))
    row_indexes,col_indexes = indexes2d[:,np.arange(0,indexes2d.shape[1])]
    
    # Init vectors
    eps1 = np.zeros(distribution1.shape[0]* distribution1.shape[0] + 1)
    eps2 = np.ones(distribution1.shape[0]* distribution1.shape[0] + 1)
    q = np.zeros_like(distribution1,dtype=np.int32)

    # Calculate vectors
    eps1[1:] = distribution1[row_indexes,col_indexes]
    eps1 = np.cumsum(eps1)
    
    eps2[1:] = -distribution2[row_indexes,col_indexes]
    eps2 = np.cumsum(eps2)

    q[row_indexes,col_indexes] = np.array(eps1[1:] < eps2[:-1], dtype=np.int32)

    opt_i = np.count_nonzero(q)

    return q, opt_i, eps1, eps2


def classify_discrete(imgs, q):
    """
    function label = classify_discrete(imgs, q)

    Classify images using discrete measurement and strategy q.

    :param imgs:    test set images, (h, w, n) uint8 np array
    :param q:       strategy (21, 21) np array of 0 or 1
    :return:        image labels, (n, ) np array of 0 or 1
    """

    lr = np.array(compute_measurement_lr_discrete(imgs) + 10,dtype=np.int32)
    ul = np.array(compute_measurement_ul_discrete(imgs) + 10,dtype=np.int32)
    labels = q[lr,ul]

    return labels


def worst_risk_cont(distribution_A, distribution_B, true_A_prior):
    """
    Find the optimal bayesian strategy for true_A_prior (assuming 0-1 loss) and compute its worst possible risk in case the priors are different.

    :param distribution_A:          parameters of the normal dist.
                                    distribution_A['Mean'], distribution_A['Sigma'] - python floats
    :param distribution_B:          the same as distribution_A
    :param true_A_prior:            true A prior probability - python float
    :return worst_risk:             worst possible bayesian risk when evaluated with different prior
    """

    distribution_A['Prior'] = true_A_prior
    distribution_B['Prior'] = 1 - true_A_prior
    q = find_strategy_2normal(distribution_A, distribution_B)

    distribution_A['Prior'] = 1
    distribution_B['Prior'] = 0
    risk1 = bayes_risk_2normal(distribution_A, distribution_B, q) # eps2

    distribution_A['Prior'] = 0
    distribution_B['Prior'] = 1
    risk2 = bayes_risk_2normal(distribution_A, distribution_B, q) # eps1

    worst_risk = np.maximum(risk1,risk2)
    
    return worst_risk


def minimax_strategy_cont(distribution_A, distribution_B):
    """
    q, worst_risk = minimax_strategy_cont(distribution_A, distribution_B)

    Find minimax strategy.

    :param distribution_A:  parameters of the normal dist.
                            distribution_A['Mean'], distribution_A['Sigma'] - python floats
    :param distribution_B:  the same as distribution_A
    :return q:              strategy dict - see bayes.find_strategy_2normal
                               q['t1'], q['t2'] - decision thresholds - python floats
                               q['decision'] - (3, ) np.int32 np.array decisions for intervals (-inf, t1>, (t1, t2>, (t2, inf)
    :return worst_risk      worst risk of the minimax strategy q - python float
    """

    def get_worst_risk_prior(prior):
        return worst_risk_cont(distribution_A,distribution_B,prior)

    prior = opt.fminbound(get_worst_risk_prior,0,1)
    distribution_A['Prior'] = prior
    distribution_B['Prior'] = 1 - prior

    q = find_strategy_2normal(distribution_A, distribution_B)
    worst_risk = worst_risk_cont(distribution_A,distribution_B,prior)

    return q, worst_risk


def risk_fix_q_cont(distribution_A, distribution_B, distribution_A_priors, q):
    """
    Computes bayesian risks for fixed strategy and various priors.

    :param distribution_A:          parameters of the normal dist.
                                    distribution_A['Mean'], distribution_A['Sigma'] - python floats
    :param distribution_B:          the same as distribution_A
    :param distribution_A_priors:   priors (n, ) np.array
    :param q:                       strategy dict - see bayes.find_strategy_2normal
                                       q['t1'], q['t2'] - decision thresholds - python floats
                                       q['decision'] - (3, ) np.int32 np.array decisions for intervals (-inf, t1>, (t1, t2>, (t2, inf)
    :return risks:                  bayesian risk of the strategy q with varying priors (n, ) np.array
    """

    risks = np.zeros_like(distribution_A_priors)
    for i in range(len(distribution_A_priors)):
        distribution_A['Prior'] = distribution_A_priors[i]
        distribution_B['Prior'] = 1 - distribution_A['Prior']
        risks[i] = bayes_risk_2normal(distribution_A, distribution_B, q)
    
    return risks


################################################################################
#####                                                                      #####
#####                Put functions from previous labs here.                #####
#####            (Sorry, we know imports would be much better)             #####
#####                                                                      #####
################################################################################

def classification_error(predictions, labels):
    """
    error = classification_error(predictions, labels)

    :param predictions: (n, ) np.array of values 0 or 1 - predicted labels
    :param labels:      (n, ) np.array of values 0 or 1 - ground truth labels
    :return:            error - classification error ~ a fraction of predictions being incorrect
                        python float in range <0, 1>
    """

    difference = np.abs(predictions - labels)
    error = np.sum(difference) / labels.shape[0]
    return error


def find_strategy_2normal(distribution_A, distribution_B):
    """
    q = find_strategy_2normal(distribution_A, distribution_B)

    Find optimal bayesian strategy for 2 normal distributions and zero-one loss function.

    :param distribution_A:  parameters of the normal dist.
                            distribution_A['Mean'], distribution_A['Sigma'], distribution_A['Prior'] - python floats
    :param distribution_B:  the same as distribution_A

    :return q:              strategy dict
                               q['t1'], q['t2'] - decision thresholds - python floats
                               q['decision'] - (3, ) np.int32 np.array decisions for intervals (-inf, t1>, (t1, t2>, (t2, inf)
                               If there is only one threshold, q['t1'] should be equal to q['t2'] and the middle decision should be 0
                               If there is no threshold, q['t1'] and q['t2'] should be -/+ infinity and all the decision values should be the same
                                (0 preferred if both strategies would have the same risk)
    """

    s_A = distribution_A['Sigma']
    m_A = distribution_A['Mean']
    p_A = distribution_A['Prior']
    s_B = distribution_B['Sigma']
    m_B = distribution_B['Mean']
    p_B = distribution_B['Prior']

    q = {}
    q['decision'] = np.zeros(3,dtype=np.int32)

    # extreme priors
    eps = 1e-10
    if p_A < eps:
        q['t1'] = -np.inf
        q['t2'] = np.inf
    elif p_B < eps:
        q['t1'] = -np.inf
        q['t2'] = np.inf
    else:
        a = s_A**2 - s_B**2
        b = 2 * m_A * s_B**2 - 2 *m_B * s_A**2
        c = m_B**2 * s_A**2 - m_A**2 * s_B**2 - np.log((p_B/p_A) *(s_A / s_B)) * 2 * s_A**2 * s_B**2
        if a == 0:
            # same sigmas -> not quadratic
            if b == 0:
                # same sigmas and same means -> not even linear
                q['t1'] = -np.inf
                q['t2'] = np.inf
            else:
                # same sigmas, different means -> linear equation
                q['t1'] = -c/b
                q['t2'] = -c/b
        else:
            # quadratic equation
            D = b**2 - 4 * a * c
            if D > 0:
                q['t1'] = (-b - np.sqrt(D)) / (2*a)
                q['t2'] = (-b + np.sqrt(D)) / (2*a)
                if(q['t1'] > q['t2']):
                    q['t1'],q['t2'] = q['t2'], q['t1']
            elif D == 0:
                q['t1'] = (-b - np.sqrt(D)) / (2*a)
                q['t2'] = q['t1']
            elif D < 0:
                q['t1'] = -np.inf
                q['t2'] = np.inf

    if q['t1'] == -np.inf:
        if p_B > p_A:
            q['decision'] = np.ones(3,dtype=np.int32)
        # else zeros as initialized
    else:
        vals = [q['t1']-10,np.random.uniform(q['t1'],q['t2']),q['t2']+10]
        for i in range(len(q['decision'])):
            cur_x = vals[i]
            res = a * cur_x**2 + b * cur_x + c
            q['decision'][i] = 0 if res >= 0 else 1
    return q


def bayes_risk_2normal(distribution_A, distribution_B, q):
    """
    R = bayes_risk_2normal(distribution_A, distribution_B, q)

    Compute bayesian risk of a strategy q for 2 normal distributions and zero-one loss function.

    :param distribution_A:  parameters of the normal dist.
                            distribution_A['Mean'], distribution_A['Sigma'], distribution_A['Prior'] python floats
    :param distribution_B:  the same as distribution_A
    :param q:               strategy
                               q['t1'], q['t2'] - float decision thresholds (python floats)
                               q['decision'] - (3, ) np.int32 np.array 0/1 decisions for intervals (-inf, t1>, (t1, t2>, (t2, inf)
    :return:    R - bayesian risk, python float
    """

    R = 0
    for i in range(len(q)):
        x1 = 0
        x2 = 0

        if i == 0:
            x1 = -np.inf
            x2 = q['t1']
        elif i == 1:
            x1 = q['t1']
            x2 = q['t2']
        elif i == 2:
            x1 = q['t2']
            x2 = np.inf
        
        dist = distribution_A if q['decision'][i] == 0 else distribution_B
        prob1 = dist['Prior'] * norm.cdf(x1,loc=dist['Mean'],scale=dist['Sigma'])
        prob2 = dist['Prior'] * norm.cdf(x2,loc=dist['Mean'],scale=dist['Sigma'])
        R += (prob2-prob1)

    return 1-R


def classify_2normal(measurements, q):
    """
    label = classify_2normal(measurements, q)

    Classify images using continuous measurements and strategy q.

    :param imgs:    test set measurements, np.array (n, )
    :param q:       strategy
                    q['t1'] q['t2'] - float decision thresholds
                    q['decision'] - (3, ) int32 np.array decisions for intervals (-inf, t1>, (t1, t2>, (t2, inf)
    :return:        label - classification labels, (n, ) int32
    """
    label = np.zeros_like(measurements,dtype=np.int32)
    for i in range(len(measurements)):
        if measurements[i] < q['t1']:
            label[i] = q['decision'][0]
        elif measurements[i] > q['t2']:
            label[i] = q['decision'][2]
        else:
            label[i] = q['decision'][1]
    
    return label


################################################################################
#####                                                                      #####
#####             Below this line are already prepared methods             #####
#####                                                                      #####
################################################################################


def plot_lr_threshold(eps1, eps2, thr):
    """
    Plot the search for the strategy

    :param eps1:  cumulative error on the first class for all thresholds, (N + 1, ) numpy array
    :param eps2:  cumulative error on the second class for all thresholds, (N + 1, ) numpy array
    :param thr:   index of the optimal solution found during the search, Python int in [0, N+1) range
    :return:      matplotlib.pyplot figure
    """

    fig = plt.figure(figsize=(15, 5))
    plt.plot(eps2, 'o-', label='$\epsilon_2$')
    plt.plot(eps1, 'o-', label='$\epsilon_1$')
    plt.plot([thr, thr], [-0.02, 1], 'k')
    plt.legend()
    plt.ylabel('classification error')
    plt.xlabel('i')
    plt.title('minimax - LR threshold search')
    plt.gca().xaxis.set_major_locator(MaxNLocator(integer=True))

    # inset axes....
    ax = plt.gca()
    axins = ax.inset_axes([0.4, 0.2, 0.4, 0.6])
    axins.plot(eps2, 'o-')
    axins.plot(eps1, 'o-')
    axins.plot([thr, thr], [-0.02, 1], 'k')
    axins.set_xlim(thr - 10, thr + 10)
    axins.set_ylim(-0.02, 1)
    axins.xaxis.set_major_locator(MaxNLocator(integer=True))
    axins.set_title('zoom in')
    # ax.indicate_inset_zoom(axins)

    return fig


def plot_discrete_strategy(q, letters):
    """
    Plot for discrete strategy

    :param q:        strategy (21, 21) np array of 0 or 1
    :param letters:  python string with letters, e.g. 'CN'
    :return:         matplotlib.pyplot figure
    """
    fig = plt.figure()
    im = plt.imshow(q, extent=[-10,10,10,-10])
    values = np.unique(q)   # values in q
    # get the colors of the values, according to the colormap used by imshow
    colors = [im.cmap(im.norm(value)) for value in values]
    # create a patch (proxy artist) for every color
    patches = [ mpatches.Patch(color=colors[i], label="Class {}".format(letters[values[i]])) for i in range(len(values))]
    # put those patched as legend-handles into the legend
    plt.legend(handles=patches, bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0. )
    plt.gca().xaxis.set_major_locator(MaxNLocator(integer=True))
    plt.gca().yaxis.set_major_locator(MaxNLocator(integer=True))
    plt.ylabel('X')
    plt.xlabel('Y')

    return fig


def compute_measurement_lr_cont(imgs):
    """
    x = compute_measurement_lr_cont(imgs)

    Compute measurement on images, subtract sum of right half from sum of
    left half.

    :param imgs:    set of images, (h, w, n) numpy array
    :return x:      measurements, (n, ) numpy array
    """
    assert len(imgs.shape) == 3

    width = imgs.shape[1]
    sum_rows = np.sum(imgs, dtype=np.float64, axis=0)

    x = np.sum(sum_rows[0:int(width / 2),:], axis=0) - np.sum(sum_rows[int(width / 2):,:], axis=0)

    assert x.shape == (imgs.shape[2], )
    return x


def compute_measurement_lr_discrete(imgs):
    """
    x = compute_measurement_lr_discrete(imgs)

    Calculates difference between left and right half of image(s).

    :param imgs:    set of images, (h, w, n) (or for color images (h, w, 3, n)) np array
    :return x:      measurements, (n, ) np array of values in range <-10, 10>,
    """
    assert len(imgs.shape) in (3, 4)
    assert (imgs.shape[2] == 3 or len(imgs.shape) == 3)

    mu = -563.9
    sigma = 2001.6

    if len(imgs.shape) == 3:
        imgs = np.expand_dims(imgs, axis=2)

    imgs = imgs.astype(np.int32)
    height, width, channels, count = imgs.shape

    x_raw = np.sum(np.sum(np.sum(imgs[:, 0:int(width / 2), :, :], axis=0), axis=0), axis=0) - \
            np.sum(np.sum(np.sum(imgs[:, int(width / 2):, :, :], axis=0), axis=0), axis=0)
    x_raw = np.squeeze(x_raw)

    x = np.atleast_1d(np.round((x_raw - mu) / (2 * sigma) * 10))
    x[x > 10] = 10
    x[x < -10] = -10

    assert x.shape == (imgs.shape[-1], )
    return x


def compute_measurement_ul_discrete(imgs):
    """
    x = compute_measurement_ul_discrete(imgs)

    Calculates difference between upper and lower half of image(s).

    :param imgs:    set of images, (h, w, n) (or for color images (h, w, 3, n)) np array
    :return x:      measurements, (n, ) np array of values in range <-10, 10>,
    """
    assert len(imgs.shape) in (3, 4)
    assert (imgs.shape[2] == 3 or len(imgs.shape) == 3)

    mu = -563.9
    sigma = 2001.6

    if len(imgs.shape) == 3:
        imgs = np.expand_dims(imgs, axis=2)

    imgs = imgs.astype(np.int32)
    height, width, channels, count = imgs.shape

    x_raw = np.sum(np.sum(np.sum(imgs[0:int(height / 2), :, :, :], axis=0), axis=0), axis=0) - \
            np.sum(np.sum(np.sum(imgs[int(height / 2):, :, :, :], axis=0), axis=0), axis=0)
    x_raw = np.squeeze(x_raw)

    x = np.atleast_1d(np.round((x_raw - mu) / (2 * sigma) * 10))
    x[x > 10] = 10
    x[x < -10] = -10

    assert x.shape == (imgs.shape[-1], )
    return x


def create_test_set(images_test, labels_test, letters, alphabet):
    """
    images, labels = create_test_set(images_test, letters, alphabet)

    Return subset of the <images_test> corresponding to <letters>

    :param images_test: test images of all letter in alphabet - np.array (h, w, n)
    :param labels_test: labels for images_test - np.array (n,)
    :param letters:     python string with letters, e.g. 'CN'
    :param alphabet:    alphabet used in images_test - ['A', 'B', ...]
    :return images:     images - np array (h, w, n)
    :return labels:     labels for images, np array (n,)
    """

    images = np.empty((images_test.shape[0], images_test.shape[1], 0), dtype=np.uint8)
    labels = np.empty((0,))
    for i in range(len(letters)):
        letter_idx = np.where(alphabet == letters[i])[0]
        images = np.append(images, images_test[:, :, labels_test == letter_idx], axis=2)
        lab = labels_test[labels_test == letter_idx]
        labels = np.append(labels, np.ones_like(lab) * i, axis=0)

    return images, labels


def show_classification(test_images, labels, letters):
    """
    show_classification(test_images, labels, letters)

    create montages of images according to estimated labels

    :param test_images:     np.array (h, w, n)
    :param labels:          labels for input images np.array (n,)
    :param letters:         string with letters, e.g. 'CN'
    """

    def montage(images, colormap='gray'):
        """
        Show images in grid.

        :param images:      np.array (h, w, n)
        :param colormap:    numpy colormap
        """
        h, w, count = np.shape(images)
        h_sq = int(np.ceil(np.sqrt(count)))
        w_sq = h_sq
        im_matrix = np.zeros((h_sq * h, w_sq * w))

        image_id = 0
        for j in range(h_sq):
            for k in range(w_sq):
                if image_id >= count:
                    break
                slice_w = j * h
                slice_h = k * w
                im_matrix[slice_h:slice_h + w, slice_w:slice_w + h] = images[:, :, image_id]
                image_id += 1
        plt.imshow(im_matrix, cmap=colormap)
        plt.axis('off')
        return im_matrix

    for i in range(len(letters)):
        imgs = test_images[:,:,labels==i]
        subfig = plt.subplot(1,len(letters),i+1)
        montage(imgs)
        plt.title(letters[i])


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
