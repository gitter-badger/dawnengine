/*
 * Dawn Engine
 * Written by David Avedissian (c) 2012-2016 (git@davedissian.com)
 */
#pragma once

// Include library prerequisites
#include "Prerequisites.h"

// Determine platform
#include "Platform.h"

// Version Information
#define DW_VERSION_MAJOR 0
#define DW_VERSION_MINOR 2
#define DW_VERSION_PATCH 0
#define DW_VERSION_STR "0.2.0 (Pre-alpha 2)"

namespace dw {

// Primitive types
using uint = unsigned int;
using i8 = std::int8_t;
using u8 = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;
using byte = u8;

// String
using String = std::string;
using StringStream = std::stringstream;
using Path = String;

// Smart pointers
template <class T> using SharedPtr = std::shared_ptr<T>;
template <class T> using WeakPtr = std::weak_ptr<T>;
template <class T> using UniquePtr = std::unique_ptr<T>;
template <class T> using EnableSharedFromThis = std::enable_shared_from_this<T>;
template <class T, class... Args> inline SharedPtr<T> makeShared(Args&&... args) {
#ifdef DW_MSVC
    // MSVC's implementation of make_shared seems to crash in seemingly random scenarios
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
#else
    return std::make_shared<T>(std::forward<Args>(args)...);
#endif
}
template <class T, class... Args> inline UniquePtr<T> makeUnique(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}
template <class T1, class T2> inline SharedPtr<T1> staticPointerCast(const SharedPtr<T2>& other) {
    return std::static_pointer_cast<T1, T2>(other);
}
template <class T1, class T2> inline SharedPtr<T1> dynamicPointerCast(const SharedPtr<T2>& other) {
    return std::dynamic_pointer_cast<T1, T2>(other);
}

// Pair and tuples
template <class T1, class T2> using Pair = std::pair<T1, T2>;
template <class... T> using Tuple = std::tuple<T...>;
template <class T1, class T2> inline Pair<T1, T2> makePair(T1&& a, T2&& b) {
    return std::pair<T1, T2>(std::forward<T1>(a), std::forward<T2>(b));
}
template <class... T> inline Tuple<T...> makeTuple(T&&... args) {
    return std::tuple<T...>(std::forward<T>(args)...);
}

// Containers
template <class T> using Vector = std::vector<T>;
template <class T> using List = std::list<T>;
template <class K, class T> using Map = std::map<K, T>;
template <class K, class T> using HashMap = std::unordered_map<K, T>;

// Threads
using Thread = std::thread;
using Mutex = std::mutex;
using ScopedMutex = std::lock_guard<std::mutex>;

// Stubs
class Camera {};  // TODO(David) stub until ECS is integrated
}

// Safe delete macros
#define SAFE_DELETE(x) \
    if (x) {           \
        delete (x);    \
        x = nullptr;   \
    }

// Error macros
#define ERROR_FATAL(x)               \
    {                                \
        getLog().error(x);           \
        throw std::runtime_error(x); \
    }

// Common includes
#include "core/Context.h"
#include "core/Object.h"
#include "core/Option.h"
#include "core/Log.h"
#include "math/Defs.h"
#include "scene/Position.h"
#include "core/EventSystem.h"  // TODO(David): Event Data pointer only?
