SET("SM_NAME" "ITGmania")
# SET("SM_SDK_PATH" "${CMAKE_CURRENT_SOURCE_DIR}")

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
