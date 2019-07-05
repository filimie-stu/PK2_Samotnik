#include "IJumpHistory.h"
#include <stdlib.h>
#include <stdio.h>



/**
 * \brief Interfejs implementowany przez model historii skoków (JumpHistory).
 * \sa jump_history
*/
typedef struct i_jump_history
{
    void *implementationObject;
    void (*destroyOverride)(void *implObject);
    void (*addRecordOverride)(void *implObject, JumpInfo jumpData);
    int (*isEmptyOverride)(void* implObject);
    JumpInfo (*extractOverride)(void *impleObject);

} IJumpHistory;

IJumpHistory *IJumpHistory_new(
    void *implementationObject,
    void (*destroyOverride)(void *implObject),
    void (*addRecordOverride)(void *implObject, JumpInfo jumpData),
    int (*isEmptyOverride)(void* implObject),
    JumpInfo (*extractOverride)(void *impleObject))
{

    if (!implementationObject || !isEmptyOverride || !destroyOverride || !addRecordOverride || !extractOverride)
    {
        printf("Error: NULL passed as interface override.\n");
    }

    IJumpHistory *created = (IJumpHistory *)malloc(sizeof(IJumpHistory));
    created->implementationObject = implementationObject;
    created->destroyOverride = destroyOverride;
    created->addRecordOverride = addRecordOverride;
    created->extractOverride = extractOverride;
    created->isEmptyOverride = isEmptyOverride;
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
int IJumpHistory_isEmpty(IJumpHistory* self)
{
    return self->isEmptyOverride(self->implementationObject);
}