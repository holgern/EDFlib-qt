project(edfFileTests)

enable_testing()

                                
set(SOURCE_FILES
		BoostTest.cpp
		tst_edfannotation.cpp
		tst_edfdate.cpp
		tst_edffile.cpp
		tst_edffileheader.cpp
		tst_edfmeasurementsetup.cpp
		tst_edfpatient.cpp
		tst_edfsignaldata.cpp
		tst_edfsignalheader.cpp
		tst_edftime.cpp
	)
	
set(HEADER_FILES
	BoostTest.h
	)
set(QT_MOC_HEADERS)	

foreach(headerFile ${HEADER_FILES} )
        file(STRINGS ${headerFile} var REGEX "Q_OBJECT")
        if(var)
            list(APPEND QT_MOC_HEADERS ${headerFile})
        endif()
endforeach()
 
list(APPEND QT_MOC_SOURCES)
 
if (QT_MOC_HEADERS)
    qt5_wrap_cpp(QT_MOC_SOURCES ${QT_MOC_HEADERS})
	source_group("Generated Files" FILES ${QT_MOC_SOURCES})
endif()
    
if (USE_PCH)
activate_precompiled_headers("Pch.h" SRC_FILES)
endif() 	



include_directories(${BOOST_DIR})

add_executable(edfFileTests ${SOURCE_FILES} ${HEADER_FILES} ${QT_MOC_SOURCES})

add_test(NAME edfFileTests COMMAND edfFileTests)



target_link_libraries(edfFileTests   Qt5::Core)

target_link_libraries(edfFileTests debug  ${EDFlib_DEBUG_LIBRARIES}
													)

target_link_libraries(edfFileTests optimized   ${EDFlib_RELEASE_LIBRARIES}
														)	

target_include_directories(edfFileTests PUBLIC
							${EDFlib_INCLUDE_DIRS}
							${Boost_INCLUDE_DIR}
							)
                            
target_compile_definitions(edfFileTests PUBLIC BOOST_ALL_NO_LIB UNIT_TEST)
if(MSVC)
    set_target_properties(edfFileTests PROPERTIES COMPILE_FLAGS "/Zm200 /bigobj")
endif()




SET(EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR})
