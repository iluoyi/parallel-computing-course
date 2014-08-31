#
#  Origin:  Lehigh CSE Department
#  Course:  CSE 375/475
#  Date:    2013-09-12
#  Version: 2
#
#  Description: To use this file, type ". switch_gcc.sh".  Doing so will
#  update your environment to use the latest experimental gcc 4.9 compiler.
#
#  For the purposes of the assignment, this file should be treated as
#  read-only.
#

#
# This step adds gcc to the path
#
for pkg in \
    /proj/spear0/sw/bin
do
    export PATH=$pkg:$PATH
done

#
# This step adds gcc, mpc, mpfr, and gmp to the library path, so that gcc
# will work, and compiled code can find the right versions of C++ runtime
# libraries
#
for pth in \
    /proj/spear0/sw/lib \
    /proj/spear0/sw/lib64
do
    export LD_LIBRARY_PATH=$pth:$LD_LIBRARY_PATH
done
