#pragma once
#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include <GarinNatives.h>

class GameBehaviour;

class GameBehaviourFactory
{
public:
    // using Creator = std::function<std::shared_ptr<GameBehaviour>()>;

    // static GameBehaviourFactory &instance()
    // {
    //     static GameBehaviourFactory factory;
    //     return factory;
    // }

    // void register_class(const std::string &class_name, Creator creator)
    // {
    //     std::cout << "REGISTRANDO NUEVA CLASE: " << class_name << std::endl;
    //     creators[class_name] = creator;
    // }

    // std::unique_ptr<GameBehaviour> create_instance_by_name(const std::string &class_name)
    // {
    //     auto it = creators.find(class_name);
    //     if (it != creators.end())
    //     {
    //         return it->second();
    //     }
    //     else
    //     {
    //         std::cerr << "Class " << class_name << " not found in registry" << std::endl;
    //         return nullptr;
    //     }
    // }

    using Creator = std::function<std::unique_ptr<GameBehaviour>()>;

    static GameBehaviourFactory &instance()
    {
        static GameBehaviourFactory factory;
        return factory;
    }

    void register_class(const std::string &class_name, Creator creator)
    {
        std::cout << "REGISTRANDO NUEVA CLASE: " << class_name << std::endl;
        creators[class_name] = std::move(creator);
    }

    std::unique_ptr<GameBehaviour> create_instance_by_name(const std::string &class_name)
    {
        auto it = creators.find(class_name);
        if (it != creators.end())
        {
            return it->second();
        }
        else
        {
            std::cerr << "Class " << class_name << " not found in registry" << std::endl;
            return nullptr;
        }
    }

    // std::vector<std::shared_ptr<GameBehaviour>> create_all()
    // {
    //     std::vector<std::shared_ptr<GameBehaviour>> instances;
    //     for (const auto &pair : creators)
    //     {
    //         instances.push_back(pair.second());
    //     }
    //     return instances;
    // }

private:
    std::unordered_map<std::string, Creator> creators;
};

#define GCLASS(ClassName)                                                                                                                                          \
    namespace                                                                                                                                                      \
    {                                                                                                                                                              \
        const bool registered_##ClassName =                                                                                                                        \
            (GameBehaviourFactory::instance().register_class(#ClassName, []() -> std::unique_ptr<GameBehaviour> { return std::make_unique<ClassName>(); }), true); \
    }

// // Macro para Registrar Clases Derivadas
// #define GARINCLASS(ClassName)                                                                                                                                      \
//     namespace                                                                                                                                                      \
//     {                                                                                                                                                              \
//         const bool registered_##ClassName =                                                                                                                        \
//             (GameBehaviourFactory::instance().register_class(#ClassName, []() -> std::shared_ptr<GameBehaviour> { return std::make_shared<ClassName>(); }), true); \
//     }
