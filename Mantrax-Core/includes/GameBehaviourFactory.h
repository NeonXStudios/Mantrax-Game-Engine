#pragma once
#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include <GarinNatives.h>
#include "Core.h"

class GARINLIBS_API GameBehaviour;
class GARINLIBS_API GameBehaviourFactory
{
public:
    using Creator = std::function<std::unique_ptr<GameBehaviour>()>;
    std::unordered_map<std::string, Creator> creators;

    static GameBehaviourFactory &instance()
    {
        static GameBehaviourFactory factory;
        return factory;
    }

    void register_class(const std::string &class_name, Creator creator)
    {
        try
        {
            creators[class_name] = std::move(creator);
            std::cout << "Class Registre: " << class_name << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Trapped exception: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Unknown exception trapped" << std::endl;
        }
    }

    void use_registered_class(const std::string &class_name)
    {
        if (creators.find(class_name) != creators.end())
        {
            std::cout << "Creating instance of: " << class_name << std::endl;
            std::unique_ptr<GameBehaviour> instance = creators[class_name]();
        }
        else
        {
            std::cout << "Error: Class not found: " << class_name << std::endl;
        }
    }

    std::unique_ptr<GameBehaviour> create_instance_by_name(const std::string &class_name)
    {
        auto it = creators.find(class_name);
        if (it != creators.end())
        {
            std::cout << "******** Clase encontrada, creando instancia: " << class_name << std::endl;
            return it->second();
        }
        else
        {
            std::cerr << "Error Clase " << class_name << " no encontrada en el registro" << std::endl;
            return nullptr;
        }
    }

    // std::vector<std::shared_ptr<GameBehaviour>> create_all()
    // {
    //     try
    //     {
    //         std::vector<std::shared_ptr<GameBehaviour>> instances;
    //         for (const auto &pair : creators)
    //         {
    //             instances.push_back(pair.second());
    //         }
    //         return instances;
    //     }
    //     catch (const std::exception &e)
    //     {
    //         std::cerr << e.what() << '\n';
    //     }
    // }

    void reload_components()
    {
        creators.clear();
        std::cout << "Components clearing succefully" << std::endl;
    }

    // std::unordered_map<std::string, Creator_global> creators_global;

    GameBehaviourFactory() = default;

    // No permitir copia ni movimiento
    GameBehaviourFactory(const GameBehaviourFactory &) = delete;
    GameBehaviourFactory &operator=(const GameBehaviourFactory &) = delete;
};

#define GCLASS(ClassName)                                                                                                                                          \
    namespace                                                                                                                                                      \
    {                                                                                                                                                              \
        const bool registered_##ClassName =                                                                                                                        \
            (GameBehaviourFactory::instance().register_class(#ClassName, []() -> std::unique_ptr<GameBehaviour> { return std::make_unique<ClassName>(); }), true); \
    }

// // // Macro para Registrar Clases Derivadas
// #define GCLASSGLOBAL(ClassName)                                                                                                                                           \
//     namespace                                                                                                                                                             \
//     {                                                                                                                                                                     \
//         const bool registered_##ClassName =                                                                                                                               \
//             (GameBehaviourFactory::instance().register_class_global(#ClassName, []() -> std::shared_ptr<GameBehaviour> { return std::make_shared<ClassName>(); }), true); \
//     }
