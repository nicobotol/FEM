# Makefile for FEM course using the matlab plotting functions
# compile by typing 'make' into a terminal, clean with 'make clean' and run with './main'. You will need to install gnu fortran (gfortran) 
# or the fortran compiler of your choice first (in which case you will need to adapt the makefile)

TARGET = main

FC = gfortran

# DEBUG = -Wall -Warray-temporaries -Wconversion -fimplicit-none -fbacktrace -ffree-line-length-0 -fcheck=all -ffpe-trap=invalid,zero,overflow,underflow -finit-real=nan  -Wno-compare-reals
DEBUG = -Wall -fbounds-check -Warray-temporaries -Wconversion -fimplicit-none -fbacktrace -ffree-line-length-0 -fcheck=all  -finit-real=nan  -Wno-compare-reals #-ffpe-trap=invalid,zero,overflow,underflow
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
OBJS = types.o fedata.o numeth.o processor.o link1.o build_matrix.o plane42rect.o fea.o main.o


# Compile the objects based on their dependencies

types.o:\
	types.f90
	$(FC) $(FCFLAGS) -c types.f90

fedata.o:\
	fedata.f90\
	types.f90
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

build_matrix.o:\
	build_matrix.f90\
	types.f90
	$(FC) $(FCFLAGS) -c build_matrix.f90

plane42rect.o:\
	plane42rect.f90\
	build_matrix.f90\
	types.f90
	$(FC) $(FCFLAGS) -c plane42rect.f90

fea.o:\
	fea.f90\
	plane42rect.f90\
	build_matrix.f90\
	link1.f90\
	processor.f90\
	fedata.f90\
	types.f90
	$(FC) $(FCFLAGS) -c fea.f90

main.o:\
	main.f90\
	fea.f90\
	plane42rect.f90\
	build_matrix.f90\
	link1.f90\
	processor.f90\
	fedata.f90\
	types.f90
	$(FC) $(FCFLAGS) -c main.f90

# Compile the target

$(TARGET): $(OBJS)
	$(FC) $(FLFLAGS) -o $@ $^



clean:
	-rm -f *.o *.mod main

cleanall:
	-rm -f *.o *.mod .fem_filename main