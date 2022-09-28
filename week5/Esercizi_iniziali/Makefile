# Makefile for FEM course using the matlab plotting functions
# compile by typing 'make' into a terminal, clean with 'make clean' and run with './main'. You will need to install gnu fortran (gfortran) 
# or the fortran compiler of your choice first (in which case you will need to adapt the makefile)

TARGET = main_1

FC = gfortran

DEBUG = -Wall -Warray-temporaries -Wconversion -fimplicit-none -fbacktrace -ffree-line-length-0 -fcheck=all -ffpe-trap=invalid,zero,overflow,underflow -finit-real=nan  -Wno-compare-reals
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
OBJS = main_1.o


# Compile the objects based on their dependencies

types.o:\
	types.f90
	$(FC) $(FCFLAGS) -c types.f90

fedata.o:\
	fedata.f90\
	types.f90-0
	$(FC) $(FCFLAGS) -c fedata.f90

numeth.o:\
	numeth.f90\
	types.f90
	$(FC) $(FCFLAGS) -c numeth.f90

processor.o:\
	processor.f90\
	fedata.f90\
	types.f90
	$(FC) $(FCFLAGS) -c processor.f90

link1.o:\
	link1.f90\
	types.f90
	$(FC) $(FCFLAGS) -c link1.f90

plane42rect.o:\
	plane42rect.f90\
	types.f90
	$(FC) $(FCFLAGS) -c plane42rect.f90

fea.o:\
	fea.f90\
	plane42rect.f90\
	link1.f90\
	processor.f90\
	fedata.f90\
	types.f90
	$(FC) $(FCFLAGS) -c fea.f90

main_1.o:\
	main_1.f90\
	$(FC) $(FCFLAGS) -c main_1.f90

# Compile the target

$(TARGET): $(OBJS)
	$(FC) $(FLFLAGS) -o $@ $^



clean:
	-rm -f *.o *.mod main_1

cleanall:
	-rm -f *.o *.mod .fem_filename main_1