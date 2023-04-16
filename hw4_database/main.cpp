#include <fstream>
#include <iostream>
#include <pqxx/pqxx>

#include "exerciser.h"

using namespace std;
using namespace pqxx;   

void createTable(string fileName, connection *C) {
    string line;
    string query;
    std::ifstream file(fileName);
    if (file.is_open()) {
        while (getline(file, line)) {
            query += line;
        }
        file.close();
    } else {
        cout << "Unable to open file";
    }
    work W(*C);  
    W.exec(query);
    W.commit();
}

void loadPlayer(string fileName, connection *C) {
    string line;
    int player_id;
    int team_id;
    int jersey_num;
    string first_name;
    string last_name;
    int mpg;
    int ppg;
    int rpg;
    int apg;
    double spg;
    double bpg;
    std::ifstream file(fileName);
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            ss >> player_id >> team_id >> jersey_num >> first_name >> last_name >> mpg >> ppg >> rpg >> apg >> spg >> bpg;
            add_player(C, team_id, jersey_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
        }
        file.close();
    } else {
        cout << "Unable to open file";
    }
}

void loadTeam(string fileName, connection *C) {
    string line;
    int team_id;
    string name;
    int state_id;
    int color_id;
    int wins;
    int losses;
    std::ifstream file(fileName);
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            ss >> team_id >> name >> state_id >> color_id >> wins >> losses;
            add_team(C, name, state_id, color_id, wins, losses);
        }
        file.close();
    } else {
        cout << "Unable to open file";
    }
}

void loadState(string fileName, connection *C) {
    string line;
    int state_id;
    string name;
    std::ifstream file(fileName);
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            ss >> state_id >> name;
            add_state(C, name);
        }
        file.close();
    } else {
        cout << "Unable to open file";
    }
}

void loadColor(string fileName, connection *C) {
    string line;
    int color_id;
    string name;
    std::ifstream file(fileName);
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            ss >> color_id >> name;
            add_color(C, name);
        }
        file.close();
    } else {
        cout << "Unable to open file";
    }
}

int main(int argc, char *argv[]) {
    // Allocate & initialize a Postgres connection object
    connection *C;

    try {
        // Establish a connection to the database
        // Parameters: database name, user name, user password
        C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
        if (C->is_open()) {
            // cout << "Opened database successfully: " << C->dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
        return 1;
    }

    // TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
    //       load each table with rows from the provided source txt files
    createTable("createTable.sql", C);
    loadPlayer("player.txt", C);
    loadTeam("team.txt", C);
    loadState("state.txt", C);
    loadColor("color.txt", C);

    exercise(C);

    // Close database connection
    C->disconnect();

    return 0;
}
