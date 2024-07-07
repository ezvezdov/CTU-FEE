def count_rows_and_words(filename):
    f= open(filename, 'r', encoding='utf-8')
    word_count = 0
    lines_count = 0

    while True:
        line = f.readline()
        if(not line):
            break
        lines_count+=1
        A = line.split()
        word_count += len(A)
    return (lines_count, word_count)

def compute_word_frequencies(filename):
    dictionary = {}
    f= open(filename, 'r', encoding='utf-8')

    line = f.readline()
    while line:
        splitline = line.split()
        for i in range(len(splitline)):
            dictionary.setdefault(splitline[i],0)
            dictionary[splitline[i]]+=1
        
        line = f.readline()
    return dictionary
