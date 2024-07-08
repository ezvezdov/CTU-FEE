from evaluation.assignment import Assignment
from evaluation.tests.assignments_1 import test_assignment_1_1, test_assignment_1_2, test_assignment_1_3
from evaluation.tests.assignments_2 import test_assignment_2_1, test_assignment_2_2
from evaluation.tests.assignments_3 import test_assignment_3_1, test_assignment_3_2
from evaluation.tests.assignments_4 import test_assignment_4_1
from evaluation.tests.assignments_5 import test_assignment_5_1, test_assignment_5_2, test_assignment_5_3, \
    test_assignment_5_4, test_assignment_5_5, test_assignment_5_6, test_assignment_5_7

SEED = 69
GENERATE = False
SRC_DIR = "assignments"

ASSIGNMENTS = [
    Assignment(name="assignment_1_1",
               test_func=test_assignment_1_1,
               verification_file="evaluation/data/assignment_1_1.npy"),
    Assignment(name="assignment_1_2",
               test_func=test_assignment_1_2,
               verification_file="evaluation/data/assignment_1_2.npy"),
    Assignment(name="assignment_1_3",
               test_func=test_assignment_1_3,
               verification_file="evaluation/data/assignment_1_3.npy"),
    Assignment(name="assignment_2_1",
               test_func=test_assignment_2_1,
               verification_file="evaluation/data/assignment_2_1.npy"),
    Assignment(name="assignment_2_2",
               test_func=test_assignment_2_2,
               verification_file="evaluation/data/assignment_2_2.npy"),
    Assignment(name="assignment_3_1",
               test_func=test_assignment_3_1,
               verification_file="evaluation/data/assignment_3_1.npy"),
    Assignment(name="assignment_3_2",
               test_func=test_assignment_3_2,
               verification_file="evaluation/data/assignment_3_2.npz"),
    Assignment(name="assignment_4_1",
               test_func=test_assignment_4_1,
               verification_file="evaluation/data/assignment_4_1.npy"),
    Assignment(name="assignment_5_1",
               test_func=test_assignment_5_1,
               verification_file="evaluation/data/assignment_5_1.npz"),
    Assignment(name="assignment_5_2",
               test_func=test_assignment_5_2,
               verification_file="evaluation/data/assignment_5_2.npz"),
    Assignment(name="assignment_5_3",
               test_func=test_assignment_5_3,
               verification_file="evaluation/data/assignment_5_3.npz"),
    Assignment(name="assignment_5_4",
               test_func=test_assignment_5_4,
               verification_file="evaluation/data/assignment_5_4.npz"),
    Assignment(name="assignment_5_5",
               test_func=test_assignment_5_5,
               verification_file="evaluation/data/assignment_5_5.npy"),
    Assignment(name="assignment_5_6",
               test_func=test_assignment_5_6,
               verification_file="evaluation/data/assignment_5_6.npz"),
    Assignment(name="assignment_5_7",
               test_func=test_assignment_5_7,
               verification_file="evaluation/data/assignment_5_7.npz")
]

if __name__ == "__main__":
    print("\n================= TESTING ASSIGNMENTS =================\n")
    for assignment in ASSIGNMENTS:
        results = assignment.test_assignment(src_dir=SRC_DIR, generate=GENERATE, seed=SEED)
        print(f"{assignment.name}: \n\t{results['message']}\n")
