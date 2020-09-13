# --------------------------------------------- #
# ------------  SUDOKU SOLVER (9x9) ----------- # 
# -- Code developed: by Carlos Pérez Ricardo -- # 
# ----- The purpose of this program is to ------#
# ----- solve a 9x9 sudoku. The objective ------# 
# ----- is to fill a 9×9 grid with digits ------#
# ----- so that each column, each row, ---------#
# ---- and each of the nine 3×3 subgrids -------#
# ----- that compose the grid ("boxes") --------#
# ---  all of the digits from 1 to 9. ----------#
# -- The puzzle setter provides a partially ----#
# -- completed grid, which for a well-posed ----#
# ------ puzzle has a single solution. ---------#
# --------------------------------------------- #

def InitialCond(n):
   #Use nested list comprehensions:

    array = [
        [7, 8, 0, 4, 0, 0, 1, 2, 0],
        [6, 0, 0, 0, 7, 5, 0, 0, 9],
        [0, 0, 0, 6, 0, 1, 0, 7, 8],
        [0, 0, 7, 0, 4, 0, 2, 6, 0],
        [0, 0, 1, 0, 5, 0, 9, 3, 0],
        [9, 0, 4, 0, 6, 0, 0, 0, 5],
        [0, 7, 0, 3, 0, 0, 0, 1, 2],
        [1, 2, 0, 0, 0, 7, 4, 0, 0],
        [0, 4, 9, 2, 0, 6, 0, 0, 7]
    ]

    return array

def FoundZeros(array):
    n = len(array)
    k = 0
    Sol = [ [0,0] ]
    found = 0

    for i in range(n):
        for j in range(n):
            if  array[i][j] == 0 :
                found = 1
                Sol.append([i,j])
                k = k + 1  

    if found == 1:
        del Sol[0]
        return Sol
    else:
        return -1


def PossibleValues_row (ind,array):

    Sol = [0]
    n = len(array)

    for j in range(n):
        if j+1 in array[ind][0:n]:
            pass
        else:
            Sol.append([j+1])

    del Sol[0]
    return Sol      


def PossibleValues_col (ind,array):

    Sol = [0]
    n = len(array)
    col = [0]

    for i in range(n):
        col.append( array[i][ind] )

    del col[0]

    for i in range(9):
        if i+1 in col[0:n]:
            pass
        else:
            Sol.append([i+1])

    del Sol[0]
    return Sol   

# Vector with the values of the little matrix
def innerMatrix (r1,r2,c1,c2,array): 
    innerMat = [0]

    for i in range(r1, r2):
        for j in range(c1, c2):
            innerMat.append(array[i][j])

    del innerMat[0]
    return innerMat



def PossibleValues_mat (row,col,array): 

    import math

    Sol = [0]
    n = len(array)
    div = int(math.sqrt(n))

    Nx = 0
    DIV_X = 0
    IdY = 0
    for i in range(n):

        if Nx == div:
            DIV_X = DIV_X + div
            Nx = 1
        else:
            Nx = Nx + 1 

        if row == i: 
            IdX = DIV_X
        
    Ny = 0
    DIV_Y = 0
    IdY = 0

    for j in range(n):

        if Ny == div:
            DIV_Y = DIV_Y + div
            Ny = 1
        else:
            Ny = Ny + 1 
        
        if col == j: 
            IdY = DIV_Y
    
    r1 = IdX
    r2 = IdX + div
    c1 = IdY 
    c2 = IdY + div

    innerMat = innerMatrix (r1,r2,c1,c2,array)

    Sol = [0]
    for i in range(n):
        if i+1 in innerMat:
            pass
        else:
            Sol.append([i+1])

    del Sol[0]
    return Sol 


def PossibleValue(Opts_row, Opts_col, Opts_mat, n):
    
    Sol = [0]

    for i in range(n):
        for ii in range(len(Opts_row)):
            for jj in range(len(Opts_col)):
                for kk in range(len(Opts_mat)):
                    if i+1 in Opts_row[ii] and i+1 in Opts_col[jj] and i+1 in Opts_mat[kk]:
                        Sol.append(i+1)
            else:
                pass

    del Sol[0]
    return Sol 


def main():
    """Main function."""
    
    array = InitialCond(3)
    PrintArray (array, 9)

    zeros = FoundZeros(array)
    n = len(zeros)

    print('BEGIN')

    pp = 1
    if zeros == -1:
        print('Already solved')
    else:
        while n != 0:
            if zeros == -1:
                n = 0
            else:
                n = len(zeros)
            
            for i in range(n):
                row = zeros[i][0]
                col = zeros[i][1]
                # Possible solutions in this row
                Opts_row = PossibleValues_row (row,array) 
                # Possible solutions in this col
                Opts_col = PossibleValues_col (col,array)
                # Possible solutions in this matrix
                Opts_mat = PossibleValues_mat (row, col, array)

                N = len(array)
                # Possible solutions ( which one repeats in the 3 vector possibilities )
                Opts = PossibleValue(Opts_row, Opts_col, Opts_mat, N)

                x = len(Opts) 
                
                if x == 1: # If there is only one possible solution               
                    value = Convert2number (Opts, 9)
                    array[row][col] = value
          
            zeros = FoundZeros(array)

                    
    print('Solution found')
    PrintArray (array, 9)

def Convert2number (value, N):
    for i in range(N):
        if i+1 in value:
            return i+1 

def PrintArray (array, N):
    for i in range(N):
        print array[i]

if __name__ == '__main__':

    main()