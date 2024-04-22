#include <GameAPI/Game.h>
#include "Objects/Menu/LevelSelect.h"

#if RETRO_USE_MOD_LOADER
DLLExport bool32 LinkModLogic(EngineInfo *info, const char *id);

#define ADD_PUBLIC_FUNC(func) Mod.AddPublicFunction(#func, (void *)(func))
#endif

void InitModAPI(void) {
    MOD_REGISTER_OBJECT_HOOK(LevelSelect);
    MOD_REGISTER_OBJECT_HOOK(UIText);
    MOD_REGISTER_OBJECT_HOOK(UIPicture);

    LevelSelect_State_FadeIn        = Mod.GetPublicFunction(NULL, "LevelSelect_State_FadeIn");
    LevelSelect_SetLabelHighlighted = Mod.GetPublicFunction(NULL, "LevelSelect_SetLabelHighlighted");
    HUD_CharacterIndexFromID        = Mod.GetPublicFunction(NULL, "HUD_CharacterIndexFromID");

    Mod.RegisterStateHook(Mod.GetPublicFunction(NULL, "LevelSelect_State_Init"), LevelSelect_State_Init_RP, true);
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
