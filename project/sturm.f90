subroutine sturm_check(lambda_vec)

  ! this subroutine performs the sturm check
  use numeth
  use fedata
  use plane42rect
  use processor
  use build_matrix

  
  integer :: j, count ! iterators and counter
  real(wp), dimension(neig), intent(in) ::  lambda_vec
  real(wp), dimension(bw, neqn) :: a
  
  call buildstiff ! build stifness and mass matrix matrix
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