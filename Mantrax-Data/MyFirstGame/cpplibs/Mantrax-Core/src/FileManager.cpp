#include "../includes/FileManager.h"

std::string FileManager::game_path = "";

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

void FileManager::copy_directory_contents(const std::filesystem::path &source, const std::filesystem::path &destination)
{
    try
    {
        if (!std::filesystem::exists(source) || !std::filesystem::is_directory(source))
        {
            std::cerr << "The source directory does not exist or is not a directory.." << std::endl;
            return;
        }

        if (!std::filesystem::exists(destination))
        {
            std::filesystem::create_directories(destination);
        }

        for (const auto &entry : std::filesystem::directory_iterator(source))
        {
            const auto &path = entry.path();                      
            auto destinationPath = destination / path.filename(); 

            if (std::filesystem::is_directory(path))
            {
                copy_directory_contents(path, destinationPath);
            }
            else if (std::filesystem::is_regular_file(path))
            {
                std::filesystem::copy_file(path, destinationPath, std::filesystem::copy_options::overwrite_existing);
            }
            else
            {
                std::cerr << "Tipo de archivo no soportado: " << path << std::endl;
            }
        }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
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
        std::cout << "Unable to open the file: " << file_name << "\n";
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
    return FileManager::game_path + "/";
}

std::string FileManager::get_game_path()
{
    return FileManager::game_path + "/";
}

bool FileManager::check_file_if_exist(const std::string &path)
{
    return std::filesystem::exists(path);
}