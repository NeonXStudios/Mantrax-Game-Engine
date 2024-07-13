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
            std::cout << "Antes de mover el creator" << std::endl;
            creators[class_name] = std::move(creator);
            std::cout << "Después de mover el creator" << std::endl;

            // Verificación de inserción
            if (creators.find(class_name) != creators.end())
            {
                std::cout << "Creator insertado en el mapa" << std::endl;
            }
            else
            {
                std::cout << "Error: Creator no se pudo insertar en el mapa" << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Excepción atrapada: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Excepción desconocida atrapada" << std::endl;
        }
    }

    void use_registered_class(const std::string &class_name)
    {
        if (creators.find(class_name) != creators.end())
        {
            std::cout << "Creando instancia de: " << class_name << std::endl;
            std::unique_ptr<GameBehaviour> instance = creators[class_name]();
        }
        else
        {
            std::cout << "Error: Clase no encontrada: " << class_name << std::endl;
        }
    }

    std::unique_ptr<GameBehaviour> create_instance_by_name(const std::string &class_name)
    {
        for (const auto &pair : creators)
        {
            std::cout << "CLASS NAME DATA: " << pair.first << std::endl;
        }

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
