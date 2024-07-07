import os
import corpus

class BaseFilter:

    def __init__(self):
        pass

    def make_corpus(self,path):
        self.corp = corpus.Corpus(path)
        self.prediction_file = open(os.path.join(path,"!prediction.txt"),"w",encoding="utf-8")

    def write_decision(self,filename, decision):
        self.prediction_file.write(filename + " " + decision + "\n")
        
    def close_file(self):
        self.prediction_file.close()