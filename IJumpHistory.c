#include "IJumpHistory.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct i_jump_history
{
    void *implementationObject;
    void (*destroyOverride)(void *implObject);
    void (*addRecordOverride)(void *implObject, JumpInfo jumpData);
    JumpInfo (*extractOverride)(void *impleObject);

} IJumpHistory;

IJumpHistory *IJumpHistory_new(
    void *implementationObject,
    void (*destroyOverride)(void *implObject),
    void (*addRecordOverride)(void *implObject, JumpInfo jumpData),
    JumpInfo (*extractOverride)(void *impleObject))
{

    if (!implementationObject || !destroyOverride || !addRecordOverride || !extractOverride)
    {
        printf("Error: NULL passed as interface override.\n");
    }

    IJumpHistory *created = (IJumpHistory *)malloc(sizeof(IJumpHistory));
    created->implementationObject = implementationObject;
    created->destroyOverride = destroyOverride;
    created->addRecordOverride = addRecordOverride;
    created->extractOverride = extractOverride;

    return created;
}
void IJumpHistory_destroy(IJumpHistory *self, int destroyDerivedTypes)
{
    if (destroyDerivedTypes)
    {
        self->destroyOverride(self->implementationObject);
    }
    else
    {
        free(self);
    }
}
JumpInfo IJumpHistory_extract(IJumpHistory *self)
{
    return self->extractOverride(self->implementationObject);
}
void IJumpHistory_addRecord(IJumpHistory *self, JumpInfo jumpData)
{
    self->addRecordOverride(self->implementationObject, jumpData);
}