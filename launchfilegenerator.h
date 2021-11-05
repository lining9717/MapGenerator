#ifndef LAUNCHFILEGENERATOR_H
#define LAUNCHFILEGENERATOR_H

#include "configure.h"
#include <QVector>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

const string explore_text1 = R"(<?xml version="1.0"?>
<launch>
    <include file="$(find hector_gazebo_worlds)/launch/start.launch">
        <arg name="world" value="$(find uavs_explore_indoor_environment)/worlds/%s.world"/>
    </include>)";

const string explore_text2 = R"(
    <group ns="uav%d">
        <include file="$(find hector_quadrotor_gazebo)/launch/spawn_quadrotor.launch">
        <arg name="name" value="uav%d" />
        <arg name="tf_prefix" value="uav%d" />
        <arg name="x" default="%.1f"/>
        <arg name="y" default="%.1f"/>
        <arg name="z" default="0.3"/>
        <arg name="model" value="$(find hector_quadrotor_description)/urdf/quadrotor_hokuyo_utm30lx.gazebo.xacro"/>
        <arg name="controllers" value="
                    controller/attitude
                    controller/velocity
                    controller/position
                    "/>
        </include>
    </group>)";
const string explore_text3 = R"(
</launch>)";

const string start_uav_text = R"(<?xml version="1.0"?>
<launch>
    <node pkg="uavs_explore_indoor_environment" type="UAVsExploreIndoorEnvironment" name="start_uavs" output="screen">
        <param name="map_file" type="string" value="/maps/%s.txt"/>
        <param name="init_positions" type="string" value="/positions/%s_init.txt"/>
        <param name="track_positions" type="string" value="/positions/%s_track.txt"/>
    </node>
</launch>)";

void generateExploreLaunchFile(const QString& name,
    const QVector<QPair<int, int> >& init_uavs_position,
    const QVector<QPair<int, int> >& track_uavs_position);

void generateStartUAVLaunchFile(const QString& name);

std::string string_format(const char* format, ...);

#endif // LAUNCHFILEGENERATOR_H
