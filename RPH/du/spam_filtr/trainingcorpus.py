import os
import json

from utils import read_classification_from_file

class TrainingCorpus:
    '''Training Corpus builds vocabularies based on "!truth.txt", email contents'''

        # Files "english_words" and "ignore_list" are text files filled with 
        # English words to help filter clearly understand what words from e-mail contents
        # Are of actual value and distinct them from trash-information like "qwerty1234"
    def __init__(self, corpus_dir):
        self.dir_path = corpus_dir
        self.truth_dict = read_classification_from_file(os.path.join(corpus_dir,"!truth.txt"))

        self.HAM_TAG = "OK"
        self.SPAM_TAG = "SPAM"

        self.spam_count = 0
        self.ham_count = 0

        file = open("english_words","r",encoding="utf-8")
        self.set_words = set(json.loads(file.read()))
        file.close()    

        file = open("ignore_list","r",encoding="utf-8")
        self.ignore_list = json.loads(file.read())
        file.close()

        # Generator function to get all spam/ham e-mail bodies into one list
    def spam_ham_generator(self,TAG):
        files_list = os.listdir(self.dir_path)
        for filename in files_list:
            if(filename[0] == '!' or self.truth_dict[filename] != TAG):
                continue
            
            if(TAG == self.SPAM_TAG):
                self.spam_count+=1
            if(TAG == self.HAM_TAG):
                self.ham_count+=1

            current_file = open(os.path.join(self.dir_path,filename),"r",encoding="utf-8")
            file_body  = current_file.read()
            current_file.close()
            
            yield filename, file_body

    def spams(self):
        for filename, file_body in self.spam_ham_generator(self.SPAM_TAG):
            yield filename, file_body
        
    def hams(self):
        for filename, file_body in self.spam_ham_generator(self.HAM_TAG):
            yield filename, file_body

        # Function takes all spam/ham e-mail bodies as argument
        # "undresses" all the words from unneccesary symbols
        # checks whether a word is a real English word
        # and adds the word to according spam/ham vocabulary.
    def get_tokens(self,filebody):

            # Experimental symbol removal that helps reach best effectiveness 
        filebody = filebody.translate(str.maketrans('.',' '))
        filebody = filebody.translate(str.maketrans('!',' '))
        filebody = filebody.translate(str.maketrans('#',' '))
        filebody = filebody.translate(str.maketrans('\'',' '))
        filebody = filebody.translate(str.maketrans(':',' '))
        filebody = filebody.translate(str.maketrans('2',' '))

        tokens = filebody.lower().split()
        english_tokens= []
        for i in range(len(tokens)):
            if tokens[i] in self.set_words:
                english_tokens.append(tokens[i])
        
        return english_tokens
