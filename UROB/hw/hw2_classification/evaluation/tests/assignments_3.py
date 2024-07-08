import sys
import numpy as np


def test_assignment_3_1(src_dir: str, verification_file: str, seed: int = 69, generate: bool = False) -> dict:
    sys.path.append(src_dir)
    from assignments.linear_classifier import LinearClassifier

    ret = {
        "points": 0,
        "message": "",
        "max_points": 1
    }

    num_classes = 10
    num_features = 5
    num_samples = 100

    np.random.seed(seed)

    W = np.random.randn(num_features, num_classes)
    b = np.random.randn(num_classes)
    X = np.random.randn(num_samples, num_features)

    try:
        model = LinearClassifier(num_classes, num_features)
        model.load_weights(W, b)
        scores = model.compute_scores(X)
    except Exception as e:
        ret["message"] = f"\tFAILED! \n\t{e}"
        return ret

    if generate:
        np.save(verification_file, scores.data)
        print(f"Successfully generated '{verification_file}'!")
    else:
        expected_scores = np.load(verification_file)

        try:
            if np.allclose(scores.data, expected_scores):
                ret["message"] = f"PASSED!"
                ret["points"] = ret["max_points"]
            else:
                difference = np.sum(np.abs(scores.data - expected_scores))
                ret["message"] = f"\tFAILED! \n\tDifference of scores: {difference}"
        except Exception as e:
            ret["message"] = f"\tFAILED! \n\t{e}"

    return ret


def test_assignment_3_2(src_dir: str, verification_file: str, seed: int = 69, generate: bool = False) -> dict:
    sys.path.append(src_dir)
    from assignments.linear_classifier import LinearClassifier

    ret = {
        "points": 0,
        "message": "",
        "max_points": 1
    }

    num_classes = 10
    num_features = 5
    num_samples = 100

    np.random.seed(seed)

    W = np.random.randn(num_features, num_classes)
    b = np.random.randn(num_classes)

    X = np.random.randn(num_samples, num_features)
    y = np.random.randint(num_classes, size=num_samples)

    try:
        model = LinearClassifier(num_classes, num_features, learning_rate=1e-3,
                                 batch_size=num_samples, num_iters=1, verbose=False)
        model.load_weights(W, b)
        loss_history = model.train(X, y)
    except Exception as e:
        ret["message"] = f"\tFAILED! \n\t{e}"
        return ret

    if generate:
        np.savez(verification_file, loss=loss_history[-1], W=model.W.data, b=model.b.data)
        print(f"Successfully generated '{verification_file}'!")
    else:
        expected_values = np.load(verification_file)

        try:
            exp_loss = expected_values["loss"]
            exp_W = expected_values["W"]
            exp_b = expected_values["b"]

            loss_diff = np.abs(loss_history[-1] - exp_loss)
            W_diff = np.sum(np.abs(model.W.data - exp_W))
            b_diff = np.sum(np.abs(model.b.data - exp_b))

            loss_passed = loss_diff < 1e-5
            W_passed = np.allclose(model.W.data, exp_W)
            b_passed = np.allclose(model.b.data, exp_b)

            if loss_passed and W_passed and b_passed:
                ret["message"] = f"PASSED!"
                ret["points"] = ret["max_points"]
            else:
                ret["message"] = (f"\tFAILED! \n\tLoss difference: {loss_diff} \n\t"
                                  f"W difference: {W_diff} \n\tb difference: {b_diff}")
        except Exception as e:
            ret["message"] = f"\tFAILED! \n\t{e}"

    return ret
