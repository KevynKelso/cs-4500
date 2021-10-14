import matplotlib.pyplot as plt
import pandas as pd

def main():
    df = pd.read_csv('./list-forming.csv')

    plt.plot('num_threads', 'lock', data=df)
    plt.plot('num_threads', 'local_list', data=df)
    plt.title('Running time of list-forming.c using mutex_lock optimization (global list) vs local list')
    plt.xlabel('num_threads')
    plt.ylabel('time (microseconds)')
    plt.legend()
    plt.show()

if __name__ == '__main__':
    main()

