// Copyright (c) 2020 The Orbit Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <map>
#include <memory>

#include "BlockChain.h"
#include "CallstackTypes.h"
#include "EventTrack.h"
#include "TextBox.h"
#include "Threading.h"
#include "TimerChain.h"
#include "Track.h"

class TextRenderer;

class ThreadTrack : public Track {
 public:
  ThreadTrack(TimeGraph* time_graph, int32_t thread_id);
  ~ThreadTrack() override = default;

  // Pickable
  void Draw(GlCanvas* canvas, bool picking) override;
  void OnDrag(int x, int y) override;
  void OnTimer(const Timer& timer);

  // Track
  void UpdatePrimitives(uint64_t min_tick, uint64_t max_tick) override;
  Type GetType() const override { return kThreadTrack; }
  float GetHeight() const override;

  std::vector<std::shared_ptr<TimerChain>> GetTimers() override;
  uint32_t GetDepth() const { return depth_; }
  std::string GetExtraInfo(const Timer& timer);

  Color GetColor() const;
  static Color GetColor(ThreadID a_TID);
  uint32_t GetNumTimers() const { return num_timers_; }
  TickType GetMinTime() const { return min_time_; }
  TickType GetMaxTime() const { return max_time_; }

  const TextBox* GetFirstAfterTime(TickType time, uint32_t depth) const;
  const TextBox* GetFirstBeforeTime(TickType time, uint32_t depth) const;

  const TextBox* GetLeft(TextBox* textbox) const;
  const TextBox* GetRight(TextBox* textbox) const;
  const TextBox* GetUp(TextBox* textbox) const;
  const TextBox* GetDown(TextBox* textbox) const;

  std::vector<std::shared_ptr<TimerChain>> GetAllChains() override;

  void SetEventTrackColor(Color color);
  bool IsEmpty() const;
  virtual bool HasEventTrack() const { return true; }

  int32_t GetThreadId() const { return thread_id_; }
  bool IsCollapsable() const override { return depth_ > 1; }
  float GetYFromDepth(uint32_t depth);

 protected:
  void UpdateDepth(uint32_t depth) {
    if (depth > depth_) depth_ = depth;
  }
  virtual float GetYFromDepth(float track_y, uint32_t depth, bool collapsed);
  std::shared_ptr<TimerChain> GetTimers(uint32_t depth) const;

 private:
  void SetTimesliceText(const Timer& timer, double elapsed_us, float min_x,
                        TextBox* text_box);

 protected:
  TextRenderer* text_renderer_ = nullptr;
  std::shared_ptr<EventTrack> event_track_;
  uint32_t depth_ = 0;
  ThreadID thread_id_;
  mutable Mutex mutex_;
  std::map<int, std::shared_ptr<TimerChain>> timers_;
};
