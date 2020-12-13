!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! PROGRAM P4
!             Traveling Salesman Problem in FORTRAN 95
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
PROGRAM P4

IMPLICIT NONE

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Variable declarations
TYPE city
    CHARACTER(20) :: name
END TYPE

INTEGER :: i, j, count, current_path, next_path
INTEGER :: temp, distance, best_distance, permutations
INTEGER :: ios
CHARACTER(20) :: filename
TYPE(city), ALLOCATABLE, DIMENSION(:) :: city_list
INTEGER, ALLOCATABLE, DIMENSION(:, :) :: d_table
INTEGER, ALLOCATABLE, DIMENSION(:) :: best_path, path

PRINT "(a)", 'Enter filename:'
READ *, filename
PRINT *

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!Open the file and read number of cities
OPEN(UNIT=9, FILE=filename, FORM="FORMATTED", ACTION="READ",&
    STATUS="OLD", IOSTAT=ios)

! try to open file
IF(ios /= 0) THEN
    PRINT *, "Could not open file ", TRIM(filename), " Error code: ", ios
    STOP
END IF

READ (UNIT=9, FMT=100) count

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!Allocate memory for all needed arrays
ALLOCATE(d_table(1:count, 1:count), city_list(1:count), path(1:count), best_path(1:count), STAT=ios)
IF(ios /= 0) THEN
    PRINT *, "ERROR, could not allocate memory."
    STOP
END IF

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!Fill in arrays from data file
DO i=1, count
    READ (UNIT=9, FMT=200) city_list(i) ! Fill in city names
    DO j=1, count ! Fill in distances
        READ (UNIT=9, FMT=100) d_table(i, j) ! follows the given table format
    END DO
    path(i) = i
    best_path(i) = i
END DO

CLOSE(9)

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!Use recursion to find minimal distance
call permute(2,count)

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!Print formatted output
best_distance = 0
DO i=1, count
    current_path = best_path(i)
    IF (i < count) THEN
        next_path = best_path(i+1) ! get i+1
    ELSE ! i == count
        next_path = best_path(1) ! loop back to 1
    END IF
    PRINT "(A, A, A, A, i0, A)", TRIM(city_list(current_path)%name), " to ", TRIM(city_list(next_path)%name), " --  ",&
        d_table(current_path, next_path), "  miles"
    best_distance = best_distance + d_table(current_path, next_path)
END DO
PRINT "(a,i6,a)", "Best distance is:", best_distance, " miles"
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!Format labels
100 FORMAT (I6)
200 FORMAT (A)

CONTAINS
!Permute function
RECURSIVE SUBROUTINE permute(first, last)
! Declare intent of parameter variables
INTEGER, INTENT(IN) :: first, last
INTEGER :: i, j, distance, temp
INTEGER :: best_distance = 10000000
        IF(first == last)  THEN
            distance = d_table(1,path(2))
            DO i=2, last-1
                distance = distance + d_table(path(i),path(i+1))
            END DO
            distance = distance + d_table(path(last),path(1))
            permutations = permutations + 1
            IF(distance < best_distance) THEN
                best_distance = distance
                DO i=2, count
                    best_path(i) = path(i)
                END DO
            END IF
        ELSE
            DO i=first, last
                temp = path(first)
                path(first) = path(i)
                path(i) = temp

                call permute(first+1,last)

                temp = path(first)
                path(first) = path(i)
                path(i) = temp
            END DO
        END IF
END SUBROUTINE permute

END PROGRAM P4
