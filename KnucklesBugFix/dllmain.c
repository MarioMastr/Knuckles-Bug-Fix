#include "GameAPI/Game.h"
#include "LevelSelect.h"

#if RETRO_USE_MOD_LOADER
DLLExport bool32 LinkModLogic(EngineInfo *info, const char *id);
#endif

DEFINE_HOOK_FUNC(LevelSelect_ManagePlayerIcon, void, void) {
    Original_LevelSelect_ManagePlayerIcon();

    RSDK_THIS(LevelSelect);

    self->player1Icon = RSDK_GET_ENTITY(108, UIPicture);
    self->player2Icon = RSDK_GET_ENTITY(109, UIPicture);

    switch (self->leaderCharacterID) {
        case LSELECT_PLAYER_SONIC:
        case LSELECT_PLAYER_TAILS:
        case LSELECT_PLAYER_KNUCKLES: self->player1Icon->animator.frameID = self->leaderCharacterID; break;
    }

    switch (self->sidekickCharacterID) {
        case LSELECT_PLAYER_TAILS: {
            self->player2Icon->animator.frameID = self->sidekickCharacterID;

            // if leader is sonic & sidekick is tails, show ST icon. otherwise remove sidekick
            if (self->leaderCharacterID != LSELECT_PLAYER_SONIC) {
                self->sidekickCharacterID = LSELECT_PLAYER_NONE;
                self->player2Icon->animator.frameID = LSELECT_PLAYER_NONE;
            }
            break;
        }
        default: 
    }
}

void InitModAPI(void)
{
    printf("Hello From Hook Example Mod!\n");
    // Register our hook
    REGISTER_HOOK_FUNC(LevelSelect_ManagePlayerIcon);
}

#if RETRO_USE_MOD_LOADER
#define ADD_PUBLIC_FUNC(func) Mod.AddPublicFunction(#func, (void *)(func))

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