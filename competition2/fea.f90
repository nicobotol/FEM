module fea

  !! This module contains procedures that are common to FEM-analysis

  implicit none
  save
  private
  public :: displ, initial, buildload, buildstiff, enforce, recover, computational_cost, element_area

contains

!
!--------------------------------------------------------------------------------------------------
!
  subroutine initial

    !! This subroutine is mainly used to allocate vectors and matrices

    use fedata
    use link1
    use plane42rect
    use build_matrix

! Hint for continuum elements:
       !integer, parameter :: mdim = 8


    ! This subroutine computes the number of global equation,
    ! half bandwidth, etc and allocates global arrays.

    ! Calculate number of equations
    neqn = 2*nn

    ! allocate the stiffness matrix depending on the approac followed
    if (.not. banded) then
        allocate (kmat(neqn, neqn))
    else
      ! compute the max band
      call bandwidth(bw, ne)
      !print*, bw
      ! allocate the memory for banded stiffness matrix
      allocate (kb(bw, neqn))
    end if
    allocate (p(neqn), d(neqn))
    allocate (strain(ne, 3), stress(ne, 3))
    allocate (stress_vm(ne))
    ! Initial stress and strain
    allocate (principal_stresses(ne, 3))
    ! principal stresses and direction
    strain = 0.0
    stress = 0.0
    stress_vm = 0.0
    principal_stresses = 0.0
    allocate (gauss_location(ng)) ! gauss positions
    allocate (gauss_weight(ng)) ! gauss weights
    allocate (gauss_location_bmat(ng_bmat)) ! gauss positions
  end subroutine initial
!
!--------------------------------------------------------------------------------------------------
!
  subroutine displ

    !! This subroutine calculates displacements

    use fedata
    use numeth
    use processor
    use build_matrix
    use plane42rect

    integer :: e
    ! integer :: i
    real(wp), dimension(:), allocatable :: plotval
    ! real(wp) :: h ! mesh size
    ! real(wp) :: h_2
    real(wp) :: time ! time for CPU time
    ! real(wp) :: start, finish ! time for star and finish the cpu timing with method 2
    real(wp) :: total_cost ! cost of operations
    
    !real(wp) :: x_pos, y_pos ! coordinates of the point that must be investigated
    !integer :: node_3_number ! node where compute the displacement
    !real(wp) :: det
    ! real(wp) :: minv
    
    ! Load the gaussian quadrature points
    call gauss_quadrature

    ! Load the gaussian quadrature points fot the evaluation of the bmat
    call gauss_quadrature_bmat

    ! Build load-vector
    call buildload
    
    ! Build stiffness matrix
    call buildstiff
    
    ! Remove rigid body modes
    call enforce
    
    ! Initialize the computation of the execution time
    call stopwatch_modified('star', time)
    
    ! ! Second method for cpu time
    ! call cpu_time(start)

    if (.not. banded) then
      ! call  compute_det(kmat, det)
      ! print*,'Determinant of kmat = ', det
      ! pause
      ! Factor stiffness matrix
      call factor(kmat)

      ! Solve for displacement vector
      call solve(kmat, p)

    else
      
      call bfactor(kb)
      
      call bsolve(kb, p)
    end if
    
    ! sopt time computation
    call stopwatch_modified('stop', time)
    
    !! Time with second method
    ! call cpu_time(finish)
    ! print*, 'time with cpu time', finish-start
    
    ! Transfer results
    d(1:neqn) = p(1:neqn)
    print*, 'Print displacement point b'
    print'(f20.8)', d(645*2) ! print displcement vector
    
    ! Recover stress
    call recover

    ! Output results
    call output

    ! Plot deformed structure
    call plotmatlabdef('Deformed')

    ! Plot element values
    allocate (plotval(ne))
    do e = 1, ne
      if (element(e)%id == 1) then
        plotval(e) = stress(e,1)
      else if (element(e)%id == 2) then
        plotval(e) = stress_vm(e)
      end if
      ! print'(i4 f15.10)', e, stress_vm(e)
    end do
    call plotmatlabeval('Stresses',plotval)
    ! print the principal stresses and direction
    call plotmatlabevec('Principal', principal_stresses(:, 1), principal_stresses(:, 2), principal_stresses(:, 3))
    ! print'(3f12.8,tr1)', transpose(principal_stresses)

    ! print'(i4 f12.1)', 574, stress_vm(574)
    ! print'(i4 f12.1)', 572, stress_vm(572)
    ! print'(i4 f12.1)', 568, stress_vm(568)
    ! print'(i4 f12.1)', 569, stress_vm(569)
    ! print'(i4 f12.1)', 570, stress_vm(570)
    ! print'(i4 f12.1)', 571, stress_vm(571)
    ! print'(i4 f12.1)', 573, stress_vm(573)
    ! print'(i4 f12.1)', 575, stress_vm(575)

    ! print on file

    ! ! element size and its square
    ! h = x(element(1)%ix(2),1) - x(element(1)%ix(1),1)
    ! h_2 = h**2

    ! if not banded form imlemented then bandwith is 0
    if ( .not. banded) then
      bw = 0
    end if

    ! calculate the computational cost
    call computational_cost(neqn, bw, total_cost)

    ! Write on file
    ! open(unit = 11, file = "results_band_renum.txt", position = "append")
    ! write(11, '(a a i4 a i5 a e9.4 a e9.3 )' ) trim(filename), ',', bw, ',', neqn, ',', time, ',', total_cost
    ! close(11)
  end subroutine displ 
!
!--------------------------------------------------------------------------------------------------
!
  subroutine buildload

    !! This subroutine builds the global load vector

    use fedata
    use plane42rect

    integer :: i, j, nen, eface
! Hint for continuum elements:
    integer, dimension(mdim) :: edof
    real(wp), dimension(mdim) :: xe
    real(wp), dimension(mdim) :: re
    real(wp), dimension(neqn) :: r
    real(wp) :: fe ! value of the pressure applied to the face
    real(wp) :: thk
    integer :: node, pos, e
    

    ! Build load vector
    p(1:neqn) = 0.0

    ! initilize vector for distributed loads
    r = 0.0


    do i = 1, np
      select case(int(loads(i, 1)))
      case( 1 )
        ! Build nodal load contribution
        node = int(loads(i, 2)) ! node number
        pos = node*2 - (2 - int(loads(i, 3))) ! position of the load in p
        p(pos) = loads(i, 4) ! insert the load in p
      case( 2 )
        ! Build uniformly distributed surface (pressure) load contribution

        ! Find coordinates and degrees of freedom
        e = int(loads(i, 2)) ! number of the node where load is applied
        nen = element(e)%numnode ! number of the nodes of e
        do j = 1, nen
          xe(2*j-1) = x(element(e)%ix(j),1)
          xe(2*j  ) = x(element(e)%ix(j),2)
          edof(2*j-1) = 2 * element(e)%ix(j) - 1
          edof(2*j)   = 2 * element(e)%ix(j)
        end do

        ! face of the element where load is applied
        eface = int(loads(i, 3))

        ! value of the pressure applied to the face
        fe = loads(i, 4)

        ! thickness where load is applied
        thk = mprop(element(e)%mat)%thk

        ! build re
        call plane42rect_re(xe, eface, fe, thk, re)

        ! combine re in r vector
        do j = 1, mdim
          r(edof(j)) = r(edof(j)) + re(j)
        end do

        !print'(f20.8)', r ! remove, only placed for testing porpuses
        !print *, 'ERROR in fea/buildload'
        !print *, 'Distributed loads not defined -- you need to add your own code here'
        !stop

      case default
        print *, 'ERROR in fea/buildload'
        print *, 'Load type not known'
        stop
      end select
    end do
    p = p + r ! add the distributed and concentrated loads
    ! print'(f12.8)', p

  end subroutine buildload
!
!--------------------------------------------------------------------------------------------------
!
  subroutine buildstiff

    !! This subroutine builds the global stiffness matrix from
    !! the local element stiffness matrices

    use fedata
    use link1
    use plane42rect

    integer :: e, i, j
    integer :: nen
! Hint for system matrix in band form:
!        integer :: irow, icol
    integer, dimension(mdim) :: edof
    real(wp), dimension(mdim) :: xe
    real(wp), dimension(mdim, mdim) :: ke
    ! Hint for modal analysis:
    !        real(wp), dimension(mdim, mdim) :: me
    real(wp) :: young, area, nu, thk
    ! Hint for modal analysis and continuum elements:
    !        real(wp) :: nu, dens, thk
    real(wp) :: e_area, surface ! area of the structure

    
    ! initialize area
    surface = 0.0

    ! Reset stiffness matrix
    if (.not. banded) then
      kmat = 0.0
    else
      kb = 0.0
      ! print*,'ERROR in fea/buildstiff'
      ! print*,'Band form not implemented -- you need to add your own code here'
      ! stop
    end if
    
    do e = 1, ne
      
      ! Find coordinates and degrees of freedom
      nen = int(element(e)%numnode)
      
      do i = 1, nen
        xe(2*i-1) = x(element(e)%ix(i),1)
        xe(2*i  ) = x(element(e)%ix(i),2)
        edof(2*i-1) = 2 * element(e)%ix(i) - 1
        edof(2*i)   = 2 * element(e)%ix(i)
      end do

      ! Gather material properties and find element stiffness matrix
      select case( element(e)%id ) ! verify if we have a truss or a continuum structure
        case( 1 )
          young = mprop(element(e)%mat)%young
          area  = mprop(element(e)%mat)%area
          call link1_ke(xe, young, area, ke)
        case( 2 )

          young = mprop(element(e)%mat)%young
          nu = mprop(element(e)%mat)%nu
          thk = mprop(element(e)%mat)%thk
          
          call plane42rect_ke(xe, young, nu, thk, ke)
            !print *, 'ERROR in fea/buildstiff:'
          !print *, 'Stiffness matrix for plane42rect elements not implemented -- you need to add your own code here'
          !stop
      end select

      ! Assemble into global matrix
      if (.not. banded) then
        do i = 1, 2*nen
          do j = 1, 2*nen
            kmat(edof(i), edof(j)) = kmat(edof(i), edof(j)) + ke(i, j)
          end do
        end do
        
        ! Hint: Can you eliminate the loops above by using a different Fortran array syntax?
      else
        do i = 1, 2*nen
          do j = 1, 2*nen
            ! check wether we are on the lower part of the Kmat
            if (edof(i) >= edof(j)) then
              kb(edof(i)-edof(j)+1, edof(j)) = kb(edof(i)-edof(j)+1, edof(j)) + ke(i, j)
            end if
          end do
        end do
        ! print *, 'ERROR in fea/buildstiff'
        ! print *, 'Band form not implemented -- you need to add our own code here'
        ! stop
      end if

      ! Compute the area of the entire structure by adding the one of the element
      call element_area(xe, e_area)
      
      surface = surface + e_area
      
    end do
    
    print*, 'The area of the structure is', surface
    ! print'(24f5.2)', transpose(kb)
  end subroutine buildstiff
!
  !--------------------------------------------------------------------------------------------------
  !
  subroutine enforce

    !! This subroutine enforces the support boundary conditions
    
    use fedata
    
    integer :: i, idof, j
    real(wp) :: penal

    ! print'(24f5.2)', transpose(kb)
    ! print*, 'pausa'

    ! Correct for supports
    if (.not. banded) then
      if (.not. penalty) then
        do i = 1, nb
          idof = int(2*(bound(i,1)-1) + bound(i,2))
          p(1:neqn) = p(1:neqn) - kmat(1:neqn, idof) * bound(i, 3)
          p(idof) = bound(i, 3)
          kmat(1:neqn, idof) = 0.0
          kmat(idof, 1:neqn) = 0.0
          kmat(idof, idof) = 1.0

          !print*,bound(i,:)
        end do
      else
        penal = penalty_fac*maxval(kmat)
        do i = 1, nb
          idof = int(2*(bound(i,1)-1) + bound(i,2))
          kmat(idof, idof) = kmat(idof, idof) + penal
          p(idof) = penal * bound(i, 3)
        end do
      end if
    else
      do i = 1, nb
        idof = 0
        idof = int(2*(bound(i,1)-1) + bound(i,2))
        do j = 2, bw
          kb(j, idof) = 0.0 ! wite 0 on the column
            if ((j <= idof)) then
              kb(j, idof - j + 1) = 0.0 ! write 0 on the diagonal
            end if
          end do
        kb(1, idof) = 1.0 ! write 1 in the first row
        p(idof) = 0.0 ! enforce boundary on p
      end do
      ! print *, 'ERROR in fea/enforce'
      ! print *, 'Band form not implemented -- you need to add your own code here'
      ! stop
    end if
    ! print'(24f6.3)', transpose(kb(3:4,:))

  end subroutine enforce
  !
  !--------------------------------------------------------------------------------------------------
  !
  subroutine recover

    !! This subroutine recovers the element stress, element strain,
    !! and nodal reaction forces
    
    use fedata
    use link1
    use plane42rect

    integer :: e, i, nen
    integer :: edof(mdim)
    real(wp), dimension(mdim) :: xe, de
    real(wp), dimension(mdim, mdim) :: ke
    real(wp) :: young, area
    ! Hint for continuum elements:
    !        real(wp):: nu, dens, thk
    real(wp), dimension(3) :: estrain, estress
    real(wp) :: estress_vm, estress_1, estress_2, psi
    real(wp) :: nu
    real(wp), dimension(neqn) :: temp
    
    ! Reset force vector
    p = 0.0
    
    do e = 1, ne
      
      ! Find coordinates etc...
      nen = int(element(e)%numnode)
      do i = 1,nen
        xe(2*i-1) = x(element(e)%ix(i), 1)
        xe(2*i)   = x(element(e)%ix(i), 2)
        edof(2*i-1) = 2 * element(e)%ix(i) - 1
        edof(2*i)   = 2 * element(e)%ix(i)
        de(2*i-1) = d(edof(2*i-1))
        de(2*i)   = d(edof(2*i))
      end do
      
      ! Find stress and strain
      select case( int(element(e)%id) )
      case( 1 ) ! thruss struct
        young = mprop(element(e)%mat)%young
        area  = mprop(element(e)%mat)%area
        call link1_ke(xe, young, area, ke)
        temp = matmul(ke(1:2*nen,1:2*nen), de(1:2*nen))
        p(edof(1:2*nen)) = p(edof(1:2*nen)) + temp
        call link1_ss(xe, de, young, estress, estrain)
        stress(e, 1:3) = estress
        strain(e, 1:3) = estrain
      case( 2 ) ! continuum
        young = mprop(element(e)%mat)%young
        nu = mprop(element(e)%mat)%nu
        call plane42rect_ss(xe, de, young, nu, estress, estrain, estress_vm, estress_1, estress_2, psi)
        stress(e, 1:3) = estress
        strain(e, 1:3) = estrain
        stress_vm(e) = estress_vm
        ! store principal stresses and direction
        principal_stresses(e, 1) = estress_1
        principal_stresses(e, 2) = estress_2
        principal_stresses(e, 3) = psi

      end select
    end do
    ! print*, 'Von mises stress for point B'
    ! print*, stress_vm(420)
    !print*, 'Von mises stress for point B'
    !print*, stress_vm(25)
    
    !print*, 'Von mises stress'
    !print'(f20.8)', stress_vm ! print von mises stress vector
    !print*, 'End Von mises stress'
  end subroutine recover

  ! compute the cost to perform the calculations
  subroutine computational_cost(n, band_w, total_cost)
    use fedata

    integer, intent(in) :: n ! number of equations
    integer, intent(in) :: band_w ! bandwidth
    real(wp), intent(out) :: total_cost ! number of equations
    real(wp) :: decomposition_cost, substitution_cost
    real(wp) :: n_real
    ! LU factorization cost
    n_real = real(n)
    if (.not. banded) then
      ! banded not implemented
      decomposition_cost = n_real**3/3
      substitution_cost = n_real**2
    else 
      ! banded implemented
      decomposition_cost = band_w**2*n_real/2
      substitution_cost = 2*band_w*n_real
    endif
    total_cost = decomposition_cost + substitution_cost
  end subroutine computational_cost


  !                                 __        _                           _   
  !    __ _ _ __ ___  __ _    ___  / _|   ___| | ___ _ __ ___   ___ _ __ | |_ 
  !   / _` | '__/ _ \/ _` |  / _ \| |_   / _ \ |/ _ \ '_ ` _ \ / _ \ '_ \| __|
  !  | (_| | | |  __/ (_| | | (_) |  _| |  __/ |  __/ | | | | |  __/ | | | |_ 
  !   \__,_|_|  \___|\__,_|  \___/|_|    \___|_|\___|_| |_| |_|\___|_| |_|\__|
  
  ! compute the area using Heron's formula
  subroutine element_area(xe, e_area)
    use fedata
    real(wp), dimension(8), intent(in):: xe
    real(wp), intent(out):: e_area
    real(wp) :: aa, bb, cc, dd, ee, s1, s2 ! edges of the block
    real(wp) :: A1, A2

    A1 = 0.0 ! area of the 1st triangle (half of quadrilater's area)
    A2 = 0.0 ! area of the 2nd triangle (half of quadrilater's area)

    aa = ((xe(3) - xe(1))**2 + (xe(4) - xe(2))**2)**0.5 ! edge
    bb = ((xe(7) - xe(1))**2 + (xe(8) - xe(2))**2)**0.5 ! edge
    cc = ((xe(3) - xe(7))**2 + (xe(4) - xe(8))**2)**0.5 ! diagonal 
    dd = ((xe(3) - xe(5))**2 + (xe(4) - xe(6))**2)**0.5 ! edge
    ee = ((xe(5) - xe(7))**2 + (xe(6) - xe(8))**2)**0.5 ! edge
    
    s1 = (aa + bb + cc)*0.5 ! semiperimeter of 1st triangle
    s2 = (ee + dd + cc)*0.5 ! semiperimeter of 2nd triangle
    
    A1 = (s1*(s1 - aa)*(s1 - bb)*(s1 - cc))**0.5 ! area of 1st triangle
    A2 = (s2*(s2 - dd)*(s2 - ee)*(s2 - cc))**0.5 ! area of 2nd triangle

    e_area = A1 + A2 ! element area
    
  end subroutine element_area
end module fea


