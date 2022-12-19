! This subrputine recovers the gradient of g
subroutine recover_g_grad
  use fedata
  use plane42

  integer :: edof(mdim)
  integer :: e 
  real(wp), dimension(mdim) :: xe
  real(wp) :: e_area, thk

  do e = 1, ne
    ! Find coordinates etc...
    nen = int(element(e)%numnode)
    do i = 1,nen
      xe(2*i-1) = x(element(e)%ix(i), 1)
      xe(2*i)   = x(element(e)%ix(i), 2)
      edof(2*i-1) = 2 * element(e)%ix(i) - 1
      edof(2*i)   = 2 * element(e)%ix(i)
    end do

    ! material properties
    thk = mprop(element(e)%mat)%thk

    ! compute the element of the element
    call element_area(xe, e_area)

    g_grad(e) = e_area*thk

  end do
end subroutine recover_g_grad

! This subrputine recovers the gradient of f
subroutine recover_f_grad
  use fedata
  use plane42

  integer :: edof(mdim)
  integer :: e 
  real(wp), dimension(mdim) :: xe, de
  real(wp), dimension(mdim, mdim) :: ke
  real(wp) :: thk, nu, young, dens

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

    ! material properties
    thk = mprop(element(e)%mat)%thk
    young = mprop(element(e)%mat)%young
    nu = mprop(element(e)%mat)%nu
    dens = mprop(element(e)%mat)%dens

    call plane42_ke(xe, young, nu, thk, ke) ! build element stifness matrix

    f_grad(e) = -p_topopt*dens**(p_topopt - 1)*dot_product(de, matmul(ke,de))

  end do
end subroutine recover_f_grad

! This subrputine recovers the value of the total f
subroutine recover_f_tot
  use fedata
  use plane42

  integer :: edof(mdim)
  integer :: e 
  real(wp), dimension(mdim) :: xe, de
  real(wp), dimension(mdim, mdim) :: ke
  real(wp) :: thk, nu, young, dens

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

    ! material properties
    thk = mprop(element(e)%mat)%thk
    young = mprop(element(e)%mat)%young
    nu = mprop(element(e)%mat)%nu
    dens = mprop(element(e)%mat)%dens

    call plane42_ke(xe, young, nu, thk, ke) ! build element stifness matrix

    f_tot = f_tot + dens**p_topopt*dot_product(de, matmul(ke, de))

  end do
end subroutine recover_f_tot