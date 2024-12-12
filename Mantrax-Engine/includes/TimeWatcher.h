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
#include <Ensambler.h>

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

    void recompile_scripts()
    {
        std::thread compile_thread(Ensambler::start_scripts_reload);
        compile_thread.detach();
    }
};