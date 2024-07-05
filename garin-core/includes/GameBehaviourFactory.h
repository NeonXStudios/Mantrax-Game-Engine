#pragma once
#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include <GarinNatives.h>

class GameBehaviour;

class GameBehaviourFactory
{
public:
    using Creator_global = std::function<std::shared_ptr<GameBehaviour>()>;

    void register_class_global(const std::string &class_name, Creator_global creator)
    {
        std::cout << "REGISTRANDO NUEVA CLASE: " << class_name << std::endl;
        creators_global[class_name] = creator;
    }

    std::shared_ptr<GameBehaviour> create_instance_by_name_global(const std::string &class_name)
    {
        try
        {
            auto it = creators_global.find(class_name);
            if (it != creators_global.end())
            {
                return it->second();
            }
            else
            {
                std::cerr << "Class " << class_name << " not found in registry" << std::endl;
                return nullptr;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

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
            std::cout << "Class found creating: " << class_name << std::endl;
            return it->second();
        }
        else
        {
            std::cerr << "Class " << class_name << " not found in registry" << std::endl;
            return nullptr;
        }
    }

    std::vector<std::shared_ptr<GameBehaviour>> create_all()
    {
        try
        {
            std::vector<std::shared_ptr<GameBehaviour>> instances;
            for (const auto &pair : creators)
            {
                instances.push_back(pair.second());
            }
            return instances;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

private:
    std::unordered_map<std::string, Creator> creators;
    std::unordered_map<std::string, Creator_global> creators_global;
};

#define GCLASS(ClassName)                                                                                                                                          \
    namespace                                                                                                                                                      \
    {                                                                                                                                                              \
        const bool registered_##ClassName =                                                                                                                        \
            (GameBehaviourFactory::instance().register_class(#ClassName, []() -> std::unique_ptr<GameBehaviour> { return std::make_unique<ClassName>(); }), true); \
    }

// // Macro para Registrar Clases Derivadas
#define GCLASSGLOBAL(ClassName)                                                                                                                                           \
    namespace                                                                                                                                                             \
    {                                                                                                                                                                     \
        const bool registered_##ClassName =                                                                                                                               \
            (GameBehaviourFactory::instance().register_class_global(#ClassName, []() -> std::shared_ptr<GameBehaviour> { return std::make_shared<ClassName>(); }), true); \
    }
