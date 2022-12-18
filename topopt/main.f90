program main
  !! The "main" routine of the program
  !!
  !! This is where execution starts when running the program
  
  use processor
  use fea
  use fedata
  
  implicit none
  
  call stopwatch('star')
  
  ! Read model data
  call input
  
  ! Initialize problem
  call initial

  ! call displ

  ! call eigen

  ! select case ( method )
  ! case ( 1 ) ! central different method
  !   call central_diff_exp
  ! case ( 2 ) ! newmark method
  !   call newmark_imp
  ! case (3)
    
  ! end select

  call topopt

  call stopwatch('stop')

end program main
