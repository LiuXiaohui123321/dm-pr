remove_definitions(-DUSE_LIBXC)

AddTest(
  TARGET XCTest_PBE
  LIBS MPI::MPI_CXX # required by global.h; for details, `remove_definitions(-D__MPI)`.
  SOURCES test_xc.cpp ../xc_functional.cpp ../xc_functional_wrapper_xc.cpp ../xc_functional_wrapper_gcxc.cpp ../xc_funct_corr_gga.cpp ../xc_funct_corr_lda.cpp ../xc_funct_exch_gga.cpp ../xc_funct_exch_lda.cpp ../xc_funct_hcth.cpp
)
