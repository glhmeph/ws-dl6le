! Exercise the actual text codec used by macro fallback, without transmitting.
subroutine jtty_macro_roundtrip(input,profile,output,nframes) bind(C)
  use iso_c_binding, only: c_char,c_int
  use jtty_mod, only: pack_jtty,unpack_jtty,MAX_FRAMES
  implicit none
  character(c_char), intent(in) :: input(80)
  integer(c_int), value :: profile
  character(c_char), intent(out) :: output(80)
  integer(c_int), intent(out) :: nframes
  character(len=80) :: message,decoded
  character(len=34) :: frames(MAX_FRAMES)
  integer :: i,count
  logical :: valid

  do i=1,80
    message(i:i)=input(i)
  enddo
  call pack_jtty(message,frames,count,int(profile))
  nframes=count
  decoded=''
  if(count.gt.0) then
    call unpack_jtty(frames,count,decoded,source_valid=valid)
    if(.not.valid) nframes=-1
  endif
  do i=1,80
    output(i)=decoded(i:i)
  enddo
end subroutine jtty_macro_roundtrip
