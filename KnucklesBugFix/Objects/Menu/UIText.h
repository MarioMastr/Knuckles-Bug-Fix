#ifndef OBJ_UITEXT_H
#define OBJ_UITEXT_H

#include <GameAPI/Game.h>

typedef enum {
    UITEXT_ALIGN_LEFT,
    UITEXT_ALIGN_CENTER,
    UITEXT_ALIGN_RIGHT,
} UITextAlignments;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUIText;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    String text;
    String tag;
    bool32 selectable;
    uint16 listID;
    int32 align;
    int32 data0;
    int32 data1;
    int32 data2;
    int32 data3;
    bool32 highlighted;
    Animator animator;
} EntityUIText;

// Object Struct
extern ObjectUIText *UIText;

#endif //! OBJ_UITEXT_H
