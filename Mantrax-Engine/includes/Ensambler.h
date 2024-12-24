#pragma once
#include <GarinNatives.h>
#include <FileManager.h>
#include <UINotification.h>

class Ensambler
{
public:
    static void start_scripts_reload()
    {
        try
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

            std::string cmake_engine = FileManager::get_execute_path() + "data/compilers/CMake/bin/cmake";
            std::string msbuild_engine = FileManager::get_execute_path() + "data/compilers/MSBuild/Current/Bin/MSBuild";

            std::string cmake_path = FileManager::get_project_path() + "wlibsgpp/";
            std::string cmake_command = "cd /d " + cmake_path + " && if not exist bin mkdir bin && cd bin && cmake ..";
            std::string msbuild_command = "cd /d " + cmake_path + " && cd bin && msbuild MantraxRuntimeCore.sln /p:Configuration=Debug";

            run_command(cmake_command, "CMake Reloaded...");
            run_command(msbuild_command, "Successfully compiled libraries...");
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception in compile_libraries: " << e.what() << std::endl;
        }
    }

    static void process_header_files(const std::string &directoryPath, const std::string &outputFilePath)
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

    static void insert_register(const std::string &filePath, const std::string &lineToInsert)
    {
        std::ifstream inFile(filePath);
        std::stringstream buffer;

        if (!inFile.is_open())
        {
            std::cerr << "Error: No se pudo abrir el archivo de entrada." << std::endl;
            return;
        }

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

    static void run_command(const std::string &command, const std::string &notification)
    {
        try
        {
            std::array<char, 128> buffer;
            std::string result;
            FILE *pipe = _popen(command.c_str(), "r");
            if (!pipe)
            {
                throw std::runtime_error("Failed to execute command: " + command);
            }

            while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
            {
                result += buffer.data();
            }

            int return_code = _pclose(pipe);
            if (return_code != 0)
            {
                std::cerr << "Command failed: " << command << "\nExit code: " << return_code << std::endl;
                UINotification::AddNotification("Error: " + notification, 10.0f);
            }
            else
            {
                UINotification::AddNotification(notification, 10.0f);
            }

            std::cout << "Output of [" << command << "]:\n"
                      << result << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception during command execution: " << e.what() << std::endl;
        }
    }
};
