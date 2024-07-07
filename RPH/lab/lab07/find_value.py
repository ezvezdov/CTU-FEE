def value_count(data,value):
    count = 0
    for i in range(len(data)):
        for j in range(len(data[i])):
            if data[i][j] == value:
                count+=1
    return count

def value_positions(data, value):
    coord = []
    for i in range(len(data)):
        for j in range(len(data[i])):
            if data[i][j] == value:
                coord.append((i,j))
    return coord
