# OpenNI 2.x Android makefile. 
# Copyright (C) 2012 PrimeSense Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License. 

# *** 
# *** Note: This module file is only in use when building via NDK! ***
# *** 

# Android should be >= v2.3
APP_PLATFORM := android-9
APP_STL := stlport_shared
APP_CPPFLAGS := -fexceptions
APP_CFLAGS := -fexceptions

# Use ARM v7a instruction set
APP_ABI := armeabi-v7a
ARCH_ARM_HAVE_ARMV7A := true