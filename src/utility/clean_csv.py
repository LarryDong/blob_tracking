
# a script to convert a csv file to a clean csv file

import pandas as pd
import numpy as np

if __name__ == '__main__':
    input_file = '/home/larrydong/codeGit/blob_tracking/src/night2.csv'
    output_file = '/home/larrydong/codeGit/blob_tracking/src/night2_output.csv'

    # data = pd.read_csv(input_file, names=['idx', 'x', 'y', 't', 'p'], delimiter=',')
    data = pd.read_csv(input_file, names=['t', 'x', 'y', 'p'], delimiter=',')
    data['t'] = data['t'] - data['t'][0]
    df = pd.DataFrame({'t':data['t'], 'x':data['x'], 'y':data['y'], 'p':data['p']})
    df.to_csv(output_file, index=False, sep=',', header=False)
    print("==> Done.")