module fea

  !! This module contains procedures that are common to FEM-analysis

  implicit none
  save
  private
  public :: displ, initial, buildload, buildstiff, buildmass, builddamping, enforce, recover, eigen, mmul, computational_cost, single_eigen, sturm_check, external_load_ft, enforce_mat, enforce_vec, central_diff_exp, courant_check, newmark_imp

contains

!
!--------------------------------------------------------------------------------------------------
!
  subroutine initial

    !! This subroutine is mainly used to allocate vectors and matrices

    use fedata
    use link1
    use plane42
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
      ! allocate the memory for banded stiffness matrix and mass matrix
      allocate (mb(bw, neqn))
      if (banded) then ! allocate the memory for the banded matrix
        allocate(mb_lumped(neqn))
      end if
      allocate (kb(bw, neqn))
      allocate (cb(bw, neqn)) ! damping stiffness matrix
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

    allocate (ematrix(neqn, neig))
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
    use plane42

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
      p = p*max_load_magnitude
      
      call bsolve(kb, p)
      ! print*, 'Print displacement point b'
      ! print'(f20.8)', p(441*2) ! print displcement vector
    end if

    ! sopt time computation
    call stopwatch_modified('stop', time)
    
    !! Time with second method
    ! call cpu_time(finish)
    ! print*, 'time with cpu time', finish-start
    
    ! Transfer results
    d(1:neqn) = p(1:neqn)

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

    print'(i4 f12.8)', 6, stress_vm(6)

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

    print*, 'Displacement from static case', d(dof_disp)
    ! print*, 'Stress from static case', stress_vm(620)
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
    use plane42

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
        call plane42_re(xe, eface, fe, thk, re)

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
    use plane42

    integer :: e, i, j
    integer :: nen
! Hint for system matrix in band form:
!        integer :: irow, icol
    integer, dimension(mdim) :: edof
    real(wp), dimension(mdim) :: xe
    real(wp), dimension(mdim, mdim) :: ke
    real(wp) :: young, area, nu, thk, dens
    ! Hint for modal analysis and continuum elements:
    !        real(wp) :: nu, dens, thk

    ! Reset stiffness matrix
    if (.not. banded) then
      kmat = 0.0
    else
      kb = 0.0
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
          dens = mprop(element(e)%mat)%dens
         

          call plane42_ke(xe, young, nu, thk, ke)
            !print *, 'ERROR in fea/buildstiff:'
            !print *, 'Stiffness matrix for plane42 elements not implemented -- you need to add your own code here'
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
              kb(edof(i) - edof(j) + 1, edof(j)) = kb(edof(i) - edof(j) + 1, edof(j)) + ke(i, j) ! stiffness matrix
            end if
          end do
        end do
      end if
    end do
    ! print'(24f5.2)', transpose(kb)
  end subroutine buildstiff


  subroutine buildmass

    !! This subroutine builds the global mass matrix from
    !! the local element mass matrices

    use fedata
    use link1
    use plane42

    integer :: e, i, j
    integer :: nen
! Hint for system matrix in band form:
!        integer :: irow, icol
    integer, dimension(mdim) :: edof
    real(wp), dimension(mdim) :: xe
    real(wp), dimension(mdim, mdim) :: me
    real(wp) :: thk, dens
    ! Hint for modal analysis and continuum elements:
    !        real(wp) :: nu, dens, thk

    mb = 0.0
    
    do e = 1, ne

      ! Find coordinates and degrees of freedom
      nen = int(element(e)%numnode)

      do i = 1, nen
        xe(2*i-1) = x(element(e)%ix(i),1)
        xe(2*i  ) = x(element(e)%ix(i),2)
        edof(2*i-1) = 2 * element(e)%ix(i) - 1
        edof(2*i)   = 2 * element(e)%ix(i)
      end do


      thk = mprop(element(e)%mat)%thk
      dens = mprop(element(e)%mat)%dens
      
      call plane42_me(xe, dens, me, thk)
        !print *, 'ERROR in fea/buildstiff:'
        !print *, 'Stiffness matrix for plane42 elements not implemented -- you need to add your own code here'
        !stop

        ! Hint: Can you eliminate the loops above by using a different Fortran array syntax?
        do i = 1, 2*nen
          do j = 1, 2*nen
            ! check wether we are on the lower part of the Kmat
            if (edof(i) >= edof(j)) then
              mb(edof(i) - edof(j) + 1, edof(j)) = mb(edof(i) - edof(j) + 1, edof(j)) + me(i, j) ! stiffness matrix
            end if
          end do
        end do
    end do
    ! print'(24f5.2)', transpose(kb)
  end subroutine buildmass

  !   _                                _                           
  !  | |_   _ _ __ ___  _ __   ___  __| |  _ __ ___   __ _ ___ ___ 
  !  | | | | | '_ ` _ \| '_ \ / _ \/ _` | | '_ ` _ \ / _` / __/ __|
  !  | | |_| | | | | | | |_) |  __/ (_| | | | | | | | (_| \__ \__ \
  !  |_|\__,_|_| |_| |_| .__/ \___|\__,_| |_| |_| |_|\__,_|___/___/
  !                    |_|                                         
  subroutine buildmass_lumped

    !! This subroutine builds the global mass matrix in lumped form from
    !! the local element mass matrices

    use fedata
    use link1
    use plane42

    integer :: e, i
    integer :: nen
! Hint for system matrix in band form:
!        integer :: irow, icol
    integer, dimension(mdim) :: edof
    real(wp), dimension(mdim) :: xe
    real(wp), dimension(mdim) :: me_lumped
    real(wp) :: thk, dens
    ! Hint for modal analysis and continuum elements:
    !        real(wp) :: nu, dens, thk

    mb_lumped = 0.0
    
    do e = 1, ne

      ! Find coordinates and degrees of freedom
      nen = int(element(e)%numnode)

      do i = 1, nen
        xe(2*i-1) = x(element(e)%ix(i),1)
        xe(2*i  ) = x(element(e)%ix(i),2)
        edof(2*i-1) = 2 * element(e)%ix(i) - 1
        edof(2*i)   = 2 * element(e)%ix(i)
      end do


      thk = mprop(element(e)%mat)%thk
      dens = mprop(element(e)%mat)%dens
      
      call plane42_me_lumped(xe, dens, me_lumped, thk)
        !print *, 'ERROR in fea/buildstiff:'
        !print *, 'Stiffness matrix for plane42 elements not implemented -- you need to add your own code here'
        !stop

        ! Hint: Can you eliminate the loops above by using a different Fortran array syntax?
        do i = 1, 2*nen
          mb_lumped(edof(i)) = mb_lumped(edof(i)) + me_lumped(i) ! lumped mass matrix
        end do
    end do
    ! print'(24f5.2)', transpose(kb)
  end subroutine buildmass_lumped

!   _           _ _     _     _                       _             
!  | |__  _   _(_) | __| | __| | __ _ _ __ ___  _ __ (_)_ __   __ _ 
!  | '_ \| | | | | |/ _` |/ _` |/ _` | '_ ` _ \| '_ \| | '_ \ / _` |
!  | |_) | |_| | | | (_| | (_| | (_| | | | | | | |_) | | | | | (_| |
!  |_.__/ \__,_|_|_|\__,_|\__,_|\__,_|_| |_| |_| .__/|_|_| |_|\__, |
!                                              |_|            |___/ 
  subroutine builddamping

    !! This subroutine builds the global mass matrix from
    !! the local element mass matrices

    use fedata
    use link1
    use plane42

    integer :: e, i, j
    integer :: nen
! Hint for system matrix in band form:
!        integer :: irow, icol
    integer, dimension(mdim) :: edof
    real(wp), dimension(mdim) :: xe
    real(wp), dimension(mdim, mdim) :: ce, me, ke
    real(wp) :: young, nu, thk, dens

    cb = 0.0
    
    do e = 1, ne

      ! Find coordinates and degrees of freedom
      nen = int(element(e)%numnode)

      do i = 1, nen
        xe(2*i-1) = x(element(e)%ix(i),1)
        xe(2*i  ) = x(element(e)%ix(i),2)
        edof(2*i-1) = 2 * element(e)%ix(i) - 1
        edof(2*i)   = 2 * element(e)%ix(i)
      end do

      thk = mprop(element(e)%mat)%thk
      dens = mprop(element(e)%mat)%dens
      nu = mprop(element(e)%mat)%nu
      young = mprop(element(e)%mat)%young
      
      if ( .not. proportional_damping) then ! case of constant damping
        call plane42_ce(xe, kappa, ce)
      else ! case of proportional damping
        call plane42_me(xe, dens, me, thk)
        call plane42_ke(xe, young, nu, thk, ke)
        ce = alpha_damping*me + beta_damping*ke

      end if

        !print *, 'ERROR in fea/buildstiff:'
        !print *, 'Stiffness matrix for plane42 elements not implemented -- you need to add your own code here'
        !stop

        ! Hint: Can you eliminate the loops above by using a different Fortran array syntax?
        do i = 1, 2*nen
          do j = 1, 2*nen
            ! check wether we are on the lower part of the Kmat
            if (edof(i) >= edof(j)) then
              cb(edof(i) - edof(j) + 1, edof(j)) = cb(edof(i) - edof(j) + 1, edof(j)) + ce(i, j) ! stiffness matrix
            end if
          end do
        end do
    end do

  end subroutine builddamping
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
              mb(j, idof - j + 1) = 0.0 ! write 0 on the diagonal
              
            end if
          end do
        kb(1, idof) = 1.0 ! write 1 in the first row
        mb(1, idof) = 1.0
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
  use plane42

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
      call plane42_ss(xe, de, young, nu, estress, estrain, estress_vm, estress_1, estress_2, psi)
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


!                 _   
!    ___ ___  ___| |_ 
!   / __/ _ \/ __| __|
!  | (_| (_) \__ \ |_ 
!   \___\___/|___/\__|
                    
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

subroutine single_eigen

  !! This subroutine performas the eigenvalues analysis
  use fedata
  use numeth
  use processor
  use build_matrix
  use plane42

  integer :: i, ii, idof
  ! integer :: i
  !real(wp), dimension(:), allocatable :: plotval
  ! real(wp) :: h ! mesh size
  ! real(wp) :: h_2
  real(wp) :: time ! time for CPU time
  ! real(wp) :: start, finish ! time for star and finish the cpu timing with method 2
  real(wp), dimension(neqn) :: Yvec 
  real(wp), dimension(neqn) :: evec_old, yvec_old
  real(wp) :: lambda, omega, rp
  real(wp) :: evec(neqn)
  
  ! Load the gaussian quadrature points
  call gauss_quadrature

  ! Load the gaussian quadrature points fot the evaluation of the bmat
  call gauss_quadrature_bmat

  ! Build load-vector
  ! call buildload
  
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
    
  end if

  ! sopt time computation
  call stopwatch_modified('stop', time)
  
  ! initial guess for eigenvector
  evec = 1.0
  Yvec = 0.0
  lambda = 0.0
  omega = 0.0
  rp = 0.0

  call mmul(evec, Yvec, 2)

  do i = 1, pmax
    Yvec_old(1:neqn) = Yvec(1:neqn) ! store the last 

    evec_old(1:neqn) = evec ! store the eig at the previous iteration

    ! enforce boundary on Yvec
    idof = 0
    do ii = 1, nb
      idof = int(2*(bound(ii, 1) - 1) + bound(ii, 2))
      Yvec_old(idof) = 0.0 
    end do

    call bsolve(kb, Yvec_old)

    evec = Yvec_old(1:neqn)

    call mmul(evec, Yvec, 2)

    rp = (dot_product(evec, Yvec))**0.5
    ! rp = (dot_product(transpose(evec), Yvec))**0.5
    
    Yvec = Yvec/rp

    if ((norm2(evec - evec_old) / norm2(evec)) < epsilon) then
      exit
    end if

  end do

  lambda = dot_product(evec, Yvec) / rp**2
  ! lambda = dot_prod(transpose(evec), Yvec_old) / rp**2
  omega = lambda**0.5

  evec = evec / rp ! compute the eigenvector

  print*, omega
  
  ! call plotmatlabeig('Mode Shape', lambda, 50.0*evec, [10.0d0, 0.10d0])

  ! if not banded form imlemented then bandwith is 0
  if ( .not. banded) then
    bw = 0
  end if


  ! Write on file
  ! open(unit = 11, file = "results_band_renum.txt", position = "append")
  ! write(11, '(a a i4 a i5 a e9.4 a e9.3 )' ) trim(filename), ',', bw, ',', neqn, ',', time, ',', total_cost
  ! close(11)
end subroutine single_eigen

!        _                  
!    ___(_) __ _  ___ _ __  
!   / _ \ |/ _` |/ _ \ '_ \ 
!  |  __/ | (_| |  __/ | | |
!   \___|_|\__, |\___|_| |_|
!          |___/            

subroutine eigen

  !! This subroutine performas the eigenvalues analysis for multiple eigen mode shape
  use fedata
  use numeth
  use processor
  use build_matrix
  use plane42

  integer :: i, ii, idof, l, j
  ! integer :: i
  !real(wp), dimension(:), allocatable :: plotval
  ! real(wp) :: h ! mesh size
  ! real(wp) :: h_2
  real(wp) :: time ! time for CPU time
  ! real(wp) :: start, finish ! time for star and finish the cpu timing with method 2
  real(wp), dimension(neqn) :: Yvec 
  real(wp), dimension(neqn) :: evec_old, yvec_old, evec
  real(wp) :: lambda, rp, cj
  real(wp), dimension(neig) :: lambda_vec, omega_vec
  real(wp), dimension(neqn, neig) :: Zmatrix 
  
  ! Load the gaussian quadrature points
  call gauss_quadrature

  ! Load the gaussian quadrature points fot the evaluation of the bmat
  call gauss_quadrature_bmat
  
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
    
  end if

  ! sopt time computation
  call stopwatch_modified('stop', time)
  
  Zmatrix = 0.0
  ematrix = 0.0
  lambda_vec = 0.0
  omega_vec = 0.0

  do l = 1, neig ! loop for different eig
    ! fors guess eigenvector
    evec = 1.0
    Yvec = 0.0 
    evec_old = 0.0

    ! calculate Y0
    call mmul(evec, Yvec, 2)

    do j = 1, l - 1
      call mmul(ematrix(:, j), Zmatrix(:, j), 2)
    end do

    do i = 1, pmax
      Yvec_old = Yvec ! store the last Y
      evec_old = evec ! store the eig at the previous iteration

      ! enforce boundary on Yvec
      idof = 0
      do ii = 1, nb
        idof = int(2*(bound(ii, 1) - 1) + bound(ii, 2))
        Yvec_old(idof) = 0.0 
      end do

      !solve Xp
      call bsolve(kb, Yvec_old)
      evec = Yvec_old

      ! ortogonalization
      do j = 1, l - 1
        cj = dot_product(evec, Zmatrix(:, j))
        evec = evec - cj*ematrix(:, j)
      end do

      ! compute Yp
      call mmul(evec, Yvec, 2)

      ! compute rp
      rp = (dot_product(evec, Yvec))**0.5

      Yvec = Yvec/rp

      if ((norm2(evec - evec_old) / norm2(evec)) < epsilon) then
        exit
      end if

    end do

    ! save values
    ematrix(:, l) = evec/rp
    lambda = dot_product(evec, Yvec) / rp**2
    lambda_vec(l) = lambda
    omega_vec(l) = lambda**0.5

    print '(a i3 a f15.5 a f15.5)', 'Eigenmode: ', l, ' omega [rad/s] = ', omega_vec(l), ' - f [Hz] = ', omega_vec(l)/(2*3.14)

  end do

  ! print '(f10.8)', omega_vec
  
  call plotmatlabeig('Mode Shape', omega_vec(2), 0.1*ematrix(:,2), [10.0d0, 0.10d0])

  ! if not banded form imlemented then bandwith is 0
  if ( .not. banded) then
    bw = 0
  end if

  ! print_thk = mprop(element(502)%mat)%thk

  ! print*, print_thk
  ! Write on file
  ! open(unit = 11, file = "results_cantilever.txt", position = "append")
  open(unit = 11, file = "results_convergence.txt", position = "append")
  write(11, '( a f7.5 a f7.5 a f7.5 a f7.5 a f7.5 a f7.5 a)' )  ',', omega_vec(1), ',', omega_vec(2), ',', omega_vec(3), ',', omega_vec(4), ',', omega_vec(5), ',', omega_vec(6)
  close(11)

  ! perform sturm check
  call sturm_check(lambda_vec)


end subroutine eigen

!                             _ 
!   _ __ ___  _ __ ___  _   _| |
!  | '_ ` _ \| '_ ` _ \| | | | |
!  | | | | | | | | | | | |_| | |
!  |_| |_| |_|_| |_| |_|\__,_|_|
                              
subroutine mmul(Xvec, Yvec, mtype)
  use fedata
  use plane42
  use processor

  real(wp), dimension(:), intent(in) :: Xvec
  real(wp), dimension(:), intent(out) :: Yvec
  integer, intent(in) :: mtype
  real(wp) ::  young, nu, thk, dens
  real(wp), dimension(8) :: xe
  real(wp), dimension(8, 8) :: me, ce, ke, m_element 
  real(wp), dimension(8) :: m_element_lumped 
  integer :: e, i ! iterators
  integer :: nen
  integer, dimension(8) :: edof
  real(wp), dimension(8) :: X_temp, Y_temp

  Yvec = 0.0
  
  do e = 1, ne
    m_element = 0.0
    ce = 0.0
    me = 0.0

    ! element properties
    nen = element(e)%numnode
    dens = mprop(element(e)%mat)%dens ! element density
    thk = mprop(element(e)%mat)%thk
    nu = mprop(element(e)%mat)%nu
    young = mprop(element(e)%mat)%young

    ! clear the previous used vector 
    X_temp = 0.0
    Y_temp = 0.0
    
    ! build edof
    do i = 1, nen
      xe(2*i-1) = x(element(e)%ix(i),1)
      xe(2*i  ) = x(element(e)%ix(i),2)
      edof(2*i-1) = 2 * element(e)%ix(i) - 1
      edof(2*i)   = 2 * element(e)%ix(i)
    end do

    select case(mtype)
    case ( 1 ) ! stiffness matrix multiplication
      call plane42_ke(xe, young, nu, thk, m_element)

    case ( 2 ) ! mass matrix multiplication
      call plane42_me(xe, dens, m_element, thk)

    case ( 3 ) ! lumped mass matrix multiplication
      call plane42_me_lumped(xe, dens, m_element_lumped, thk)

    case ( 4 ) ! case for central difference explicit method
      call plane42_me(xe, dens, me, thk)
      call plane42_ce(xe, kappa, ce)

      m_element = me/(delta_t**2) - ce/(2.0*delta_t)

    case ( 5 ) ! case for central difference explicit method
      call plane42_me_lumped(xe, dens, m_element_lumped, thk)
      call plane42_ce(xe, kappa, ce)

      m_element = -ce/(2.0*delta_t)
      do i = 1, 8
        m_element(i,i) = m_element(i,i) + m_element_lumped(i)/(delta_t**2)
      end do

    case( 6 ) ! element damping (for initial conditions)
      if (.not. proportional_damping) then
        call plane42_ce(xe, kappa, m_element)
      else
        call plane42_me(xe, dens, me, thk)
        call plane42_ke(xe, young, nu, thk, ke)
        m_element = alpha_damping*me + beta_damping*ke ! = ce, element damping matrix
      end if

    end select
    
    ! build element X
    do i = 1, 2*nen
      X_temp(i) = Xvec(edof(i))
    end do

    select case(mtype)
    case(1, 2, 4, 5, 6)
      ! do the multiplication
      Y_temp = matmul(m_element, X_temp)
    case(3)
      do i = 1,8
        Y_temp(i) = m_element_lumped(i)*X_temp(i)
      end do
    end select

    ! assemble Yvect
    do i = 1, 2*nen
      Yvec(edof(i)) = Yvec(edof(i)) + Y_temp(i)
    end do

  end do

end subroutine mmul

!      _                        
!  ___| |_ _   _ _ __ _ __ ___  
! / __| __| | | | '__| '_ ` _ \ 
! \__ \ |_| |_| | |  | | | | | |
! |___/\__|\__,_|_|  |_| |_| |_|
                                
subroutine sturm_check(lambda_vec)

  ! this subroutine performs the sturm check
  use numeth
  use fedata
  use plane42
  use processor
  use build_matrix

  
  integer :: j, count ! iterators and counter
  real(wp), dimension(neig), intent(in) ::  lambda_vec
  real(wp), dimension(bw, neqn) :: a
  
  call buildstiff ! build stifness and mass matrix matrix
  call buildmass
  call enforce ! enforce boundary conditions

  a = 0.0 ! initialize a
  count = 0 ! initialize the counter

  a = kb - lambda_vec(neig)*mb ! compute the 

  call bfactor(a)
  
  do j = 1, neqn
    ! print '(f10.6)', kb(1,j)
    if (a(1, j) < 0.0) then
      count = count + 1
    end if
  end do

  print '(a i3)', 'Count: ', count

  if (count == neig) then
    print '(a i3 a)', 'Modeshape ', neig, ' satisfies the strurm check'
  else
    print '(a i3 a)', 'Modeshape ', neig, ' does not satisfy the strurm check'
  end if

end subroutine sturm_check

!                  _             _       _ _  __  __ 
!    ___ ___ _ __ | |_ _ __ __ _| |   __| (_)/ _|/ _|
!   / __/ _ \ '_ \| __| '__/ _` | |  / _` | | |_| |_ 
!  | (_|  __/ | | | |_| | | (_| | | | (_| | |  _|  _|
!   \___\___|_| |_|\__|_|  \__,_|_|  \__,_|_|_| |_|  
                                                   
subroutine central_diff_exp
  ! This subroutine implements the central difference explicit method
  use fedata
  use plane42
  use numeth

  real(wp), dimension(bw, neqn) :: lhs ! left hand side of the equation
  real(wp) :: actual_time ! time step, total_time
  real(wp), dimension(neqn) :: d_n, d_n_minus, d_n_plus, r_ext, r_int, md_prod, mcd_prod, rhs, d_dot, d_dotdot, initial_vector, kd_n, cd_dot
  integer :: i
  real(wp), dimension(transient_iter_max) :: d_store ! vector where to save data
  integer, dimension(transient_iter_max) :: i_store ! vector where to save iteration number
  
  call courant_check

  ! initialize all the varaibles
  lhs = 0.0
  rhs = 0.0
  actual_time = 0.0
  d_n = 0.0
  d_n_minus = 0.0
  d_n_plus = 0.0
  r_ext = 0.0
  r_int = 0.0
  d_store = 0.0
  d_dot = 0.0 ! initial velocity
  d_dotdot = 0.0 ! initial acceleration
  initial_vector = 0.0 
  kd_n = 0.0
  cd_dot = 0.0

  ! Load the gaussian quadrature points
  call gauss_quadrature

  ! build global matrix
  call builddamping
  
  ! initialize vectors for the initial conditions
  call external_load_ft(actual_time, r_ext) ! build the intial load
  call mmul(d_n, kd_n, 1) ! product between [K]D0
  call mmul(d_dot, cd_dot, 6) ! product between [K]D0
  initial_vector = r_ext - kd_n - cd_dot

  ! build global mass matrix
  if (.not. lumped) then
    call buildmass
    ! build the matrix on the lhs of the equation
    lhs = 1/delta_t**2*mb + cb/delta_t*0.5

    call bsolve(mb, initial_vector)
    d_dotdot = initial_vector
  else 
    call buildmass_lumped
    lhs = cb/delta_t*0.5
    do i = 1, neqn
      lhs(1, i) = lhs(1, i) + 1/delta_t**2*mb_lumped(i)
      d_dotdot(i) = initial_vector(i) / mb_lumped(i)
    end do
  end if

  call enforce_mat(lhs) ! enforce boundaries on lhs
  call bfactor(lhs)

  ! initialize [D]-1
  d_n = d_n - delta_t*d_dot + delta_t**2/2*d_dotdot

  do i = 1, transient_iter_max ! loop over the maximum transient time
    
    ! update d_n_minus and d_n
    d_n_minus = d_n
    d_n = d_n_plus
    
    actual_time = actual_time + delta_t 
    call external_load_ft(actual_time, r_ext) ! build the time dependent load

    call internal_load_ft(d_n, r_int) ! build the internal load

    if (.not. lumped) then ! select whether we have lumped mass or not
      call mmul(d_n, md_prod, 2) ! due the product between [M]d
      
      call mmul(d_n_minus, mcd_prod, 4) ! last term of the sum
    else 
      call mmul(d_n, md_prod, 3) ! due the product between [M]d
      
      call mmul(d_n_minus, mcd_prod, 5) ! last term of the sum
    end if
    
    ! build the right hand side
    rhs = r_ext - r_int + 2.0/delta_t**2*md_prod - mcd_prod
    
    call enforce_vec(rhs) ! enforce boundaries on rhs

    call bsolve(lhs, rhs)
    
    d_n_plus = rhs ! store the previous 

    ! d_store(i) = r_ext(dof_disp) ! store the displacement of one point
    d_store(i) = d_n_plus(dof_disp) ! store the displacement of one point
    i_store(i) = i
    
  end do

  ! Write on file
  ! open(unit = 11, file = "results.txt", position = "append")
  open(unit = 11, file = "results.txt", status = "replace")
  write(11, '(f15.8, a)' ) (d_store(i), ',', i = 1, transient_iter_max) 
  close(11)

  print*, 'Internla load on the element ', r_int(1)
end subroutine central_diff_exp

!                                            _    
!   _ __   _____      ___ __ ___   __ _ _ __| | __
!  | '_ \ / _ \ \ /\ / / '_ ` _ \ / _` | '__| |/ /
!  | | | |  __/\ V  V /| | | | | | (_| | |  |   < 
!  |_| |_|\___| \_/\_/ |_| |_| |_|\__,_|_|  |_|\_\
                                                
subroutine newmark_imp
  ! This subroutine implements the Newmark implicit method
  use fedata
  use plane42
  use numeth

  real(wp), dimension(bw, neqn) :: keff ! left hand side of the equation
  real(wp) :: actual_time ! time step, total_time
  real(wp), dimension(neqn) :: d_n, d_n_plus, r_ext, r_int, msum_1, csum_2, rhs, d_dot, d_dotdot, d_dotdot_old, initial_vector, kd_n, cd_dot, sum_1, sum_2
  integer :: i
  real(wp), dimension(transient_iter_max) :: d_store, f_store ! vector where to save data
  integer, dimension(transient_iter_max) :: i_store ! vector where to save iteration number

  ! initialize all the varaibles
  keff = 0.0
  rhs = 0.0
  actual_time = 0.0
  d_n = 0.0
  d_n_plus = 0.0
  r_ext = 0.0
  d_store = 0.0
  d_dot = 0.0 ! initial velocity
  d_dotdot = 0.0 ! initial acceleration
  d_dotdot_old = 0.0 ! old acceleration
  initial_vector = 0.0 
  kd_n = 0.0
  cd_dot = 0.0
  sum_1 = 0.0
  sum_2 = 0.0

  ! Load the gaussian quadrature points
  call gauss_quadrature

  ! build global matrix
  call builddamping
  call buildmass
  call buildstiff
  
  ! initialize vectors for the initial conditions
  call external_load_ft(actual_time, r_ext) ! build the intial load
  call mmul(d_n, kd_n, 1) ! product between [K]D0
  call mmul(d_dot, cd_dot, 6) ! product between [K]D0
  initial_vector = r_ext - kd_n - cd_dot
  call bsolve(mb, initial_vector)
  d_dotdot = initial_vector ! initialize the acceleration
  ! initialize the displacement [D]-1
  d_n = d_n - delta_t*d_dot + delta_t**2/2*d_dotdot

  ! build the matrix on the lhs of the equation
  keff = mb/(beta*delta_t**2) + gamma*cb/(beta*delta_t) + kb

  call enforce_mat(keff) ! enforce boundaries on lhs
  call bfactor(keff) ! factorize the lhs

  do i = 1, transient_iter_max ! loop over the maximum transient time
    
    ! update d_n
    d_n = d_n_plus
    
    actual_time = actual_time + delta_t 
    call external_load_ft(actual_time + delta_t, r_ext) ! build the time dependent load

    sum_1 = d_n/(beta*delta_t**2) + d_dot/(beta*delta_t) + (0.5/beta - 1.0)*d_dotdot ! term to be multiplied by [M]
    sum_2 = d_n*gamma/(beta*delta_t) + (gamma/beta - 1)*d_dot + delta_t*(0.5*gamma/beta - 1)*d_dotdot ! term to be multiplied by [C]

    call mmul(sum_1, msum_1, 2) ! do the product [M]sum_1
    call mmul(sum_2, csum_2, 6) ! do the product [C]sum_2

    ! build the right hand side
    rhs = r_ext + msum_1 + csum_2
    
    call enforce_vec(rhs) ! enforce boundaries on rhs

    call bsolve(keff, rhs)
    
    ! update variables
    d_n_plus = rhs ! displcement
    d_dotdot_old = d_dotdot ! store actual acceleration
    d_dotdot = (d_n_plus - d_n - delta_t*d_dot)/(beta*delta_t**2) - (0.5/beta - 1.0)*d_dotdot ! acceleration
    d_dot = gamma*(d_n_plus - d_n)/(beta*delta_t) - (gamma/beta - 1.0)*d_dot - delta_t*(0.5*gamma/beta - 1.0)*d_dotdot_old
    
    d_store(i) = d_n(dof_disp) ! store the displacement of one point
    f_store(i) = r_ext(dof_disp)
    ! d_store(i) = d_dotdot(dof_disp) ! store the displacement of one point
    ! d_store(i) = d_n_plus(dof_disp) ! store the displacement of one point
    i_store(i) = i

    
  end do
  
  ! Write on file
  ! open(unit = 11, file = "results.txt", position = "append")
  open(unit = 11, file = "results.txt", status = "replace")
  write(11, '(f15.8, a)' ) (d_store(i), ',', i = 1, transient_iter_max) 
  close(11)

  !  ! Write on file
  ! ! open(unit = 11, file = "results.txt", position = "append")
  ! open(unit = 11, file = "results_force.txt", status = "replace")
  ! write(11, '(f15.8, a)' ) (f_store(i), ',', i = 1, transient_iter_max) 
  ! close(11)
  
  
  ! print (f8.4 f8.4 e12.4)', '' maxval(d_store)

  ! !  print on file the max displacement
  ! open(unit = 12, file = "results_damping.txt", position = "append")
  ! write(12, '(f8.4 f8.4 f8.3 e12.4)' ), alpha_damping, beta_damping, omega_load, maxval(abs(d_store)) 
  ! close(12)

end subroutine newmark_imp

!             _                        _   _                 _ 
!    _____  _| |_ ___ _ __ _ __   __ _| | | | ___   __ _  __| |
!   / _ \ \/ / __/ _ \ '__| '_ \ / _` | | | |/ _ \ / _` |/ _` |
!  |  __/>  <| ||  __/ |  | | | | (_| | | | | (_) | (_| | (_| |
!   \___/_/\_\\__\___|_|  |_| |_|\__,_|_| |_|\___/ \__,_|\__,_|

subroutine external_load_ft(actual_time, r_ext)
  use fedata
  
  real(wp), intent(in) :: actual_time
  real(wp), dimension(neqn), intent(out) :: r_ext
  real(wp) :: slope, mag ! slope and magnitude of the force
  
  call buildload ! build the load vector

  select case(load_type)
  case ( 1 ) ! ramp
    slope = max_load_magnitude/(delta_t*transient_iter_max) ! slope of the ramp
    mag = slope*actual_time ! magnitude of the force
    r_ext = mag*p ! give the magnitude to the load vector

  case ( 2 ) ! step
    if (actual_time < delta_t*transient_iter_max/100) then
      r_ext = max_load_magnitude*p
    else
      r_ext = 0.0
    end if
    
  case ( 3 ) ! sine
    r_ext = max_load_magnitude*sin(omega_load*actual_time)*p
    
  case ( 4 ) ! 
    if (actual_time < delta_t*transient_iter_max/3.0) then
      slope = 3.0*max_load_magnitude/(delta_t*transient_iter_max) ! slope of the ramp
      mag = slope*actual_time ! magnitude of the force
      r_ext = mag*p ! give the magnitude to the load vector
    else 
      r_ext = max_load_magnitude*p
    end if
  case ( 5 ) ! ramp + remove of the load
    if (actual_time < delta_t*transient_iter_max/2) then
      slope = 2*max_load_magnitude/(delta_t*transient_iter_max) ! slope of the ramp
      mag = slope*actual_time ! magnitude of the force
      r_ext = mag*p ! give the magnitude to the load vector
    else 
      r_ext = 0.0
    end if
    
  end select

end subroutine external_load_ft


!               __                                        _   
!    ___ _ __  / _| ___  _ __ ___ ___     _ __ ___   __ _| |_ 
!   / _ \ '_ \| |_ / _ \| '__/ __/ _ \   | '_ ` _ \ / _` | __|
!  |  __/ | | |  _| (_) | | | (_|  __/   | | | | | | (_| | |_ 
!   \___|_| |_|_|  \___/|_|  \___\___|___|_| |_| |_|\__,_|\__|
!                                   |_____|                   
subroutine enforce_mat(matr)

  !! This subroutine enforces the support boundary conditions on a matrix

  use fedata
  real(wp), dimension(:,:), intent(inout) :: matr
  integer :: i, idof, j
 
  do i = 1, nb
    idof = 0
    idof = int(2*(bound(i,1)-1) + bound(i,2))
      do j = 2, bw
        matr(j, idof) = 0.0 ! wite 0 on the column
        if ((j <= idof)) then
          matr(j, idof - j + 1) = 0.0 ! write 0 on the diagonal
        end if
      end do
    matr(1, idof) = 1.0 ! write 1 in the first row
  end do

end subroutine enforce_mat


!               __                                     
!    ___ _ __  / _| ___  _ __ ___ ___  __   _____  ___ 
!   / _ \ '_ \| |_ / _ \| '__/ __/ _ \ \ \ / / _ \/ __|
!  |  __/ | | |  _| (_) | | | (_|  __/  \ V /  __/ (__ 
!   \___|_| |_|_|  \___/|_|  \___\___|___\_/ \___|\___|
!                                   |_____|            

subroutine enforce_vec(vect)

  !! This subroutine enforces the support boundary conditions on a vector

  use fedata

  integer :: i, idof
  real(wp), dimension(:), intent(inout) :: vect
 
  idof = 0
  do i = 1, nb
    idof = int(2*(bound(i,1)-1) + bound(i,2))
    vect(idof) = 0.0 ! enforce boundary on the vector
  end do

end subroutine enforce_vec

!   _       _                        _   _                 _ 
!  (_)_ __ | |_ ___ _ __ _ __   __ _| | | | ___   __ _  __| |
!  | | '_ \| __/ _ \ '__| '_ \ / _` | | | |/ _ \ / _` |/ _` |
!  | | | | | ||  __/ |  | | | | (_| | | | | (_) | (_| | (_| |
!  |_|_| |_|\__\___|_|  |_| |_|\__,_|_| |_|\___/ \__,_|\__,_|
                                                           
subroutine internal_load_ft(d_n, r_int)

  ! This subroutine computes the internal loads 

  use fedata

  real(wp), dimension(neqn), intent(in) :: d_n
  real(wp), dimension(neqn), intent(out) :: r_int

  select case(material_type) ! select the type of material used
  case ( 1 ) ! linear elastic material
    call mmul(d_n, r_int, 1) ! compute the internal load element by element
  case ( 2 ) ! nonlinear material
    print*, 'Material not implemented yet'
    stop
  end select

end subroutine internal_load_ft

!                                   _   
!    ___ ___  _   _ _ __ __ _ _ __ | |_ 
!   / __/ _ \| | | | '__/ _` | '_ \| __|
!  | (_| (_) | |_| | | | (_| | | | | |_ 
!   \___\___/ \__,_|_|  \__,_|_| |_|\__|
                                      
subroutine courant_check
  ! courant check

  use fedata
  use plane42

  real(wp) :: c 
  real(wp) :: l, dens, young, courant_value, lmax
  integer :: e, nen, j, emax
  real(wp), dimension(8) :: xe

  l = 0.0
  lmax = 0.0

  do e = 1, ne
    nen = element(e)%numnode ! number of the nodes of e

    do j = 1, nen
      xe(2*j-1) = x(element(e)%ix(j),1)
      xe(2*j  ) = x(element(e)%ix(j),2)
      
    end do

    call element_edges(xe, l)
    
    if (l > lmax) then
      lmax = l
      emax = e
    end if
    
  end do

  young = mprop(element(emax)%mat)%young
  dens = mprop(element(emax)%mat)%dens

  c = sqrt(young/dens) ! sound speed in the bar
  courant_value = l/c

  print*, 'Courant time ', courant_value

  if (courant_value < delta_t) then
    print*, 'Courant check not atisfied'
    stop
  else 
    print*, 'Courant check satisfied'
  end if

end subroutine courant_check


end module fea