module pes_module_xxxxx
    implicit none

    integer, parameter :: nL            = PES_NL
    integer, parameter :: tNode         = PES_TNODE
    integer, parameter :: tW            = PES_TW
    integer, parameter :: tb            = PES_TB
    integer, parameter :: dimP          = PES_DIMP
    integer, parameter :: dimR          = PES_DIMR
    integer, parameter :: dimX          = PES_DIMX
    integer, parameter :: nNode(0 : nL) = PES_NNODE
    integer, parameter :: nW(nL)        = PES_NW
    real*8  :: W(tW)
    real*8  :: b(tb)
    real*8  :: minP(dimP)
    real*8  :: maxP(dimP)
    real*8  :: minE
    real*8  :: maxE

!___insert_network_parameters

end module pes_module_xxxxx