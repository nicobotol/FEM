program main
  !! The "main" routine of the program
  !!
  !! This is where execution starts when running the program
  
  use processor
  use fea
  
  implicit none
  
  !call stopwatch('star')
  
  ! Read model data
  call input
  
  ! Initialize problem
  call initial
  
! if (antype == 'static') then
!   ! Calculate displacements
!   call displ
! else if(antype == 'modal') then
!   ! Perform the modal analysis
!   call eigen
! else
!     print*, ' Case not defined'
!     stop
! end if

!  call displ

  call eigen

  call stopwatch('stop')
end program main
