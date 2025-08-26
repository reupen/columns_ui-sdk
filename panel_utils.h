#pragma once

namespace uie::utils {

/**
 * Remaps panel categories to updated category names used in Columns UI.
 *
 * For example, ‘Splitters’ is mapped to ‘Containers’.
 *
 * @param category Category name to remap
 * @return Remapped category name
 */
std::string remap_category(const char* category);

/**
 * Gets a remapped category name for a window instance.
 */
std::string get_remapped_category(const window::ptr& panel);

} // namespace uie::utils
