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
            std::string register_core = FileManager::get_project_path() + "cpplibs/GarinGameCore.cpp";

            std::cout << "Verificando rutas:" << std::endl;
            std::cout << "Directory path: " << directoryPath << " exists: " << std::filesystem::exists(directoryPath) << std::endl;
            std::cout << "Register core path: " << register_core << " exists: " << std::filesystem::exists(register_core) << std::endl;


            std::ofstream clearFile(outputFilePath, std::ofstream::out | std::ofstream::trunc);
            clearFile.close();

            process_header_files(directoryPath, outputFilePath);

            std::vector<std::string> classNames;
            for (const auto& entry : fs::directory_iterator(directoryPath))
            {
                if (entry.path().extension() == ".h")
                {
                    std::cout << "Found class: " << entry.path().filename().stem().string() << std::endl;
                    classNames.push_back(entry.path().filename().stem().string());
                }
            }

            if (!classNames.empty())
            {
                modify_register_block(register_core, classNames);
                std::cout << "Updated register block with " << classNames.size() << " classes" << std::endl;
            }

            UINotification::AddNotification("Recompiling libraries...", 10.0f);

            std::string cmake_path = FileManager::get_project_path() + "wlibsgpp/";
            std::string cmake_command = "cd /d " + cmake_path + " && if not exist bin mkdir bin && cd bin && cmake ..";
            std::string msbuild_command = "cd /d " + cmake_path + " && cd bin && msbuild MantraxRuntimeCore.sln /p:Configuration=Debug";

            run_command(cmake_command, "CMake Reloaded...");
            run_command(msbuild_command, "Successfully compiled libraries...");
        }
        catch (const std::exception& e)
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
                buffer << "GCLASSDLL(" << lineToInsert << ");" << std::endl;
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

    static void modify_register_block(const std::string& filePath, const std::vector<std::string>& classNames) {
        // Imprimir información de diagnóstico
        std::cout << "Intentando abrir archivo: " << filePath << std::endl;

        // Verificar si el archivo existe
        if (!std::filesystem::exists(filePath)) {
            std::cerr << "Error: El archivo no existe en la ruta especificada: " << filePath << std::endl;
            return;
        }

        // Leer el archivo
        std::ifstream inFile(filePath);
        std::stringstream buffer;

        if (!inFile.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo para lectura: " << filePath << std::endl;
            std::cerr << "Errno: " << std::strerror(errno) << std::endl;
            return;
        }

        std::cout << "Archivo abierto exitosamente para lectura" << std::endl;

        std::string line;
        bool insideBlock = false;
        bool blockFound = false;
        int linesRead = 0;

        // Procesar el archivo línea por línea
        while (std::getline(inFile, line)) {
            linesRead++;
            if (line.find("///// INYECT_HERE") != std::string::npos) {
                std::cout << "Marcador de inicio encontrado en línea " << linesRead << std::endl;
                insideBlock = true;
                blockFound = true;
                buffer << line << std::endl;

                // Insertar las nuevas clases
                for (const auto& className : classNames) {
                    buffer << "    GCLASSDLL(" << className << ");" << std::endl;
                }

                buffer << "    std::cout << \"GClasses Inyected\" << std::endl;" << std::endl;
                continue;
            }

            if (line.find("//// FINAL INYECTION") != std::string::npos) {
                std::cout << "Marcador final encontrado en línea " << linesRead << std::endl;
                insideBlock = false;
                buffer << line << std::endl;
                continue;
            }

            if (!insideBlock) {
                buffer << line << std::endl;
            }
        }

        inFile.close();

        if (!blockFound) {
            std::cerr << "Error: No se encontró el bloque de inyección después de leer " << linesRead << " líneas." << std::endl;
            return;
        }

        std::cout << "Intentando abrir archivo para escritura..." << std::endl;

        // Escribir el contenido modificado de vuelta al archivo
        std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
        if (!outFile.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo para escritura: " << filePath << std::endl;
            std::cerr << "Errno: " << std::strerror(errno) << std::endl;
            return;
        }

        outFile << buffer.str();
        outFile.close();

        std::cout << "Bloque de registro modificado exitosamente." << std::endl;
        std::cout << "Clases inyectadas: " << classNames.size() << std::endl;
        for (const auto& className : classNames) {
            std::cout << "  - " << className << std::endl;
        }
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
