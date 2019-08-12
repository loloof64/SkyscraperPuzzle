#include "gridcellid.h"

GridCellIdType GridCellId::getType() const
{
    return this->type;
}

int GridCellId::getId() const
{
    return this->id;
}
