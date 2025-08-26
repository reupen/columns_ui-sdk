#include "ui_extension.h"

namespace uie::utils {

std::string remap_category(const char* category)
{
    if (!stricmp_utf8(category, "splitters"))
        return "Containers";

    if (!stricmp_utf8(category, "playlist views"))
        return "Playlist views";

    return category;
}

std::string get_remapped_category(const window::ptr& panel)
{
    pfc::string8 category;
    panel->get_category(category);

    return remap_category(category);
}

} // namespace uie::utils
