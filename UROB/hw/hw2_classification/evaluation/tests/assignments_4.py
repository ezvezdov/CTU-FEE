import sys
import numpy as np


def test_assignment_4_1(src_dir: str, verification_file: str, seed: int = 69, generate: bool = False) -> dict:
    sys.path.append(src_dir)
    from assignments.preprocessing import normalize_data

    ret = {
        "points": 0,
        "message": "",
        "max_points": 1
    }

    np.random.seed(seed)

    data = np.random.rand(10, 3)
    try:
        normalized_data = normalize_data(data)
    except Exception as e:
        ret["message"] += f"FAILED! \n\t{e}"

    if generate:
        np.save(verification_file, normalized_data)
        print(f"Successfully generated '{verification_file}'!")
    else:
        expected_data = np.load(verification_file)

        try:
            if np.allclose(normalized_data, expected_data):
                ret["message"] += f"PASSED!"
                ret["points"] = ret["max_points"]
            else:
                diff = np.sum(np.abs(normalized_data - expected_data))
                ret["message"] += f"\tFAILED! \n\tThe difference between the normalized data and the expected data " \
                                  f"is {diff}, but should be 0."
        except Exception as e:
            ret["message"] += f"\tFAILED! \n\t{e}"

    return ret
