#import collections
import sys
#import string
import os


for k in sys.argv[1:]: 
    linelen = 0
    nlines = 0

    with open(k, 'r') as f:
        lines = f.readlines()
        firstLine = lines[0]
        lines = lines[1:] # tolgo la prima riga
        matrixLength = int(firstLine[0]) + 1
        matrixWidth = int(firstLine[2])
        nlines = 1
        for line in lines:
            nlines += 1
    print(lines)

    print("no. of lines = ", nlines, file=sys.stderr)
    print("matrix length = ", matrixLength, file= sys.stderr )
    print("matrix width = ", matrixWidth, file= sys.stderr )

    path = k + ".d"
    if not os.path.exists(path):
        os.makedirs(path)

    i=0
    nMatrix = (nlines-2)/(matrixLength)
    while i < nMatrix:
        filename = "out_n_" + str(i+1)
        buff = firstLine
        for j in range(matrixLength):
            buff = buff + lines[j+((i)*matrixLength)]
        with open(os.path.join(path, filename), 'wb') as temp_f:
            temp_f.write(bytes(buff, 'utf-8'))
            temp_f.close()
        buff = ""
        i +=1
    

