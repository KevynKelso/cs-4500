import matplotlib.pyplot as plt
import pandas as pd

def main():
    df = pd.read_csv('./mutex_lock_vs_try_lock.csv')
    plt.plot('num_threads', 'try_lock', data=df)
    plt.plot('num_threads', 'lock', data=df)
    plt.title('Running time of list-forming.c using try_lock vs lock')
    plt.xlabel('num_threads')
    plt.ylabel('time (microseconds)')
    plt.legend()
    plt.show()

if __name__ == '__main__':
    main()

