// Copyright (c) 2018 LG Electronics, Inc.
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

#include "content/public/common/content_neva_switches.h"

namespace switches {

// Enable aggressive GC on foreground tabs
const char kEnableAggressiveForegroundGC[] = "enable-aggressive-foreground-gc";

// Loads browser control injection
const char kEnableBrowserControlInjection[]
    = "enable-browser-control-injection";

// Loads sample injection
const char kEnableSampleInjection[]     = "enable-sample-injection";

// Overrides the timeout, in seconds, that FirstMeaningfulPaintDetector waits
// for a network stable timer to be fired.
const char kNetworkStableTimeout[] = "network-stable-timeout";

// Use platform implementation for Input Controls (File picker,
// Color chooser, e.t.c.)
const char kUseExternalInputControls[]  = "use-external-input-controls";

}  // namespace switches
