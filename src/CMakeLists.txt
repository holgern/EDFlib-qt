macro(RemoveDebugCXXFlag flag)
    string(REPLACE "${flag}" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endmacro()
macro(RemoveReleaseCXXFlag flag)
    string(REPLACE "${flag}" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
endmacro()

#message(${CMAKE_CXX_FLAGS_DEBUG}) # Print Debug Flags Before Change
RemoveDebugCXXFlag("/RTC1")
message(${CMAKE_CXX_FLAGS_DEBUG}) # Print Debug Flags After Change

#message(${CMAKE_CXX_FLAGS_RELEASE}) # Print Release Flags Before Change
#RemoveReleaseCXXFlag("/RTC1")
#message(${CMAKE_CXX_FLAGS_RELEASE}) # Print Release Flags After Change
set(SOURCE_FILES
    EDFError.cpp 
    EDFFileManager.cpp 
    EDFFile.cpp 
    EDFPatient.cpp 
    EDFFileHeader.cpp 
    EDFAnnotation.cpp
    EDFDate.cpp 
    EDFTime.cpp
    EDFSignalHeader.cpp
    EDFMeasurementSetup.cpp
    EDFSignalData.cpp
	)
	
set(HEADER_FILES
    EDFError.h
    EDFFileManager.h
    EDFFile.h
    EDFPatient.h
    EDFFileHeader.h
    EDFAnnotation.h
    EDFDate.h
    EDFTime.h
    EDFSignalHeader.h
    EDFMeasurementSetup.h
    EDFSignalData.h
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

   add_definitions(/WX)

add_library(EDFlib STATIC ${SOURCE_FILES} ${HEADER_FILES} ${QT_MOC_SOURCES})
target_link_libraries(EDFlib Qt5::Core )
target_compile_definitions(EDFlib PUBLIC)
set_target_properties(EDFlib PROPERTIES DEBUG_POSTFIX d)

