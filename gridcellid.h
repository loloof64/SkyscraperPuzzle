#ifndef GRIDCELLID_H
#define GRIDCELLID_H

enum GridCellIdType {
    ClueTop,
    ClueBottom,
    ClueLeft,
    ClueRight,
    Game,
};

class GridCellId
{
public:
    GridCellId(GridCellIdType type, int id): type(type), id(id) {}
    GridCellIdType getType() const;
    int getId() const;
private:
    const GridCellIdType type;
    const int id;
};

#endif // GRIDCELLID_H
