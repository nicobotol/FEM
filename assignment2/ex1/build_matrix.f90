module build_matrix
  use types
  use fedata
  implicit none

  !! This module is used to build matrices
  private
  public :: build_matrix_cmat, bandwidth, compute_det

  contains

    subroutine build_matrix_cmat(young, nu, cmat)
      !! This subroutine builds the constituitive matrix for a linear istropic material
      real(wp), intent(in) :: young
      real(wp), intent(in) :: nu
      real(wp), intent(out) :: cmat(3, 3)
      
      real(wp) :: fact = 0.0
      
      cmat = 0.0
      fact = young/(1-nu**2)
      cmat(1,1) = fact
      cmat(1,2) = fact*nu
      cmat(2,1) = fact*nu
      cmat(2,2) = fact
      cmat(3,3) = fact*(1-nu)/2
      
    end subroutine
    
    !! This subroutine computes the bandwidth of the matrix
    subroutine bandwidth(bw, ne)
      
      integer :: max_diff, nen, i, e
      integer, dimension(mdim) :: edof
      integer, intent(in) :: ne
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


    subroutine compute_det(aa, det) 
    real(wp), intent(inout), dimension(neqn, neqn) :: aa
    real(wp), intent(out) :: det
    real(wp) tmp,c(size(aa,dim=1),size(aa,dim=2))
    real(wp) max
    integer i,j,k,l,m,num(size(aa,dim=1))
      ! n=size(aa,dim=1)
      det=1.0
      do k=1,neqn
        max=aa(k,k);num(k)=k;
        do i=k+1,neqn 
          if(abs(max)<abs(aa(i,k))) then
            max=aa(i,k)
            num(k)=i
          endif
        enddo
        if (num(k)/=k) then
          do l=k,neqn 
            tmp=aa(k,l)
            aa(k,l)=aa(num(k),l)
            aa(num(k),l)=tmp
          enddo
          det=-1.*det
        endif
        do m=k+1,neqn
          c(m,k)=aa(m,k)/aa(k,k)
          do l=k,neqn 
            aa(m,l)=aa(m,l)-c(m,k)*aa(k,l)
          enddo
        enddo !There we made matrix triangular!	
      enddo
    
      do i=1,neqn
      det=det*aa(i,i)
      enddo
      return
    end subroutine
end module