LMW = 11
LMH = 10

LMP = [0, 12,  101, 113]

LNUM = LMW * LMH + len(LMP)

import math





def clac(y, x):
    index = -1
    if (y % 2) == 0:
        index = x + (y * LMW)
    else:
        # FIX
        index = ((LMW)-x) + (y*LMW)-1





    offset = 0
    past_led = 0
    for pos in LMP:
        past_led = past_led + 1
        if (index+past_led) > pos:
            offset = offset + 1


    index = index + offset

    return str(index)







# X Y
for x in range(LMH):
    print(clac(x, 0).zfill(2).zfill(2), clac(x, 1).zfill(2), clac(x, 2).zfill(2), clac(x, 3).zfill(2), clac(x, 4).zfill(2), clac(x, 5).zfill(2), clac(x, 6).zfill(2), clac(x, 7).zfill(2), clac(x, 8).zfill(2), clac(x, 9).zfill(2), clac(x, 10).zfill(2))


def rotate(origin, point, angle):
    """
    Rotate a point counterclockwise by a given angle around a given origin.

    The angle should be given in radians.
    """
    ox, oy = origin
    px, py = point

    qx = ox + math.cos(angle) * (px - ox) - math.sin(angle) * (py - oy)
    qy = oy + math.sin(angle) * (px - ox) + math.cos(angle) * (py - oy)
    return qx, qy




rx, ry = rotate(((LMW-2)/2,(LMH-2)/2), (0,0), 90)

for x in range(LMH):
    r = ''
    for y in range(LMW):
        if x == int(rx) and y == int(ry):
            r += '# '
        else:
            r += '. '
    print(r)
