def fsum(x):
    fsum = 0
    for i in range(len(x)):
        fsum += x[i]
    return fsum

def click_for_point(event):
    if event.button is MouseButton.LEFT:
        x_pos = event.xdata
        y_pos = event.ydata
        try:
            x.append(float(x_pos))
            y.append(float(y_pos))
            fig.canvas.draw()
            line.set_xdata(x)
            line.set_ydata(y)
        except:
            return
    if event.button is MouseButton.RIGHT and len(x) >= 2:
        a, b = my_linfit(x, y)
        xp = np.arange(-3, 5, 0.1)
        plt.plot(xp, a * xp + b, 'r-')
        print(f'{a}*x+{b}')
        print(f'My_fit : _a={a} : and _b={b}')
        plt.show()
        fig.canvas.mpl_disconnect(cid)


def my_linfit(x,y):


    fx = x
    fy = y
    n = len(fx)
    if n == 0:
        n = 1
    xy = [0]*len(fx)
    xx = [0]*len(fx)

    for i in range(len(fy)):
        xy[i] = fx[i] * fy[i]

    for i in range(len(fy)):
        xx[i] = fx[i] * fx[i]

    a_nominator = fsum(xy) - (fsum(fy) * fsum(fx)) / (n)
    a_denominator = fsum(xx) - (fsum(fx) * fsum(fx)) / (n)

    a = a_nominator/a_denominator
    b = (fsum(fy)-a*fsum(fx))/n

    return a,b

from matplotlib.backend_bases import MouseButton
import matplotlib.pyplot as plt
import numpy as np



x = []
y = []

fig, ax = plt.subplots()
ax.plot([-3,5],[-3,5], linewidth = 0.0)
line, = ax.plot(x, y, marker = "x", linewidth = 0.0)

cid = fig.canvas.mpl_connect('button_press_event', click_for_point)
plt.show()
