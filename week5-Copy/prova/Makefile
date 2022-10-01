# Makefile for FEM course using the matlab plotting functions
# compile by typing 'make' into a terminal, clean with 'make clean' and run with './main'. You will need to install gnu fortran (gfortran) 
# or the fortran compiler of your choice first (in which case you will need to adapt the makefile)

TARGET = main

FC = gfortran

DEBUG = -g -Wall -Warray-temporaries -Wconversion -fimplicit-none -fbacktrace -ffree-line-length-0 -fcheck=all -ffpe-trap=invalid,zero,overflow,underflow -finit-real=nan  -Wno-compare-reals
OPT = -O3
FCFLAGS = -ffree-form  -Wall -Wconversion -fbounds-check
FLFLAGS = -g
#FCFLAGS += $(OPT)
#FLFLAGS += $(OPT)
FCFLAGS += $(DEBUG)
FLFLAGS += $(DEBUG)

# Remove limit on line length ()
FFLAGS += -ffree-line-length-512

all: $(TARGET)

# Declare objects, in hierachical order
OBJS = library.o main.o

# Compile the objects based on their dependencies

library.o:\
	library.f90
	$(FC) $(FCFLAGS) -c library.f90

main.o:\
	main.f90\
	library.f90
	$(FC) $(FCFLAGS) -c main.f90

# Compile the target

$(TARGET): $(OBJS)
	$(FC) $(FLFLAGS) -o $@ $^

clean:
	-rm -f *.o *.mod main

cleanall:
	-rm -f *.o *.mod .fem_filename main