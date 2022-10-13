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
    strain = 0
    stress = 0
    stress_vm = 0
    principal_stresses = 0
  end subroutine initial
!
!--------------------------------------------------------------------------------------------------
!
  subroutine displ

    !! This subroutine calculates displacements

    use fedata
    use numeth
    use processor

    integer :: e, i
    real(wp), dimension(:), allocatable :: plotval
    real(wp) :: h, h_2
    real(wp) :: x_pos, y_pos ! coordinates of the point that must be investigated
    integer :: node_3_number ! node where compute the displacement
  
    ! real(wp) :: minv
    ! Build load-vector
    call buildload

    ! Build stiffness matrix
    call buildstiff

    ! Remove rigid body modes
    call enforce

    if (.not. banded) then
      ! Factor stiffness matrix
      call factor(kmat)

      ! Solve for displacement vector
      call solve(kmat, p)

      !print*, 'Print displacement vector'
      !print'(f20.8)', p ! print displcement vector
    else
      call bfactor(kb)

      call bsolve(kb, p)
    end if

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
    end do
    call plotmatlabeval('Stresses',plotval)
    ! print the principal stresses and direction
    call plotmatlabevec('Principal', principal_stresses(:, 1), principal_stresses(:, 2), principal_stresses(:, 3))
    !print'(3f6.2,tr1)', transpose(principal_stresses)

    ! print on file

    ! element size and its square
    h = x(element(1)%ix(2),1) - x(element(1)%ix(1),1)
    h_2 = h**2

    ! if not banded form imlemented then bandwith is 0
    if ( .not. banded) then 
      bw = 0
    end if

    x_pos = 1.0
    y_pos = 1.0
    do i = 1, ne
      if( (x(element(i)%ix(3), 1) == x_pos) .and. (x(element(i)%ix(4), 1) == x_pos - h) .and. (x(element(i)%ix(3), 2) == y_pos) .and. (x(element(i)%ix(2), 2) == y_pos - h) ) then
        exit
      end if
    end do

    node_3_number = int(element(i)%ix(3)) ! node number where compute the displacement

    open(unit = 11, file = "test_results.txt", position = "append")
    ! name of the file, stress B, displacement B, element size, square element size, bandwidth
  !  write(11, '(a a e9.3 a e9.3 a e9.3 a e9.3 a i3)' ) trim(filename), ',', stress_vm(420), ',', d(882), ',', h, ',', h_2, ',', bw
    write(11, '(a a e9.3 a e9.3 a e9.3 a e9.3 a i3)' ) trim(filename), ',', stress_vm(i), ',', d(node_3_number * 2), ',', h, ',', h_2, ',', bw
    close(11)
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
        p(pos) = int(loads(i, 4)) ! insert the load in p
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
        
    
   ! pause
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
      kb = 0
      ! print*,'ERROR in fea/buildstiff'
      ! print*,'Band form not implemented -- you need to add your own code here'
      ! stop
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
    end do
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
          kmat(1:neqn, idof) = 0
          kmat(idof, 1:neqn) = 0
          kmat(idof, idof) = 1

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
            kb(j, idof) = 0 ! wite 0 on the column
            if ((j <= idof)) then
              kb(j, idof - j + 1) = 0 ! write 0 on the diagonal
            end if
          end do
        kb(1, idof) = 1 ! write 1 in the first row
        p(idof) = 0 ! enforce boundary on p
      end do
      ! print *, 'ERROR in fea/enforce'
      ! print *, 'Band form not implemented -- you need to add your own code here'
      ! stop
    end if
    !print'(24f6.3)', transpose(kb)

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
    p = 0

    do e = 1, ne

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

      ! Find stress and strain
      select case( element(e)%id )
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
    print*, 'Von mises stress for point A'
    print*, stress_vm(4)
    print*, 'Von mises stress for point B'
    print*, stress_vm(25)

    !print*, 'Von mises stress'
    !print'(f20.8)', stress_vm ! print von mises stress vector
    !print*, 'End Von mises stress'
  end subroutine recover

end module fea
