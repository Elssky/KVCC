import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import LogLocator

plt.xticks(fontsize=13)
plt.yticks(fontsize=13)

# x = [10, 15, 20, 25]
y = [[12060, 11659, 4090, 43],
      [12220, 9693, 2598, 30],
    #[1011, 313, 147, 94],]
     ]

x = np.arange(4)
bar_width = 0.3
labels = ['3', '4', '5']
plt.title('Amazon', fontsize=20)
plt.bar(x - bar_width/2, y[0], bar_width, color='blue', label='BkVCC')
plt.bar(x + bar_width/2, y[1], bar_width, color='red', label='BkVCC(clique)')

for i in range(2):
    for a, b in zip(x + (i*2 - 1)/2 * bar_width, y[i]):
        plt.text(a, b,
                 b,
                 ha='center',
                 va='bottom',
                 )

plt.gca().set_yscale('log')
plt.gca().yaxis.set_major_locator(LogLocator(base=10.0, numticks=4))
plt.gca().set_ylim(0,10 ** 5)

plt.legend()  # 显示图例，即label
# plt.xticks(x+bar_width/2,tick_label)
plt.xticks(x, labels=labels)
plt.show()
