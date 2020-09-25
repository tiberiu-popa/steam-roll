import argparse
import difflib
import glob

def dbg(vals, *names):
    arr = ['{}={}'.format(name, vals.get(name)) for name in names]
    print(' '.join(arr))

def read_ints(filename):
    with open(filename) as f:
        for line in f:
            tokens = line.rstrip().split()
            for token in tokens:
                yield int(token)

def test_from_script(script, callback):
    script_base = base_filename(script)
    input_filenames = glob.glob('files/{}_*.in'.format(script_base))
    output_filenames = glob.glob('files/{}_*.out'.format(script_base))
    outputs = {base_filename(filename): filename for filename in output_filenames}
    for in_filename in input_filenames:
        base = base_filename(in_filename)
        res_filename = '{}.res'.format(base)
        with open(res_filename, 'w') as res_file:
            callback(in_filename, res_file)
        if base in outputs:
            out_filename = outputs[base]
            print('Comparing', res_filename, out_filename)
            compare_files(res_filename, out_filename)
            print()

def run_script(script, callback):
    args = parse_args()
    if args.test_from_script:
        test_from_script(script, callback)
    else:
        for filename in args.filenames:
            callback(filename)

def base_filename(filename):
    tokens = filename.split('.')
    return '.'.join(tokens[:-1])

def compare_files(filename1, filename2):
    differ = difflib.Differ()
    with open(filename1) as f1:
        with open(filename2) as f2:
            x = [line.rstrip() for line in f1.readlines()]
            y = [line.rstrip() for line in f2.readlines()]
            for diff in differ.compare(x, y):
                print(diff)

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '--filenames',
        nargs='+',
        type=str,
        help='input files',
    )
    parser.add_argument(
        '--test_from_script',
        dest='test_from_script',
        action='store_true',
        help='generate tests from script name',
    )
    return parser.parse_args()
