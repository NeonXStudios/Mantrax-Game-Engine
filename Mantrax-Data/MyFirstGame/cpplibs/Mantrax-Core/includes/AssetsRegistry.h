#pragma once
#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

class AssetsRegistry
{
public:
    std::string directoryPath;

    AssetsRegistry(const std::string &directoryPath, int intervalSeconds)
        : directoryPath(directoryPath), interval(intervalSeconds), stopProcessing(false) {}

    void start()
    {
        processingThread = std::thread(&AssetsRegistry::periodicFileProcessing, this);
    }

    // Detener el procesamiento de archivos
    void stop()
    {
        stopProcessing = true;
        if (processingThread.joinable())
        {
            processingThread.join();
        }
    }

    ~AssetsRegistry()
    {
        stop();
    }

private:
    int interval;
    std::mutex fileMutex;
    std::thread processingThread;
    bool stopProcessing;

    // Generar un ID aleatorio para el campo Type
    std::string generateRandomID()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(100000, 999999);
        return std::to_string(distrib(gen));
    }

    // Crear un archivo .garin con la estructura JSON
    void createGarinFile(const fs::path &filePath)
    {
        std::string fileName = filePath.stem().string();
        std::string extension = filePath.extension().string();

        fs::path garinPath = filePath.parent_path() / (fileName + ".garin");

        // Verificar si el archivo .garin ya existe
        if (fs::exists(garinPath))
        {
            return; // No hacer nada si ya existe
        }

        json garinFile = {
            {"ID", generateRandomID()},
            {"Type", extension},
            {"Name", fileName}};

        std::lock_guard<std::mutex> lock(fileMutex);
        std::ofstream outFile(garinPath);
        if (outFile.is_open())
        {
            outFile << garinFile.dump(4);
            outFile.close();
        }
        else
        {
            std::cerr << "Error on create meta file: " << garinPath << std::endl;
        }
    }

    // Procesar archivos en la carpeta de forma recursiva
    void processFilesInDirectory(const fs::path &dirPath)
    {
        std::vector<std::thread> threads;

        for (const auto &entry : fs::recursive_directory_iterator(dirPath))
        {
            if (entry.is_regular_file())
            {
                threads.emplace_back(&AssetsRegistry::createGarinFile, this, entry.path());
            }
        }

        for (auto &t : threads)
        {
            if (t.joinable())
            {
                t.join();
            }
        }
    }

    void periodicFileProcessing()
    {
        while (!stopProcessing)
        {
            processFilesInDirectory(directoryPath);
            std::this_thread::sleep_for(std::chrono::seconds(interval));
        }
    }
};
