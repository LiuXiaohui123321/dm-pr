AddTest(
  TARGET hsolver_diago_elpa
  LIBS ${math_libs} ELPA::ELPA
  SOURCES diago_elpa_test.cpp
)

if(USE_CUSOLVER_LCAO)
    list(APPEND cuda_sources ../diag_cusolver.cu)
    list(APPEND cuda_libs cusolver)
endif()

install(FILES H-KPoints.dat DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(FILES H-GammaOnly.dat DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(FILES S-KPoints.dat DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(FILES S-GammaOnly.dat DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(FILES H-KPoints-large.dat DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(FILES H-GammaOnly-large.dat DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(FILES S-KPoints-large.dat DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(FILES S-GammaOnly-large.dat DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(FILES diago_parallel_test.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(FILES diago_elpa_parallel_test.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_program(BASH bash)
add_test(NAME hsolver_diago_elpa_parallel
      COMMAND ${BASH} diago_elpa_parallel_test.sh
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
