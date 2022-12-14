add_executable(
    test_deepks
    main_deepks.cpp klist_1.cpp LCAO_deepks_test_prep.cpp LCAO_deepks_test.cpp nnr.cpp parallel_orbitals.cpp
)

if(ENABLE_COVERAGE)
  add_coverage(test_deepks)
endif()

get_target_property(ABACUS_LINK_LIBRARIES ${ABACUS_BIN_NAME} LINK_LIBRARIES)
target_link_libraries(
    test_deepks
    base cell symmetry md surchem xc_
    neighbor orb io relax gint lcao parallel mrrr pdiag pw ri driver esolver hsolver psi elecstate hamilt planewave
    pthread genelpa vdw
    deepks rpa
    ${ABACUS_LINK_LIBRARIES}
)

install(
    TARGETS test_deepks
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../tests
)
