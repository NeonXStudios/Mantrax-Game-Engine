#pragma once
#include <GarinNatives.h>
#include <Core.h>
#include <GMaterial.h>

class GARINLIBS_API MaterialService
{
public:
    std::vector<GMaterial*> materials = std::vector<GMaterial*>();

    void start_services();
    void delete_material(GMaterial* p_reference);
    GMaterial* get_material(int id);
    GMaterial* add_new_material();
};