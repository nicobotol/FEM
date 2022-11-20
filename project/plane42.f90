module plane42 

  !! This module contains subroutines specific to the plane42 element 
  !!        
  !! The plane42 element has 4 nodes. Each node has 2 degrees-of-freedom,
  !! namely, displacement along the \(x\)- and \(y\)-coordinate directions.
  !!
  !! The nodes are numbered counter-clockwise as indicated below. The
  !! figure also shows how the element edges are labelled. For example,
  !! edge 1 is the edge between element node 1 and 2.
  !!
  !!       N4    E3    N3
  !!          o------o 
  !!          |      |
  !!       E4 |      | E2
  !!          |      |
  !!          o------o
  !!       N1    E1    N2
  !!             
  !!
  !! `N1` = element node 1, `N2` = element node 2, etc  
  !! `E1` = element face 1, `E2` = element face 2, etc  
  
  use types
  use build_matrix
  use fedata
  implicit none
  save
  
  private
  public :: plane42_ke, plane42_re, plane42_ss, shape, gauss_quadrature, gauss_quadrature_bmat, plane42_me, plane42_ce, element_edges, element_area, plane42_me_lumped, shape_lumped

contains

  subroutine plane42_ke(xe, young, nu, thk, ke)
    

    !! This subroutine constructs the stiffness matrix for
    !! a rectangular 4-noded quad element.

    real(wp), intent(in) :: young
        !! Young's Modulus for this element
    real(wp), intent(in) :: nu
        !! Poisson's Ratio for this element
    real(wp), intent(in) :: thk
        !! Thickness of this element
    real(wp), dimension(:), intent(in) :: xe
        !! Nodal coordinates of this element in undeformed configuration
        !!
        !! * `xe(1:2)` = \((x,y)\)-coordinates of element node 1
        !! * `xe(3:4)` = \((x,y)\)-coordinates of element node 1
        !! * `xe(5:6)` = \((x,y)\)-coordinates of element node 2
        !! * `xe(7:8)` = \((x,y)\)-coordinates of element node 2
        !!
        !! See also [[plane42]]
    real(wp), dimension(:,:), intent(out) :: ke
        !! Stiffness matrix

    ! real(wp) :: fact, aa, bb
    real(wp) :: cmat(3,3), aa, bb
    real(wp) :: t1, t2, t3, t4, t5, t6, t9, t11, t13, t15, &
    t16, t17, t18, t19, t22, t25, t29, t33, &
    t34, t38, t42, t43, t47, t51, t55, t56, &
    t57, t58, t59, t62, t66, t70, t74, t78, &
    t82, t86, t90, t94, t98, t102, t106, t110
    real(wp) :: d11, d12, d13, d22, d23, d33
    ! locations and weight for gauss quadrature
    ! integer :: ng = 2 ! gaussian quadrature points
    ! real(wp), dimension(ng) :: gauss_location
    ! real(wp), dimension(ng) :: gauss_weight 
    real(wp) :: eta ! point where to evaluate the gaussian quadrature
    real(wp) :: xi ! point where to evaluate the gaussian quadrature
    real(wp) :: det_J
    real(wp), dimension(3,8) :: B
    real(wp), dimension(2,8) :: N
    integer :: i, ii ! iterators
    real(wp), dimension(2,2) :: J
    real(wp), dimension(3,8) :: temp_prod1
    real(wp), dimension(8,8) :: temp_prod2



    ! build constitutive matrix (plane stress)
    ! cmat = 0
    ! fact = young/(1-nu**2)
    ! cmat(1,1) = fact
    ! cmat(1,2) = fact*nu
    ! cmat(2,1) = fact*nu
    ! cmat(2,2) = fact
    ! cmat(3,3) = fact*(1-nu)/2

    call build_matrix_cmat(young, nu, cmat)
    
    if (.not. isoparametric) then
      
      aa = (xe(3)-xe(1))/2
      bb = (xe(8)-xe(2))/2
      
      d11 = cmat(1,1)
      d12 = cmat(1,2)
      d13 = cmat(1,3)
      d22 = cmat(2,2)
      d23 = cmat(2,3)
      d33 = cmat(3,3)
      
      t1 = bb**2
      t2 = t1*d11
      t3 = 2*t2
      t4 = aa**2
      t5 = t4*d33
      t6 = 2*t5
      t9 = 3*d13*aa*bb
      t11 = 1/aa
      t13 = 1/bb
      t15 = (t3+t6+t9)*t11*t13/6
      t16 = d13*t1
      t17 = 4*t16
      t18 = t4*d23
      t19 = 4*t18
      t22 = 3*d12*aa*bb
      t25 = 3*aa*d33*bb
      t29 = (t17+t19+t22+t25)*t11*t13/12
      t33 = (-t3+t5)*t11*t13/6
      t34 = 2*t18
      t38 = (-t17+t34+t22-t25)*t11*t13/12
      t42 = (t2+t5+t9)*t11*t13/6
      t43 = 2*t16
      t47 = (t43+t34+t22+t25)*t11*t13/12
      t51 = (-t2+t6)*t11*t13/6
      t55 = (-t43+t19+t22-t25)*t11*t13/12
      t56 = d33*t1
      t57 = 2*t56
      t58 = t4*d22
      t59 = 2*t58
      t62 = 3*aa*d23*bb
      t66 = (t57+t59+t62)*t11*t13/6
      t70 = (-t17+t34-t22+t25)*t11*t13/12
      t74 = (-t57+t58)*t11*t13/6
      t78 = (t56+t58+t62)*t11*t13/6
      t82 = (-t43+t19-t22+t25)*t11*t13/12
      t86 = (-t56+t59)*t11*t13/6
      t90 = (t3+t6-t9)*t11*t13/6
      t94 = (t17+t19-t22-t25)*t11*t13/12
      t98 = (t2+t5-t9)*t11*t13/6
      t102 = (t43+t34-t22-t25)*t11*t13/12
      t106 = (t57+t59-t62)*t11*t13/6
      t110 = (t56+t58-t62)*t11*t13/6
      ke(1,1) = t15
      ke(1,2) = t29
      ke(1,3) = t33
      ke(1,4) = t38
      ke(1,5) = -t42
      ke(1,6) = -t47
      ke(1,7) = -t51
      ke(1,8) = -t55
      ke(2,1) = t29
      ke(2,2) = t66
      ke(2,3) = t70
      ke(2,4) = t74
      ke(2,5) = -t47
      ke(2,6) = -t78
      ke(2,7) = -t82
      ke(2,8) = -t86
      ke(3,1) = t33
      ke(3,2) = t70
      ke(3,3) = t90
      ke(3,4) = t94
      ke(3,5) = -t51
      ke(3,6) = -t82
      ke(3,7) = -t98
      ke(3,8) = -t102
      ke(4,1) = t38
      ke(4,2) = t74
      ke(4,3) = t94
      ke(4,4) = t106
      ke(4,5) = -t55
      ke(4,6) = -t86
      ke(4,7) = -t102
      ke(4,8) = -t110
      ke(5,1) = -t42
      ke(5,2) = -t47
      ke(5,3) = -t51
      ke(5,4) = -t55
      ke(5,5) = t15
      ke(5,6) = t29
      ke(5,7) = t33
      ke(5,8) = t38
      ke(6,1) = -t47
      ke(6,2) = -t78
      ke(6,3) = -t82
      ke(6,4) = -t86
      ke(6,5) = t29
      ke(6,6) = t66
      ke(6,7) = t70
      ke(6,8) = t74
      ke(7,1) = -t51
      ke(7,2) = -t82
      ke(7,3) = -t98
      ke(7,4) = -t102
      ke(7,5) = t33
      ke(7,6) = t70
      ke(7,7) = t90
      ke(7,8) = t94
      ke(8,1) = -t55
      ke(8,2) = -t86
      ke(8,3) = -t102
      ke(8,4) = -t110
      ke(8,5) = t38
      ke(8,6) = t74
      ke(8,7) = t94
      ke(8,8) = t106
      
      ke = ke*thk

    else ! isoparametric formulation
      ! ! build the vectors with gaussian points and weights
      ! gauss_location(1) = -1.0/3.0**0.5
      ! gauss_location(2) = 1.0/3.0**0.5
      ! gauss_weight(1) = 1.0
      ! gauss_weight(2) = 1.0

      ke = 0.0 ! initilize ke
      do i = 1, ng
        do ii = 1, ng
          
          xi = gauss_location(i)
          eta = gauss_location(ii)

          call shape(eta, xi, xe, N, B, J, det_J)

          ! build local stiffness matrix
          temp_prod1 = matmul(cmat, B)
          temp_prod2 = matmul(transpose(B), temp_prod1)
          ke = ke + gauss_weight(i)*gauss_weight(ii)*thk*temp_prod2*det_J

          ! build local stiffness matrix (in case of modal analysis)
      

        end do
      end do
    end if
  end subroutine plane42_ke
  
  subroutine plane42_me(xe, dens, me, thk)
    

    !! This subroutine constructs the mass matrix for
    !! a rectangular 4-noded element.

    real(wp), intent(in) :: dens
    real(wp), intent(in) :: thk
        !! Thickness of this element
    real(wp), dimension(:), intent(in) :: xe
        !! Nodal coordinates of this element in undeformed configuration
        !!
        !! * `xe(1:2)` = \((x,y)\)-coordinates of element node 1
        !! * `xe(3:4)` = \((x,y)\)-coordinates of element node 1
        !! * `xe(5:6)` = \((x,y)\)-coordinates of element node 2
        !! * `xe(7:8)` = \((x,y)\)-coordinates of element node 2
        !!
        !! See also [[plane42]]
    real(wp), dimension(:,:), intent(out) :: me
        !! Mass matrix

    real(wp) :: eta ! point where to evaluate the gaussian quadrature
    real(wp) :: xi ! point where to evaluate the gaussian quadrature
    real(wp) :: det_J
    real(wp), dimension(3,8) :: B
    real(wp), dimension(2,8) :: N
    integer :: i, ii ! iterators
    real(wp), dimension(2,2) :: J
    real(wp), dimension(8,8) :: temp_prod3


    ! build constitutive matrix (plane stress)
    ! cmat = 0
    ! fact = young/(1-nu**2)
    ! cmat(1,1) = fact
    ! cmat(1,2) = fact*nu
    ! cmat(2,1) = fact*nu
    ! cmat(2,2) = fact
    ! cmat(3,3) = fact*(1-nu)/2
    
      me = 0.0 ! initialize me
      do i = 1, ng
        do ii = 1, ng
          
          xi = gauss_location(i)
          eta = gauss_location(ii)

          call shape(eta, xi, xe, N, B, J, det_J)

          ! build local stiffness matrix (in case of modal analysis)
      
          temp_prod3 = matmul(transpose(N), N)
          me = me + gauss_weight(i)*gauss_weight(ii)*temp_prod3*det_J*dens*thk

        end do
      end do
  end subroutine plane42_me

!   _                                _                            
!  | |_   _ _ __ ___  _ __   ___  __| |  _ __ ___   __ _ ___ ___  
!  | | | | | '_ ` _ \| '_ \ / _ \/ _` | | '_ ` _ \ / _` / __/ __| 
!  | | |_| | | | | | | |_) |  __/ (_| | | | | | | | (_| \__ \__ \ 
!  |_|\__,_|_| |_| |_| .__/ \___|\__,_| |_| |_| |_|\__,_|___/___/ 
!                    |_|                                          
  subroutine plane42_me_lumped(xe, dens, me_lumped, thk)
    

    !! This subroutine constructs the lumped mass matrix 

    real(wp), intent(in) :: dens
    real(wp), intent(in) :: thk
        !! Thickness of this element
    real(wp), dimension(:), intent(in) :: xe
        !! Nodal coordinates of this element in undeformed configuration
        !!
        !! * `xe(1:2)` = \((x,y)\)-coordinates of element node 1
        !! * `xe(3:4)` = \((x,y)\)-coordinates of element node 1
        !! * `xe(5:6)` = \((x,y)\)-coordinates of element node 2
        !! * `xe(7:8)` = \((x,y)\)-coordinates of element node 2
        !!
        !! See also [[plane42]]
    real(wp), dimension(:), intent(out) :: me_lumped
        !! Lumped mass matrix

    real(wp) :: eta ! point where to evaluate the gaussian quadrature
    real(wp) :: xi ! point where to evaluate the gaussian quadrature
    real(wp) :: det_J
    real(wp), dimension(8) :: N
    integer :: i, ii ! iterators
    real(wp), dimension(2,2) :: J
    real(wp), dimension(8) :: temp_prod3
    real(wp) :: e_area, m ! element area and mass
    real(wp) :: sx, sy ! scaling coefficinents

    call element_area(xe, e_area) ! calculate the area of the element

    m = e_area*thk*dens ! total mass of the element
    
    me_lumped = 0.0 ! initialize lumped me
    sx = 0.0
    sy = 0.0

    do i = 1, ng
      do ii = 1, ng
        
        xi = gauss_location(i)
        eta = gauss_location(ii)

        call shape_lumped(eta, xi, xe, N, J, det_J)

        ! build local stiffness matrix (in case of modal analysis)
    
        temp_prod3(1) = N(1)*N(1)
        temp_prod3(2) = N(2)*N(2)
        temp_prod3(3) = N(3)*N(3)
        temp_prod3(4) = N(4)*N(4)
        temp_prod3(5) = N(5)*N(5)
        temp_prod3(6) = N(6)*N(6)
        temp_prod3(7) = N(7)*N(7)
        temp_prod3(8) = N(8)*N(8)

        me_lumped = me_lumped + gauss_weight(i)*gauss_weight(ii)*temp_prod3*det_J*dens*thk

      end do
    end do

    ! compute s coefficients for the 2 dofs
    do i = 1, 4
      sx = sx + me_lumped(2*i - 1)
      sy = sy + me_lumped(2*i)
    end do

    ! scale the elements of the lumped matrix 
    do i = 1, 4
      me_lumped(2*i - 1) = me_lumped(2*i - 1)/sx
      me_lumped(2*i) = me_lumped(2*i)/sy
    end do

    ! scale the local matrix by element the element matrix 
    me_lumped = me_lumped*m

  end subroutine plane42_me_lumped

  subroutine plane42_ce(xe, kappa, ce)
    

    !! This subroutine constructs the damping matrix for
    !! a rectangular 4-noded element.

        !! Poisson's Ratio for this element
    real(wp), intent(in) :: kappa
        !! Thickness of this element
    real(wp), dimension(:), intent(in) :: xe
        !! Nodal coordinates of this element in undeformed configuration
        !!
        !! * `xe(1:2)` = \((x,y)\)-coordinates of element node 1
        !! * `xe(3:4)` = \((x,y)\)-coordinates of element node 1
        !! * `xe(5:6)` = \((x,y)\)-coordinates of element node 2
        !! * `xe(7:8)` = \((x,y)\)-coordinates of element node 2
        !!
        !! See also [[plane42]]
    real(wp), dimension(:,:), intent(out) :: ce
        !! Mass matrix

    real(wp) :: eta ! point where to evaluate the gaussian quadrature
    real(wp) :: xi ! point where to evaluate the gaussian quadrature
    real(wp) :: det_J
    real(wp), dimension(3,8) :: B
    real(wp), dimension(2,8) :: N
    integer :: i, ii ! iterators
    real(wp), dimension(2,2) :: J
    real(wp), dimension(8,8) :: temp_prod3
    
      ce = 0.0 ! initialize me
      do i = 1, ng
        do ii = 1, ng
          
          xi = gauss_location(i)
          eta = gauss_location(ii)

          call shape(eta, xi, xe, N, B, J, det_J)

          ! build local stiffness matrix (in case of modal analysis)
      
          temp_prod3 = matmul(transpose(N), N)
          ce = ce + gauss_weight(i)*gauss_weight(ii)*temp_prod3*det_J*kappa

        end do
      end do
  end subroutine plane42_ce
  
  subroutine shape(eta, xi, xe, N, B, J, det_J)
    real(wp), intent(in) :: eta
    real(wp), intent(in) :: xi
    real(wp), dimension(:), intent(in) :: xe
    real(wp), dimension(2, 8), intent(out) :: N ! element shape matrix
    real(wp), dimension(3, 8), intent(out) :: B
    real(wp), dimension(2,2), intent(out) :: J ! jacobian
    real(wp), intent(out) :: det_J
    real(wp), dimension(4) :: dN_dxi, dN_deta
    real(wp), dimension(3,4) :: L
    integer :: i ! iterator
    real(wp), dimension(2,2) :: gamma ! inverse jacobian
    real(wp), dimension(4, 4) :: gamma_tilde
    real(wp), dimension(4, 8) :: N_tilde
    real(wp), dimension(4, 8) :: temp_prod
    real(wp) :: N1, N2, N3, N4

    ! initialize l matrix
    L = 0
    L(1, 1) = 1.0
    L(2, 4) = 1.0
    L(3, 2) = 1.0
    L(3, 3) = 1.0

    ! build the element N matrix (shape function)
    N = 0.0 ! initialize the matrix
    N1 = 0.25*(1 - xi)*(1 - eta) ! build the entrance of the matrix
    N2 = 0.25*(1 + xi)*(1 - eta)
    N3 = 0.25*(1 + xi)*(1 + eta)
    N4 = 0.25*(1 - xi)*(1 + eta)
    ! insert the elements
    N(1, 1) = N1
    N(2, 2) = N1
    N(1, 3) = N2
    N(2, 4) = N2
    N(1, 5) = N3
    N(2, 6) = N3
    N(1, 7) = N4
    N(2, 8) = N4

    ! build the vectors of N derivate
    dN_dxi(1) = - 0.25*(1 - eta)
    dN_dxi(2) =  0.25*(1 - eta)
    dN_dxi(3) =  0.25*(1 + eta)
    dN_dxi(4) = - 0.25*(1 + eta)

    dN_deta(1) = - 0.25*(1 - xi )
    dN_deta(2) = - 0.25*(1 + xi )
    dN_deta(3) =  0.25*(1 + xi )
    dN_deta(4) =  0.25*(1 - xi )

    ! initialize Jacobian
    J = 0;
    do i = 1, 4
      J(1, 1) = J(1, 1) + dN_dxi(i)*xe(2*i - 1)
      J(1, 2) = J(1, 2) + dN_dxi(i)*xe(2*i)
      J(2, 1) = J(2, 1) + dN_deta(i)*xe(2*i - 1)
      J(2, 2) = J(2, 2) + dN_deta(i)*xe(2*i)
    end do

    ! compute the determinat of the jacobian
    det_J = J(1, 1)*J(2, 2) - J(1, 2)*J(2, 1)

    ! initilize the inverse jacobian
    gamma = 0
    ! compute the inverse jacobian
    gamma(1, 1) = J(2, 2)
    gamma(1, 2) = - J(1, 2)
    gamma(2, 1) = - J(2, 1)
    gamma(2, 2) = J(1, 1)
    gamma = gamma/det_J

    ! build gamma_tilde
    gamma_tilde = 0
    gamma_tilde(1, 1) = gamma(1, 1)
    gamma_tilde(1, 2) = gamma(1, 2)
    gamma_tilde(2, 1) = gamma(2, 1)
    gamma_tilde(2, 2) = gamma(2, 2)
    gamma_tilde(3, 3) = gamma(1, 1)
    gamma_tilde(3, 4) = gamma(1, 2)
    gamma_tilde(4, 3) = gamma(2, 1)
    gamma_tilde(4, 4) = gamma(2, 2)

    ! build N_tilde
    N_tilde = 0
    do i = 1, 4
      N_tilde(1, 2*i - 1) = dN_dxi(i)
      N_tilde(2, 2*i - 1) = dN_deta(i)
      N_tilde(3, 2*i) = dN_dxi(i)
      N_tilde(4, 2*i) = dN_deta(i)
    end do

    ! build B
    temp_prod = matmul(gamma_tilde, N_tilde)
    B = matmul(L, temp_prod)

  end subroutine shape

  !       _                        _                                _ 
  !   ___| |__   __ _ _ __   ___  | |_   _ _ __ ___  _ __   ___  __| |
  !  / __| '_ \ / _` | '_ \ / _ \ | | | | | '_ ` _ \| '_ \ / _ \/ _` |
  !  \__ \ | | | (_| | |_) |  __/ | | |_| | | | | | | |_) |  __/ (_| |
  !  |___/_| |_|\__,_| .__/ \___| |_|\__,_|_| |_| |_| .__/ \___|\__,_|
  !                  |_|                            |_|               
  subroutine shape_lumped(eta, xi, xe, N, J, det_J)
    real(wp), intent(in) :: eta
    real(wp), intent(in) :: xi
    real(wp), dimension(:), intent(in) :: xe
    real(wp), dimension(8), intent(out) :: N ! element shape matrix
    real(wp), dimension(2,2), intent(out) :: J ! jacobian
    real(wp), intent(out) :: det_J
    real(wp), dimension(4) :: dN_dxi, dN_deta
    real(wp), dimension(3,4) :: L
    integer :: i ! iterator
    real(wp) :: N1, N2, N3, N4

    ! initialize l matrix
    L = 0
    L(1, 1) = 1.0
    L(2, 4) = 1.0
    L(3, 2) = 1.0
    L(3, 3) = 1.0

    ! build the element N matrix (shape function)
    N = 0.0 ! initialize the matrix
    N1 = 0.25*(1 - xi)*(1 - eta) ! build the entrance of the matrix
    N2 = 0.25*(1 + xi)*(1 - eta)
    N3 = 0.25*(1 + xi)*(1 + eta)
    N4 = 0.25*(1 - xi)*(1 + eta)
    ! insert the elements
    N(1) = N1
    N(2) = N1
    N(3) = N2
    N(4) = N2
    N(5) = N3
    N(6) = N3
    N(7) = N4
    N(8) = N4

    ! build the vectors of N derivate
    dN_dxi(1) = - 0.25*(1 - eta)
    dN_dxi(2) =  0.25*(1 - eta)
    dN_dxi(3) =  0.25*(1 + eta)
    dN_dxi(4) = - 0.25*(1 + eta)

    dN_deta(1) = - 0.25*(1 - xi )
    dN_deta(2) = - 0.25*(1 + xi )
    dN_deta(3) =  0.25*(1 + xi )
    dN_deta(4) =  0.25*(1 - xi )

    ! initialize Jacobian
    J = 0.0;
    do i = 1, 4
      J(1, 1) = J(1, 1) + dN_dxi(i)*xe(2*i - 1)
      J(1, 2) = J(1, 2) + dN_dxi(i)*xe(2*i)
      J(2, 1) = J(2, 1) + dN_deta(i)*xe(2*i - 1)
      J(2, 2) = J(2, 2) + dN_deta(i)*xe(2*i)
    end do

    ! compute the determinat of the jacobian
    det_J = J(1, 1)*J(2, 2) - J(1, 2)*J(2, 1)

  end subroutine shape_lumped

  !
  !--------------------------------------------------------------------------------------------------
  !
  subroutine plane42_re(xe, eface, fe, thk, re)
    
    !! This subroutine computes the element load vector due
    !! to surface traction (traction is always perpendicular
    !! to element face).
    
    integer, intent(in) :: eface
      !! Element face where traction (pressure) is applied
    
    real(wp), intent(in) :: fe
    !! Value of surface traction (pressure)
    real(wp), intent(in) :: thk
      !! Thickness of this element
    real(wp), dimension(:), intent(in) :: xe
    !! Nodal coordinates of this element in undeformed configuration (see also [[plane42_ke]])
    real(wp), intent(out) :: re(8)
    !! Element force vector
      !!
      !! * `re(1:2)` = \((f_x^1, f_y^1)\) force at element node 1 in \(x\)- and y-direction
      !! * `re(3:4)` = \((f_x^2, f_y^2)\) force at element node 1 in \(x\)- and y-direction
      !! * etc...
    real(wp) :: aa, bb, nface(2,8), f(2)
    real(wp), dimension(8):: temp = 0.0
    real(wp) :: eta
    real(wp) :: xi
    real(wp), dimension(3, 8) :: B
    real(wp), dimension(2,2) :: J ! jacobian
    real(wp), dimension(2, 8) :: N ! shape function matrix
    real(wp), dimension(8, 2) :: N_T ! transpose shape function matrix
    real(wp) :: det_J
    real(wp), dimension(2) :: J_temp
    real(wp), dimension(8) :: temp_prod
    integer :: i ! iterator
    ! integer :: ng = 2 ! gaussian quadrature points
    ! real(wp), dimension(ng) :: gauss_location ! gaussian points
    ! real(wp), dimension(ng) :: gauss_weight ! gaussian weights
    
    re = 0.0 ! initialize the load vector

    if (.not. isoparametric) then

      aa = (xe(3)-xe(1))/2
      bb = (xe(8)-xe(2))/2
      
      nface = 0.0
      f = 0.0 ! initialize the force vector to 0
      if (eface == 1) then
        nface(1,1) = aa
        nface(1,3) = aa
        nface(2,2) = aa
        nface(2,4) = aa
        f(2) = fe
      elseif (eface == 2) then
        nface(1,3) = bb
        nface(1,5) = bb
        nface(2,4) = bb
        nface(2,6) = bb
        f(1) = -fe
      elseif (eface == 3) then
        nface(1,5) = aa
        nface(1,7) = aa
        nface(2,6) = aa
        nface(2,8) = aa
        f(2) = -fe
      elseif (eface == 4) then
        nface(1,1) = bb
        nface(1,7) = bb
        nface(2,2) = bb
        nface(2,8) = bb
        f(1) = fe
      endif

      ! print*, 'The thk for the element is'
      ! print*, thk
      temp = matmul(transpose(nface), f) 
      re = temp * thk

  else ! isoparametric case
    ! build the vectors with gaussian points and weights
    ! gauss_location(1) = -1.0/3.0**0.5
    ! gauss_location(2) = 1.0/3.0**0.5
    ! gauss_weight(1) = 1.0
    ! gauss_weight(2) = 1.0
    
    N = 0.0 ! initialize shape function matrix
    J_temp = 0.0
    do i = 1, ng
      eta = gauss_location(i)
      xi = gauss_location(i)

      if (eface == 1) then
        eta = -1.0

        call shape(eta, xi, xe, N, B, J, det_J)
        J_temp(1) = -J(1, 2)
        J_temp(2) = J(1, 1)
        
      elseif (eface == 2) then
        xi = 1.0
      
        call shape(eta, xi, xe, N, B, J, det_J)
        J_temp(1) = -J(2, 2)
        J_temp(2) = J(2, 1)

      elseif (eface == 3) then
        eta = 1.0

        call shape(eta, xi, xe, N, B, J, det_J)
        J_temp(1) = J(1, 2)
        J_temp(2) = -J(1, 1)

      elseif (eface == 4) then
        xi = -1.0

        call shape(eta, xi, xe, N, B, J, det_J)
        J_temp(1) = J(2, 2)
        J_temp(2) = -J(2, 1)

      end if

      !print*, J_temp
      !print'(24f5.2)', transpose(N)
      !print*, gauss_weight(i)

      N_T = transpose(N) ! transpose N 
      temp_prod = matmul(N_T, J_temp)
      re = re + gauss_weight(i)*thk*fe*temp_prod
      
    end do
  end if
    !print *, 'ERROR in plane42/plane42_re'
    !print *, 'subroutine incomplete -- you need to add some code in this subroutine'
    !stop
  end subroutine plane42_re
  !
  !--------------------------------------------------------------------------------------------------
  !
  subroutine plane42_ss(xe, de, young, nu, estress, estrain, estress_vm, estress_1, estress_2, psi)
    
  !! This subrotuine computes the element stress and strain (The location inside the element where stress and and strain is evaluated, is defined inside the subroutine).
    
  real(wp), intent(in) :: young
    !! Young's Modulus for this element
  real(wp), intent(in) :: nu 
    !! Poisson's Ratio for this element
  real(wp), dimension(:), intent(in)  :: xe
  !! Nodal coordinates of this element in undeformed configuration (see also [[plane42_ke]])
  real(wp), dimension(:), intent(in)  :: de
  !! Displacement field
    !!
  !! * `de(1:2)` = displacement of element node 1 along \(x\)- and \(y\)-axis, respectively
  !! * `de(3:4)` = displacement of element node 2 along \(x\)- and \(y\)-axis, respectively
    !! * etc...
  real(wp), dimension(:), intent(out) :: estress
  !! Stress at a point inside the element
  !!
  !! * `estress(1)` = \(\sigma_{11}\)
  !! * `estress(2)` = \(\sigma_{22}\)
  !! * `estress(3)` = \(\sigma_{12}\)
  real(wp), dimension(:), intent(out) :: estrain
    !! Strain at a point inside the element
  !!
  !! * `estrain(1)` = \(\epsilon_{11}\)
  !! * `estrain(2)` = \(\epsilon_{22}\)
  !! * `estrain(3)` = \(\epsilon_{12}\)
  real(wp), intent(out) :: estress_vm ! element von Mises stress
  real(wp), intent(out) :: psi ! element principal direction
  real(wp), intent(out) :: estress_1, estress_2 ! element principal stresses
  real(wp) :: bmat(3, 8), cmat(3, 3), bmat_temp(3, 8)
  real(wp) :: aa, bb
  real(wp) :: location(2) ! (x,y) where stress and strain are evaluated inside the element
  real(wp) :: c_2psi, s_2psi ! element principal directions
  real(wp) :: det_J ! determinant if the jacobian in isoparametric case
  real(wp) :: eta, xi ! point where to calculate the bmat
  real(wp), dimension(2, 2) :: J ! jacobian matrix 
  REAL(wp), dimension(2, 8) :: N

  bmat = 0.0
  bmat_temp = 0.0
  if(.not. isoparametric) then
  aa = (xe(3)-xe(1))/2 ! x undeformed dimension
  bb = (xe(8)-xe(2))/2 ! y undeformed dimension
  
  location(1) = -aa ! x coord. where stress-strain is evaluated
  location(2) = bb ! y coord. where stress-strain is evaluated
  
  ! Build strain-displacement matrix
  bmat(1, 1) = -(bb - location(2))
  bmat(1, 2) = 0
  bmat(1, 3) = (bb - location(2))
  bmat(1, 4) = 0
  bmat(1, 5) = bb + location(2)
  bmat(1, 6) = 0
  bmat(1, 7) = -(bb + location(2))
  bmat(1, 8) = 0
  Bmat(2, 1) = 0
  bmat(2, 2) = -(aa - location(1))
  bmat(2, 3) = 0
  bmat(2, 4) = -(aa + location(1))
  bmat(2, 5) = 0
  bmat(2, 6) = aa + location(1)
  bmat(2, 7) = 0
  bmat(2, 8) = aa - location(1)
  bmat(3, 1) = -(aa - location(1))
  bmat(3, 2) = -(bb - location(2))
  bmat(3, 3) = -(aa + location(1))
  bmat(3, 4) = bb - location(2)
  bmat(3, 5) = aa + location(1)
  bmat(3, 6) = bb + location(2)
  bmat(3, 7) = aa - location(1)
  bmat(3, 8) = -(bb + location(2))  
  
  bmat = bmat / (4*aa*bb)

  else ! isoparametric formulation

  ! compute the bmat wheighting the bmat evaluated over the gauss points
  
  eta = 0.0
  xi = 0.0
  call shape(xi, eta, xe, N, bmat, J, det_J)

  end if
  ! Compute element strain
  estrain = matmul(bmat, de)
  
  ! build constitutive matrix
  call build_matrix_cmat(young, nu, cmat)

  ! Compute element stress
  estress = matmul(cmat, estrain)
  
  ! Compute principal stress
  estress_1 = 0.5*(estress(1) + estress(2)) + (0.25*(estress(1) - estress(2))**2 + estress(3)**2)**0.5

  estress_2 = 0.5*(estress(1) + estress(2)) - (0.25*(estress(1) - estress(2))**2 + estress(3)**2)**0.5
  
  ! Compute principal directions
  c_2psi = (estress(1) - estress(2)) / (estress_1 - estress_2)
  s_2psi = (-2*estress(3)) / (estress_1 - estress_2)
  psi = 0.5*atan2(s_2psi, c_2psi)

  ! Compute Von Mises quivalent stress
  estress_vm = (estress_1**2 + estress_2**2 - estress_1*estress_2)**0.5

end subroutine plane42_ss

subroutine gauss_quadrature
  use fedata
  ! This subroutine defines the gauss quadrature locations and weights

  select case(ng)
  case( 1 )
    ! position where to evaluate the gauss quadrature
    gauss_location(1) = 0.0

    ! weight for the gauss quadrature
    gauss_weight(1) = 2.0

  case( 2 )
    gauss_location(1) = -1.0/3.0**0.5
    gauss_location(2) = 1.0/3.0**0.5
    
    gauss_weight(1) = 1.0
    gauss_weight(2) = 1.0
      
  case( 3 )
    gauss_location(1) = -0.6**0.5
    gauss_location(2) = 0
    gauss_location(3) = 0.6**0.5
    
    gauss_weight(1) = 5.0/9.0
    gauss_weight(2) = 8.0/9.0
    gauss_weight(3) = 5.0/9.0

  case ( 4 )
    gauss_location(1) = -(3.0/7.0 + 2.0/7.0*(6.0/5.0)**0.5)**0.5
    gauss_location(2) = -(3.0/7.0 - 2.0/7.0*(6.0/5.0)**0.5)**0.5
    gauss_location(3) = (3.0/7.0 - 2.0/7.0*(6.0/5.0)**0.5)**0.5
    gauss_location(4) = (3.0/7.0 + 2.0/7.0*(6.0/5.0)**0.5)**0.5
    
    gauss_weight(1) = (18.0 - 30.0**0.5) / 36.00
    gauss_weight(2) = (18.0 + 30.0**0.5) / 36.00
    gauss_weight(3) = (18.0 + 30.0**0.5) / 36.00
    gauss_weight(4) = (18.0 - 30.0**0.5) / 36.00

  case default 
    print*, 'This number of gauss point is not valid'
    stop
  end select

end subroutine gauss_quadrature

subroutine gauss_quadrature_bmat
  use fedata
  ! This subroutine defines the gauss quadrature locations where to evaluate the bmat 

  select case(ng_bmat)

  case( 1 )
    !position where to evaluate the gauss quadrature for stresses
    gauss_location_bmat(1) = 0.0
    
  case( 2 )
    
    gauss_location_bmat(1) = -1.0/3.0**0.5
    gauss_location_bmat(2) = 1.0/3.0**0.5

  case ( 3 )
    
    gauss_location_bmat(1) = -0.6**0.5
    gauss_location_bmat(2) = 0
    gauss_location_bmat(3) = 0.6**0.5

  case default 
    print*, 'This number of gauss point is not valid'
    stop
  end select

end subroutine gauss_quadrature_bmat

!        _                           _              _                 
!    ___| | ___ _ __ ___   ___ _ __ | |_    ___  __| | __ _  ___  ___ 
!   / _ \ |/ _ \ '_ ` _ \ / _ \ '_ \| __|  / _ \/ _` |/ _` |/ _ \/ __|
!  |  __/ |  __/ | | | | |  __/ | | | |_  |  __/ (_| | (_| |  __/\__ \
!   \___|_|\___|_| |_| |_|\___|_| |_|\__|  \___|\__,_|\__, |\___||___/
!                                                     |___/           

subroutine element_edges(xe, l)
  ! This subroutine computes the longest edge of an element
  use fedata
  real(wp), dimension(8), intent(in):: xe
  real(wp), intent(out)::  l
  real(wp) ::  aa, bb, dd, ee

  l = 0.0

  aa = ((xe(3) - xe(1))**2 + (xe(4) - xe(2))**2)**0.5 ! edge
  bb = ((xe(7) - xe(1))**2 + (xe(8) - xe(2))**2)**0.5 ! edge 
  dd = ((xe(3) - xe(5))**2 + (xe(4) - xe(6))**2)**0.5 ! edge
  ee = ((xe(5) - xe(7))**2 + (xe(6) - xe(8))**2)**0.5 ! edge

  l = min(abs(aa), abs(bb), abs(dd), abs(ee))
  
end subroutine element_edges

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

end module plane42
