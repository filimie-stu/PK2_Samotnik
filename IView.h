#pragma once

typedef struct i_view IView;

IView* IView_new(
    void* implementationObject,
    void(*destroyOverride)(void* implSelf),
    void(*displayOverride)(void* implSelf),
    void(*hideOverride)(void* implSelf)
);

void IView_destroy(IView* self, int destroyDerviedTypes);
void IView_display(IView* self);
void IView_hide(IView* self);