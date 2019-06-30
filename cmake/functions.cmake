function (build_sources_list SRCS )
    #Albert, si mai veus això: ets un màquina
    file(GLOB_RECURSE  SOURCEFILES CONFIGURE_DEPENDS *.cpp *.h *.cu *.cuh *.hpp *.ui *.qrc *.rc)
    #traverse unit test files and remove from sources
    file(GLOB_RECURSE SRCS_UTEST CONFIGURE_DEPENDS  *_UTest.* )
    if(SRCS_UTEST)
        list(REMOVE_ITEM SOURCEFILES ${SRCS_UTEST})
    endif()
    set (SRCS ${SOURCEFILES} PARENT_SCOPE)
endfunction()