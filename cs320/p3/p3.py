# Program 3
# Perform operations on matrices.
# CS320
# 19 Oct 2020
# @author Brandon Nguyen cssc2650
import sys


def print_id():
    """Prints student information."""
    print("Program #3, cssc2650, Brandon Nguyen")


def read_matricies(A, B):
    """Reads and creates matrix from a file.

    This function reads m, n, and p from the datafile.
    Then matricies A and B are filled from the datafile.
    Matrix C is then allocated size m x p.
    The values for m, n, and p are local values filled in by this function.

    :param A List[List[int]]: matrix A
    :param B List[List[int]]: matrix B
    :rtype List[List[int]]: matrix C
    """
    with open(sys.argv[1], "r") as file:
        lines = file.read().splitlines()  # strip newlines
    m = int(lines[0])
    n = int(lines[1])
    p = int(lines[2])
    lines = lines[3:]  # get rid of m, n, p
    # first handle matrix A (m x n)
    # note: we use the [:] syntax so that we assign to elements of that list
    #       and achieve the in place modification desired.
    A[:] = [[int(x) for x in lines[row_count].split()] for row_count in range(m)]
    # now handle matrix B (n x p)
    B[:] = [[int(x) for x in lines[m + row_count].split()] for row_count in range(n)]
    return [
        [0 for col in range(p)] for row in range(m)
    ]  # matrix C initialized to all 0


def print_matrix(matrix):
    """Prints a matrix.

    This function prints a matrix. Rows and columns should be preserved.

    :param matrix List[List[int]]: The matrix to print
    """
    for row in matrix:
        for col in row:
            print(f"{col:>5}", end="")
        print()


def mult_matrices(A, B, C):
    """Multiplies A and B.

    The two matrices A and B are multiplied, and matrix C contains the result.

    :param A List[List[int]]: Matrix A
    :param B List[List[int]]: Matrix B
    :param C List[List[int]]: Matrix C (all zeros at this point)
    """
    m = len(C)
    n = len(B)
    p = len(C[0])
    for i in range(m):
        for j in range(p):
            for k in range(n):
                C[i][j] += A[i][k] * B[k][j]  # probably doesn't need to be +=


def main():
    """
    This function begins execution of program.
    Verify data input filename provided on command line: len(sys.argv)
    If error, output message of user: 'Usage: p3.py datafilename'
    and quit, using sys.exit()

    Declare A, B, call read_matrices to initialize A, B, and store
      return value as C

    Print A and B contents

    Call mult_matrices

    Print result contents
    """
    if len(sys.argv) != 2:
        print("Usage: p3.py dataFileName")
        sys.exit(-1)
    try:
        # try to access the file
        with open(sys.argv[1], "r"):
            pass
    except FileNotFoundError:
        print("Usage: p3.py dataFileName")
        sys.exit(-1)

    print_id()
    A = []
    B = []
    C = read_matricies(A, B)
    print("Matrix A contents:")
    print_matrix(A)
    print("\nMatrix B contents:")
    print_matrix(B)
    print("\nMatrix A * B is:")
    mult_matrices(A, B, C)
    print_matrix(C)


# Begin program
if __name__ == "__main__":
    main()
