from sqlalchemy import Integer, String, create_engine, Column, select
from sqlalchemy.orm import DeclarativeBase
from sqlalchemy.orm import sessionmaker


# set up the connection parameters
host = '127.0.0.1'
username = 'postgres'
password = 'passw0rd'
database_name = 'ACC_BBALL'

# create the engine
engine = create_engine(
    'postgresql://postgres:passw0rd@localhost:5432/ACC_BBALL')

Session = sessionmaker(bind=engine)
session = Session()

class Base(DeclarativeBase):
    pass

class PLAYER(Base):
    __tablename__ = "player"
    player_id = Column(Integer, primary_key=True)
    team_id = Column(Integer)
    uniform_num = Column(Integer)
    first_name = Column(String)
    last_name = Column(String)
    mpg = Column(Integer)
    ppg = Column(Integer)
    rpg = Column(Integer)
    apg = Column(Integer)
    spg = Column(Integer)
    bpg = Column(Integer)

class TEAM(Base):
    __tablename__ = "team"
    team_id = Column(Integer, primary_key=True)
    name = Column(String)
    state_id = Column(Integer)
    color_id = Column(Integer)
    wins = Column(Integer)
    losses = Column(Integer)

class State(Base):
    __tablename__ = "state"
    state_id = Column(Integer, primary_key=True)
    name = Column(String)

class COLOR(Base):
    __tablename__ = "color"
    color_id = Column(Integer, primary_key=True)
    name = Column(String)

def query1(use_mpg, min_mpg, max_mpg,
            use_ppg, min_ppg, max_ppg,
            use_rpg, min_rpg, max_rpg,
            use_apg, min_apg, max_apg,
            use_spg, min_spg, max_spg,
            use_bpg, min_bpg, max_bpg):
    stmt = select(PLAYER)
    if use_mpg:
        stmt = stmt.where(
            PLAYER.mpg.between(min_mpg, max_mpg))
    if use_ppg:
        stmt = stmt.where(
            PLAYER.ppg.between(min_ppg, max_ppg))
    if use_rpg:
        stmt = stmt.where(
            PLAYER.rpg.between(min_rpg, max_rpg))
    if use_apg:
        stmt = stmt.where(
            PLAYER.apg.between(min_apg, max_apg))
    if use_spg:
        stmt = stmt.where(
            PLAYER.spg.between(min_spg, max_spg))
    if use_bpg:
        stmt = stmt.where(
            PLAYER.bpg.between(min_bpg, max_bpg))       
    results = session.scalars(stmt)
    # print the results
    print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
    for result in results:
        print(result.player_id, result.team_id, result.uniform_num, result.first_name, result.last_name, result.mpg, result.ppg, result.rpg, result.apg, result.spg, result.bpg)
    return

def query2(team_color):
    stmt = select(TEAM.name)
    stmt = stmt.where(TEAM.color_id == COLOR.color_id, COLOR.name == team_color)
    results = session.scalars(stmt)
    # print the results
    print("NAME")
    for result in results:
        print(result)
    return

def query3(team_name):
    stmt = select(PLAYER)
    stmt = stmt.where(PLAYER.team_id == TEAM.team_id, TEAM.name == team_name)
    results = session.scalars(stmt)
    # print the results
    print("FIRST_NAME LAST_NAME")
    for result in results:
        print(result.first_name, result.last_name)
    return

def query4(team_state,team_color):
    stmt = select(PLAYER)
    stmt = stmt.where(PLAYER.team_id == TEAM.team_id, TEAM.state_id == State.state_id, State.name == team_state, TEAM.color_id == COLOR.color_id, COLOR.name == team_color)
    results = session.scalars(stmt)
    # print the results
    print("UNIFORM_NUM FIRST_NAME LAST_NAME")
    for result in results:
        print(result.uniform_num, result.first_name, result.last_name)
    return

def query5(num_wins):
    stmt = select(PLAYER.first_name, PLAYER.last_name,TEAM.name, TEAM.wins)
    stmt = stmt.where(PLAYER.team_id == TEAM.team_id, TEAM.wins > num_wins)
    results = session.execute(stmt)
    # print the results
    print("FIRST_NAME LAST_NAME NAME WINS")
    for result in results:
        print(result.first_name, result.last_name, result.name, result.wins)
    return

query1(1, 20, 30, 1, 10, 20, 1, 2, 5, 1, 1, 3, 1, 1, 2, 1, 0, 1)
query1(1, 35, 40, 0, 10, 20, 0, 2, 5, 0, 1, 3, 0, 1, 2, 0, 0, 1)
query2("Maroon")
query3("Duke")
query4("VA", "Maroon")
query5(10)
query1(1, 35, 40,
           0, 0, 0,
           0, 0, 0,
           0, 0, 0,
           0, 0, 0,
           0, 0, 0)
query2("Red")
query3("UNC")
query4("NC", "LightBlue")
query5(9)

    