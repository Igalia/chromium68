// Copyright 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone/wayland/input/keyboard.h"

#include "ozone/wayland/seat.h"
#include "ozone/wayland/window.h"

namespace ozonewayland {

WaylandKeyboard::WaylandKeyboard(WaylandDisplay* dispatcher)
    : HotplugDevice(dispatcher), input_keyboard_(nullptr) {}

WaylandKeyboard::~WaylandKeyboard() {
  if (input_keyboard_)
    wl_keyboard_destroy(input_keyboard_);
}

void WaylandKeyboard::OnSeatCapabilities(wl_seat *seat, uint32_t caps) {
  static const struct wl_keyboard_listener kInputKeyboardListener = {
    WaylandKeyboard::OnKeyboardKeymap,
    WaylandKeyboard::OnKeyboardEnter,
    WaylandKeyboard::OnKeyboardLeave,
    WaylandKeyboard::OnKeyNotify,
    WaylandKeyboard::OnKeyModifiers,
  };

  if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !input_keyboard_) {
    input_keyboard_ = wl_seat_get_keyboard(seat);
    wl_keyboard_add_listener(input_keyboard_, &kInputKeyboardListener,
        this);
  }
}

void WaylandKeyboard::OnKeyNotify(void* data,
                                  wl_keyboard* input_keyboard,
                                  uint32_t serial,
                                  uint32_t time,
                                  uint32_t key,
                                  uint32_t state) {
  WaylandKeyboard* device = static_cast<WaylandKeyboard*>(data);
  ui::EventType type = ui::ET_KEY_PRESSED;
  WaylandDisplay::GetInstance()->SetSerial(serial);
  if (state == WL_KEYBOARD_KEY_STATE_RELEASED)
    type = ui::ET_KEY_RELEASED;
  const uint32_t device_id = wl_proxy_get_id(
      reinterpret_cast<wl_proxy*>(input_keyboard));
  device->GetDispatcher()->KeyNotify(type, key, device_id);
}

void WaylandKeyboard::OnKeyboardKeymap(void *data,
                                       struct wl_keyboard *keyboard,
                                       uint32_t format,
                                       int fd,
                                       uint32_t size) {
  WaylandKeyboard* device = static_cast<WaylandKeyboard*>(data);

  if (!data) {
    close(fd);
    return;
  }

  if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
    close(fd);
    return;
  }

  base::SharedMemoryHandle section = base::SharedMemoryHandle::ImportHandle(fd, size);
  device->GetDispatcher()->InitializeXKB(section, size);
}

void WaylandKeyboard::OnKeyboardEnter(void* data,
                                      wl_keyboard* input_keyboard,
                                      uint32_t serial,
                                      wl_surface* surface,
                                      wl_array* keys) {
  WaylandDisplay::GetInstance()->SetSerial(serial);

  WaylandSeat* seat = WaylandDisplay::GetInstance()->PrimarySeat();

  if (!surface) {
    seat->SetFocusWindowHandle(0);
    return;
  }

  if (!data)
    return;

  WaylandKeyboard* device = static_cast<WaylandKeyboard*>(data);
  WaylandWindow* window =
    static_cast<WaylandWindow*>(wl_surface_get_user_data(surface));
  seat->SetFocusWindowHandle(window->Handle());
  device->GetDispatcher()->KeyboardEnter(window->Handle());
}

void WaylandKeyboard::OnKeyboardLeave(void* data,
                                      wl_keyboard* input_keyboard,
                                      uint32_t serial,
                                      wl_surface* surface) {
  WaylandDisplay::GetInstance()->SetSerial(serial);

  if (!surface || !data)
    return;

  WaylandSeat* seat = WaylandDisplay::GetInstance()->PrimarySeat();
  WaylandKeyboard* device = static_cast<WaylandKeyboard*>(data);
  WaylandWindow* window =
    static_cast<WaylandWindow*>(wl_surface_get_user_data(surface));
  device->GetDispatcher()->KeyboardLeave(window->Handle());
  seat->SetFocusWindowHandle(0);
}

void WaylandKeyboard::OnKeyModifiers(void *data,
                                     wl_keyboard *keyboard,
                                     uint32_t serial,
                                     uint32_t mods_depressed,
                                     uint32_t mods_latched,
                                     uint32_t mods_locked,
                                     uint32_t group) {
}

}  // namespace ozonewayland
