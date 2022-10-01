module CircleModule
  implicit none
  contains

  subroutine area(a, r)
    real :: a, r, pi = 3.14
    a = pi*r**2
  end subroutine

end module