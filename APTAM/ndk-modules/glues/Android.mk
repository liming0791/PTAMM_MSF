LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := glues

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
		glues_error.c \
		glues_mipmap.c \
		glues_project.c \
		glues_quad.c \
		glues_registry.c

LOCAL_LDLIBS    := -llog -lGLESv1_CM
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

include $(BUILD_STATIC_LIBRARY)

