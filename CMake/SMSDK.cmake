IF(SM_EXE_NAME)
  set(SM_TARGET "ITGmania")

  SET("SM_SDK_PATH" "${CMAKE_CURRENT_LIST_DIR}/..")
  SET(SM_SDK_INCLUDE_PATH
          "${SM_SDK_PATH}/src"
          "${SM_SDK_PATH}/src/generated"
          "${SM_SDK_PATH}/extern/lua-5.1/src"
          "${SM_SDK_PATH}/extern/dynalo/include"
  )
ELSE()
  SET("SM_NAME" "ITGmania")
  SET("SM_SDK_PATH" "${CMAKE_CURRENT_LIST_DIR}")
  set(SM_TARGET "StepMania")

  SET(SM_SDK_INCLUDE_PATH
          "${SM_SDK_PATH}/include"
          "${SM_SDK_PATH}/include/generated"
          "${SM_SDK_PATH}/extern/lua-5.1/src"
          "${SM_SDK_PATH}/extern/dynalo/include"
  )

  add_library("StepMania" IMPORTED STATIC GLOBAL)
  set_target_properties(StepMania PROPERTIES
          IMPORTED_LOCATION "${SM_SDK_PATH}/lib/${SM_NAME}.lib")

  add_library("lua-5.1" STATIC IMPORTED)
  set_property(TARGET "lua-5.1" PROPERTY
              IMPORTED_LOCATION "${SM_SDK_PATH}/lib/lua-5.1.lib")

ENDIF()

# IF(WIN32)
        # add_compile_options(
        #         # /MTd
        #         $<$<CONFIG:>:/MT> #---------|
        #         $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        #         $<$<CONFIG:Release>:/MT> #--|
        # )
# ENDIF()

MACRO(REGISTER_PLUGIN SOURCE_DIR PLUGIN_NAME)
        add_library(
                ${PLUGIN_NAME} SHARED

                "${SOURCE_DIR}/${PLUGIN_NAME}.cpp"
                "${SOURCE_DIR}/${PLUGIN_NAME}.h"
        )

        SET(PLUGIN_DEFS
                CMAKE_POWERED
                LTM_DESC
                LTC_BASE64
                LTC_BASE64_URL
                GLOBALS_IMPORT
                UNIX
        )

        IF(LINUX)
                list(APPEND PLUGIN_DEFS "UNIX")
        ENDIF()

        IF(WIN32)
                target_link_libraries(${PLUGIN_NAME} "setupapi")

                if(MSVC)
                        set_target_properties(${PLUGIN_NAME} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:LIBCMTD.lib")
                endif()
        ENDIF()

        target_compile_definitions(${PLUGIN_NAME} PRIVATE ${PLUGIN_DEFS})
        target_link_libraries(${PLUGIN_NAME} ${SM_TARGET})
        target_include_directories(${PLUGIN_NAME} PUBLIC ${SM_SDK_INCLUDE_PATH})
        set_property(TARGET ${PLUGIN_NAME} PROPERTY FOLDER "Plugins")
        set_target_properties(${PLUGIN_NAME} PROPERTIES
                CXX_STANDARD 20
                CXX_EXTENSIONS OFF
                PREFIX "")

        if(PLUGIN_OUT_DIR)
                set_target_properties(${PLUGIN_NAME} PROPERTIES
                LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUT_DIR})
        endif()

        message(${PLUGIN_NAME} " registered")

ENDMACRO()

message("SMSDK Loaded")
