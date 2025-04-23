#pragma once
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <stdexcept>
#include <string>
#include <iostream>
#include <Core.h>

class GARINLIBS_API ServiceLocatorException : public std::runtime_error {
public:
    explicit ServiceLocatorException(const std::string& message) : std::runtime_error(message) {}
};

class GARINLIBS_API ServiceLocator {
private:
    using TypeId = std::type_index;
    using ServicePtr = std::shared_ptr<void>;
    
    std::unordered_map<TypeId, ServicePtr> services;
    
    static ServiceLocator& getInstance() {
        static ServiceLocator instance;
        return instance;
    }
    
    ServiceLocator() = default;
    ServiceLocator(const ServiceLocator&) = delete;
    ServiceLocator& operator=(const ServiceLocator&) = delete;

public:
    template<typename T>
    static void provide(std::shared_ptr<T> service) {
        if (!service) {
            throw ServiceLocatorException("Null service provided");
        }
        
        TypeId typeId = std::type_index(typeid(T));
        getInstance().services[typeId] = std::static_pointer_cast<void>(service);
    }
    
    template<typename T, typename... Args>
    static void provide_new(Args&&... args) {
        provide<T>(std::make_shared<T>(std::forward<Args>(args)...));
    }
    
    template<typename T>
    static std::shared_ptr<T> get() {
        TypeId typeId = std::type_index(typeid(T));
        auto& serviceMap = getInstance().services;
        
        auto it = serviceMap.find(typeId);
        if (it == serviceMap.end()) {
            throw ServiceLocatorException("Service not registered: " + std::string(typeid(T).name()));
        }
        
        return std::static_pointer_cast<T>(it->second);
    }
    
    template<typename T>
    static bool is_registered() {
        TypeId typeId = std::type_index(typeid(T));
        auto& serviceMap = getInstance().services;
        return serviceMap.find(typeId) != serviceMap.end();
    }
    
    static void clear() {
        getInstance().services.clear();
    }
    
    static void print_registered_services() {
        auto& serviceMap = getInstance().services;
        std::cout << "Registered services (" << serviceMap.size() << "):\n";
        for (const auto& pair : serviceMap) {
            std::cout << " - " << pair.first.name() << std::endl;
        }
    }
};