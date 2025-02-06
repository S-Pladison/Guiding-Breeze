# Находим все исходные файлы в папке '*/src/*'.
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.11.0")
  file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp *.h)
else()
  file(GLOB SRC_FILES *.cpp *.h */*.h */*.cpp)
endif()

# Добавляем исходники проекта.
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})