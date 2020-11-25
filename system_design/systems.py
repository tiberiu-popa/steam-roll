import argparse
import random
import sys

problems = [
    'TinyURL',
    'Pastebin',
    'Instagram',
    'Dropbox',
    'Facebook Messenger',
    'Twitter',
    'Youtube/Netflix',
    'Typeahead Suggestion',
    'API Rate Limiter',
    'Twitter Search',
    'Web Crawler',
    'Facebook Newsfeed',
    'Yelp or Nearby Friends',
    'Uber backend',
    'Ticketmaster',
]

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '--section',
        type=str,
        help='section of the system design document',
    )
    return parser.parse_args()

def shuffled_problems():
    new_problems = problems[:]
    random.shuffle(new_problems)
    return new_problems

def gen_header(problem, section, output_file=sys.stdout):
    print('||| Design {} |||'.format(problem), file=output_file)
    print(file=output_file)
    print('============', file=output_file)
    print(section, file=output_file)
    print('============', file=output_file)
    print(file=output_file)

def gen_requirements(problem, output_file=sys.stdout):
    gen_header(problem, 'Requirements', output_file)
    print('Functional Requirements:', file=output_file)
    print('- ', file=output_file)
    print(file=output_file)
    print('Non-functional Requirements:', file=output_file)
    print('- ', file=output_file)
    print(file=output_file)
    print(file=output_file)

def gen_problem(problem, section, output_file=sys.stdout):
    gen_header(problem, section, output_file)
    print('- ', file=output_file)
    print(file=output_file)
    print(file=output_file)

def gen_document(section):
    with open('systems_document.txt', 'x') as f:
        for problem in shuffled_problems():
            gen_problem(problem, section, f)

def main():
    args = parse_args()
    gen_document(args.section)

if __name__ == '__main__':
    main()
