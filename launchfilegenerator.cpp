#include "launchfilegenerator.h"

std::string string_format(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int count = vsnprintf(NULL, 0, format, args);
    va_end(args);

    va_start(args, format);
    char* buff = (char*)malloc((count + 1) * sizeof(char));
    vsnprintf(buff, (count + 1), format, args);
    va_end(args);

    std::string str(buff, count);
    free(buff);
    return str;
}

void generateExploreLaunchFile(const QString& name,
    const QVector<QPair<int, int> >& init_uavs_position,
    const QVector<QPair<int, int> >& track_uavs_position)
{
    string explore_launch_text = string_format(explore_text1.c_str(), name.toStdString().c_str());
    int cnt = 0;
    for (auto& p : init_uavs_position) {
        string uav_temp_text = string_format(explore_text2.c_str(), cnt, cnt, cnt, (float)p.first, (float)p.second);
        explore_launch_text += uav_temp_text;
        ++cnt;
    }
    for (auto& p : track_uavs_position) {
        string uav_temp_text = string_format(explore_text2.c_str(), cnt, cnt, cnt, (float)p.first, (float)p.second);
        explore_launch_text += uav_temp_text;
        ++cnt;
    }
    explore_launch_text += explore_text3;

    std::ofstream fileout;
    fileout.open(SAVE_CONTENT.toStdString() + "/launch/" + name.toStdString() + "_uav_explore.launch");
    if (!fileout.is_open()) {
        cout << "open file error" << endl;
        return;
    }
    fileout << explore_launch_text;
    fileout.close();
}

void generateStartUAVLaunchFile(const QString& name)
{
    string start_launch_text = string_format(start_uav_text.c_str(),
        name.toStdString().c_str(),
        name.toStdString().c_str(),
        name.toStdString().c_str());
    std::ofstream fileout;
    fileout.open(SAVE_CONTENT.toStdString() + "/launch/" + name.toStdString() + "_start_uavs.launch");
    if (!fileout.is_open()) {
        cout << "open file error" << endl;
        return;
    }
    fileout << start_launch_text;
    fileout.close();
}
