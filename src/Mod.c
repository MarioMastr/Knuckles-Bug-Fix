#include "Game.h"
#include "All.h"

#if RETRO_USE_MOD_LOADER
DLLExport bool32 LinkModLogic(EngineInfo *info, const char *id);

#define ADD_PUBLIC_FUNC(func) Mod.AddPublicFunction(#func, (void *)(func))
#endif

void InitModAPI(void) {
    MOD_REGISTER_OBJECT_HOOK(LevelSelect);
    MOD_REGISTER_OBJECT_HOOK(UIText);
    MOD_REGISTER_OBJECT_HOOK(UIPicture);

    REGISTER_FUNC_HOOK(LevelSelect_ManagePlayerIcon);
}

#if RETRO_USE_MOD_LOADER
void InitModAPI(void);

bool32 LinkModLogic(EngineInfo *info, const char *id)
{
#if MANIA_USE_PLUS
    LinkGameLogicDLL(info);
#else
    LinkGameLogicDLL(*info);
#endif

    globals = Mod.GetGlobals();

    modID = id;

    InitModAPI();

    return true;
}
#endif
