#include "query_funcs.h"


void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
    work W(*C);
    string query = "INSERT INTO player (team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg) VALUES (" 
    + to_string(team_id) + ", " + to_string(jersey_num) + ", " + W.quote(first_name) + ", " + W.quote(last_name) + ", " + to_string(mpg) + ", " 
    + to_string(ppg) + ", " + to_string(rpg) + ", " + to_string(apg) + ", " + to_string(spg) + ", " + to_string(bpg) + ");";
    W.exec(query);
    W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
    work W(*C);
    string query = "INSERT INTO team (name, state_id, color_id, wins, losses) VALUES (" + W.quote(name) + ", " + to_string(state_id) + ", "
    + to_string(color_id) + ", " + to_string(wins) + ", " + to_string(losses) + ");";
    W.exec(query);
    W.commit();
}


void add_state(connection *C, string name)
{
    work W(*C);
    string query = "INSERT INTO state (name) VALUES (" + W.quote(name) + ");";
    W.exec(query);
    W.commit();
}


void add_color(connection *C, string name)
{
    work W(*C);
    string query = "INSERT INTO color (name) VALUES (" + W.quote(name) + ");";
    W.exec(query);
    W.commit();
}

/*
 * All use_ params are used as flags for corresponding attributes
 * a 1 for a use_ param means this attribute is enabled (i.e. a WHERE clause is needed)
 * a 0 for a use_ param means this attribute is disabled
 */
void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
    work W(*C);
    string query = "SELECT * FROM player";
    int numEnabled = 0;//number of enabled attributes, 0 if none, 1 if one or more

    if(use_mpg == 1)
    {
        if(numEnabled == 1)
            query += " AND ";
        else
            query += " WHERE ";
        query += "mpg >= " + to_string(min_mpg) + " AND mpg <= " + to_string(max_mpg);
        numEnabled = 1;
    }

    if(use_ppg == 1)
    {
        if(numEnabled == 1)
            query += " AND ";
        else
            query += " WHERE ";
        query += "ppg >= " + to_string(min_ppg) + " AND ppg <= " + to_string(max_ppg);
        numEnabled = 1;
    }

    if(use_rpg == 1)
    {
        if(numEnabled == 1)
            query += " AND ";
        else
            query += " WHERE ";
        query += "rpg >= " + to_string(min_rpg) + " AND rpg <= " + to_string(max_rpg);
        numEnabled = 1;
    }

    if(use_apg == 1)
    {
        if(numEnabled == 1)
            query += " AND ";
        else
            query += " WHERE ";
        query += "apg >= " + to_string(min_apg) + " AND apg <= " + to_string(max_apg);
        numEnabled = 1;
    }

    if(use_spg == 1)
    {
        if(numEnabled == 1)
            query += " AND ";
        else
            query += " WHERE ";
        query += "spg >= " + to_string(min_spg) + " AND spg <= " + to_string(max_spg);
        numEnabled = 1;
    }

    if(use_bpg == 1)
    {
        if(numEnabled == 1)
            query += " AND ";
        else
            query += " WHERE ";
        query += "bpg >= " + to_string(min_bpg) + " AND bpg <= " + to_string(max_bpg);
        numEnabled = 1;
    }

    query+=";";
    W.commit();
    nontransaction N(*C);

    result R(N.exec(query));

    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG"//-----------modify???
         << endl;
    for (result::const_iterator i = R.begin(); i != R.end(); ++i) {
        cout << i[0].as<int>() << " " << i[1].as<int>() << " " << i[2].as<int>() << " "
             << i[3].as<string>() << " " << i[4].as<string>() << " " << i[5].as<int>() << " "
             << i[6].as<int>() << " " << i[7].as<int>() << " " << i[8].as<int>() << " "
             << fixed << setprecision(1) << i[9].as<double>() << " " << i[10].as<double>()//-----------modify???
             << endl;
    }
}

void query2(connection *C, string team_color)
{
    work W(*C);
    string query = "SELECT TEAM.NAME FROM TEAM, COLOR WHERE TEAM.COLOR_ID = COLOR.COLOR_ID AND COLOR.NAME = " + W.quote(team_color) + ";";
    W.commit();
    nontransaction N(*C);
    result R(N.exec(query));
    cout << "NAME" << endl;
    for(result::const_iterator i = R.begin(); i != R.end(); ++i)
    {
        cout << i[0].as<string>() << endl;
    }
}


void query3(connection *C, string team_name)
{
    work W(*C);
    string query = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.NAME = " + W.quote(team_name) 
    + "ORDER BY ppg desc;";
    W.commit();
    nontransaction N(*C);
    result R(N.exec(query));
    cout << "FIRST_NAME LAST_NAME" << endl;
    for(result::const_iterator i = R.begin(); i != R.end(); ++i)
    {
        cout << i[0].as<string>() << " " << i[1].as<string>() << endl;
    }
}


void query4(connection *C, string team_state, string team_color)
{
    work W(*C);
    string query = "SELECT PLAYER.UNIFORM_NUM, PLAYER.FIRST_NAME, PLAYER.LAST_NAME FROM PLAYER, TEAM, STATE, COLOR WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.STATE_ID = STATE.STATE_ID AND TEAM.COLOR_ID = COLOR.COLOR_ID AND STATE.NAME = " + W.quote(team_state) + " AND COLOR.NAME = " + W.quote(team_color) + ";";
    W.commit();
    nontransaction N(*C);
    result R(N.exec(query));
    cout << "UNIFORM_NUM FIRST_NAME LAST_NAME" << endl;
    for(result::const_iterator i = R.begin(); i != R.end(); ++i)
    {
        cout << i[0].as<int>() << " " << i[1].as<string>() << " " << i[2].as<string>() << endl;
    }
}


void query5(connection *C, int num_wins)
{
    work W(*C);
    string query= "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.WINS > " + to_string(num_wins);
    W.commit();
    nontransaction N(*C);
    
    
    result R(N.exec(query));
    cout << "FIRST_NAME LAST_NAME NAME WINS" << endl;
    for(result::const_iterator i = R.begin(); i != R.end(); ++i)
    {
        cout << i[0].as<string>() << " " << i[1].as<string>() << " " << i[2].as<string>() << " " << i[3].as<int>() << endl;
    }
}
