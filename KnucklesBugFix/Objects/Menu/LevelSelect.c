#include "LevelSelect.h"
#include "Objects/Menu/UIPicture.h"
#include "Objects/Menu/UIText.h"

ObjectLevelSelect *LevelSelect;
ObjectUIPicture *UIPicture;
ObjectUIText *UIText;

void (*LevelSelect_State_FadeIn)(void);
void (*LevelSelect_SetLabelHighlighted)(bool32 highlight);
int32 (*HUD_CharacterIndexFromID)(int32 characterID);

bool32 LevelSelect_State_Init_RP(bool32 skippedState) {
    RSDK_THIS(LevelSelect);

    int32 labelPos[32];
    int32 lastY = 0;
    foreach_all(UIText, labelPosPtrL)
    {
        if (labelPosPtrL->position.x < 0x1000000 && labelPosPtrL->position.y > lastY) {
            lastY                        = labelPosPtrL->position.y;
            labelPos[self->labelCount++] = labelPosPtrL->position.y;
        }
    }

    foreach_all(UIText, labelL)
    {
        if (labelL->position.x < 0x1000000) {
            for (int32 i = 0; i < self->labelCount; ++i) {
                if (labelL->position.y == labelPos[i]) {
                    switch (labelL->align) {
                        case UITEXT_ALIGN_LEFT: self->zoneNameLabels[i] = labelL; break;

                        default:
                        case UITEXT_ALIGN_CENTER: break;

                        case UITEXT_ALIGN_RIGHT: self->stageIDLabels[i] = labelL; break;
                    }
                }
            }
        }
    }

    if (self->labelCount > 0)
        memset(labelPos, 0, sizeof(int32) * self->labelCount);

    lastY = 0;

    foreach_all(UIText, labelPosPtrR)
    {
        if (labelPosPtrR->position.x > 0x1000000 && labelPosPtrR->position.y > lastY) {
            lastY                        = labelPosPtrR->position.y;
            labelPos[self->labelCount++] = labelPosPtrR->position.y;
        }
    }

    foreach_all(UIText, labelR)
    {
        if (labelR->position.x > 0x1000000 && self->labelCount > 0) {
            for (int32 i = 0; i < self->labelCount; ++i) {
                if (labelR->position.y == labelPos[i]) {
                    switch (labelR->align) {
                        case UITEXT_ALIGN_LEFT: self->zoneNameLabels[i] = labelR; break;

                        default:
                        case UITEXT_ALIGN_CENTER: break;

                        case UITEXT_ALIGN_RIGHT: self->stageIDLabels[i] = labelR;
#if MANIA_USE_PLUS
                            if (!labelR->data0 && labelR->data1 == 15)
                                self->pinballLabel = labelR;
#endif
                            break;
                    }
                }
            }
        }
    }

    for (int32 i = 0; i < self->labelCount; ++i) {
        if (!self->zoneNameLabels[i]) {
            for (int32 v = i; v >= 0; --v) {
                if (self->zoneNameLabels[v]) {
                    self->zoneNameLabels[i] = self->zoneNameLabels[v];
                    break;
                }
            }
        }
    }

    LevelSelect_SetLabelHighlighted(true);

    foreach_all(UIText, soundTestLabel)
    {
        if (soundTestLabel->align == UITEXT_ALIGN_CENTER) {
            self->soundTestLabel  = soundTestLabel;
            soundTestLabel->align = UITEXT_ALIGN_LEFT;
        }
    }

    self->leaderCharacterID   = HUD_CharacterIndexFromID(GET_CHARACTER_ID(1)) + 1;
    self->sidekickCharacterID = HUD_CharacterIndexFromID(GET_CHARACTER_ID(2)) + 1;

    foreach_all(UIPicture, picture)
    {
        int32 slot = RSDK.GetEntitySlot(picture);
        RSDK.PrintInt32(PRINT_NORMAL, "SLOT", slot);

        if (picture->listID == 1) {
            self->zoneIcon = picture;

            // Bug Details(?):
            // frameID is equal to... playerID...?
            // this feels like a slight oversight, though idk what it is meant to be
            picture->animator.frameID = self->leaderCharacterID;
        }
        else if (picture->listID == 3) {
            if (picture->frameID)
                self->player2Icon = picture;
            else
                self->player1Icon = picture;
        }
    }

    self->stageIDLabels[self->labelCount - 1] = self->soundTestLabel;

    LevelSelect_ManagePlayerIcon_RP();

    self->state = LevelSelect_State_FadeIn;

    return true;
}

void LevelSelect_ManagePlayerIcon_RP(void) {
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