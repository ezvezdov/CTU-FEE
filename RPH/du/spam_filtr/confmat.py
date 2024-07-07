class BinaryConfusionMatrix:
    def __init__(self,pos_tag,neg_tag):
        self.pos_tag = pos_tag
        self.neg_tag = neg_tag

        self.matrix = {'tp': 0, 'tn': 0, 'fp': 0, 'fn': 0}
    
    def  as_dict(self):
        return self.matrix
        
    def update(self,truth, prediction):
        if truth != self.pos_tag and truth != self.neg_tag or prediction != self.pos_tag and prediction != self.neg_tag:
            raise ValueError

        if truth == self.pos_tag and prediction == self.pos_tag:
            self.matrix['tp']+=1
        elif truth == self.neg_tag and prediction == self.neg_tag:
            self.matrix['tn']+=1
        elif truth == self.pos_tag and prediction == self.neg_tag:
            self.matrix['fn']+=1
        elif truth == self.neg_tag and prediction == self.pos_tag:
            self.matrix['fp']+=1

    def compute_from_dicts(self,truth_dict, pred_dict):
        for key in truth_dict:
            if truth_dict[key] == self.pos_tag and pred_dict[key] == self.pos_tag:
                self.matrix['tp']+=1
            elif truth_dict[key] == self.neg_tag and pred_dict[key] == self.neg_tag:
                self.matrix['tn']+=1
            elif truth_dict[key] == self.pos_tag and pred_dict[key] == self.neg_tag:
                self.matrix['fn']+=1
            elif truth_dict[key] == self.neg_tag and pred_dict[key] == self.pos_tag:
                self.matrix['fp']+=1

'''
Třída bude mít metodu compute_from_dicts(truth_dict, pred_dict), která napočítá statistiky TP, TN, FP a FN ze dvou slovníků: v prvním bude správná klasifikace emailů, ve druhém klasifikace emailů predikovaná filtrem.
'''