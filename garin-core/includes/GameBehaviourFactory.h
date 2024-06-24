#pragma once
#include <GarinNatives.h>

class GameBehaviour;

class GameBehaviourFactory
{
public:
    using Creator = std::function<std::shared_ptr<GameBehaviour>()>;

    static GameBehaviourFactory &instance()
    {
        static GameBehaviourFactory factory;
        return factory;
    }

    void register_class(const std::string &class_name, Creator creator)
    {
        std::cout << "REGISTRANDO NUEVA CLASE: " << class_name << std::endl;
        creators[class_name] = creator;
    }

    std::vector<std::shared_ptr<GameBehaviour>> create_all()
    {
        std::vector<std::shared_ptr<GameBehaviour>> instances;
        for (const auto &pair : creators)
        {
            instances.push_back(pair.second());
        }
        return instances;
    }

private:
    std::unordered_map<std::string, Creator> creators;
};

// Macro para Registrar Clases Derivadas
#define REGISTER_GAMEBEHAVIOUR_CLASS(ClassName)                                                                                                                    \
    namespace                                                                                                                                                      \
    {                                                                                                                                                              \
        const bool registered_##ClassName =                                                                                                                        \
            (GameBehaviourFactory::instance().register_class(#ClassName, []() -> std::shared_ptr<GameBehaviour> { return std::make_shared<ClassName>(); }), true); \
    }
