
# Find header files
find_path(KML_INCLUDE_DIRS kmldom.h PATHS
    /usr/include
    /usr/local/include
    /usr/include/kml/dom
    /usr/local/include/kml/dom
)
# Find librarie
find_library(KMLBASE_LIBRARY kmlbase PATHS
    /usr/lib
    /usr/local/lib
    /usr/lib/kmlbase
    /usr/local/lib/kmlbase
)
find_library(KMLDOM_LIBRARY kmldom PATHS
    /usr/lib
    /usr/local/lib
    /usr/lib/dom
    /usr/local/lib/dom
)
if(KMLBASE_LIBRARY AND KMLDOM_LIBRARY)
    set(KML_LIBRARIES ${KMLBASE_LIBRARY} ${KMLDOM_LIBRARY})
endif()

if(KML_INCLUDE_DIRS AND KML_LIBRARIES)
    set(KML_FOUND TRUE)
else()
    set(KML_FOUND FALSE)
endif()

message(STATUS "KML_INCLUDE_DIRS: ${KML_INCLUDE_DIRS}")
message(STATUS "KML_LIBRARIES: ${KML_LIBRARIES}")

mark_as_advanced(KML_INCLUDE_DIRS KML_LIBRARIES)

