
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import ticker
from matplotlib.ticker import FuncFormatter, MultipleLocator, LogLocator

#DBLP
#VCCE
# x = [10, 15, 20]
# y1 = [1678.964000, 48.304000, 8.784000]
# #BkVCC(lk)
# y2 = [12.649000, 0.919000,  0.698000]
# #BkVCC(mc)
# y2_ = [7.321000, 0.202000,  0.133000]
# #HKEAM(lk)
# y3 = [74.776000, 7.076000,  2.124000]
# #HKEAM(mc)
# y3_ = [38.930000, 21.566000,  4.675000]

#Amazon
#VCCE
x = [10, 15, 20]
y1 = [1678.964000, 48.304000, 8.784000]
#BkVCC(lk)
y2 = [12.649000, 0.919000,  0.698000]
#BkVCC(mc)
y2_ = [7.321000, 0.202000,  0.133000]
#HKEAM(lk)
y3 = [74.776000, 7.076000,  2.124000]
#HKEAM(mc)
y3_ = [38.930000, 21.566000,  4.675000]

# x = np.arange(20, 350)
l1 = plt.plot(x, y1, 'o--', color='black',  label='VCCE')
l2 = plt.plot(x, y2, '^--', color='blue',label='BkVCC(lk)')
l3 = plt.plot(x, y2_, 'x--', color='blue',label='BkVCC(mc)')
l2_ = plt.plot(x, y3, 's--',color='red', label='HKEAM(lk)')
l3_ = plt.plot(x, y3_, '*--', color='red',label='HKEAM(mc)')
plt.title('DBLP',fontsize=20)

plt.ylabel('Running Time(s)',fontsize=13)
def to_percent(temp, position):
    return '%1.0f'%(10*temp) + '%'
plt.gca().xaxis.set_major_locator(ticker.MultipleLocator(1))
# plt.gca().xaxis.set_major_formatter(FuncFormatter(to_percent))
# plt.gca().xaxis.set_major_locator(MultipleLocator(2))
plt.gca().set_yscale('log')

plt.gca().yaxis.set_major_locator(LogLocator(base=100.0, numticks=5))
plt.gca().set_ylim(10 ** -2, 10 ** 6)
plt.legend(fontsize=25)
plt.xticks(x)
plt.show()