import sys
import numpy as np


def test_assignment_2_1(src_dir: str, verification_file: str, seed: int = 69, generate: bool = False) -> dict:
    sys.path.append(src_dir)
    from assignments.preprocessing import reshape_to_vectors

    ret = {
        "points": 0,
        "message": "",
        "max_points": 1
    }

    np.random.seed(seed)

    data = np.random.rand(10, 3, 4, 5, 6)

    try:
        reshaped_data = reshape_to_vectors(data)
    except Exception as e:
        ret["message"] = f"\tFAILED! \n\t{e}"
        return ret

    if generate:
        np.save(verification_file, reshaped_data)
        print(f"Successfully generated '{verification_file}'!")
    else:
        expected_data = np.load(verification_file)

        try:
            if np.allclose(reshaped_data, expected_data):
                ret["message"] = f"PASSED!"
                ret["points"] = ret["max_points"]
            else:
                ret["message"] = f"\tFAILED! \n\tThe reshaped data is of shape {reshaped_data.shape}, " \
                                 f"but should be of shape {expected_data.shape}."
        except Exception as e:
            ret["message"] = f"\tFAILED! \n\t{e}"

    return ret


def test_assignment_2_2(src_dir: str, verification_file: str, seed: int = 69, generate: bool = False) -> dict:
    sys.path.append(src_dir)
    from assignments.tuning import cross_validate_knn
    from assignments.knn_classifier import KNNClassifier

    ret = {
        "points": 0,
        "message": "",
        "max_points": 1
    }

    np.random.seed(seed)

    num_samples = 1000
    num_features = 2
    num_classes = 3

    num_folds = 4
    k_choices = np.array([1, 3, 5, 7])

    # Generate random dataset
    X = np.random.randn(num_samples, num_features)
    y = np.random.randint(num_classes, size=num_samples)

    try:
        classifier = KNNClassifier(k=3, vectorized=True)
        k_to_metrics = cross_validate_knn(classifier, X, y, k_choices, num_folds)
    except Exception as e:
        ret["message"] = f"\tFAILED! \n\t{e}"
        return ret

    if generate:
        np.save(verification_file, k_to_metrics)
        print(f"Successfully generated '{verification_file}'!")
    else:
        expected_k_to_metrics = np.load(verification_file, allow_pickle=True).item()

        def metric_difference(pred_k_to_metrics, expected_k_to_metrics, metric):
            pred_metric_values = pred_k_to_metrics[metric].values()
            expected_metric_values = expected_k_to_metrics[metric].values()
            differences = [np.sum(np.abs(m - exp_m)) for m, exp_m in zip(pred_metric_values, expected_metric_values)]
            return np.sum(differences)

        try:
            # Check every metric
            acc_diff = metric_difference(k_to_metrics, expected_k_to_metrics, 'accuracy')
            prec_diff = metric_difference(k_to_metrics, expected_k_to_metrics, 'precision')
            recall_diff = metric_difference(k_to_metrics, expected_k_to_metrics, 'recall')
            f1_diff = metric_difference(k_to_metrics, expected_k_to_metrics, 'f1')

            # Check if the differences are small enough
            if acc_diff < 1e-5 and prec_diff < 1e-5 and recall_diff < 1e-5 and f1_diff < 1e-5:
                ret["message"] = f"PASSED!"
                ret["points"] = ret["max_points"]
            else:
                ret["message"] = f"\tFAILED! \n\tThe difference between the metrics is " \
                                 f"acc: {acc_diff}, prec: {prec_diff}, recall: {recall_diff}, f1: {f1_diff}."
        except Exception as e:
            ret["message"] += f"\tFAILED! \n\t{e}"

    return ret
