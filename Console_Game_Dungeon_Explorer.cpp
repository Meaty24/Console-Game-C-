#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>

using namespace std;

/// /////////////////////////////////Global Variables//////////////////////////////////////////////1

struct leaderboard
{
    string name;
    int score;
    int totalmoves;
};

bool comparePoints(const leaderboard& person1, const leaderboard& person2)
{
  return person1.score > person2.score;
}

struct Player
{
    int x;
    int y;
    int roomX;
    int roomY;
    int acquiredpoints;
    int totalmoves;
};

struct Enemy
{
    int id;
    int x;
    int y;
    int spawnx;
    int spawny;
    int dist;
    int roomX;
    int roomY;
    int Direction=0;
};

struct Room
{
    int number;
    char layout[7][7];
    int points;
    bool visited = false;
};

Room roomvoid,roomend,room3,room4,room5,room6,room7,room8,room9,roomstart;
Player player;
char board[49][49];
vector<vector<int>> map1;
vector<vector<Room>> rooms;
vector<Enemy> enemies;
int moves=0;
int point=0;
string displayname="Peach";
vector<leaderboard> Scores;
short GameState = 1; ///1 - Game is being played. 0 - Game is lost. 2 - Level has been beaten.

int getRandomInt(int min, int max)
{
    return min + rand() % (max - min + 1);
}

void color(int color_code)
{
    HANDLE var_name = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(var_name, color_code);
}

/// /////////////////////////////////BOARD GENERATION////////////////////////////////////////////2

void manipulateBoard()
{
    bool test = false;
    for(int i=0; i<map1.size(); i++)
    {
        for(int j=0; j<map1.size(); j++)
        {
            if(map1[i][j] == 0)
            {
                map1[i][j]=20;
                test=true;
                break;
            }
        }
        if(test) break;
    }
    test = false;
    for(int i=map1.size()-1; i>-1; i--)
    {
        for(int j=map1.size()-1; j>-1; j--)
        {
            if(map1[i][j] == 0)
            {
                map1[i][j]=2;
                test=true;
                break;
            }
        }
        if(test) break;
    }
    for(int i=0; i<map1.size(); i++)
        for(int j=0; j<map1.size(); j++)
            if(map1[i][j] == 0)
                map1[i][j] = rand()%7 + 3;
}

void buildBoard()
{
    ///Void room layout
    char voidroom[7][7] =
    {
        {'#', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', '#', '#', '#', '#', '#', '#'}
    };
    ///Start room layout
    char start[7][7] =
    {
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', 'P', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '}
    };
    ///End room layout
    char endroom[7][7] =
    {
        {'#', ' ', ' ', ' ', '#', '#', '#'},
        {' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {' ', ' ', ' ', 'W', ' ', ' ', ' '},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', '#', '#', ' ', '#', '#', '#'}
    };
    ///Room 3 layout
    char Room3[7][7] =
    {
        {char(176), char(176), ' ', ' ', ' ', ' ', ' '},
        {char(176), ' ', ' ', ' ', ' ', char(176), ' '},
        {' ', ' ', char(176), char(176), ' ', char(176), char(176)},
        {' ', ' ', char(176), char(176), ' ', ' ', ' '},
        {' ', ' ', ' ', char(176), char(176), ' ', ' '},
        {' ', char(176), ' ', ' ', ' ', ' ', ' '},
        {' ', char(176), ' ', ' ', ' ', ' ', ' '}
    };
    ///Room 4 layout
    char Room4[7][7] =
    {
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', 'O', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', 'O', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', char(254), ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '}
    };
    ///Room 5 layout
    char Room5[7][7] =
    {
        {'#', '#', '#', ' ', '#', '#', '#'},
        {'#', ' ', char(176), ' ', char(176), ' ', '#'},
        {'#', char(176), char(176), ' ', char(176), char(176), '#'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'#', char(176), char(176), ' ', char(176), char(176), '#'},
        {'#', ' ', char(176), ' ', char(176), ' ', '#'},
        {'#', '#', '#', ' ', '#', '#', '#'}
    };
    ///Room 6 layout
    char Room6[7][7] =
    {
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', 'E', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '}
    };
    ///Room 7 layout
    char Room7[7][7] =
    {
        {'#', '#', '#', ' ', '#', '#', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', '#'},
        {'#', ' ', '#', ' ', '#', '#', '#'},
        {' ', ' ', ' ', '@', ' ', ' ', ' '},
        {'#', '#', '#', ' ', '#', ' ', '#'},
        {'#', ' ', ' ', ' ', '#', ' ', '#'},
        {'#', '#', '#', ' ', '#', '#', '#'}
    };
    ///Room 8 layout - char(167) & char(248)
    char Room8[7][7] =
    {
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', '?', ' ', '?', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', '?', ' ', '?', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'}
    };
    char Room9[7][7] =
    {
        {'#', '#', ' ', ' ', ' ', '#', '#'},
        {'#', '*', ' ', ' ', ' ', '*', '#'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'#', '*', ' ', ' ', ' ', '*', '#'},
        {'#', '#', ' ', ' ', ' ', '#', '#'}
    };
    for(int i=0; i<7; i++)
        for(int j=0; j<7; j++)
            roomvoid.layout[i][j]=voidroom[i][j];
    for(int i=0; i<7; i++)
        for(int j=0; j<7; j++)
            roomstart.layout[i][j]=start[i][j];
    for(int i=0; i<7; i++)
        for(int j=0; j<7; j++)
            roomend.layout[i][j]=endroom[i][j];
    for(int i=0; i<7; i++)
        for(int j=0; j<7; j++)
            room3.layout[i][j]=Room3[i][j];
    for(int i=0; i<7; i++)
        for(int j=0; j<7; j++)
            room4.layout[i][j]=Room4[i][j];
    for(int i=0; i<7; i++)
        for(int j=0; j<7; j++)
            room5.layout[i][j]=Room5[i][j];
    for(int i=0; i<7; i++)
        for(int j=0; j<7; j++)
            room6.layout[i][j]=Room6[i][j];
    for(int i=0; i<7; i++)
        for(int j=0; j<7; j++)
            room7.layout[i][j]=Room7[i][j];
    for(int i=0; i<7; i++)
        for(int j=0; j<7; j++)
            room8.layout[i][j]=Room8[i][j];
    for(int i=0; i<7; i++)
        for(int j=0; j<7; j++)
            room9.layout[i][j]=Room9[i][j];

///Board Fill
    for(int x=0; x<7; x++)
        for(int y=0; y<7; y++)
        {
            switch(map1[x][y])
            {
            case 1:
                for(int i=0; i<7; i++)
                    for(int j=0; j<7; j++)
                        board[i+7*x][j+7*y]=roomvoid.layout[i][j];
                break;
            case 2:
                for(int i=0; i<7; i++)
                    for(int j=0; j<7; j++)
                        board[i+7*x][j+7*y]=roomend.layout[i][j];
                break;
            case 20:
                for(int i=0; i<7; i++)
                    for(int j=0; j<7; j++)
                        board[i+7*x][j+7*y]=roomstart.layout[i][j];
                break;
            case 3:
                for(int i=0; i<7; i++)
                    for(int j=0; j<7; j++)
                        board[i+7*x][j+7*y]=room3.layout[i][j];
                break;
            case 4:
                for(int i=0; i<7; i++)
                    for(int j=0; j<7; j++)
                        board[i+7*x][j+7*y]=room4.layout[i][j];
                break;
            case 5:
                for(int i=0; i<7; i++)
                    for(int j=0; j<7; j++)
                        board[i+7*x][j+7*y]=room5.layout[i][j];
                break;
            case 6:
                for(int i=0; i<7; i++)
                    for(int j=0; j<7; j++)
                        board[i+7*x][j+7*y]=room6.layout[i][j];
                break;
            case 7:
                for(int i=0; i<7; i++)
                    for(int j=0; j<7; j++)
                        board[i+7*x][j+7*y]=room7.layout[i][j];
                break;
            case 8:
                for(int i=0; i<7; i++)
                    for(int j=0; j<7; j++)
                        board[i+7*x][j+7*y]=room8.layout[i][j];
                break;
            case 9:
                for(int i=0; i<7; i++)
                    for(int j=0; j<7; j++)
                        board[i+7*x][j+7*y]=room9.layout[i][j];
                break;
            default:
                break;
            }
        }
///End Board Fill

    for (int i = 0; i < 49; i++)
    {
        board[0][i] = '#';
        board[48][i] = '#';
    }
    for (int i = 0; i < 49; i++)
    {
        board[i][0] = '#';
        board[i][48] = '#';
    }
}

/// //////////////////////////////////////ROOM INITIALIZATION/////////////////////3

vector<vector<Room>> createRooms()
{
    vector<vector<Room>> rooms(map1.size(), vector<Room>(map1[0].size()));
    for (int i = 0; i < map1.size(); i++)
    {
        for (int j = 0; j < map1[0].size(); j++)
        {
            int roomNumber = map1[j][i];
            Room room;
            room.number = roomNumber;
            switch(roomNumber)
            {
            case 1:
                room.points=0;
                break;
            case 2:
                room.points=5000;
                break;
            case 3:
                room.points=1500;
                break;
            case 4:
                room.points=1000;
                break;
            case 5:
                room.points=2000;
                break;
            case 6:
                room.points=2500;
                break;
            case 7:
                room.points=1000;
                break;
            case 8:
                room.points=2500;
                break;
            case 9:
                room.points=1500;
                break;
            default:
                room.points=0;

            }
            room.visited = false;
            rooms[i][j] = room;
        }
    }

    return rooms;
}

void roomGeneration()
{
    ///Room void:
    roomvoid.number=1;
    roomvoid.points=0;
    ///Room end:
    roomend.number=2;
    roomend.points=5000;
    ///Room 3:
    room3.number=3;
    room3.points=1500;
    ///Room 4:
    room4.number=4;
    room4.points=1000;
    ///Room 5:
    room5.number=5;
    room5.points=2000;
    ///Room 6:
    room6.number=6;
    room6.points=2500;
    ///Room 7:
    room7.number=7;
    room7.points=1000;
    ///Room 8:
    room8.number=8;
    room8.points=2500;
    ///Room 9:
    room9.number=9;
    room9.points=1500;
    ///Room Start:
    roomstart.number=20;
    roomstart.points=0;
}

/// ///////////////////////////////////////Random Walk Algorithm///////////////////////4

vector<vector<int>> createArray(int num, int dimensions)
{
    vector<vector<int>> arr(dimensions, vector<int>(dimensions, num));
    return arr;
}

vector<vector<int>> createMap()
{
    int dimensions = 7;
    int maxTunnels = 15;
    int maxLength = 6;
    vector<vector<int>> map1 = createArray(1, dimensions);
    int currentRow = rand() % dimensions;
    int currentColumn = rand() % dimensions;
    vector<vector<int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    vector<int> lastDirection = {0,0};
    vector<int> randomDirection;
    while(maxTunnels)
    {
        do
        {
            randomDirection = directions[rand() % directions.size()];
        }
        while((randomDirection[0] == -lastDirection[0] && randomDirection[1] == -lastDirection[1]) || (randomDirection[0] == lastDirection[0] && randomDirection[1] == lastDirection[1]));
        int randomLength = rand() % maxLength + 1;
        int tunnelLength = 0;
        while (tunnelLength < randomLength)
        {
            if (((currentRow == 0) && (randomDirection[0] == -1)) ||
                    ((currentColumn == 0) && (randomDirection[1] == -1)) ||
                    ((currentRow == dimensions - 1) && (randomDirection[0] == 1)) ||
                    ((currentColumn == dimensions - 1) && (randomDirection[1] == 1)))
            {
                break;
            }
            else
            {
                map1[currentRow][currentColumn] = 0;
                currentRow += randomDirection[0];
                currentColumn += randomDirection[1];
                tunnelLength++;
            }
        }
        if (tunnelLength)
        {
            lastDirection = randomDirection;
            maxTunnels--;
        }
    }
    return map1;
}

/// /////////////////////////////////VISUAL DISPLAY///////////////////////////////////5

void help()
{
    system("cls");
    cout<<endl<<endl<<endl<<"     >>GENERAL INFORMATION<<";
    cout<<endl<<"   Game Objective: Move the player ("; color(11); cout<<"P"; color(15); cout<<") with the keyboard to reach the end room (W).";
    cout<<endl<<"   Avoid enemies, such as E, O, *, and traps: "<<char(176);
    cout<<endl<<"   WASD to move. W - UP. A - LEFT. S - DOWN. D - RIGHT.";
    cout<<endl<<"   Press Spacebar to skip a turn. (Player does not move, but the enemies will move)";
    color(4);
    cout<<endl<<"   E"; color(15); cout<<" - Enemy. Tracks player and chases them.";
    color(6);
    cout<<endl<<"   O"; color(15); cout<<" - Enemy with random movement. It does not chase the player.";
    color(2);
    cout<<endl<<"   *"; color(15); cout<<" - Enemy that moves in a circular pattern.";
    cout<<endl<<"  ";color(14);cout<<char(176);color(15);cout<<" - Trap. Does not move. Is always active. Avoid stepping over it.";
    cout<<endl<<endl<<"   ";color(2);cout<<char(254);color(15);cout<<" - Treasure chest. Adds 2000 points when picked up.";
    cout<<endl<<"   @ - Teleporter. Teleports the player two blocks diagonally in a random direction. They remain in the same room.";
    cout<<endl<<"   ";color(8);cout<<"?";color(15);cout<<" - Mystery Box. Are you willing to gamble? The mystery box has 4 random effects:";
    cout<<endl<<"   1) Adds 10,000 points.";
    cout<<endl<<"   2) Adds 1,000 points.";
    cout<<endl<<"   3) Removes 3,000 points.";
    cout<<endl<<"   4) Removes ALL OBTAINED points for this run.";
    cout<<endl<<"   Every mystery box is random. One room may contain more than one prize, but it may also remove all of your points!";
    cout<<endl<<"   Use at your own risk!!!";
    cout<<endl<<"   The walls ("; color(1); cout<<"#"; color(15);cout<<") are impassable by the player. The player may only move in open spots.";
    cout<<endl<<"   Tip: You will only lose the game if you end up on the same square as an enemy simultaneously. If you have";
    cout<<endl<<"   moving enemies directly next to you, you may still be able to move in that direction. As long as they move away";
    cout<<endl<<"   in their next move - you are safe. Will mostly happen with the * enemies.";
    cout<<endl<<endl<<"   Each move the player makes removes 100 points from the final score. Try to";
    cout<<endl<<"    beat each level with the least amount of moves possible. But also aim to collect as many";
    cout<<endl<<"    points as you can ;). Remember, points are added once you enter a room. You do not";
    cout<<endl<<"    have to complete the challenge inside.";
    cout<<endl<<"    ALL enemies have a maximum chase radius of 4 squares. After they exceed that radius, they are";
    cout<<endl<<"    transported back to their spawn location. This mechanic has killed me before (Coming from the creator),";
    cout<<endl<<"    so do keep it in mind! Good luck Adventurer! Press h on your keyboard at any time to open this menu again!";
    cout<<endl<<"    Press any key to go back.";
    _getch();
}

void displayMap()
{
    for (int i = 0; i < map1.size(); i++)
    {
        for (int j = 0; j < map1[0].size(); j++)
        {
            cout << map1[i][j] << " ";
        }
        cout << endl;
    }
    cout<<"Press any key to return to main menu.";
    _getch();
}

void displayBoard()
{
    system("cls");
    for (int i = 0; i < 49; i++)
    {
        for (int j = 0; j < 49; j++)
        {
            bool enemyPresent = false;
            for (const auto& enemy : enemies)
            {
                if (enemy.x == j && enemy.y == i)
                {
                    enemyPresent = true;
                    if(enemy.id==0)
                    {
                        color(6);
                        cout << 'O';
                        color(15);
                    }
                    else if(enemy.id==1)
                        {
                        color(4);
                        cout << 'E';
                        color(15);
                    }
                    else if(enemy.id==2)
                        {
                        color(2);
                        cout << '*';
                        color(15);
                    }
                    break;
                }
            }
            if (!enemyPresent)
            {
                if (i == player.y && j == player.x)
                {
                        color(11);
                        cout << 'P';
                        color(15);

                }
                else
                {
                    if(board[i][j]=='#')
                    {
                        color(1);
                        cout << board[i][j];
                        color(15);
                    } else if (board[i][j]==char(176))
                    {
                        color(14);
                        cout<<board[i][j];
                        color(15);
                    } else if(board[i][j]==char(254))
                    {
                        color(2);
                        cout<<board[i][j];
                        color(15);
                    } else if(board[i][j]=='?')
                    {
                        color(8);
                        cout<<board[i][j];
                        color(15);
                    } else cout<<board[i][j];
                }
            }
        }
        cout << endl;
    }
}

/// /////////////////////////////////PLAYER MOVEMENT//////////////////////////////6

void moveEnemiesInRoom(Room& room)
{
    for (auto& enemy : enemies)
    {
        if (enemy.roomX == player.roomX && enemy.roomY == player.roomY)
        {
            if(enemy.id==0)
            {
                int direction = rand() % 4;
                switch (direction)
                {
                case 0:
                    if (enemy.y > 0 && room.layout[enemy.y - 1][enemy.x] != '#' && room.layout[enemy.y][enemy.x - 1] != '*' && room.layout[enemy.y][enemy.x - 1] != char(176) && room.layout[enemy.y][enemy.x - 1] != 'E' && room.layout[enemy.y][enemy.x - 1] != 'O' && room.layout[enemy.y][enemy.x - 1] != char(254))
                    {
                        enemy.y--;
                    }
                    break;
                case 1:
                    if (enemy.y < 6 && room.layout[enemy.y + 1][enemy.x] != '#' && room.layout[enemy.y][enemy.x - 1] != '*' && room.layout[enemy.y][enemy.x - 1] != char(176) && room.layout[enemy.y][enemy.x - 1] != 'E' && room.layout[enemy.y][enemy.x - 1] != 'O' && room.layout[enemy.y][enemy.x - 1] != char(254))
                    {
                        enemy.y++;
                    }
                    break;
                case 2:
                    if (enemy.x > 0 && room.layout[enemy.y][enemy.x - 1] != '#' && room.layout[enemy.y][enemy.x - 1] != '*' && room.layout[enemy.y][enemy.x - 1] != char(176) && room.layout[enemy.y][enemy.x - 1] != 'E' && room.layout[enemy.y][enemy.x - 1] != 'O' && room.layout[enemy.y][enemy.x - 1] != char(254))
                    {
                        enemy.x--;
                    }
                    break;
                case 3:
                    if (enemy.x < 6 && room.layout[enemy.y][enemy.x + 1] != '#' && room.layout[enemy.y][enemy.x - 1] != '*' && room.layout[enemy.y][enemy.x - 1] != char(176) && room.layout[enemy.y][enemy.x - 1] != 'E' && room.layout[enemy.y][enemy.x - 1] != 'O' && room.layout[enemy.y][enemy.x - 1] != char(254))
                    {
                        enemy.x++;
                    }
                    break;
                default:
                    break;
                }
            }
            else if(enemy.id==1)
            {
                if (player.y < enemy.y && room.layout[enemy.y - 1][enemy.x] != '#' && room.layout[enemy.y][enemy.x - 1] != '*' && room.layout[enemy.y][enemy.x - 1] != char(176) && room.layout[enemy.y][enemy.x - 1] != 'E' && room.layout[enemy.y][enemy.x - 1] != 'O' && room.layout[enemy.y][enemy.x - 1] != char(254))
                {
                    enemy.y--;
                }
                else if (player.y > enemy.y && room.layout[enemy.y + 1][enemy.x] != '#' && room.layout[enemy.y][enemy.x - 1] != '*' && room.layout[enemy.y][enemy.x - 1] != char(176) && room.layout[enemy.y][enemy.x - 1] != 'E' && room.layout[enemy.y][enemy.x - 1] != 'O' && room.layout[enemy.y][enemy.x - 1] != char(254))
                {
                    enemy.y++;
                }
                else if (player.x < enemy.x && room.layout[enemy.y][enemy.x - 1] != '#' && room.layout[enemy.y][enemy.x - 1] != '*' && room.layout[enemy.y][enemy.x - 1] != char(176) && room.layout[enemy.y][enemy.x - 1] != 'E' && room.layout[enemy.y][enemy.x - 1] != 'O' && room.layout[enemy.y][enemy.x - 1] != char(254))
                {
                    enemy.x--;
                }
                else if (player.x > enemy.x && room.layout[enemy.y][enemy.x + 1] != '#' && room.layout[enemy.y][enemy.x - 1] != '*' && room.layout[enemy.y][enemy.x - 1] != char(176) && room.layout[enemy.y][enemy.x - 1] != 'E' && room.layout[enemy.y][enemy.x - 1] != 'O' && room.layout[enemy.y][enemy.x - 1] != char(254))
                {
                    enemy.x++;
                }
            }
            else if(enemy.id==2)
            {
                switch(enemy.Direction)
                {
                case 0:
                    if(room.layout[enemy.y-1][enemy.x]!='#')
                    {
                        enemy.y--;
                    }
                    else enemy.Direction=1;
                    break;
                case 1:
                    if(room.layout[enemy.y][enemy.x+1]!='#')
                    {
                        enemy.x++;
                    }
                    else enemy.Direction=2;
                    break;
                case 2:
                    if(room.layout[enemy.y+1][enemy.x]!='#')
                    {
                        enemy.y++;
                    }
                    else enemy.Direction=3;
                    break;
                case 3:
                    if(room.layout[enemy.y][enemy.x-1]!='#')
                    {
                        enemy.x--;
                    }
                    else enemy.Direction=0;
                }
                enemy.dist=abs(enemy.x-enemy.spawnx)+abs(enemy.y-enemy.spawny);
            if(enemy.dist>3)
            {
                enemy.x=enemy.spawnx;
                enemy.y=enemy.spawny;
            }
            }
            enemy.dist=abs(enemy.x-enemy.spawnx)+abs(enemy.y-enemy.spawny);
            if(enemy.dist>4)
            {
                enemy.x=enemy.spawnx;
                enemy.y=enemy.spawny;
            }
            if(player.x == enemy.x && player.y == enemy.y)
                GameState=0;
        }
    }
}

void playerLocation()
{
    player.roomX=player.x/7;
    player.roomY=player.y/7;
    Room& currentRoom = rooms[player.roomX][player.roomY];
    if (!currentRoom.visited)
    {
        currentRoom.visited = true;
        int points = currentRoom.points;
        point += points;
    }
    moveEnemiesInRoom(currentRoom);
}

void movePlayer(char direction)
{
    int newX = player.x;
    int newY = player.y;

    switch (direction)
    {
    case 'w':
        newY--;
        break;
    case 'a':
        newX--;
        break;
    case 's':
        newY++;
        break;
    case 'd':
        newX++;
        break;
    case 'h':
        help();
        moves--;
        break;
    default:
        break;
    }

    if(board[newY][newX] != '#')
    {
        player.x = newX;
        player.y = newY;
    }
    if(board[newY][newX]=='W')
        GameState=2;
    if(board[newY][newX]==char(254))
    {
        point+=2000;
        board[newY][newX]=' ';
    }
    if(board[newY][newX]=='E' || board[newY][newX]=='O' || board[newY][newX]==char(176) || board[newY][newX]=='*')
    {
        GameState=0;
    }
    if(board[newY][newX]=='@')
    {
        int dx = 0;
        int dy = 0;
        while (dx == 0 || dy == 0)
        {
            dx = getRandomInt(-1, 1);
            dy = getRandomInt(-1, 1);
        }

        player.x += (2 * dy);
        player.y += (2 * dx);
    }
    if(board[newY][newX]=='?')
    {
        int reward = getRandomInt(0,3);
        switch(reward)
        {
        case 0:
            point+=10000;
            break;
        case 1:
            point+=1000;
            break;
        case 2:
            if(point>2999)
                point-=3000;
            else point=0;
            break;
        case 3:
            point=0;
            break;
            default: break;
        }
        board[newY][newX]=' ';
    }
    playerLocation();
}

/// /////////////////////Enemies initialization/////////////////////////7
void createEnemies()
{
    for(int i=0; i<49; i++)
        for(int j=0; j<49; j++)
            if(board[i][j]=='O')
            {
                Enemy enemy;
                enemy.id=0;
                enemy.roomX=j/7;
                enemy.roomY=i/7;
                enemy.x=j;
                enemy.y=i;
                enemy.spawnx=j;
                enemy.spawny=i;
                board[i][j]=' ';
                enemies.push_back(enemy);
            }
            else if(board[i][j]=='E')
            {
                Enemy enemy;
                enemy.id=1;
                enemy.roomX=j/7;
                enemy.roomY=i/7;
                enemy.x=j;
                enemy.y=i;
                enemy.spawnx=j;
                enemy.spawny=i;
                board[i][j]=' ';
                enemies.push_back(enemy);
            }
            else if(board[i][j]=='*')
            {
                Enemy enemy;
                enemy.id=2;
                enemy.roomX=j/7;
                enemy.roomY=i/7;
                enemy.x=j;
                enemy.y=i;
                enemy.spawnx=j;
                enemy.spawny=i;
                board[i][j]=' ';
                if(board[i-1][j]=='#' && board[i][j-1]=='#')
                    enemy.Direction=1;
                else if(board[i-1][j]=='#' && board[i][j+1]=='#')
                    enemy.Direction=2;
                else if(board[i][j+1]=='#' && board[i+1][j]=='#')
                    enemy.Direction=3;
                enemies.push_back(enemy);
            }
}

/// MAIN /// MAIN /// MAIN /// MAIN /// MAIN /// MAIN /// MAIN /// MAIN /// MAIN /// MAIN /// MAIN ///0

void playGame()
{
    ///Reset Game state and vectors
    GameState=1;
    enemies.clear();
    rooms.clear();
    moves=0;
    point=0;
    ///Regenerate the map and objects
    map1 = createMap();
    roomGeneration();
    manipulateBoard();
    rooms = createRooms();
    buildBoard();
    createEnemies();

    for(int i=0; i<49; i++)
    {
        bool found = false;
        for(int j=0; j<49; j++)
            if(board[i][j]=='P')
            {
                player.x = j;
                player.y = i;
                found = true;
                board[i][j]=' ';
                break;
            }
        if(found==true)
            break;
    }
    while (GameState)
    {
        displayBoard();
        cout<<"Points: "<<point<<endl;
        char direction = _getch();
        movePlayer(direction);
        moves++;
        if(GameState==2) break;
    }
    if(GameState==2)
    {
        int newpoints=0;
        if(point>moves*100)
            newpoints=point-moves*100;
        player.acquiredpoints+=newpoints;
        player.totalmoves+=moves;
        system("cls");
        cout<<endl<<endl<<"   Congratulations!!! You've completed this level and earned "<<point<<" points this run."<<endl;
        cout<<"    Your total amount of points is "<<player.acquiredpoints<<"!"<<endl;
        cout<<"    You used "<<moves<<" moves to complete the level, deducting "<<moves*100<<" points from your score."<<endl;
        cout<<endl<<"    You can choose to end your run, or continue playing to obtain more points."<<endl;
        Sleep(4000);
        cout<<"    Your Score has been saved! Press any key to return to the main menu."<<endl;
        point=0;
        moves=0;
        _getch();
    } else if(GameState==0)
    {
        system("cls");
        cout<<endl<<endl<<"    Oh No! Looks like you died. You should be more careful with your moves. Take it slower next time."<<endl;
        cout<<endl<<"   This run you accumulated "<<point<<" points, though they are now lost :("<<endl;
        cout<<endl<<"    Your total acquired points are "<<player.acquiredpoints<<"!"<<endl;
        Sleep(4000);
        cout<<"   Press any key to return to the main menu."<<endl;
        _getch();
    }

}

void settings()
{
    cout<<"   Our current settings only allow you to change the name under which you are saved in the Leaderboard."<<endl;
    cout<<"   The default name is Peach. Would you like to change your name? 1 to change, 0 to go back."<<endl;
    char choice = _getch();
    if(choice=='0')
        return;
    while(choice!='1')
    {
        cout<<endl<<"   Sorry, the key you've selected is not yet applicable to any setting. Please read the menu carefully and make a new selection!"<<endl;
        choice=_getch();
    }
    cout<<"   You've selected the Name Change option. Please, enter a new name. (No character limit): ";
    string newname;
    cin>>newname;
    displayname=newname;
    cout<<"   Congratulations, You are now named "<<displayname<<"! Press any key to return to main menu.";
    _getch();
}

void readLeaderboard()
{
    Scores.clear();
    ifstream file("Leaderboard.txt");
    if(file.is_open())
    {
        string name;
        int pts;
        int mvs;
        while(file >> name >> pts >> mvs)
        {
            leaderboard Person;
            Person.name = name;
            Person.score = pts;
            Person.totalmoves = mvs;
            Scores.push_back(Person);
        }
        file.close();

        sort(Scores.begin(),Scores.end(),comparePoints);
        for(const leaderboard& person : Scores)
        {
            cout<<"Name: "<<person.name<<", Points: "<<person.score<<" in "<<person.totalmoves<<" moves."<<endl;
        }
    }else cout<<"Failed to open file?"<<endl;
    cout<<"Press any key to return to main menu.";
    _getch();
}

void playerChoice(char choice)
{
    system("cls");
    switch(choice)
    {
    case 'a':
        playGame();
        break;
    case 'b':
        readLeaderboard();
        break;
    case 'c':
        settings();
        break;
    case 'd':
        help();
        break;
    case 'p':
        map1 = createMap();
        displayMap();
        break;
    }
}

int main()
{
    srand(time(NULL));
while(true)
{
    system("cls");
    color(10);
    cout<<"|\\     |      |   |\\      |   |-------   |------     ---     |\\      |"<<endl; color(3);
    cout<<"| \\    |      |   | \\     |   |          |          /   \\    | \\     |"<<endl; color(2);
    cout<<"|  \\   |      |   |  \\    |   |          |         /     \\   |  \\    |"<<endl; color(3);
    cout<<"|   |  |      |   |   \\   |   |  ----|   |------  |       |  |   \\   |"<<endl; color(10);
    cout<<"|  /   |      |   |    \\  |   |      |   |         \\     /   |    \\  |"<<endl; color(3);
    cout<<"| /    |      |   |     \\ |   |      |   |          \\   /    |     \\ |"<<endl; color(2);
    cout<<"|/     |______|   |      \\|   |______|   |------     ---     |      \\|"<<endl; color(10);
    cout<<endl<<endl;
    cout<<"|------  \\     /    |----|  |           ---     |----|   |------   |----|"<<endl; color(3);
    cout<<"|         \\   /     |    |  |          /   \\    |    |   |         |    |"<<endl; color(2);
    cout<<"|          \\ /      |    |  |         /     \\   |    |   |         |    |"<<endl; color(3);
    cout<<"|------     X       |----|  |        |       |  |----|   |------   |----|"<<endl; color(10);
    cout<<"|          / \\      |       |         \\     /   |\\       |         |\\   "<<endl; color(3);
    cout<<"|         /   \\     |       |          \\   /    | \\      |         | \\  "<<endl; color(2);
    cout<<"|------  /     \\    |       |______     ---     |  \\     |------   |  \\ "<<endl<<endl<<endl;
    color(15);
    cout<<"                      Hello Adventurer! Are you ready to embark on a 2D adventure?"<<endl<<endl;
    cout<<"                 Please press the corresponding letter on your keyboard to select an option."<<endl<<endl;
    cout<<"                                       Main Menu"<<endl<<endl;
    cout<<"                                    a) Play Game"<<endl<<endl;
    cout<<"                                    b) View Leaderboard"<<endl<<endl;
    cout<<"                                    c) Settings"<<endl<<endl;
    cout<<"                                    d) Help"<<endl<<endl<<endl;
    cout<<"                                    p) Generate and Display Map (Random Walk Algorithm)"<<endl<<endl<<endl;
    cout<<"                                    q) Quit"<<endl<<endl;
    cout<<"   DISCLAIMER: After you're done playing, come back to main menu and press q to quit the game manually."<<endl;
    cout<<"   Otherwise your score WILL NOT be submitted into the Leaderboard."<<endl;
    cin.clear();
    char choice = _getch();
    if(choice=='q')
        {
            Scores.clear();
            leaderboard Person;
            Person.name = displayname;
            Person.score = player.acquiredpoints;
            Person.totalmoves = player.totalmoves;
            Scores.push_back(Person);

            ifstream file("Leaderboard.txt");
            if(file.is_open())
            {
                string name;
                int pts;
                int mvs;
                while(file >> name >> pts >> mvs)
                {
                    leaderboard Person;
                    Person.name = name;
                    Person.score = pts;
                    Person.totalmoves = mvs;
                    Scores.push_back(Person);
                }
                file.close();

                sort(Scores.begin(),Scores.end(),comparePoints);
                ofstream outputFile("leaderboard.txt", ios::trunc);

                if (outputFile.is_open())
                    {
                    for (const leaderboard& person : Scores)
                    {
                    outputFile << person.name << " " << person.score << " " << person.totalmoves <<endl;
                    }

                outputFile.close();
  } else cout << "Failed to open output file!" << endl;

                cout<<"Score has been saved successfully. See you next time!"<<endl<<"Press any key to close the program";
                _getch();
            } else cout << "Failed to open output file!" <<endl;


            return 0;
        }
    else while(choice!='a' && choice!='b' && choice!='c' && choice!='d' && choice!='p')
    {
        cout<<endl<<"Invalid selection! Please select an option a, b, c, d or p. Press q to exit."<<endl;
        choice = _getch();
    }
    playerChoice(choice);
}

    return 0;
}
