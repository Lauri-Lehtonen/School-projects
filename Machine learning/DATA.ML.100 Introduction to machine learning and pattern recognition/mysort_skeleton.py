import sys

# Read command line arguments and convert to a list of integers
arr = sys.argv[1].split(',')
my_numbers = [None]*len(arr)
for idx, arr_val in enumerate(arr):
    my_numbers[idx] = int(arr_val)

# Print
print(f'Before sorting {my_numbers}')

# My sorting (e.g. bubble sort)
# ADD HERE YOUR CODE
for i in range(len(my_numbers)):

    for n in range(0, len(my_numbers) - i - 1):

        if my_numbers[n] > my_numbers[n + 1]:
            temp = my_numbers[n]
            my_numbers[n] = my_numbers[n + 1]
            my_numbers[n + 1] = temp



# Print
print(f'After sorting {my_numbers}')