#include "../includes/GAnimator.h"

void GAnimator::defines()
{
    GVAR(AnimatorPath, "SETUP ANIMATOR PATH", std::string);
    GVAR(SelectState, "NONE", std::string);
}

void GAnimator::init()
{
    std::string load_path = FileManager::get_game_path() + GETVAR(AnimatorPath, std::string);
    std::string file_content = FileManager::read_file(load_path);

    nlohmann::json animations_tree;
    try
    {
        animations_tree = nlohmann::json::parse(file_content);
    }
    catch (const nlohmann::json::parse_error &e)
    {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return;
    }

    if (animations_tree.contains("animator_name"))
    {
        _name = animations_tree["animator_name"].get<std::string>();
    }

    if (animations_tree.contains("animations_data"))
    {
        animations.clear();

        for (const auto &animation_info : animations_tree["animations_data"])
        {
            GAnimator::Animation anim;

            anim.name = animation_info.value("name_file", "");
            anim.loop = animation_info.value("loop", false);

            if (animation_info.contains("animations_frames"))
            {
                for (const auto &frame_data : animation_info["animations_frames"])
                {
                    GAnimator::Frame frame;

                    frame.imagePath = frame_data.value("file_path", "");
                    frame.duration = frame_data.value("frame_duration", 0.1f);

                    frame.process_texture();

                    anim.frames.push_back(frame);
                }
            }

            animations.push_back(anim);
        }

        std::cout << "Animator data loaded successfully from " << load_path << std::endl;
    }

    std::thread t([this]()
                  { next_frame(); });

    t.detach();
}

void GAnimator::update()
{
}

void GAnimator::clean()
{
    pause = true;
}

void GAnimator::next_frame()
{
    if (!pause)
    {
        int current_frame = 0;
        float wait_time = 0.1f;

        while (animations.size() > 0)
        {

            for (Animation &stateFound : animations)
            {

                if (stateFound.name == GETVAR(SelectState, std::string))
                {
                    if (entity->hasComponent<ModelComponent>() && stateFound.frames.size() > 0)
                    {
                        entity->getComponent<ModelComponent>().texture_sampler->set_texture(stateFound.frames[current_frame].texture_loaded->get_texture());
                        wait_time = stateFound.frames[current_frame].duration;
                    }
                }

                current_frame++;

                if (current_frame >= stateFound.frames.size())
                {
                    current_frame = 0;
                }
            }

            if (wait_time <= 0.0f)
            {
                wait_time = 0.1;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(wait_time * 1000)));
        }
    }
}