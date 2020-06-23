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

//this->getTime() - coordinate.getTime(),
//this->getCell_id() - coordinate.getCell_id(),
//this->getVx() - coordinate.getVx(),
//this->getVy() - coordinate.getVy(),
//this->getVz() - coordinate.getVz()

TEST(coordinates,difference) {
    int cell1 = 42;
    int cell2 = 41;
    int vx1 = 32;
    int vx2 = 62;
    int vy1 = 20;
    int vy2 = 91;
    int vz1 = 23;
    int vz2 = 31;
    int time1 = 23;
    int time2 = 41;

    bool all_clear(true);

    coordinate_phase_space_time c1 = coordinate_phase_space_time(cell1,vx1,vy1,vz1,time1);
    coordinate_phase_space_time c2 = coordinate_phase_space_time(cell2,vx2,vy2,vz2,time2);
    coordinate_phase_space_time test1 = c1-c2;

    if(test1.getCell_id() != (cell1-cell2) ||
            test1.getVx() != (vx1-vx2) ||
            test1.getVy() != (vy1-vy2) ||
            test1.getVz() != (vz1-vz2) ||
            test1.getTime() != (time1-time2)) {
        all_clear = false;
    }

    ASSERT_TRUE(all_clear);
}

TEST(coordinates,differenceEqual) {
    int cell1 = 42;
    int vx1 = 32;
    int vy1 = 20;
    int vz1 = 23;
    int time1 = 23;

    bool all_clear(true);

    coordinate_phase_space_time c1 = coordinate_phase_space_time(cell1,vx1,vy1,vz1,time1);
    coordinate_phase_space_time c2 = c1;
    coordinate_phase_space_time test1 = c1-c2;

    if(test1.getCell_id() != 0 ||
            test1.getVx() != 0 ||
            test1.getVy() != 0 ||
            test1.getVz() != 0 ||
            test1.getTime() != 0) {
        all_clear = false;
    }

    ASSERT_TRUE(all_clear);
}

TEST(coordinates,toString) {
    int cell1 = 42;
    int vx1 = 32;
    int vy1 = 20;
    int vz1 = 23;
    int time1 = 23;

    bool all_clear(true);

    coordinate_phase_space_time c1 = coordinate_phase_space_time(cell1,vx1,vy1,vz1,time1);
   std::string test_string =  "{" + std::to_string(time1) + ", " + std::to_string(0.0) + ", "
            + std::to_string(0.0) + ", "
            + std::to_string(0.0) + ", "
            + std::to_string(vx1) + ", "
            + std::to_string(vy1) + ", "
            + std::to_string(vz1) + "}";

   ASSERT_STREQ(test_string,c1.toString());
}

