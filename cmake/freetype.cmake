if (${UNIX})
    set(FREETYPE_INCLUDE_DIRS "/usr/include/freetype2;/usr/include/libpng16")
elseif(${MINGW})
    set(FREETYPE_DISTRIBUTION freetype-windows-binaries)
    set(FREETYPE_INCLUDE_DIRS  ${DEPS_DIR}/${FREETYPE_DISTRIBUTION}/include)
    set(FREETYPE_LINK_DIR ${DEPS_DIR}/${FREETYPE_DISTRIBUTION}/release\ dll/win64)
else()
    message(FATAL_ERROR "Not using a supported platform for freetype")
endif()