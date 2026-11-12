#include "LevelSelect.h"

ObjectLevelSelect *LevelSelect;
ObjectUIPicture *UIPicture;
ObjectUIText *UIText;

DEFINE_GAME_FUNC_HOOK("LevelSelect_ManagePlayerIcon", LevelSelect_ManagePlayerIcon, void) {
    RSDK_THIS(LevelSelect);

    self->player1Icon = RSDK_GET_ENTITY(108, UIPicture);
    self->player2Icon = RSDK_GET_ENTITY(109, UIPicture);

    switch (self->leaderCharacterID) {
        case LSELECT_PLAYER_SONIC:
        case LSELECT_PLAYER_TAILS:
        case LSELECT_PLAYER_KNUCKLES: self->player1Icon->animator.frameID = self->leaderCharacterID; break;

#if MANIA_USE_PLUS
        case LSELECT_PLAYER_MIGHTY:
        case LSELECT_PLAYER_RAY:
            if (!API.CheckDLC(DLC_PLUS))
                self->leaderCharacterID = LSELECT_PLAYER_SONIC;

            self->player1Icon->animator.frameID = self->leaderCharacterID;
            break;
#endif

        default:
            self->leaderCharacterID   = LSELECT_PLAYER_SONIC;
            self->player1Icon->animator.frameID = LSELECT_PLAYER_SONIC;
            break;
    }

    switch (self->sidekickCharacterID) {
        case LSELECT_PLAYER_TAILS:
            self->player2Icon->animator.frameID = self->sidekickCharacterID;

            // if leader is sonic & sidekick is tails, show ST icon. otherwise remove sidekick
            if (self->leaderCharacterID != LSELECT_PLAYER_SONIC) {
                self->sidekickCharacterID = LSELECT_PLAYER_NONE;
                self->player2Icon->animator.frameID = LSELECT_PLAYER_NONE;
            }
            break;

        default:
            self->sidekickCharacterID = LSELECT_PLAYER_NONE;
            self->player2Icon->animator.frameID = LSELECT_PLAYER_NONE;
            break;

        // if P2 is sonic, no he's not thats tails actually
        case LSELECT_PLAYER_SONIC:
            self->sidekickCharacterID = LSELECT_PLAYER_TAILS;
            self->player2Icon->animator.frameID = LSELECT_PLAYER_TAILS;

            // if leader is sonic & sidekick is tails, show ST icon. otherwise remove sidekick
            if (self->leaderCharacterID != LSELECT_PLAYER_SONIC) {
                self->sidekickCharacterID = LSELECT_PLAYER_NONE;
                self->player2Icon->animator.frameID = LSELECT_PLAYER_NONE;
            }
            break;
    }
}
