#include "ui_extension.h"

namespace cui::fonts {

#if CUI_SDK_DWRITE_ENABLED
const GUID rendering_options::class_guid{0x0ee8a35c, 0xda58, 0x4d1d, {0xb8, 0xae, 0x20, 0x2c, 0xe2, 0xf4, 0x39, 0xd6}};
const GUID font::class_guid{0x361eb5b3, 0xac98, 0x4ead, {0xa0, 0xb1, 0x39, 0x4b, 0x7e, 0x44, 0x92, 0x98}};
const GUID manager_v3::class_guid{0x7f24c59f, 0x4e4b, 0x4f59, {0x9a, 0xf6, 0xc9, 0xe9, 0x13, 0x8d, 0x97, 0x14}};
#endif

} // namespace cui::fonts
