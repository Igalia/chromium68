// Copyright (c) 2016-2018 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#include "webos/common/webos_runtime_delegate.h"

#include "webos/public/runtime.h"

namespace webos {

WebOSRuntimeDelegate::~WebOSRuntimeDelegate() {}

void* WebOSRuntimeDelegate::GetLunaServiceHandle() {
  void* ls_handle = webos::Runtime::GetInstance()->GetLSHandle();
  if (ls_handle)
    return ls_handle;
  return nullptr;
}

bool WebOSRuntimeDelegate::IsForegroundAppEnyo() {
  return webos::Runtime::GetInstance()->GetIsForegroundAppEnyo();
}

}  // namespace webos
