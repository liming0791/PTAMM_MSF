LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS :=

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_CFLAGS += -DHAVE_NEON=1
    LOCAL_CFLAGS += -DLOCAL_ARM_NEON=1
    #LOCAL_CFLAGS += -D__ARM_NEON__=1
    LOCAL_ARM_NEON  := true
endif

LOCAL_MODULE    := lib3ds

LOCAL_SRC_FILES += \
../src/lib3ds_camera.c       \
../src/lib3ds_file.c         \
../src/lib3ds_io.c           \
../src/lib3ds_math.c         \
../src/lib3ds_node.c         \
../src/lib3ds_shadow.c       \
../src/lib3ds_vector.c       \
../src/lib3ds_atmosphere.c   \
../src/lib3ds_chunk.c        \
../src/lib3ds_light.c        \
../src/lib3ds_matrix.c       \
../src/lib3ds_quat.c         \
../src/lib3ds_track.c        \
../src/lib3ds_viewport.c     \
../src/lib3ds_background.c   \
../src/lib3ds_chunktable.c   \
../src/lib3ds_material.c     \
../src/lib3ds_mesh.c         \
../src/lib3ds.rc             \
../src/lib3ds_util.c             

LOCAL_LDLIBSIBS +=  -llog -ldl -lstdc++
LOCAL_LDLIBS    += -landroid -lz

LOCAL_C_INCLUDES +=$(LOCAL_PATH)/../src
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES) #export includes

$(info $(LOCAL_C_INCLUDES)) 
$(info hello) 

LOCAL_EXPORT_LDLIBS := $(LOCAL_LDLIBS) #export linker cmds
LOCAL_EXPORT_CFLAGS := $(LOCAL_CFLAGS) #export c flgs
LOCAL_EXPORT_CPPFLAGS := $(LOCAL_CPPFLAGS) #export cpp flgs
LOCAL_EXPORT_CXXFLAGS := $(LOCAL_CXXFLAGS) #export cpp flgs

include $(BUILD_STATIC_LIBRARY)
