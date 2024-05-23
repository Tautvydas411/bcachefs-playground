cmake_minimum_required(VERSION 3.5)

project(bcachefslibtest LANGUAGES C)

add_executable(bcachefslibtest main.c)


target_compile_definitions(bcachefslibtest PUBLIC _FILE_OFFSET_BITS=64)
target_compile_definitions(bcachefslibtest PUBLIC _GNU_SOURCE)
target_compile_definitions(bcachefslibtest PUBLIC _LGPL_SOURCE)
target_compile_definitions(bcachefslibtest PUBLIC RCU_MEMBARRIER)
target_compile_definitions(bcachefslibtest PUBLIC ZSTD_STATIC_LINKING_ONLY)
target_compile_definitions(bcachefslibtest PUBLIC NO_BCACHEFS_CHARDEV)
target_compile_definitions(bcachefslibtest PUBLIC NO_BCACHEFS_FS)
target_compile_definitions(bcachefslibtest PUBLIC NO_BCACHEFS_SYSFS)
target_compile_definitions(bcachefslibtest PUBLIC __SANE_USERSPACE_TYPES__)
target_compile_definitions(bcachefslibtest PUBLIC VERSION_STRING="1.7.0")


target_include_directories(bcachefslibtest PUBLIC BEFORE  /home/as/proj/bcachefs-tools/include) # need before, because this include clashes with system includes
target_include_directories(bcachefslibtest PUBLIC /home/as/proj/bcachefs-tools)
target_include_directories(bcachefslibtest PUBLIC /home/as/proj/bcachefs-tools/c_src)
target_link_libraries(bcachefslibtest PUBLIC /home/as/proj/bcachefs-tools/libbcachefs.so)


include(GNUInstallDirs)
install(TARGETS bcachefslibtest
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
