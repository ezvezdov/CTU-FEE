import sys
import numpy as np


def test_assignment_5_1(src_dir: str, verification_file: str, seed: int = 69, generate: bool = False) -> dict:
    sys.path.append(src_dir)
    from utils.engine import Tensor
    from assignments.mlp_classifier import MLPClassifier

    ret = {
        "points": 0,
        "message": "",
        "max_points": 1
    }

    np.random.seed(seed)

    num_features = 5
    num_classes = 10
    hidden_dim_1 = 10
    hidden_dim_2 = 20
    num_samples = 100

    try:
        X = Tensor(np.random.randn(num_samples, num_features))
        mlp = MLPClassifier(num_features, hidden_dim_1, hidden_dim_2, num_classes)
        out = mlp._first_layer(X)
        out.backward()
    except Exception as e:
        ret["message"] = f"\tFAILED! \n\t{e}"
        return ret

    if generate:
        np.savez(verification_file, out=out.data, dW1=mlp.params['W1'].grad, db1=mlp.params['b1'].grad)
        print(f"Successfully generated '{verification_file}'!")
    else:
        expected_out = np.load(verification_file)['out']
        expected_dW1 = np.load(verification_file)['dW1']
        expected_db1 = np.load(verification_file)['db1']

        try:
            if np.allclose(out.data, expected_out) and np.allclose(mlp.params['W1'].grad, expected_dW1) and \
                    np.allclose(mlp.params['b1'].grad, expected_db1):
                ret["message"] = f"PASSED!"
                ret["points"] = ret["max_points"]
            else:
                difference_out = np.sum(np.abs(out.data - expected_out))
                difference_dW1 = np.sum(np.abs(mlp.params['W1'].grad - expected_dW1))
                difference_db1 = np.sum(np.abs(mlp.params['b1'].grad - expected_db1))

                ret["message"] = f"\tFAILED! \n\tDifference of the output vectors: {difference_out} \n\t" \
                                 f"Difference of the W1 gradients: {difference_dW1} \n\t" \
                                 f"Difference of the b1 gradients: {difference_db1}"
        except Exception as e:
            ret["message"] = f"\tFAILED! \n\t{e}"

    return ret


def test_assignment_5_2(src_dir: str, verification_file: str, seed: int = 69, generate: bool = False) -> dict:
    sys.path.append(src_dir)
    from utils.engine import Tensor
    from assignments.mlp_classifier import MLPClassifier

    ret = {
        "points": 0,
        "message": "",
        "max_points": 1
    }

    np.random.seed(seed)

    num_features = 5
    num_classes = 10
    hidden_dim_1 = 10
    hidden_dim_2 = 20
    num_samples = 100

    try:
        X = Tensor(np.random.randn(num_samples, hidden_dim_1))
        mlp = MLPClassifier(num_features, hidden_dim_1, hidden_dim_2, num_classes)
        out = mlp._second_layer(X)
        out.backward()
    except Exception as e:
        ret["message"] = f"\tFAILED! \n\t{e}"
        return ret

    if generate:
        np.savez(verification_file, out=out.data, dW2=mlp.params['W2'].grad, db2=mlp.params['b2'].grad)
        print(f"Successfully generated '{verification_file}'!")
    else:
        expected_out = np.load(verification_file)['out']
        expected_dW2 = np.load(verification_file)['dW2']
        expected_db2 = np.load(verification_file)['db2']

        try:
            if np.allclose(out.data, expected_out) and np.allclose(mlp.params['W2'].grad, expected_dW2) and \
                    np.allclose(mlp.params['b2'].grad, expected_db2):
                ret["message"] = f"PASSED!"
                ret["points"] = ret["max_points"]
            else:
                difference_out = np.sum(np.abs(out.data - expected_out))
                difference_dW = np.sum(np.abs(mlp.params['W2'].grad - expected_dW2))
                difference_db = np.sum(np.abs(mlp.params['b2'].grad - expected_db2))

                ret["message"] = f"\tFAILED! \n\tDifference of the output vectors: {difference_out} \n\t" \
                                 f"Difference of the W2 gradients: {difference_dW} \n\t" \
                                 f"Difference of the b2 gradients: {difference_db}"
        except Exception as e:
            ret["message"] = f"\tFAILED! \n\t{e}"

    return ret


def test_assignment_5_3(src_dir: str, verification_file: str, seed: int = 69, generate: bool = False) -> dict:
    sys.path.append(src_dir)
    from utils.engine import Tensor
    from assignments.mlp_classifier import MLPClassifier

    ret = {
        "points": 0,
        "message": "",
        "max_points": 1
    }

    np.random.seed(seed)

    num_features = 5
    num_classes = 10
    hidden_dim_1 = 10
    hidden_dim_2 = 20
    num_samples = 100

    try:
        X = Tensor(np.random.randn(num_samples, hidden_dim_2))
        mlp = MLPClassifier(num_features, hidden_dim_1, hidden_dim_2, num_classes)
        out = mlp._third_layer(X)
        out.backward()
    except Exception as e:
        ret["message"] = f"\tFAILED! \n\t{e}"
        return ret

    if generate:
        np.savez(verification_file, out=out.data, dW3=mlp.params['W3'].grad, db3=mlp.params['b3'].grad)
        print(f"Successfully generated '{verification_file}'!")
    else:
        expected_out = np.load(verification_file)['out']
        expected_dW3 = np.load(verification_file)['dW3']
        expected_db3 = np.load(verification_file)['db3']

        try:
            if np.allclose(out.data, expected_out) and np.allclose(mlp.params['W3'].grad, expected_dW3) and \
                    np.allclose(mlp.params['b3'].grad, expected_db3):
                ret["message"] = f"PASSED!"
                ret["points"] = ret["max_points"]
            else:
                difference_out = np.sum(np.abs(out.data - expected_out))
                difference_dW = np.sum(np.abs(mlp.params['W3'].grad - expected_dW3))
                difference_db = np.sum(np.abs(mlp.params['b3'].grad - expected_db3))

                ret["message"] = f"\tFAILED! \n\tDifference of the output vectors: {difference_out} \n\t" \
                                 f"Difference of the W3 gradients: {difference_dW} \n\t" \
                                 f"Difference of the b3 gradients: {difference_db}"
        except Exception as e:
            ret["message"] = f"\tFAILED! \n\t{e}"

    return ret


def test_assignment_5_4(src_dir: str, verification_file: str, seed: int = 69, generate: bool = False) -> dict:
    sys.path.append(src_dir)
    from utils.engine import Tensor
    from assignments.mlp_classifier import MLPClassifier

    ret = {
        "points": 0,
        "message": "",
        "max_points": 1
    }

    np.random.seed(seed)

    num_features = 5
    num_classes = 10
    hidden_dim_1 = 10
    hidden_dim_2 = 20
    num_samples = 100

    try:
        X = Tensor(np.random.randn(num_samples, num_features))
        mlp = MLPClassifier(num_features, hidden_dim_1, hidden_dim_2, num_classes)
        out = mlp.forward(X)
        out.backward()
    except Exception as e:
        ret["message"] = f"\tFAILED! \n\t{e}"
        return ret

    if generate:
        np.savez(verification_file, out=out.data,
                 dW1=mlp.params['W1'].grad, db1=mlp.params['b1'].grad,
                 dW2=mlp.params['W2'].grad, db2=mlp.params['b2'].grad,
                 dW3=mlp.params['W3'].grad, db3=mlp.params['b3'].grad)
        print(f"Successfully generated '{verification_file}'!")
    else:
        expected = dict(
            out=np.load(verification_file)['out'],
            dW1=np.load(verification_file)['dW1'],
            db1=np.load(verification_file)['db1'],
            dW2=np.load(verification_file)['dW2'],
            db2=np.load(verification_file)['db2'],
            dW3=np.load(verification_file)['dW3'],
            db3=np.load(verification_file)['db3']
        )

        try:
            if not np.allclose(out.data, expected['out']):
                ret["message"] = f"\tFAILED! \n\tDifference of the output vectors: " \
                                 f"{np.sum(np.abs(out.data - expected['out']))}"
                return ret

            for name in mlp.params.keys():
                if not np.allclose(mlp.params[name].grad, expected[f"d{name}"]):
                    ret["message"] = f"\tFAILED! \n\tDifference of the gradients of {name}: " \
                                     f"{np.sum(np.abs(mlp.params[name].grad - expected[f'd{name}']))}"
                    return ret

            ret["message"] = f"PASSED!"
            ret["points"] = ret["max_points"]
        except Exception as e:
            ret["message"] = f"\tFAILED! \n\t{e}"
            return ret

    return ret


def test_assignment_5_5(src_dir: str, verification_file: str, seed: int = 69, generate: bool = False) -> dict:
    sys.path.append(src_dir)
    from assignments.mlp_classifier import MLPClassifier

    ret = {
        "points": 0,
        "message": "",
        "max_points": 1
    }

    np.random.seed(seed)

    num_features = 5
    num_classes = 10
    hidden_dim_1 = 10
    hidden_dim_2 = 20
    num_samples = 100

    X = np.random.randn(num_samples, num_features)

    try:
        mlp = MLPClassifier(num_features, hidden_dim_1, hidden_dim_2, num_classes)
        y_pred = mlp.predict(X)
    except Exception as e:
        ret["message"] = f"\tFAILED! \n\t{e}"
        return ret

    if generate:
        np.save(verification_file, y_pred)
        print(f"Successfully generated '{verification_file}'!")
    else:
        expected = np.load(verification_file)

        try:
            if np.allclose(y_pred, expected):
                ret["message"] = f"PASSED!"
                ret["points"] = ret["max_points"]
            else:
                difference = np.sum(np.abs(y_pred - expected))
                ret["message"] = f"\tFAILED! \n\tDifference of the output vectors: {difference}"

        except Exception as e:
            ret["message"] = f"\tFAILED! \n\t{e}"

    return ret


def test_assignment_5_6(src_dir: str, verification_file: str, seed: int = 69, generate: bool = False) -> dict:
    sys.path.append(src_dir)
    from assignments.mlp_classifier import MLPClassifier

    ret = {
        "points": 0,
        "message": "",
        "max_points": 1
    }

    np.random.seed(seed)

    num_features = 5
    num_classes = 10
    hidden_dim_1 = 10
    hidden_dim_2 = 20
    num_samples = 100

    X = np.random.randn(num_samples, num_features)
    y = np.random.randint(num_classes, size=num_samples)

    try:
        mlp = MLPClassifier(num_features, hidden_dim_1, hidden_dim_2, num_classes)
        loss = mlp.loss(X, y)
        loss.backward()
    except Exception as e:
        ret["message"] = f"\tFAILED! \n\t{e}"
        return ret

    if generate:
        np.savez(verification_file, loss=loss.data,
                 dW1=mlp.params['W1'].grad, db1=mlp.params['b1'].grad,
                 dW2=mlp.params['W2'].grad, db2=mlp.params['b2'].grad,
                 dW3=mlp.params['W3'].grad, db3=mlp.params['b3'].grad)
        print(f"Successfully generated '{verification_file}'!")
    else:
        expected = dict(
            loss=np.load(verification_file)['loss'],
            dW1=np.load(verification_file)['dW1'],
            db1=np.load(verification_file)['db1'],
            dW2=np.load(verification_file)['dW2'],
            db2=np.load(verification_file)['db2'],
            dW3=np.load(verification_file)['dW3'],
            db3=np.load(verification_file)['db3']
        )

        try:
            if not np.allclose(loss.data, expected['loss']):
                ret["message"] = f"\tFAILED! \n\tDifference of the loss: " \
                                 f"{np.sum(np.abs(loss.data - expected['loss']))}"
                return ret

            for name in mlp.params.keys():
                if not np.allclose(mlp.params[name].grad, expected[f"d{name}"]):
                    ret["message"] = f"\tFAILED! \n\tDifference of the gradients of {name}: " \
                                     f"{np.sum(np.abs(mlp.params[name].grad - expected[f'd{name}']))}"
                    return ret

            ret["message"] = f"PASSED!"
            ret["points"] = ret["max_points"]

        except Exception as e:
            ret["message"] = f"\tFAILED! \n\t{e}"

    return ret


def test_assignment_5_7(src_dir: str, verification_file: str, seed: int = 69, generate: bool = False) -> dict:
    sys.path.append(src_dir)
    from assignments.mlp_classifier import MLPClassifier

    ret = {
        "points": 0,
        "message": "",
        "max_points": 1
    }

    np.random.seed(seed)

    num_features = 5
    num_classes = 10
    hidden_dim_1 = 10
    hidden_dim_2 = 20
    num_samples = 100

    X_train = np.random.randn(num_samples, num_features)
    y_train = np.random.randint(num_classes, size=num_samples)
    X_val = np.random.randn(num_samples, num_features)
    y_val = np.random.randint(num_classes, size=num_samples)

    try:
        mlp = MLPClassifier(num_features, hidden_dim_1, hidden_dim_2, num_classes, num_iters=1)
        mlp.train(X_train, y_train, X_val, y_val)
    except Exception as e:
        ret["message"] = f"\tFAILED! \n\t{e}"
        return ret

    if generate:
        np.savez(verification_file,
                 W1=mlp.params['W1'].data, b1=mlp.params['b1'].data,
                 W2=mlp.params['W2'].data, b2=mlp.params['b2'].data,
                 W3=mlp.params['W3'].data, b3=mlp.params['b3'].data)
        print(f"Successfully generated '{verification_file}'!")

    else:
        expected = dict(
            W1=np.load(verification_file)['W1'],
            b1=np.load(verification_file)['b1'],
            W2=np.load(verification_file)['W2'],
            b2=np.load(verification_file)['b2'],
            W3=np.load(verification_file)['W3'],
            b3=np.load(verification_file)['b3'],
        )

        try:
            for name in mlp.params.keys():
                if not np.allclose(mlp.params[name].data, expected[name]):
                    ret["message"] = f"\tFAILED! \n\tDifference of the {name}: " \
                                     f"{np.sum(np.abs(mlp.params[name].data - expected[name]))}"
                    return ret

                # Check if the gradients are zero
                if not np.allclose(mlp.params[name].grad, np.zeros_like(mlp.params[name].grad)):
                    ret["message"] = f"\tFAILED! \n\tThe gradients of {name} are not zero!"
                    return ret

            ret["message"] = f"PASSED!"
            ret["points"] = ret["max_points"]

        except Exception as e:
            ret["message"] = f"\tFAILED! \n\t{e}"

    return ret
