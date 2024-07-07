'''
Vstupy:	cesta k textovému souboru (v našem případě to budou typicky soubory !truth.txt a !prediction.txt)
Výstupy:	dictionary obsahující pro každý název souboru identifikátor SPAM nebo OK
'''

def read_classification_from_file(fpath):
    dictionary = {}

    f = open(fpath,"r",encoding='utf-8')
    line = f.readline()
    while line:
        splitline = line.split()
        dictionary[splitline[0]] = splitline[1]
        
        line = f.readline()
    f.close()
    return dictionary