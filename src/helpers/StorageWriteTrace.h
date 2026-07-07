#pragma once

#include <Arduino.h>

// Set to 1 (here or via -DDEBUG_TRACE_STORAGE_WRITES=1) to log blocking storage
// writes on Serial: BEGIN line at entry, END line with elapsed ms at exit.
#ifndef DEBUG_TRACE_STORAGE_WRITES
#define DEBUG_TRACE_STORAGE_WRITES 0
#endif

#if DEBUG_TRACE_STORAGE_WRITES

class StorageWriteTraceScope {
  char _label[40];
  unsigned long _t0;

 public:
  explicit StorageWriteTraceScope(const char* label) : _t0(millis()) {
    strncpy(_label, label ? label : "?", sizeof _label - 1);
    _label[sizeof _label - 1] = '\0';
    Serial.printf("[STORAGE] BEGIN %s\n", _label);
  }

  StorageWriteTraceScope(const char* label, const char* detail) : _t0(millis()) {
    strncpy(_label, label ? label : "?", sizeof _label - 1);
    _label[sizeof _label - 1] = '\0';
    if (detail && detail[0])
      Serial.printf("[STORAGE] BEGIN %s %s\n", _label, detail);
    else
      Serial.printf("[STORAGE] BEGIN %s\n", _label);
  }

  ~StorageWriteTraceScope() {
    Serial.printf("[STORAGE] END %s %lu ms\n", _label,
                  (unsigned long)(millis() - _t0));
  }
};

#define STORAGE_WRITE_TRACE_SCOPE(label) \
  StorageWriteTraceScope _storage_write_trace_scope(label)

#define STORAGE_WRITE_TRACE_SCOPE2(label, detail) \
  StorageWriteTraceScope _storage_write_trace_scope(label, detail)

#else

struct StorageWriteTraceScope {
  explicit StorageWriteTraceScope(const char*) {}
  StorageWriteTraceScope(const char*, const char*) {}
};

#define STORAGE_WRITE_TRACE_SCOPE(label) \
  StorageWriteTraceScope _storage_write_trace_scope(label)

#define STORAGE_WRITE_TRACE_SCOPE2(label, detail) \
  StorageWriteTraceScope _storage_write_trace_scope(label, detail)

#endif
