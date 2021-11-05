#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <QString>

const QString VERSION = "v1.1";

const QString SIMULATION_LAUNCH_NAME = "Simulation Launcher";

const QString SAVE_CONTENT = "/home/ln/ros_ws/src/uavs_explore_indoor_environment";

enum WINDOWS_TYPES {
    LOADWORLD = 0xfff1,
    CREATWORLD = 0xfff2
};

enum GRID_STATUS {
    EDIT_DRAWING = 0xffe1,
    UAV_DRAWING = 0xffe2,
    IDLE = 0xffe3
};

#endif // CONFIGURE_H
