import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def plot_lock_vs_local_list(df):
    df = df.loc[df['k'] == 200]
    plt.plot('num_threads', 'lock', data=df)
    plt.plot('num_threads', 'local_list', data=df)
    plt.title('Running time of list-forming.c using mutex_lock optimization (global list) vs local list')
    plt.xlabel('num_threads')
    plt.ylabel('time (microseconds)')
    plt.legend()
    plt.show()

def plot_local_list_vs_unbounded(df):
    df = df.loc[df['k'] == 200]
    plt.plot('num_threads', 'local_list', data=df)
    plt.plot('num_threads', 'unbounded', data=df)
    plt.title('Running time of list-forming.c using local_list optimization vs unbounded threads')
    plt.xlabel('num_threads')
    plt.ylabel('time (microseconds)')
    plt.legend()
    plt.show()

def plot_performance_vs_num_threads(df):
    df = df.loc[df['k'] == 200]
    x = df['num_threads']
    fig, axs = plt.subplots(2, 2)
    fig.suptitle('Optimizations of list-forming (K = 200)')
    fig.supxlabel('num_threads')
    fig.supylabel('time (microsec)')

    axs[0, 0].plot(x, df['try_lock'], color='blue')
    axs[0, 0].set_title('Original list-forming.c (try_lock / busy waiting) vs. num_threads')
    axs[0, 1].plot(x, df['lock'], color='green')
    axs[0, 1].set_title('Blocking list-forming (mutex_lock) vs. num_threads')
    axs[1, 0].plot(x, df['local_list'], color='orange')
    axs[1, 0].set_title('Blocking list-forming w/ local list gen vs. num_threads')
    axs[1, 1].plot(x, df['unbounded'], color='red')
    axs[1, 1].set_title('list-forming w/ unbounded threads & blocking & local list vs. num_threads')

    plt.show()

# always, title = y vs. x
def plot_performance_vs_k(df):
    # need to average the time
    x = df['k']
    fig, axs = plt.subplots(2, 2)
    fig.suptitle('Optimizations of list-forming with different K\'s (num_threads = 50 "worst case")')
    fig.supxlabel('K')
    fig.supylabel('time (microsec)')

    axs[0, 0].bar(x, df['try_lock'], color='blue', width=10)
    axs[0, 0].set_title('Original list-forming.c (try_lock / busy waiting) vs. K')
    axs[0, 1].bar(x, df['lock'], color='green', width=10)
    axs[0, 1].set_title('Blocking list-forming (mutex_lock) vs. K')
    axs[1, 0].bar(x, df['local_list'], color='orange', width=10)
    axs[1, 0].set_title('Blocking list-forming w/ local list gen vs. K')
    axs[1, 1].bar(x, df['unbounded'], color='red', width=10)
    axs[1, 1].set_title('list-forming w/ unbounded threads & blocking & local list vs. K')

    plt.show()


def plot_3d_comparison(df):
    ax = plt.axes(projection='3d')

    ax.scatter3D(df['num_threads'], df['k'], df['try_lock'], color='blue', s=5, label='try_lock (original)')
    ax.scatter3D(df['num_threads'], df['k'], df['lock'], color='green', s=5, label='lock')
    ax.scatter3D(df['num_threads'], df['k'], df['local_list'], color='orange', s=5, label='local_list (w/ lock)')
    ax.scatter3D(df['num_threads'], df['k'], df['unbounded'], color='red', s=5, label='unbounded (w/ lock & local_list)')
    ax.set_title('list-forming optimization comparison')
    ax.set_xlabel('num_threads')
    ax.set_ylabel('K')
    ax.set_zlabel('time (microseconds)')
    ax.set_zlim([0, 6000])
    ax.legend()
    plt.show()

if __name__ == '__main__':
    df = pd.read_csv('./list-forming.csv')
    # plot_3d_comparison(df)
    # plot_performance_vs_k(df)
    # plot_performance_vs_num_threads(df)
    plot_local_list_vs_unbounded(df)

