add_library(
    mrrr
    OBJECT
    dcopy.cpp
    dlae2.cpp
    dlaebz.cpp
    dlaev2.cpp
    dlaneg.cpp
    dlanst.cpp
    dlar1v.cpp
    dlarnv.cpp
    dlarra.cpp
    dlarrb.cpp
    dlarrc.cpp
    dlarrd.cpp
    dlarre.cpp
    dlarrf.cpp
    dlarrj.cpp
    dlarrk.cpp
    dlarrr.cpp
    dlarrv.cpp
    dlaruv.cpp
    dlas2.cpp
    dlascl.cpp
    dlaset.cpp
    dlasq2.cpp
    dlasq3.cpp
    dlasq4.cpp
    dlasq5.cpp
    dlasq6.cpp
    dlasrt.cpp
    dlassq.cpp
    dscal.cpp
    dstemr_mpi.cpp
    dswap.cpp
    ieeeck.cpp
    ilaenv.cpp
    i_nint.cpp
    iparmq.cpp
    lsame.cpp
    psort_w.cpp
    s_cmp.cpp
    s_copy.cpp
    xerbla.cpp
)

if(ENABLE_COVERAGE)
  add_coverage(mrrr)
endif()
