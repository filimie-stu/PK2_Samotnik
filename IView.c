#include "IView.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct i_view
{
    void *implementationObject;
    void (*destroyOverride)(void *implSelf);
    void (*displayOverride)(void *implSelf);
    void (*hideOverride)(void *implSelf);
} IView;

IView *IView_new(
    void *implementationObject,
    void (*destroyOverride)(void *implSelf),
    void (*displayOverride)(void *implSelf),
    void (*hideOverride)(void *implSelf))
{
    if (!implementationObject || !destroyOverride || !displayOverride || !hideOverride)
    {
        printf("Error: NULL passed as interface override.\n");
    }

    IView *created = (IView *)malloc(sizeof(IView));
    created->implementationObject = implementationObject;
    created->destroyOverride = destroyOverride;
    created->displayOverride = displayOverride;
    created->hideOverride = hideOverride;
    return created;
}

void IView_destroy(IView *self, int destroyDerviedTypes)
{
    if (destroyDerviedTypes)
    {
        self->destroyOverride(self->implementationObject);
    }
    else
    {
        free(self);
    }
}
void IView_display(IView *self)
{
    self->displayOverride(self->implementationObject);
}
void IView_hide(IView *self)
{
    self->hideOverride(self->implementationObject);
}