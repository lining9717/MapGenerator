#include "worldfilegenerator.h"

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

WorldFileGenerator::WorldFileGenerator() {}
WorldFileGenerator::~WorldFileGenerator() {}
void WorldFileGenerator::setMapFile(const string& map)
{
    map_file = map;
    walls.clear();
    parse_walls();
}

void WorldFileGenerator::generateWorldFile(const string& to_file)
{
    string world_text = "";
    world_text += world_sdf0;
    world_text += add_walls1();
    world_text += world_sdf1;
    world_text += add_walls2();
    world_text += world_sdf2;
    std::ofstream fileout;
    fileout.open(to_file);
    if (!fileout.is_open()) {
        cout << "open file error" << endl;
        return;
    }
    fileout << world_text;
    fileout.close();
}

string WorldFileGenerator::add_walls1()
{
    string ret = "";
    int count = 0;
    for (auto& wall : walls) {
        stringstream ss;
        ss << "Wall_" << count;
        string link_name = ss.str();
        string collision_name = link_name + "_Collision";
        string visual_name = link_name + "_Visual";
        int start_x = wall[0];
        int start_y = wall[1];
        int end_x = wall[2];
        int end_y = wall[3];
        float pose_x = float((start_x + end_x) / 2.0);
        float pose_y = float((start_y + end_y) / 2.0);
        float wall_length = sqrt((end_x - start_x) * (end_x - start_x) + (end_y - start_y) * (end_y - start_y)) + 0.15;
        float yaw = 0.0;
        if (start_x == end_x) {
            yaw = -M_PI / 2;
        }
        string temp = string_format(add_wall1_str.c_str(),
            link_name.c_str(), collision_name.c_str(), wall_length,
            visual_name.c_str(), wall_length, pose_x, pose_y + 0.5, yaw);
        ret += temp;
        ++count;
    }
    return ret;
}

string WorldFileGenerator::add_walls2()
{
    string ret = "";
    int count = 0;
    for (auto& wall : walls) {
        stringstream ss;
        ss << "Wall_" << count;
        string link_name = ss.str();
        int start_x = wall[0];
        int start_y = wall[1];
        int end_x = wall[2];
        int end_y = wall[3];
        float pose_x = float((start_x + end_x) / 2.0);
        float pose_y = float((start_y + end_y) / 2.0);
        float yaw = 0.0;
        if (start_x == end_x) {
            yaw = -M_PI / 2;
        }
        string temp = string_format(add_wall2_str.c_str(),
            link_name.c_str(), pose_x, pose_y, yaw);
        ret += temp;
        ++count;
    }
    return ret;
}

void WorldFileGenerator::parse_walls()
{
    fstream f(map_file);
    vector<string> map;
    string input;
    width = height = 0;
    while (f >> input) {
        map.push_back(input);
        width = max(width, (int)input.length());
    }
    height = map.size();
    int x_bias = width / 2;
    int y_bias = height / 2;
    for (int i = 0; i < height; ++i) {
        vector<int> item;
        for (int j = 1; j < width - 1; ++j) {
            if (map[i][j - 1] == '.' and map[i][j] == '#' and map[i][j + 1] == '#') {
                item.push_back(j - x_bias);
                item.push_back(y_bias - i);
            }
            if (map[i][j - 1] == '#' and map[i][j] == '#' and map[i][j + 1] == '.') {
                item.push_back(j - x_bias);
                item.push_back(y_bias - i);
                walls.push_back(item);
                item.clear();
            }
        }
    }

    for (int i = 0; i < width; ++i) {
        vector<int> item;
        for (int j = 1; j < height - 1; ++j) {
            if (map[j - 1][i] == '.' and map[j][i] == '#' and map[j + 1][i] == '#') {
                item.push_back(i - x_bias);
                item.push_back(y_bias - j);
            }

            if (map[j - 1][i] == '#' and map[j][i] == '#' and map[j + 1][i] == '.') {
                item.push_back(i - x_bias);
                item.push_back(y_bias - j);
                walls.push_back(item);
                item.clear();
            }
        }
    }
}
