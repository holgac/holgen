// This file is partially generated by holgen. Only modify the custom sections.
#include "../gen/Character.h"
// HOLGEN_USER_DEFINED_BEGIN:Character_CustomIncludes
// HOLGEN_USER_DEFINED_END:Character_CustomIncludes

namespace ex2_schemas {
void Character::Initialize() {
// HOLGEN_USER_DEFINED_BEGIN:Character_Initialize
  static uint32_t idCounter = 0;
  mId = idCounter++;
// HOLGEN_USER_DEFINED_END:Character_Initialize
}

}
