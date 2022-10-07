cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(VulkanEngine CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################



























set(PROJECT_NAME VulkanEngine)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "src/Application.hpp"
    "src/Buffer/Buffer.hpp"
    "src/Buffer/IndexBuffer.hpp"
    "src/Buffer/StagingBuffer.hpp"
    "src/Buffer/UniformBuffer.hpp"
    "src/Buffer/VertexBuffer.hpp"
    "src/Descriptor/DescriptorSet.hpp"
    "src/Device.hpp"
    "src/Model.hpp"
    "src/Pipeline.hpp"
    "src/Renderer.hpp"
    "src/Swapchain.hpp"
    "src/Texture.hpp"
    "src/Utilities.hpp"
    "src/vendor/glm/common.hpp"
    "src/vendor/glm/detail/_features.hpp"
    "src/vendor/glm/detail/_fixes.hpp"
    "src/vendor/glm/detail/_noise.hpp"
    "src/vendor/glm/detail/_swizzle.hpp"
    "src/vendor/glm/detail/_swizzle_func.hpp"
    "src/vendor/glm/detail/_vectorize.hpp"
    "src/vendor/glm/detail/compute_common.hpp"
    "src/vendor/glm/detail/compute_vector_relational.hpp"
    "src/vendor/glm/detail/func_common.inl"
    "src/vendor/glm/detail/func_common_simd.inl"
    "src/vendor/glm/detail/func_exponential.inl"
    "src/vendor/glm/detail/func_exponential_simd.inl"
    "src/vendor/glm/detail/func_geometric.inl"
    "src/vendor/glm/detail/func_geometric_simd.inl"
    "src/vendor/glm/detail/func_integer.inl"
    "src/vendor/glm/detail/func_integer_simd.inl"
    "src/vendor/glm/detail/func_matrix.inl"
    "src/vendor/glm/detail/func_matrix_simd.inl"
    "src/vendor/glm/detail/func_packing.inl"
    "src/vendor/glm/detail/func_packing_simd.inl"
    "src/vendor/glm/detail/func_trigonometric.inl"
    "src/vendor/glm/detail/func_trigonometric_simd.inl"
    "src/vendor/glm/detail/func_vector_relational.inl"
    "src/vendor/glm/detail/func_vector_relational_simd.inl"
    "src/vendor/glm/detail/qualifier.hpp"
    "src/vendor/glm/detail/setup.hpp"
    "src/vendor/glm/detail/type_float.hpp"
    "src/vendor/glm/detail/type_half.hpp"
    "src/vendor/glm/detail/type_half.inl"
    "src/vendor/glm/detail/type_mat2x2.hpp"
    "src/vendor/glm/detail/type_mat2x2.inl"
    "src/vendor/glm/detail/type_mat2x3.hpp"
    "src/vendor/glm/detail/type_mat2x3.inl"
    "src/vendor/glm/detail/type_mat2x4.hpp"
    "src/vendor/glm/detail/type_mat2x4.inl"
    "src/vendor/glm/detail/type_mat3x2.hpp"
    "src/vendor/glm/detail/type_mat3x2.inl"
    "src/vendor/glm/detail/type_mat3x3.hpp"
    "src/vendor/glm/detail/type_mat3x3.inl"
    "src/vendor/glm/detail/type_mat3x4.hpp"
    "src/vendor/glm/detail/type_mat3x4.inl"
    "src/vendor/glm/detail/type_mat4x2.hpp"
    "src/vendor/glm/detail/type_mat4x2.inl"
    "src/vendor/glm/detail/type_mat4x3.hpp"
    "src/vendor/glm/detail/type_mat4x3.inl"
    "src/vendor/glm/detail/type_mat4x4.hpp"
    "src/vendor/glm/detail/type_mat4x4.inl"
    "src/vendor/glm/detail/type_mat4x4_simd.inl"
    "src/vendor/glm/detail/type_quat.hpp"
    "src/vendor/glm/detail/type_quat.inl"
    "src/vendor/glm/detail/type_quat_simd.inl"
    "src/vendor/glm/detail/type_vec1.hpp"
    "src/vendor/glm/detail/type_vec1.inl"
    "src/vendor/glm/detail/type_vec2.hpp"
    "src/vendor/glm/detail/type_vec2.inl"
    "src/vendor/glm/detail/type_vec3.hpp"
    "src/vendor/glm/detail/type_vec3.inl"
    "src/vendor/glm/detail/type_vec4.hpp"
    "src/vendor/glm/detail/type_vec4.inl"
    "src/vendor/glm/detail/type_vec4_simd.inl"
    "src/vendor/glm/exponential.hpp"
    "src/vendor/glm/ext.hpp"
    "src/vendor/glm/ext/matrix_clip_space.hpp"
    "src/vendor/glm/ext/matrix_clip_space.inl"
    "src/vendor/glm/ext/matrix_common.hpp"
    "src/vendor/glm/ext/matrix_common.inl"
    "src/vendor/glm/ext/matrix_double2x2.hpp"
    "src/vendor/glm/ext/matrix_double2x2_precision.hpp"
    "src/vendor/glm/ext/matrix_double2x3.hpp"
    "src/vendor/glm/ext/matrix_double2x3_precision.hpp"
    "src/vendor/glm/ext/matrix_double2x4.hpp"
    "src/vendor/glm/ext/matrix_double2x4_precision.hpp"
    "src/vendor/glm/ext/matrix_double3x2.hpp"
    "src/vendor/glm/ext/matrix_double3x2_precision.hpp"
    "src/vendor/glm/ext/matrix_double3x3.hpp"
    "src/vendor/glm/ext/matrix_double3x3_precision.hpp"
    "src/vendor/glm/ext/matrix_double3x4.hpp"
    "src/vendor/glm/ext/matrix_double3x4_precision.hpp"
    "src/vendor/glm/ext/matrix_double4x2.hpp"
    "src/vendor/glm/ext/matrix_double4x2_precision.hpp"
    "src/vendor/glm/ext/matrix_double4x3.hpp"
    "src/vendor/glm/ext/matrix_double4x3_precision.hpp"
    "src/vendor/glm/ext/matrix_double4x4.hpp"
    "src/vendor/glm/ext/matrix_double4x4_precision.hpp"
    "src/vendor/glm/ext/matrix_float2x2.hpp"
    "src/vendor/glm/ext/matrix_float2x2_precision.hpp"
    "src/vendor/glm/ext/matrix_float2x3.hpp"
    "src/vendor/glm/ext/matrix_float2x3_precision.hpp"
    "src/vendor/glm/ext/matrix_float2x4.hpp"
    "src/vendor/glm/ext/matrix_float2x4_precision.hpp"
    "src/vendor/glm/ext/matrix_float3x2.hpp"
    "src/vendor/glm/ext/matrix_float3x2_precision.hpp"
    "src/vendor/glm/ext/matrix_float3x3.hpp"
    "src/vendor/glm/ext/matrix_float3x3_precision.hpp"
    "src/vendor/glm/ext/matrix_float3x4.hpp"
    "src/vendor/glm/ext/matrix_float3x4_precision.hpp"
    "src/vendor/glm/ext/matrix_float4x2.hpp"
    "src/vendor/glm/ext/matrix_float4x2_precision.hpp"
    "src/vendor/glm/ext/matrix_float4x3.hpp"
    "src/vendor/glm/ext/matrix_float4x3_precision.hpp"
    "src/vendor/glm/ext/matrix_float4x4.hpp"
    "src/vendor/glm/ext/matrix_float4x4_precision.hpp"
    "src/vendor/glm/ext/matrix_int2x2.hpp"
    "src/vendor/glm/ext/matrix_int2x2_sized.hpp"
    "src/vendor/glm/ext/matrix_int2x3.hpp"
    "src/vendor/glm/ext/matrix_int2x3_sized.hpp"
    "src/vendor/glm/ext/matrix_int2x4.hpp"
    "src/vendor/glm/ext/matrix_int2x4_sized.hpp"
    "src/vendor/glm/ext/matrix_int3x2.hpp"
    "src/vendor/glm/ext/matrix_int3x2_sized.hpp"
    "src/vendor/glm/ext/matrix_int3x3.hpp"
    "src/vendor/glm/ext/matrix_int3x3_sized.hpp"
    "src/vendor/glm/ext/matrix_int3x4.hpp"
    "src/vendor/glm/ext/matrix_int3x4_sized.hpp"
    "src/vendor/glm/ext/matrix_int4x2.hpp"
    "src/vendor/glm/ext/matrix_int4x2_sized.hpp"
    "src/vendor/glm/ext/matrix_int4x3.hpp"
    "src/vendor/glm/ext/matrix_int4x3_sized.hpp"
    "src/vendor/glm/ext/matrix_int4x4.hpp"
    "src/vendor/glm/ext/matrix_int4x4_sized.hpp"
    "src/vendor/glm/ext/matrix_projection.hpp"
    "src/vendor/glm/ext/matrix_projection.inl"
    "src/vendor/glm/ext/matrix_relational.hpp"
    "src/vendor/glm/ext/matrix_relational.inl"
    "src/vendor/glm/ext/matrix_transform.hpp"
    "src/vendor/glm/ext/matrix_transform.inl"
    "src/vendor/glm/ext/matrix_uint2x2.hpp"
    "src/vendor/glm/ext/matrix_uint2x2_sized.hpp"
    "src/vendor/glm/ext/matrix_uint2x3.hpp"
    "src/vendor/glm/ext/matrix_uint2x3_sized.hpp"
    "src/vendor/glm/ext/matrix_uint2x4.hpp"
    "src/vendor/glm/ext/matrix_uint2x4_sized.hpp"
    "src/vendor/glm/ext/matrix_uint3x2.hpp"
    "src/vendor/glm/ext/matrix_uint3x2_sized.hpp"
    "src/vendor/glm/ext/matrix_uint3x3.hpp"
    "src/vendor/glm/ext/matrix_uint3x3_sized.hpp"
    "src/vendor/glm/ext/matrix_uint3x4.hpp"
    "src/vendor/glm/ext/matrix_uint3x4_sized.hpp"
    "src/vendor/glm/ext/matrix_uint4x2.hpp"
    "src/vendor/glm/ext/matrix_uint4x2_sized.hpp"
    "src/vendor/glm/ext/matrix_uint4x3.hpp"
    "src/vendor/glm/ext/matrix_uint4x3_sized.hpp"
    "src/vendor/glm/ext/matrix_uint4x4.hpp"
    "src/vendor/glm/ext/matrix_uint4x4_sized.hpp"
    "src/vendor/glm/ext/quaternion_common.hpp"
    "src/vendor/glm/ext/quaternion_common.inl"
    "src/vendor/glm/ext/quaternion_common_simd.inl"
    "src/vendor/glm/ext/quaternion_double.hpp"
    "src/vendor/glm/ext/quaternion_double_precision.hpp"
    "src/vendor/glm/ext/quaternion_exponential.hpp"
    "src/vendor/glm/ext/quaternion_exponential.inl"
    "src/vendor/glm/ext/quaternion_float.hpp"
    "src/vendor/glm/ext/quaternion_float_precision.hpp"
    "src/vendor/glm/ext/quaternion_geometric.hpp"
    "src/vendor/glm/ext/quaternion_geometric.inl"
    "src/vendor/glm/ext/quaternion_relational.hpp"
    "src/vendor/glm/ext/quaternion_relational.inl"
    "src/vendor/glm/ext/quaternion_transform.hpp"
    "src/vendor/glm/ext/quaternion_transform.inl"
    "src/vendor/glm/ext/quaternion_trigonometric.hpp"
    "src/vendor/glm/ext/quaternion_trigonometric.inl"
    "src/vendor/glm/ext/scalar_common.hpp"
    "src/vendor/glm/ext/scalar_common.inl"
    "src/vendor/glm/ext/scalar_constants.hpp"
    "src/vendor/glm/ext/scalar_constants.inl"
    "src/vendor/glm/ext/scalar_int_sized.hpp"
    "src/vendor/glm/ext/scalar_integer.hpp"
    "src/vendor/glm/ext/scalar_integer.inl"
    "src/vendor/glm/ext/scalar_packing.hpp"
    "src/vendor/glm/ext/scalar_packing.inl"
    "src/vendor/glm/ext/scalar_relational.hpp"
    "src/vendor/glm/ext/scalar_relational.inl"
    "src/vendor/glm/ext/scalar_uint_sized.hpp"
    "src/vendor/glm/ext/scalar_ulp.hpp"
    "src/vendor/glm/ext/scalar_ulp.inl"
    "src/vendor/glm/ext/vector_bool1.hpp"
    "src/vendor/glm/ext/vector_bool1_precision.hpp"
    "src/vendor/glm/ext/vector_bool2.hpp"
    "src/vendor/glm/ext/vector_bool2_precision.hpp"
    "src/vendor/glm/ext/vector_bool3.hpp"
    "src/vendor/glm/ext/vector_bool3_precision.hpp"
    "src/vendor/glm/ext/vector_bool4.hpp"
    "src/vendor/glm/ext/vector_bool4_precision.hpp"
    "src/vendor/glm/ext/vector_common.hpp"
    "src/vendor/glm/ext/vector_common.inl"
    "src/vendor/glm/ext/vector_double1.hpp"
    "src/vendor/glm/ext/vector_double1_precision.hpp"
    "src/vendor/glm/ext/vector_double2.hpp"
    "src/vendor/glm/ext/vector_double2_precision.hpp"
    "src/vendor/glm/ext/vector_double3.hpp"
    "src/vendor/glm/ext/vector_double3_precision.hpp"
    "src/vendor/glm/ext/vector_double4.hpp"
    "src/vendor/glm/ext/vector_double4_precision.hpp"
    "src/vendor/glm/ext/vector_float1.hpp"
    "src/vendor/glm/ext/vector_float1_precision.hpp"
    "src/vendor/glm/ext/vector_float2.hpp"
    "src/vendor/glm/ext/vector_float2_precision.hpp"
    "src/vendor/glm/ext/vector_float3.hpp"
    "src/vendor/glm/ext/vector_float3_precision.hpp"
    "src/vendor/glm/ext/vector_float4.hpp"
    "src/vendor/glm/ext/vector_float4_precision.hpp"
    "src/vendor/glm/ext/vector_int1.hpp"
    "src/vendor/glm/ext/vector_int1_sized.hpp"
    "src/vendor/glm/ext/vector_int2.hpp"
    "src/vendor/glm/ext/vector_int2_sized.hpp"
    "src/vendor/glm/ext/vector_int3.hpp"
    "src/vendor/glm/ext/vector_int3_sized.hpp"
    "src/vendor/glm/ext/vector_int4.hpp"
    "src/vendor/glm/ext/vector_int4_sized.hpp"
    "src/vendor/glm/ext/vector_integer.hpp"
    "src/vendor/glm/ext/vector_integer.inl"
    "src/vendor/glm/ext/vector_packing.hpp"
    "src/vendor/glm/ext/vector_packing.inl"
    "src/vendor/glm/ext/vector_relational.hpp"
    "src/vendor/glm/ext/vector_relational.inl"
    "src/vendor/glm/ext/vector_uint1.hpp"
    "src/vendor/glm/ext/vector_uint1_sized.hpp"
    "src/vendor/glm/ext/vector_uint2.hpp"
    "src/vendor/glm/ext/vector_uint2_sized.hpp"
    "src/vendor/glm/ext/vector_uint3.hpp"
    "src/vendor/glm/ext/vector_uint3_sized.hpp"
    "src/vendor/glm/ext/vector_uint4.hpp"
    "src/vendor/glm/ext/vector_uint4_sized.hpp"
    "src/vendor/glm/ext/vector_ulp.hpp"
    "src/vendor/glm/ext/vector_ulp.inl"
    "src/vendor/glm/fwd.hpp"
    "src/vendor/glm/geometric.hpp"
    "src/vendor/glm/glm.hpp"
    "src/vendor/glm/gtc/bitfield.hpp"
    "src/vendor/glm/gtc/bitfield.inl"
    "src/vendor/glm/gtc/color_space.hpp"
    "src/vendor/glm/gtc/color_space.inl"
    "src/vendor/glm/gtc/constants.hpp"
    "src/vendor/glm/gtc/constants.inl"
    "src/vendor/glm/gtc/epsilon.hpp"
    "src/vendor/glm/gtc/epsilon.inl"
    "src/vendor/glm/gtc/integer.hpp"
    "src/vendor/glm/gtc/integer.inl"
    "src/vendor/glm/gtc/matrix_access.hpp"
    "src/vendor/glm/gtc/matrix_access.inl"
    "src/vendor/glm/gtc/matrix_integer.hpp"
    "src/vendor/glm/gtc/matrix_inverse.hpp"
    "src/vendor/glm/gtc/matrix_inverse.inl"
    "src/vendor/glm/gtc/matrix_transform.hpp"
    "src/vendor/glm/gtc/matrix_transform.inl"
    "src/vendor/glm/gtc/noise.hpp"
    "src/vendor/glm/gtc/noise.inl"
    "src/vendor/glm/gtc/packing.hpp"
    "src/vendor/glm/gtc/packing.inl"
    "src/vendor/glm/gtc/quaternion.hpp"
    "src/vendor/glm/gtc/quaternion.inl"
    "src/vendor/glm/gtc/quaternion_simd.inl"
    "src/vendor/glm/gtc/random.hpp"
    "src/vendor/glm/gtc/random.inl"
    "src/vendor/glm/gtc/reciprocal.hpp"
    "src/vendor/glm/gtc/reciprocal.inl"
    "src/vendor/glm/gtc/round.hpp"
    "src/vendor/glm/gtc/round.inl"
    "src/vendor/glm/gtc/type_aligned.hpp"
    "src/vendor/glm/gtc/type_precision.hpp"
    "src/vendor/glm/gtc/type_precision.inl"
    "src/vendor/glm/gtc/type_ptr.hpp"
    "src/vendor/glm/gtc/type_ptr.inl"
    "src/vendor/glm/gtc/ulp.hpp"
    "src/vendor/glm/gtc/ulp.inl"
    "src/vendor/glm/gtc/vec1.hpp"
    "src/vendor/glm/gtx/associated_min_max.hpp"
    "src/vendor/glm/gtx/associated_min_max.inl"
    "src/vendor/glm/gtx/bit.hpp"
    "src/vendor/glm/gtx/bit.inl"
    "src/vendor/glm/gtx/closest_point.hpp"
    "src/vendor/glm/gtx/closest_point.inl"
    "src/vendor/glm/gtx/color_encoding.hpp"
    "src/vendor/glm/gtx/color_encoding.inl"
    "src/vendor/glm/gtx/color_space.hpp"
    "src/vendor/glm/gtx/color_space.inl"
    "src/vendor/glm/gtx/color_space_YCoCg.hpp"
    "src/vendor/glm/gtx/color_space_YCoCg.inl"
    "src/vendor/glm/gtx/common.hpp"
    "src/vendor/glm/gtx/common.inl"
    "src/vendor/glm/gtx/compatibility.hpp"
    "src/vendor/glm/gtx/compatibility.inl"
    "src/vendor/glm/gtx/component_wise.hpp"
    "src/vendor/glm/gtx/component_wise.inl"
    "src/vendor/glm/gtx/dual_quaternion.hpp"
    "src/vendor/glm/gtx/dual_quaternion.inl"
    "src/vendor/glm/gtx/easing.hpp"
    "src/vendor/glm/gtx/easing.inl"
    "src/vendor/glm/gtx/euler_angles.hpp"
    "src/vendor/glm/gtx/euler_angles.inl"
    "src/vendor/glm/gtx/extend.hpp"
    "src/vendor/glm/gtx/extend.inl"
    "src/vendor/glm/gtx/extended_min_max.hpp"
    "src/vendor/glm/gtx/extended_min_max.inl"
    "src/vendor/glm/gtx/exterior_product.hpp"
    "src/vendor/glm/gtx/exterior_product.inl"
    "src/vendor/glm/gtx/fast_exponential.hpp"
    "src/vendor/glm/gtx/fast_exponential.inl"
    "src/vendor/glm/gtx/fast_square_root.hpp"
    "src/vendor/glm/gtx/fast_square_root.inl"
    "src/vendor/glm/gtx/fast_trigonometry.hpp"
    "src/vendor/glm/gtx/fast_trigonometry.inl"
    "src/vendor/glm/gtx/float_notmalize.inl"
    "src/vendor/glm/gtx/functions.hpp"
    "src/vendor/glm/gtx/functions.inl"
    "src/vendor/glm/gtx/gradient_paint.hpp"
    "src/vendor/glm/gtx/gradient_paint.inl"
    "src/vendor/glm/gtx/handed_coordinate_space.hpp"
    "src/vendor/glm/gtx/handed_coordinate_space.inl"
    "src/vendor/glm/gtx/hash.hpp"
    "src/vendor/glm/gtx/hash.inl"
    "src/vendor/glm/gtx/integer.hpp"
    "src/vendor/glm/gtx/integer.inl"
    "src/vendor/glm/gtx/intersect.hpp"
    "src/vendor/glm/gtx/intersect.inl"
    "src/vendor/glm/gtx/io.hpp"
    "src/vendor/glm/gtx/io.inl"
    "src/vendor/glm/gtx/log_base.hpp"
    "src/vendor/glm/gtx/log_base.inl"
    "src/vendor/glm/gtx/matrix_cross_product.hpp"
    "src/vendor/glm/gtx/matrix_cross_product.inl"
    "src/vendor/glm/gtx/matrix_decompose.hpp"
    "src/vendor/glm/gtx/matrix_decompose.inl"
    "src/vendor/glm/gtx/matrix_factorisation.hpp"
    "src/vendor/glm/gtx/matrix_factorisation.inl"
    "src/vendor/glm/gtx/matrix_interpolation.hpp"
    "src/vendor/glm/gtx/matrix_interpolation.inl"
    "src/vendor/glm/gtx/matrix_major_storage.hpp"
    "src/vendor/glm/gtx/matrix_major_storage.inl"
    "src/vendor/glm/gtx/matrix_operation.hpp"
    "src/vendor/glm/gtx/matrix_operation.inl"
    "src/vendor/glm/gtx/matrix_query.hpp"
    "src/vendor/glm/gtx/matrix_query.inl"
    "src/vendor/glm/gtx/matrix_transform_2d.hpp"
    "src/vendor/glm/gtx/matrix_transform_2d.inl"
    "src/vendor/glm/gtx/mixed_product.hpp"
    "src/vendor/glm/gtx/mixed_product.inl"
    "src/vendor/glm/gtx/norm.hpp"
    "src/vendor/glm/gtx/norm.inl"
    "src/vendor/glm/gtx/normal.hpp"
    "src/vendor/glm/gtx/normal.inl"
    "src/vendor/glm/gtx/normalize_dot.hpp"
    "src/vendor/glm/gtx/normalize_dot.inl"
    "src/vendor/glm/gtx/number_precision.hpp"
    "src/vendor/glm/gtx/number_precision.inl"
    "src/vendor/glm/gtx/optimum_pow.hpp"
    "src/vendor/glm/gtx/optimum_pow.inl"
    "src/vendor/glm/gtx/orthonormalize.hpp"
    "src/vendor/glm/gtx/orthonormalize.inl"
    "src/vendor/glm/gtx/perpendicular.hpp"
    "src/vendor/glm/gtx/perpendicular.inl"
    "src/vendor/glm/gtx/polar_coordinates.hpp"
    "src/vendor/glm/gtx/polar_coordinates.inl"
    "src/vendor/glm/gtx/projection.hpp"
    "src/vendor/glm/gtx/projection.inl"
    "src/vendor/glm/gtx/quaternion.hpp"
    "src/vendor/glm/gtx/quaternion.inl"
    "src/vendor/glm/gtx/range.hpp"
    "src/vendor/glm/gtx/raw_data.hpp"
    "src/vendor/glm/gtx/raw_data.inl"
    "src/vendor/glm/gtx/rotate_normalized_axis.hpp"
    "src/vendor/glm/gtx/rotate_normalized_axis.inl"
    "src/vendor/glm/gtx/rotate_vector.hpp"
    "src/vendor/glm/gtx/rotate_vector.inl"
    "src/vendor/glm/gtx/scalar_multiplication.hpp"
    "src/vendor/glm/gtx/scalar_relational.hpp"
    "src/vendor/glm/gtx/scalar_relational.inl"
    "src/vendor/glm/gtx/spline.hpp"
    "src/vendor/glm/gtx/spline.inl"
    "src/vendor/glm/gtx/std_based_type.hpp"
    "src/vendor/glm/gtx/std_based_type.inl"
    "src/vendor/glm/gtx/string_cast.hpp"
    "src/vendor/glm/gtx/string_cast.inl"
    "src/vendor/glm/gtx/texture.hpp"
    "src/vendor/glm/gtx/texture.inl"
    "src/vendor/glm/gtx/transform.hpp"
    "src/vendor/glm/gtx/transform.inl"
    "src/vendor/glm/gtx/transform2.hpp"
    "src/vendor/glm/gtx/transform2.inl"
    "src/vendor/glm/gtx/type_aligned.hpp"
    "src/vendor/glm/gtx/type_aligned.inl"
    "src/vendor/glm/gtx/type_trait.hpp"
    "src/vendor/glm/gtx/type_trait.inl"
    "src/vendor/glm/gtx/vec_swizzle.hpp"
    "src/vendor/glm/gtx/vector_angle.hpp"
    "src/vendor/glm/gtx/vector_angle.inl"
    "src/vendor/glm/gtx/vector_query.hpp"
    "src/vendor/glm/gtx/vector_query.inl"
    "src/vendor/glm/gtx/wrap.hpp"
    "src/vendor/glm/gtx/wrap.inl"
    "src/vendor/glm/integer.hpp"
    "src/vendor/glm/mat2x2.hpp"
    "src/vendor/glm/mat2x3.hpp"
    "src/vendor/glm/mat2x4.hpp"
    "src/vendor/glm/mat3x2.hpp"
    "src/vendor/glm/mat3x3.hpp"
    "src/vendor/glm/mat3x4.hpp"
    "src/vendor/glm/mat4x2.hpp"
    "src/vendor/glm/mat4x3.hpp"
    "src/vendor/glm/mat4x4.hpp"
    "src/vendor/glm/matrix.hpp"
    "src/vendor/glm/packing.hpp"
    "src/vendor/glm/simd/common.h"
    "src/vendor/glm/simd/exponential.h"
    "src/vendor/glm/simd/geometric.h"
    "src/vendor/glm/simd/integer.h"
    "src/vendor/glm/simd/matrix.h"
    "src/vendor/glm/simd/neon.h"
    "src/vendor/glm/simd/packing.h"
    "src/vendor/glm/simd/platform.h"
    "src/vendor/glm/simd/trigonometric.h"
    "src/vendor/glm/simd/vector_relational.h"
    "src/vendor/glm/trigonometric.hpp"
    "src/vendor/glm/vec2.hpp"
    "src/vendor/glm/vec3.hpp"
    "src/vendor/glm/vec4.hpp"
    "src/vendor/glm/vector_relational.hpp"
    "src/Window.hpp"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "src/Application.cpp"
    "src/Buffer/Buffer.cpp"
    "src/Buffer/IndexBuffer.cpp"
    "src/Buffer/StagingBuffer.cpp"
    "src/Buffer/UniformBuffer.cpp"
    "src/Buffer/VertexBuffer.cpp"
    "src/Descriptor/DescriptorSet.cpp"
    "src/Device.cpp"
    "src/Main.cpp"
    "src/Model.cpp"
    "src/Pipeline.cpp"
    "src/Renderer.cpp"
    "src/Swapchain.cpp"
    "src/Texture.cpp"
    "src/Window.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE VulkanEngine)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Users/alifayed/CLionProjects/Vulkan-Renderer/Dependencies/Vulkan/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/Users/alifayed/CLionProjects/Vulkan-Renderer/Dependencies/GLFW/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/Users/alifayed/CLionProjects/Vulkan-Renderer/src/vendor;"
        "${CMAKE_CURRENT_SOURCE_DIR}/Users/alifayed/CLionProjects/Vulkan-Renderer/src/vendor/glm;"
        "${CMAKE_CURRENT_SOURCE_DIR}/Users/alifayed/CLionProjects/Vulkan-Renderer/src"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Users/alifayed/CLionProjects/Vulkan-Renderer/Dependencies/Vulkan/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/Users/alifayed/CLionProjects/Vulkan-Renderer/Dependencies/GLFW/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/Users/alifayed/CLionProjects/Vulkan-Renderer/src/vendor"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "vulkan-1;"
        "glfw3"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "vulkan-1;"
        "glfw3"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "D:/OpenGL Projects/VulkanEngine/Dependencies/Vulkan/lib;"
        "${CMAKE_SOURCE_DIR}/Dependencies/GLFW/build/src/$<CONFIG>"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "D:/OpenGL Projects/VulkanEngine/Dependencies/Vulkan/lib;"
        "${CMAKE_SOURCE_DIR}/Dependencies/GLFW/build/src/$<CONFIG>"
    )
endif()

