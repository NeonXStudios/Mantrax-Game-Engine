#include "../includes/FileManager.h"

std::vector<fs::directory_entry> FileManager::get_files_noT(const std::string &path)
{
    std::vector<fs::directory_entry> files;
    try
    {
        for (const auto &entry : fs::directory_iterator(path))
        {
            files.push_back(entry);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error reading directory: " << e.what() << std::endl;
    }
    return files;
}

std::string FileManager::read_file(const std::string &file_name)
{
    try
    {
        std::ifstream file = FileManager::get_file<std::ifstream>(file_name);
        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return "";
    }
}

template <typename T>
T FileManager::get_file(const std::string &path)
{
    if constexpr (std::is_same_v<T, std::ifstream>)
    {
        std::ifstream file(path);
        if (!file)
        {
            throw std::runtime_error("Failed to open file: " + path);
        }
        return std::move(file);
    }
    else
    {
        static_assert(!sizeof(T), "Unsupported type for get_file");
    }
}

void FileManager::write_file(const std::string &file_name, const std::string &content)
{
    std::ofstream myfile(file_name);

    if (myfile.is_open())
    {
        myfile << content;

        myfile.close();
    }
    else
    {
        std::cout << "No se pudo abrir el archivo: " << file_name << "\n";
    }
}

std::string FileManager::get_execute_path()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    std::string currentPath = std::string(buffer).substr(0, pos);

    std::byte stdByte = std::byte{0xFF};
    return currentPath + "/";
}

std::string FileManager::get_project_path()
{
    return "C:/Proyects/GarinEngine/build/Debug/projects/test/";
}