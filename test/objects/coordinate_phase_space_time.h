#include <gtest/gtest.h>

#include "../../src/objects/coordinate_phase_space_time.h"

TEST(coordinates,constructor) {
    int cell_id = 42;
    int vx = 10;
    int vy =1;
    int vz = 2;
    int time = 2;

    bool all_clear(true);

    coordinate_phase_space_time coordinate = coordinate_phase_space_time(cell_id, vx, vy, vz, time);

    if (coordinate.getCell_id()!=cell_id || coordinate.getVx()!=vx || coordinate.getVy()!=vy
            || coordinate.getVz()!=vz || coordinate.getTime() != time) {
        all_clear = false;
    }

    ASSERT_TRUE(all_clear);
}

TEST(coordinates,equals){
    int cell_id = 42;
    int vx = 10;
    int vy =1;
    int vz = 2;
    int time = 2;

    bool all_clear(true);

    coordinate_phase_space_time coordinate = coordinate_phase_space_time(cell_id, vx, vy, vz, time);
    coordinate_phase_space_time coordinate2 = coordinate_phase_space_time(cell_id, vx, vy, vz, time);

    if (!(coordinate==coordinate2)) {
        all_clear = false;
    }

    ASSERT_TRUE(all_clear);
}

