program AddToNumbers
  implicit none ! must be always added

  ! variable declaration
  integer :: a, b, c

  ! add to numbers
  ! integer (kind = 8):: x ! kind decides the amout of space to use for x
  ! character(len = 8) :: char 

  y = 1.2d0 ! double precision number
  a = 3
  b = 10
  c = b / a

  ! print the results
  print*, 'The integer division is ', c
  
  ! definition of new data type
  type Material
    real :: E, nu
    integer :: id
  end type

  type(Material) :: steel

  ! flow control
  if (a == 1) then
  else if (a < 0) then
  end if

  do 
    ! runs forever
  end do

  do i = 1, 10
    ! variable added at the end of the loop
  end do
  ! after the loop, i = 11

  do i = n, 1, -1 ! -1 is the increment
  end do

  do while (x > 0)

  end do

  increments: do
    iterations: do
    exit increments
    exit iterations
end do

end program