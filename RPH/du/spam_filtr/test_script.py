import os

import filter
import quality as quality
from utils import read_classification_from_file
from confmat import BinaryConfusionMatrix
path2 = "/home/ezvezdov/Programming/rph/du/spam_filtr/spam-data-12-s75-h25/1plus2"
path1 = "/home/ezvezdov/Programming/rph/du/spam_filtr/spam-data-12-s75-h25/1"

filtr = filter.MyFilter()
filtr.train(path2)
filtr.test(path1)

print(quality.compute_quality_for_corpus(path1))

#os.remove(os.path.join(path,"!prediction.txt"))
