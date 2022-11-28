LMW = 11
LMH = 10

LMP = [0, 12,  101, 113]

LNUM = LMW * LMH + len(LMP)







def clac(x, y):
    index = -1
    if (y % 2) == 0:
        index = x + (y * LMW)
    else:
        # FIX
        index = ((LMW-1)-x) + (y*LMH)

    offset = 0

    for pos in LMP:
        if index >= pos:
            offset = offset + 1

    index = index + offset

    return index







# X Y
print(clac(0, 8))




