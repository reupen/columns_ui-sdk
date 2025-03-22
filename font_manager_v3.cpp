#include "ui_extension.h"

namespace cui::fonts {

#if CUI_SDK_DWRITE_ENABLED
const GUID font::class_guid{0x5e59f5c4, 0x2000, 0x4c0b, {0x94, 0x3c, 0x5d, 0x69, 0x55, 0xbc, 0x3e, 0x30}};
const GUID manager_v3::class_guid{0x16c17842, 0x42cb, 0x47a0, {0xba, 0x96, 0x68, 0x68, 0x78, 0x65, 0xa2, 0xa6}};
#endif

} // namespace cui::fonts
