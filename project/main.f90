program main
  !! The "main" routine of the program
  !!
  !! This is where execution starts when running the program
  
  use processor
  use fea
  
  implicit none
  
  call stopwatch('star')
  
  ! Read model data
  call input
  
  ! Initialize problem
  call initial

  call displ

  ! call eigen

  call central_diff_exp

  call stopwatch('stop')

end program main
