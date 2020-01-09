



########### next target ###############

set(ctest_SRCS ctest.c )


kde4_add_executable(ctest ${ctest_SRCS})

target_link_libraries(ctest  ${KDE4_KDECORE_LIBS} kmid )


########### next target ###############

set(apitest_SRCS apitest.cc )


kde4_add_executable(apitest ${apitest_SRCS})

target_link_libraries(apitest  ${KDE4_KDECORE_LIBS} kmid )


########### next target ###############

set(notesoff_SRCS notesoff.cc )


kde4_add_executable(notesoff ${notesoff_SRCS})

target_link_libraries(notesoff  ${KDE4_KDECORE_LIBS} kmid )


########### install files ###############





