module build_matrix
  use types
  use fedata
  implicit none

  !! This module is used to build matrices
  private
  public :: build_matrix_cmat, bandwidth

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
    
    !! This subroutine computes the bandwidth of the matrix
    subroutine bandwidth(bw)
      
      integer :: max_diff, nen, i, e, ne
      integer, dimension(mdim) :: edof
      integer, intent(out) :: bw

      max_diff = 0
      bw = 0
      ! loop over all the elements
      do e = 1, ne
          nen = element(e)%numnode
        do i = 1, nen
          edof(2*i-1) = 2 * element(e)%ix(i) - 1
          edof(2*i)   = 2 * element(e)%ix(i)
        end do
          ! compute the difference between the maximum and minimum element in the edof vector. This is the "local bandwidth"
          max_diff = maxval(edof) - minval(edof)
          ! compare the "local bandwidth" with the actual max bandwidth and store it if the actual is grater
          if (max_diff > bw) then
            bw = max_diff
          end if
          
      end do
      
      ! increas e the bandwidth to take into account the diagonal item
      bw = bw + 1 
      
    end subroutine
end module