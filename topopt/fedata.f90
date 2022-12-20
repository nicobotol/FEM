module fedata
    use types
    implicit none
    save

    !! This module is used to define global values and matrices

    integer, parameter :: mdim = 8
        !! Maximum number of element degrees-of-freedom

    integer :: ne
        !! Total number of elements
    integer :: nn
        !! Total number of nodes
    integer :: nb
        !! Number of boundary conditions
    integer :: np
        !! Number of loads
    integer :: nm
        !! Number of materials
    integer :: neqn
        !! Number of equations
    integer :: bw
        !! Bandwidth of system matrix

    real(wp), dimension(:,:), allocatable :: x
        !! Nodal coordinates:
        !! * _i_-th row: coordinates for node _i_
        !! * column 1: \(x\)-coordinate
        !! * column 2: \(y\)-coordinate

    type element_def
        !! The "element" data type, which is used to
        !! describe a single element of a structure
        integer, dimension(4) :: ix
            !! Node list
        integer :: id
            !! Element id:
            !! * 1: truss element
            !! * 2: continuum element
        integer :: mat
            !! Identification number for material used by this element
        integer :: numnode
            !! Number of nodes on this element
    end type element_def
    type(element_def), dimension(:), allocatable :: element
        !! The elements of a structure

    type matprop
        !! The "material" data type, which is used to
        !! describe a material. Note that a given material
        !! might not use all of the properties defined below
        real(wp) :: young
            !! Young's Modulus
        real(wp) :: nu
            !! Poisson's Ratio
        real(wp) :: thk
            !! The thickness of the structure where this material is used
        real(wp) :: area
            !! The cross-sectional area of the structure where this material is used
        real(wp) :: dens
            !! Density
        real(wp) :: youngy
            !! Young's Modulus in the transverse direction (used for orthotropic materials)
        real(wp) :: shear
            !! Shear Modulus (used for orthotropic materials)
    end type matprop
    type(matprop), dimension(:), allocatable :: mprop
        !! The materials of a structure

    ! Boundary conditions
    real(wp), dimension(:,:), allocatable :: bound
        !! Boundary conditions
        !!
        !! * _i_-th row: boundary condition number _i_
        !! * column 1: index of node where boundary condition is applied
        !! * column 2: degree of freedom _i_ affected by boundary condition
        !!     * _i_ = 1: \(x\)-component
        !!     * _i_ = 2: \(y\)-component
        !! * column 3: prescribed value for the displacement
    real(wp), dimension(:,:), allocatable :: loads
        !! External loading
        !!
        !! * _i_-th row: external load number _i_
        !! * column 1: type _j_ of external loading
        !!     * _j_ = 1: point load
        !!     * _j_ = 2: pressure load
        !!
        !! The meaning of columns 2 - 4 depend on the type of external loading:
        !!
        !! * For point loads:
        !!     * column 2: index of node where load is applied
        !!     * column 3: degree of freedom _k_ affected by load
        !!         - _k_ = 1: \(x\)-component
        !!         - _k_ = 2: \(y\)-component
        !!      * column 4: value of applied force
        !!
        !! * For surface loads:
        !!     * column 2: index of element where pressure load is applied
        !!     * column 3: index of element face where pressure is applied
        !!     * column 4: value of applied pressure (always normal to surface)
    real(wp) :: accel(2)
        !! Acceleration forces
        !!
        !! * index 1: acceleration along \(x\)-component
        !! * index 2: acceleration along \(y\)-component

    ! Working arrays:
      !! Stiffness matrix
    real(wp), dimension(:,:), allocatable :: kmat
      !! Stiffness bounded matrix
    real(wp), dimension(:,:), allocatable :: kb
      !! Strains at different places in the structure
    real(wp), dimension(:,:), allocatable :: mb
      !! Mass matrix
    real(wp), dimension(:,:), allocatable :: cb
      !! Damping matrix
    real(wp), dimension(:,:), allocatable :: strain
        !!
        !! * _i_-th row: strain in element _i_
        !! * column 1: \(\epsilon_{11}\)
        !! * column 2: \(\epsilon_{22}\)
        !! * column 3: \(\epsilon_{12}\)
    real(wp), dimension(:,:), allocatable :: stress
        !! Stresses at different places in the structure
        !!
        !! * _i_-th row: stress in element _i_
        !! * column 1: \(\sigma_{11}\)
        !! * column 2: \(\sigma_{22}\)
        !! * column 3: \(\sigma_{12}\)
    real(wp), dimension(:), allocatable :: stress_vm
        !! von mises stress
    real(wp), dimension(:,:), allocatable :: principal_stresses
        !! principal stress and direction
        !! 
        !! * _i_-th row: stressesa and direction in element _i_
        !! * column 1: \(\sigma_1\)
        !! * column 2: \(\sigma_2\)
        !! * column 3: psi
    real(wp), dimension(:),   allocatable :: p
        !! Force vector
    real(wp), dimension(:),   allocatable :: d
        !! Displacement vector
    
    ! Input/Output
    character(len=50) :: filename
        !! Name of input file

    ! Analysis type
    character(len=20) :: antype
        !! The type of analysis to be carried out
        !!
        !! STATIC
        !! : Static linear analysis (default)
        !!
        !! STATIC_NL
        !! : Static geometrically nonlinear analysis (HINT: not implemented yet)
        !!
        !! MODAL
        !! : Analysis of eigenfrequencies and mode shapes (HINT: not implemented yet)
        !!
        !! ANGLE
        !! : Optimization of fiber orientation of anisotropic material (HINT: not implemented yet)
        !!
        !! TRANS
        !! : Transient analysis (HINT: not implemented yet)

    ! Parameters
    real(wp), parameter :: scale_def = 1.0_wp
        !! Scale deformations when plotting
    real(wp), parameter :: scale_vec = 1.0_wp
        !! Scale length of vectors for plotting
    real(wp), parameter :: scale_thk = 1.0_wp
        !! Scale thickness of lines
    logical, parameter :: banded = .true.
        !! Indicate whether the system matrix is in banded form or not (full matrix)
    logical, parameter :: penalty = .false.
        !! Indicate whether boundary conditions are imposed by the penalty method or not (zero-one method)
    real(wp), parameter :: penalty_fac = 1.0e10_wp
        !! Scaling factor for boundary conditions imposed by the penalty method
    logical, parameter :: isoparametric = .true.
        !! Indicate wheather apply or not the isoparamteric formulation


    integer :: ng = 2
    !! number of gauss points
    integer :: ng_bmat = 1
    !! number of gauss points where to evaluate bmat 
    real(wp), dimension(:), allocatable :: gauss_location
    !! location of the gauss quadrature points
    real(wp), dimension(:), allocatable :: gauss_weight
    !! weights of the gauss quadrature points
    real(wp), dimension(:), allocatable :: gauss_location_bmat
    !! location where to evaluate the gauss quarature for stress computation

    ! EIGENVECTOR ANALISYS
    real(wp), dimension(:, :), allocatable :: ematrix
        !! Matrix containing the eigenvectors
    integer :: neig = 6
        !! number of eigenvector we are looking for
    integer :: pmax = 1000
        !! max number of iterations
    real(wp) :: epsilon = 1E-12

    ! TRANSIENT ANALYSIS
    real(wp) :: kappa = 0.0 ! viscous damping coefficient
    real(wp) :: delta_t = 1E-4 ! time step
    integer :: transient_iter_max = 3000 ! maximum number of iteration for the transien analysis
    integer :: load_type = 2 ! type of external load
      ! load_type = 1 -> ramp
      ! load_type = 2 -> step
      ! load_type = 3 -> sine
      ! load_type = 4 -> ramp + constant 
      ! load_type = 5 -> ramp + remove of the load 
    real(wp) :: max_load_magnitude = 10 ! max value or amplitude of the load
    integer :: material_type = 1 ! type of material
      ! material_type = 1 -> linear elastic
      ! material_type = 2 -> non linear elastic
    real(wp) :: omega_load = 187.7 ! frequency of the harmonic input
    integer :: dof_disp = 2*339 ! degree of freedom to be displayed
    logical, parameter :: lumped = .false. ! use lumped mass
    real(wp),  dimension(:), allocatable :: mb_lumped
    real(wp) :: beta = 1.0/4.0 ! coefficient for newmark method
    real(wp) :: gamma = 1.0/2.0 ! coefficient for newmark method
    integer :: method = 2 ! type of transient method
      ! method = 1 -> central difference explicit 
      ! method = 2 -> newmark
      ! method = 3 -> no dynamic analysis 
    logical, parameter :: proportional_damping = .true.
    real(wp) :: alpha_damping = 1E-3
    real(wp) :: beta_damping = 1E-5

    ! TOPOLOGY OPTIMIZATION
    logical, parameter :: topopt_flag = .false.  ! flag to enable the topology optimization
    real(wp), dimension(:), allocatable :: g_grad
    real(wp), dimension(:), allocatable :: f_grad
    real(wp), dimension(:), allocatable :: dens_old
    real(wp), dimension(:), allocatable :: f_vector ! vector for store the compliance 
    real(wp) :: p_topopt = 1.4 ! p coefficient for topology optimization
    real(wp) :: f_tot = 0.0 ! total compliance
    real(wp) :: V_star = 10 ! maximum volume allowed
    integer :: iopt_max = 1000 ! max number of iterations for the topology optimization
    real(wp) :: epsilon_topopt = 1e-12 ! convergence criterion
    real(wp) :: lambda_lower = 1e-10 ! guess lower value 
    real(wp) :: lambda_upper = 1e10 ! guess lower value 
    real(wp) :: dens_min = 1e-6 ! minimum density
    real(wp) :: eta_topopt = 0.5 ! etaa coefficient

end module fedata
