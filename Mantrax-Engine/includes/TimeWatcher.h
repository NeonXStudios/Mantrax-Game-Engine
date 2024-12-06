#pragma once
#include <AppSettings.h>
#include <Core.h>
#include <FileManager.h>
#include <GarinNatives.h>
#include <DynamicLibLoader.h>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <mutex>

namespace fs = std::filesystem;

class GARINLIBS_API TimeWatcher
{
public:
    std::atomic<bool> recompiling{false};
    std::mutex recompilation_mutex;

    void check_files_in_directory(const std::string &dir_path, std::function<void()> action)
    {
        static std::map<std::string, std::filesystem::file_time_type> file_times;

        try
        {
            if (!fs::exists(dir_path) || !fs::is_directory(dir_path))
            {
                std::cerr << "Directory not valid or not exist: " << dir_path << std::endl;
                return;
            }

            for (const auto &entry : fs::directory_iterator(dir_path))
            {
                if (!fs::is_regular_file(entry.status()))
                    continue;

                const auto &path = entry.path().string();
                auto current_time = fs::last_write_time(entry);

                if (file_times.find(path) == file_times.end())
                {
                    file_times[path] = current_time;
                    continue;
                }

                if (file_times[path] != current_time)
                {
                    std::lock_guard<std::mutex> lock(recompilation_mutex);
                    if (!recompiling)
                    {
                        file_times[path] = current_time;

                        action();

                        recompiling = true;
                        recompile_scripts();
                        recompiling = false;
                    }
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error on verify: " << e.what() << '\n';
        }
    }

    void start_file_watcher(const std::string &dir_path, std::function<void()> action)
    {
        check_files_in_directory(dir_path, action);
    }

    void process_header_files(const std::string &directoryPath, const std::string &outputFilePath)
    {
        std::ofstream outputFile(outputFilePath);

        for (const auto &entry : fs::directory_iterator(directoryPath))
        {
            if (entry.path().extension() == ".h")
            {
                outputFile << "#include <" << entry.path().filename().string() << ">" << std::endl;
            }
        }

        outputFile.close();
    }

    void insert_register(const std::string &filePath, const std::string &lineToInsert)
    {
        std::ifstream inFile(filePath);
        std::stringstream buffer;

        if (!inFile.is_open())
        {
            std::cerr << "Error: No se pudo abrir el archivo de entrada." << std::endl;
            return;
        }

        // Leer el contenido del archivo y buscar las líneas de inserción
        std::string line;
        bool foundFinalRegisterData = false;

        while (std::getline(inFile, line))
        {
            if (line.find("/////////_FINALREGISTERDATA_/////////") != std::string::npos)
            {
                buffer << "GCLASSDLL(" << lineToInsert << ");" << std::endl; // Insertar las líneas nuevas antes de la marca final
                foundFinalRegisterData = true;
            }
            buffer << line << std::endl;
        }

        inFile.close();

        if (!foundFinalRegisterData)
        {
            std::cerr << "Error: No se encontró la marca final en el archivo." << std::endl;
            return;
        }

        // Escribir el contenido modificado de vuelta al archivo
        std::ofstream outFile(filePath);
        if (!outFile.is_open())
        {
            std::cerr << "Error: No se pudo abrir el archivo de salida para escritura." << std::endl;
            return;
        }

        outFile << buffer.str();
        outFile.close();

        std::cout << "Se agregó la línea correctamente antes de FINALREGISTERDATA." << std::endl;
    }

    void recompile_scripts()
    {
        std::string directoryPath = FileManager::get_project_path() + "clscpp/";
        std::string outputFilePath = FileManager::get_project_path() + "wlibsgpp/GameLibsLoader.h";

        std::ofstream clearFile(outputFilePath, std::ofstream::out | std::ofstream::trunc);
        clearFile.close();

        process_header_files(directoryPath, outputFilePath);

        std::string register_core = FileManager::get_project_path() + "wlibsgpp/GarinGameCore.cpp";
        for (const auto &entry : fs::directory_iterator(directoryPath))
        {
            if (entry.path().extension() == ".h")
            {
                std::cout << "MAKING H CLASS GETTER: " << entry.path().filename().stem().string() << std::endl;
                insert_register(register_core, entry.path().filename().stem().string());
            }
        }

        UINotification::AddNotification("Recompiling libraries...", 10.0f);

        // GameBehaviourFactory::instance().reload_components();

        UINotification::AddNotification("Compiling libraries (wait for it)...", 10.0f);

        std::string cmake_path = FileManager::get_project_path() + "wlibsgpp/";

        std::string cmake_command =
            "cd /d " + cmake_path +
            " && if not exist bin mkdir bin && cd bin && cmake ..";

        std::string msbuild_command = "cd /d " + cmake_path + " && cd bin && msbuild MantraxRuntimeCore.sln /p:Configuration=Debug";

        int result = system(cmake_command.c_str());

        if (result == 0)
        {
            UINotification::AddNotification("CMake Reloaded...", 10.0f);
            UINotification::AddNotification("Starting compilation of the libraries (Wait)...", 10.0f);

            int result_build = system(msbuild_command.c_str());

            if (result_build == 0)
            {
                UINotification::AddNotification("Successfully compiled libraries...", 10.0f);
            }
            else
            {
                UINotification::AddNotification("Error during the compilation of the libraries...", 10.0f);
                std::cerr << "Error during msbuild execution, result code: " << result_build << std::endl;
                recompiling = false;
            }
        }
        else
        {
            UINotification::AddNotification("Error during CMake execution...", 10.0f);
            std::cerr << "Error during cmake execution, result code: " << result << std::endl;
            recompiling = false;
        }

        recompiling = false;
    }
};