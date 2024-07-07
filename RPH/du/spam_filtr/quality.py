import os
from utils import read_classification_from_file
from confmat import BinaryConfusionMatrix

def quality_score(tp, tn, fp, fn):
    return (tp+tn) / (tp + tn + 10*fp +fn)

def compute_quality_for_corpus(corpus_dir):
    truth_dict = read_classification_from_file(os.path.join(corpus_dir,"!truth.txt"))
    prediction_dict = read_classification_from_file(os.path.join(corpus_dir,"!prediction.txt"))

    standard_matrix = BinaryConfusionMatrix("SPAM","OK")
    standard_matrix.compute_from_dicts(truth_dict,prediction_dict)
    matix_dict = standard_matrix.as_dict()
    
    return quality_score(matix_dict['tp'],matix_dict['tn'],matix_dict['fp'],matix_dict['fn'])
