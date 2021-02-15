import numpy as np
import matplotlib.pyplot as plt
import matplotlib
matplotlib.use('MacOSX')


# Simple script to generate the two figures of the paper. It will replace the Matlab's scripts once it becomes more ellaborated.

def read_data(experiment, data, column):
    return np.genfromtxt(f'results/{experiment}/{data}.csv', delimiter=',', names=True)[column]


# FIGURE 1

N = read_data('figure_1', 'sample_size', 'N')

fig, ax = plt.subplots()
ax.plot(N, read_data('figure_1', 'precision', 'LotterySampling'), label='LotterySampling', linewidth=1)
ax.plot(N, read_data('figure_1', 'precision', 'SpaceSaving'), label='SpaceSaving', linewidth=1)
ax.plot(N, read_data('figure_1', 'precision', 'StickySampling'), label='StickySampling', linewidth=1)
ax.set_ylim((0, None))
ax.set_xlabel('N')
ax.set_ylabel('Precision')
ax.legend(loc='center left')
fig.savefig(f'results/figure_1/precision.eps', format='eps')


fig, ax = plt.subplots()
ax.plot(N, read_data('figure_1', 'sample_size', 'LotterySampling'), label='LotterySampling', linewidth=1)
ax.plot(N, read_data('figure_1', 'sample_size', 'SpaceSaving'), label='SpaceSaving', linewidth=1)
ax.plot(N, read_data('figure_1', 'sample_size', 'StickySampling'), label='StickySampling', linewidth=1)
ax.set_ylim((0, None))
ax.set_xlabel('N')
ax.set_ylabel('Sample size')
ax.legend(loc='best', bbox_to_anchor=(0.36, 0.4))
fig.savefig(f'results/figure_1/sample_size.eps', format='eps')


# FIGURE 2

N = read_data('figure_2', 'sample_size', 'N')

fig, ax = plt.subplots()
ax.plot(N, read_data('figure_2', 'precision', 'LotterySampling'), label='LotterySampling', linewidth=1)
ax.plot(N, read_data('figure_2', 'precision', 'SpaceSaving'), label='SpaceSaving', linewidth=1)
ax.set_ylim((0, None))
ax.set_xlabel('N')
ax.set_ylabel('Precision')
ax.legend(loc='center left')
fig.savefig(f'results/figure_2/precision.eps', format='eps')


fig, ax = plt.subplots()
ax.plot(N, read_data('figure_2', 'recall', 'LotterySampling'), label='LotterySampling', linewidth=1)
ax.plot(N, read_data('figure_2', 'recall', 'SpaceSaving'), label='SpaceSaving', linewidth=1)
ax.set_ylim((0, None))
ax.set_xlabel('N')
ax.set_ylabel('Recall')
ax.legend(loc='center left')
fig.savefig(f'results/figure_2/recall.eps', format='eps')


fig, ax = plt.subplots()
ax.plot(N, read_data('figure_2', 'sample_size', 'LotterySampling'), label='LotterySampling', linewidth=1)
ax.plot(N, read_data('figure_2', 'sample_size', 'SpaceSaving'), label='SpaceSaving', linewidth=1)
ax.set_ylim((0, None))
ax.set_xlabel('N')
ax.set_ylabel('Sample size')
ax.legend(loc='center left')
ax_2 = ax.twinx()
ax_2.plot(N, read_data('figure_2', 'p(x_k)', 'px_k'), label=r'$p_{x_k}$', color='tab:green', linewidth=1, linestyle=':')
ax_2.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
ax_2.set_ylabel('Freq')
ax_2.legend(loc='center right')
fig.savefig(f'results/figure_2/sample_size.eps', format='eps')