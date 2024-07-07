import os
import corpus
import random
import basefilter
from quality import compute_quality_for_corpus

class NaiveFilter(basefilter.BaseFilter):
    def __init__(self):
        pass
    def train(self,path):
        pass
    def test(self,path):
        super().make_corpus(path)
        for filename, file_body in self.corp.emails():
            super().write_decision(filename,"OK")
        super().close_file()
        
class ParanoidFilter(basefilter.BaseFilter):
    def __init__(self) -> None:
        pass
    def train(self,path):
        pass
    def test(self,path):
        super().make_corpus(path)
        for filename, file_body in self.corp.emails():
            super().write_decision(filename,"SPAM")
        super().close_file()

class RandomFilter(basefilter.BaseFilter):
    def __init__(self) -> None:
        pass
    def train(self,path):
        pass
    def test(self, path):
        super().make_corpus(path)

        for filename, file_body in self.corp.emails():
            is_spam = random.randint(0,1)
            cur_letter = "OK"
            if is_spam == 1:
                cur_letter = "SPAM"
            super().write_decision(filename,cur_letter)
        super().close_file()