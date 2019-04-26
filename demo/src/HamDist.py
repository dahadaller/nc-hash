import sys

def hamming(h1, h2):
    if (len(h1) < len(h2)):
        r = len(h2) - len(h1)
        for i in range(r):
            h1 = '0' + h1
    elif (len(h2) < len(h1)):
        r = len(h1) - len(h2)
        for i in range(r):
            h2 = '0' + h2
    
    b1 = []
    b2 = []
    count = 0
    for i in range(len(h1)):
        if (h1[i] != h2[i]):
            count += 1
        # b1.append(ord(h1[i]) - 48)
        # b2.append(ord(h2[i]) - 48)
    res = count / len(h1)
    print(res)

    


# h1 = bin(21194984)
# h2 = bin(16213941)
# h11 = "{0:010b}".format(16854459)
# h22 = "{0:010b}".format(17267339)
# hamming(h11, h22)
# print(sys.argv[1])
# print(sys.argv[2])
h111 = "{0:010b}".format(int(sys.argv[1]))
h222 = "{0:010b}".format(int(sys.argv[2]))
hamming(h111, h222)
# print(h1)
# print(h2)


