#pragma once

#pragma once

#include <memory>

#ifdef DEBUG
#define ENABLE_ASSERTS
#endif

#ifdef ENABLE_ASSERTS
#define CORE_ASSERT(x, ...) { if(!(x)) { CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define THROW_CORE_ERROR(...) {CORE_ERROR("Runtime error: {0}", __VA_ARGS__); std::abort();}

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
