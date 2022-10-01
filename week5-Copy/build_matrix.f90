module build_matrix
  use types
  implicit none

  !! This module is used to build matrices
  private
  public :: build_matrix_cmat

  contains

    subroutine build_matrix_cmat(young, nu, cmat)
      !! This subroutine builds the constituitive matrix for a linear istropic material
      real(wp), intent(in) :: young
      real(wp), intent(in) :: nu
      real(wp), intent(out) :: cmat(3, 3)

      real(wp) :: fact = 0

      cmat = 0
      fact = young/(1-nu**2)
      cmat(1,1) = fact
      cmat(1,2) = fact*nu
      cmat(2,1) = fact*nu
      cmat(2,2) = fact
      cmat(3,3) = fact*(1-nu)/2

    end subroutine
end module