# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.ccrg/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cc .cpp .cxx

LOCAL_C_INCLUDES += $(LOCAL_PATH)/..
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../msf_core/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../msf_core/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../msf_core/include/msf_core/implementation
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../msf_timing/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../msf_updates/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../msf_updates/src

LOCAL_MODULE    := msf

LOCAL_CFLAGS += -I$(LOCAL_PATH)/../../boost_1_53_0/include -pthread

LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti

LOCAL_SRC_FILES += ../msf_core/src/all.cc                      \
../msf_timing/src/Timer.cc                       \
../msf_updates/src/msf_distort/PoseDistorter.cc                       \
../msf_updates/src/pose_msf/pose_sensormanager.cc                      \

LOCAL_STATIC_LIBRARIES += cpufeatures
LOCAL_STATIC_LIBRARIES += eigen
LOCAL_LDLIBS    += -landroid -llog
#LOCAL_LDLIBS += -L$(LOCAL_PATH)/../../boost_1_53_0/armeabi-v7a/lib/ -lboost_system

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES) #export includes
LOCAL_EXPORT_LDLIBS := $(LOCAL_LDLIBS) #export linker cmds
LOCAL_EXPORT_CFLAGS := $(LOCAL_CFLAGS) #export c flgs
LOCAL_EXPORT_CPPFLAGS := $(LOCAL_CPPFLAGS) #export cpp flgs
LOCAL_EXPORT_CXXFLAGS := $(LOCAL_CXXFLAGS) #export cpp flgs

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/cpufeatures)
$(call import-add-path,../../)
$(call import-module,eigen)
