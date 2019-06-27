module pes_module_xxxxx
    implicit none

    ! number of layers (hidden + output) in neural network
    integer, parameter :: nL    = 3    
    ! total number of nodes (input + hidden + output) in neural network
    integer, parameter :: tNode = 127
    ! total number of weights in neural network
    integer, parameter :: tW    = 2910
    ! total number of bias in neural network    
    integer, parameter :: tb    = 61
    ! dimesion of p
    integer, parameter :: dimP  = 66
    ! dimesion of r
    integer, parameter :: dimR  = 15
    ! dimesion of x(3)
    integer, parameter :: dimX  = 6
    ! number of nodes for each layer
    integer, parameter :: nNode(0 : nL) = (/66,30,30,1/)
    ! number of weights for each layer
    integer, parameter :: nW(nL)        = (/1980,900,30/)
    real*8  :: W(tW)
    real*8  :: b(tb)
    real*8  :: minP(dimP)
    real*8  :: maxP(dimP)
    real*8  :: minE
    real*8  :: maxE

!___insert_network_parameters    

end module pes_module_xxxxx

module pes_variable_mod_xxxxx
    implicit none

!$  integer, external   :: omp_get_thread_num, omp_get_max_threads
    real*8, allocatable :: r(:,:)
    real*8, allocatable :: p(:,:)
    real*8, allocatable :: dedp(:,:)
    real*8, allocatable :: dedr(:,:)
    real*8, allocatable :: dpdr(:,:,:)
    real*8, allocatable :: drdx(:,:,:,:)
!___insert_declaration_rn

end module pes_variable_mod_xxxxx

subroutine pes_destruct_xxxxx
    use pes_variable_mod_xxxxx
    implicit none

    if (allocated(r))     deallocate(r)
    if (allocated(p))     deallocate(p)
    if (allocated(dedp))  deallocate(dedp)
    if (allocated(dedr))  deallocate(dedr)
    if (allocated(dpdr))  deallocate(dpdr)
    if (allocated(drdx))  deallocate(drdx)
!___insert_deallocate_rn    

end subroutine pes_destruct_xxxxx

subroutine pes_init_xxxxx
    use pes_variable_mod_xxxxx
    implicit none
    integer :: nTh

    nTh = 1
!$  nTh = omp_get_max_threads()

    allocate(r(dimR,nTh))
    allocate(p(NP,nTh))
    allocate(dedp(NP,nTh))
    allocate(dedr(dimR,nTh))
    allocate(dpdr(NP,dimR,nTh))
    allocate(drdx(3,dimX,dimR,nTh))
!___insert_allocate_rn_xxxxx

end subroutine pes_init_xxxxx

subroutine pes_calc_dedp_xxxxx(p, dedp)
    use pes_module_xxxxx
    implicit none
    real*8, intent(in)  :: p(dimP)
    real*8, intent(out) :: dedp(dimP)

    real*8  :: z(tNode - dimP - 1)
    real*8  :: a(tNode - dimP - 1)
    real*8  :: dedz(tNode - dimP - 1)
    integer :: idz(2,nL), idw(2,nL), iL

    idz(1,1) = 1
    idz(2,1) = nNode(1)
    idw(1,1) = 1
    idw(2,2) = nW(1)

    do iL = 2, nL
        idz(1, iL) = idz(2, iL-1) + 1
        idz(2, iL) = idz(2, iL-1) + nNode(iL)
        idw(1, iL) = idw(2, iL-1) + 1
        idw(2, iL) = idw(2, iL-1) + nW(iL)
    end do

    ! ========== calculate Z1 ==========
    z(idz(1,1) : idz(2,1)) = b(idz(1,1) : idz(2,1))
    call dgemv('N', nNode(1), dimP, 1.d0, W(idw(1,1):idw(2,1)), &
               nNode(1), p, 1, 1.d0, z(idz(1,1):idz(2,1)), 1)

    ! ========== calculate Z(2) - Z(nL-1) ==========
    do iL = 2, nL - 1
        call network_calc_activate(nNode(iL-1), z(idz(1,iL-1):idz(2,iL-1)), a(idz(1,iL-1):idz(2,iL-1)))

        z(idz(1, iL) : idz(2, iL)) = b(idz(1, iL) : idz(2, iL))
        call degmv('N', nNode(iL), nNode(iL - 1), 1.d0, W(idw(1, iL) : idw(2, iL)), &
                   nNode(iL), z(idz(1, iL-1) : idz(2, iL-1)), 1, 1.d0, z(idz(1, iL) : idz(2, iL)), 1)
    end do

    ! ========== calculate dfdz ==========
    call network_calc_dfdz(tNode - dimP - 1, z)

    ! ========== calculate dedz ==========
    dedz = 0.d0

    dedz(idz(1, 2) : idz(2, 2)) = W(idw(1, 3) : idw(2, 3)) * z(idz(1, 2) : idz(2, 2))
    do iL = nL - 2, 1, -1
        call degmv('T', nNode(iL+1), nNode(iL), 1.d0, W(idw(1, iL+1) : idw(2, iL+1)), &
                   nNode(iL+1), dedz(idz(1,iL+1) : idz(2,iL+1)), 1, 1.d0, dedz(idz(1, iL) : idz(2, iL)))
        dedz(idz(1,iL) : idz(2,iL)) = dedz(idz(1,iL) : idz(2,iL)) * z(idz(1,iL) : idz(2,iL))
    end do

    call degmv('T', nNode(1), nNode(0), 1.d0, W(idw(1,1) : idw(2,1)), &
               nNode(1), dedz(idz(1,1) : idz(2,1)), 1, 1.d0, dedp(1 : dimP))
               
end subroutine pes_calc_dedp_xxxxx

subroutine pes_calc_dedx_xxxxx(x,dedx)
    use pes_module_xxxxx
    use pes_variable_mod_xxxxx
    implicit none

    real*8, intent(in)  :: x(3,dimX)
    real*8, intent(out) :: dedx(3,dimX)
    integer :: pos, i, j, iTh

    iTh = 1
!$  iTh = omp_get_thread_num()

    pos = 0
    do i = 1, dimX - 1
        do j = i + 1, dimX
            pos = pos + 1
            r(pos, iTh) = sum((x(:,i) - x(:,j))**2)
        end do
    end do
    r(:,iTh) = 1.d0 / sqrt(r(:,iTh))

!___insert_call_pes_calc_rn_xxxxx
!___insert_call_pes_calc_p_xxxxx
!___insert_call_pes_calc_dpdr_xxxxx

    call pes_calc_dedp_xxxxx(p(:,iTh), dedp(:,iTh))
    call pes_calc_drdx_xxxxx(r(:,iTh), r3(:,iTh), x, drdx(:,:,:,iTh))

    dedr(:,iTh) = 0.d0
    call degmv('T', dimP, dimR, 1.d0, dpdr, dimP, dedp(:,iTh), 1, 1.d0, dedr(:,iTh))

    dedx = 0.d0
    call degmv('N', 3*dimX, dimR, 1.d0, drdx(:,:,:,iTh), 3*dimX, dedr(:,iTh), 1, 1.d0, dedx)

end subroutine pes_calc_dedx_xxxxx