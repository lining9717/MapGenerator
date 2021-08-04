#ifndef CONFIGURE_H
#define CONFIGURE_H

#include<QString>

const QString VERSION = "v0.1";

const QString SIMULATION_LAUNCH_NAME = "Simulation Launcher";

enum WINDOWS_TYPES {
    LOADWORLD = 0xfff1,
    CREATWORLD = 0xfff2
};

enum GRID_STATUS {
    DRAWING = 0xffe1,
    ERASING = 0xffe2,
    IDLE = 0xffe3
};


#endif // CONFIGURE_H
