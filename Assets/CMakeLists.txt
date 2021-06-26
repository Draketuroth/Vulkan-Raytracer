cmake_minimum_required(VERSION 3.11-3.18)

file(GLOB SHADER_FILES shaders/*.vert shaders/*.frag shaders/*.rgen shaders/*.rchit shaders/*.rint shaders/*.rmiss)
file(GLOB TEXTURE_FILES textures/*.jpg textures/*.png textures/*.txt)

file(GLOB shader_EXTRA_FILES shaders/*.glsl)
set_source_files_properties(${SHADER_EXTRA_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

# Shader compilation
foreach(shader ${SHADER_FILES})
	message("SHADER ${shader}")
	get_filename_component(FILE_NAME ${shader} NAME)
	get_filename_component(FULL_PATH ${shader} ABSOLUTE)
	set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Compiled)
	message("Output Directory ${OUTPUT_DIR}")
	set(OUTPUT_FILE ${OUTPUT_DIR}/${FILE_NAME}.spv)
	message("Output File ${OUTPUT_FILE}")
	set(COMPILED_SHADERS ${COMPILED_SHADERS} ${OUTPUT_FILE})
	set(COMPILED_SHADERS ${COMPILED_SHADERS} PARENT_SCOPE)
	set_source_files_properties(${shader} PROPERTIES HEADER_FILE_ONLY TRUE)
		add_custom_command(
			OUTPUT ${OUTPUT_FILE}
			COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
			COMMAND ${Vulkan_GLSLANG_VALIDATOR} --target-env vulkan1.2 -V ${FULL_PATH} -o ${OUTPUT_FILE}
			DEPENDS ${FULL_PATH}
		)
endforeach()

source_group("Shaders" FILES ${SHADER_FILES} ${SHADER_EXTRA_FILES})
source_group("Textures" FILES ${TEXTURE_FILES})

add_custom_target(
	Assets 
	DEPENDS ${COMPILED_SHADERS}
	SOURCES ${SHADER_FILES} ${SHADER_EXTRA_FILES} ${TEXTURE_FILES})


