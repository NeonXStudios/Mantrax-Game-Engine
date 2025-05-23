#include "../includes/FileManager.h"

std::string FileManager::game_path = "";

bool FileManager::delete_folder(std::string folder_path)
{
    try
    {
        if (fs::exists(folder_path.c_str()))
        {
            fs::remove_all(folder_path.c_str());
            std::cout << "Succefully on try delete folder" << std::endl;
            return true;
        }
        else
        {
            std::cout << "Folder not found.\n";
            return false;
        }
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << "Error on try delete folder: " << e.what() << '\n';
        return false;
    }
}

bool FileManager::check_file_extension(const std::string &path)
{
    fs::path p(path);
    return p.has_extension();
}

bool FileManager::open_file(const std::string &file_path)
{
    try
    {
        fs::path p = fs::path(file_path).lexically_normal();

        if (!fs::exists(p) || !fs::is_regular_file(p))
        {
            std::cerr << "El archivo no existe o no es un archivo válido: " << p << std::endl;
            return false;
        }

#ifdef _WIN32
        std::string comando = "start \"\" \"" + p.string() + "\"";
#elif __APPLE__
        std::string comando = "open \"" + p.string() + "\"";
#elif __linux__
        std::string comando = "xdg-open \"" + p.string() + "\"";
#else
        std::cerr << "Plataforma no soportada.\n";
        return false;
#endif

        int resultado = system(comando.c_str());
        return resultado == 0;
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << "Error on try get file: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::open_explorer(const std::string &folder_path)
{
    try
    {
        bool is_file = check_file_extension(folder_path);

        if (!fs::exists(folder_path) || !fs::is_directory(folder_path))
        {
            std::cerr << "La ruta no existe o no es una carpeta: " << folder_path << std::endl;
            return false;
        }

        fs::path clean_path = fs::path(folder_path).lexically_normal();
        std::string normalized_path = clean_path.string();

#ifdef _WIN32
        std::string command = "explorer \"" + normalized_path + "\"";
#elif __linux__
        std::string command = "xdg-open \"" + normalized_path + "\"";
#elif __APPLE__
        std::string command = "open \"" + normalized_path + "\"";
#else
        std::cerr << "Plataforma no soportada para abrir el explorador.\n";
        return false;
#endif

        int result = system(command.c_str());
        return result == 0;
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << "Error al acceder a la carpeta: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::make_dir(const std::string &path)
{
    try
    {
        if (fs::create_directories(path))
        {
            std::cout << "Directory created successfully.\n";
            return true;
        }
        else
        {
            std::cout << "Directory already exists or could not be created.\n";
            return false;
        }
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << "Error creating directory: " << e.what() << '\n';
        return false;
    }
}

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

bool FileManager::check_file_if_exist(const std::string &path)
{
    return std::filesystem::exists(path);
}

FileData FileManager::get_file_info(std::string file_path)
{
    std::filesystem::path path(file_path);

    FileData data;
    data.file_name = path.filename().string();
    data.file_stem = path.stem().string();
    data.file_extension = path.extension().string();

    return data;
}

std::vector<std::string> FileManager::get_files_by_extension(std::string dir_to_it, std::string file_extension)
{
    std::vector<std::string> files;

    try
    {
        for (const auto &entry : std::filesystem::recursive_directory_iterator(dir_to_it, std::filesystem::directory_options::skip_permission_denied))
        {
            if (entry.is_regular_file())
            {
                std::string filename = entry.path().filename().string();

                if (!filename.empty() && filename[0] != '.' && entry.path().extension() == file_extension)
                {
                    files.push_back(entry.path().string());
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return files;
}