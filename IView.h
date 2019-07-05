#pragma once

typedef struct i_view IView;

/** \memberof i_view */
IView* IView_new(
    void* implementationObject,
    void(*destroyOverride)(void* implSelf),
    void(*displayOverride)(void* implSelf),
    void(*hideOverride)(void* implSelf)
);

/** \memberof i_view */
void IView_destroy(IView* self, int destroyDerviedTypes);

/** \memberof i_view */
void IView_display(IView* self);

/** \memberof i_view */
void IView_hide(IView* self);