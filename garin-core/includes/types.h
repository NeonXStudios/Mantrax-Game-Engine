#pragma once

#include <functional>

template <typename T>
using Function = std::function<T>;

#include <memory>

template <typename T>
using Shared = std::shared_ptr<T>;

template <typename T>
using Weak = std::weak_ptr<T>;

template <typename T, typename... Args>
inline Shared<T> new_shared(Args &&...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
using Unique = std::unique_ptr<T>;

template <typename T, typename... Args>
inline Unique<T> new_unique(Args &&...args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ptr = T *;

template <typename T>
using Ref = T &;

#include <optional>

template <typename T>
using Optional = std::optional<T>;

typedef size_t typeid_t;

class TypeIDCounter
{
public:
    static size_t value;
};

template <typename T>
class TypeID
{
public:
    inline static size_t value()
    {
        static size_t id = TypeIDCounter::value++;
        return id;
    }
};