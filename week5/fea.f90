module fea

  !! This module contains procedures that are common to FEM-analysis

  implicit none
  save
  private
  public :: displ, initial, buildload, buildstiff, enforce, recover

contains

!
!--------------------------------------------------------------------------------------------------
!
  subroutine initial

    !! This subroutine is mainly used to allocate vectors and matrices

    use fedata
    use link1
    use plane42rect

! Hint for continuum elements:
!        integer, parameter :: mdim = 8
!        integer, dimension(mdim) :: edof

    ! This subroutine computes the number of global equation,
    ! half bandwidth, etc and allocates global arrays.

    ! Calculate number of equations
    neqn = 2*nn

    if (.not. banded) then
        allocate (kmat(neqn, neqn))
    else
      print *, 'ERROR in fea/initial'
      print *, 'Band form not implemented -- you need to add your own code here'
      stop
    end if
    allocate (p(neqn), d(neqn))
    allocate (strain(ne, 3), stress(ne, 3))
    allocate (stress_vm(ne))

    ! Initial stress and strain
    strain = 0
    stress = 0
    stress_vm = 0
  end subroutine initial
!
!--------------------------------------------------------------------------------------------------
!
  subroutine displ

    !! This subroutine calculates displacements

    use fedata
    use numeth
    use processor

    integer :: e
    real(wp), dimension(:), allocatable :: plotval

    ! Build load-vector
    call buildload

    ! Build stiffness matrix
    call buildstiff

    ! Remove rigid body modes
    call enforce

    if (.not. banded) then
      ! Factor stiffness matrix
      print*, 'TEST'
      call factor(kmat)
      ! Solve for displacement vector
      print*, 'Print load vector p'
      print'(f20.8)', p ! print load vector
      call solve(kmat, p)
      print*, 'Print displacement vector'
      print'(f12.8)', p ! print displcement vector
      print*, 'End displ'
    else
      print *, 'ERROR in fea/displ'
      print *, 'Band form not implemented -- you need to add your own code here'
      stop
    end if

    ! Transfer results
    d(1:neqn) = p(1:neqn)
    print*, 'transfer done done'
    
    ! Recover stress
    call recover
    print*, 'Recover done'
    ! Output results
    call output
    print*, 'Output done'

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
    end do
    call plotmatlabeval('Stresses',plotval)

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
    p(1:neqn) = 0

    ! initilize vector for distributed loads
    r = 0

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

    ! Reset stiffness matrix
    if (.not. banded) then
        kmat = 0
    else
        
      print*,'ERROR in fea/buildstiff'
      print*,'Band form not implemented -- you need to add your own code here'
      stop
    end if

    do e = 1, ne

      ! Find coordinates and degrees of freedom
      nen = element(e)%numnode
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
        print *, 'ERROR in fea/buildstiff'
        print *, 'Band form not implemented -- you need to add our own code here'
        stop
      end if
    end do
  end subroutine buildstiff
!
!--------------------------------------------------------------------------------------------------
!
  subroutine enforce

    !! This subroutine enforces the support boundary conditions

    use fedata

    integer :: i, idof
    real(wp) :: penal

    ! Correct for supports
    if (.not. banded) then
      if (.not. penalty) then
        do i = 1, nb
          idof = int(2*(bound(i,1)-1) + bound(i,2))
          p(1:neqn) = p(1:neqn) - kmat(1:neqn, idof) * bound(i, 3)
          p(idof) = bound(i, 3)
          kmat(1:neqn, idof) = 0
          kmat(idof, 1:neqn) = 0
          kmat(idof, idof) = 1
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
      print *, 'ERROR in fea/enforce'
      print *, 'Band form not implemented -- you need to add your own code here'
      stop
    end if
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
    real(wp) :: estress_vm
    real(wp) :: nu
    real(wp), dimension(neqn) :: temp
    ! Reset force vector
    p = 0
    
    do e = 1, ne
      temp = 0

      print*, 'Print test1'
      ! Find coordinates etc...
      nen = element(e)%numnode
      do i = 1,nen
        xe(2*i-1) = x(element(e)%ix(i), 1)
        xe(2*i)   = x(element(e)%ix(i), 2)
        edof(2*i-1) = 2 * element(e)%ix(i) - 1
        edof(2*i)   = 2 * element(e)%ix(i)
        de(2*i-1) = d(edof(2*i-1))
        de(2*i)   = d(edof(2*i))
      end do
      print*, 'Print test2'

      ! Find stress and strain
      select case( element(e)%id )
      case( 1 )
        young = mprop(element(e)%mat)%young
        area  = mprop(element(e)%mat)%area
        call link1_ke(xe, young, area, ke)
          temp = matmul(ke(1:2*nen,1:2*nen), de(1:2*nen))
          p(edof(1:2*nen)) = p(edof(1:2*nen)) + temp
        call link1_ss(xe, de, young, estress, estrain)
        stress(e, 1:3) = estress
        strain(e, 1:3) = estrain  
      case( 2 )
        print*, 'Print test13'
        young = mprop(element(e)%mat)%young
        nu = mprop(element(e)%mat)%nu
        print*, 'before'
        call plane42rect_ss(xe, de, young, nu, estress, estrain, estress_vm)
        stress(e, 1:3) = estress
        strain(e, 1:3) = estrain
        stress_vm(e) = estress_vm 
        print*, 'after'
      end select
    end do
    print*, 'Von mises stress'
    print'(f20.8)', stress_vm ! print von mises stress vector
    print*, 'End Von mises stress'
  end subroutine recover

end module fea
