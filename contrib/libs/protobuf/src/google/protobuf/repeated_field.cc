// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#include <google/protobuf/repeated_field.h>

#include <algorithm>

#include <google/protobuf/stubs/logging.h>
#include <google/protobuf/stubs/common.h>

// Must be included last.
#include <google/protobuf/port_def.inc>

namespace google {
namespace protobuf {


template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<bool>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<arc_i32>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<arc_ui32>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<arc_i64>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<arc_ui64>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<float>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedField<double>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedPtrField<TProtoStringType>;

namespace internal {
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedIterator<bool>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedIterator<arc_i32>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedIterator<arc_ui32>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedIterator<arc_i64>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedIterator<arc_ui64>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedIterator<float>;
template class PROTOBUF_EXPORT_TEMPLATE_DEFINE RepeatedIterator<double>;
}  // namespace internal

}  // namespace protobuf
}  // namespace google

#include <google/protobuf/port_undef.inc>
