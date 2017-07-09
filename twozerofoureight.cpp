#include<iostream>
#include <stdio.h>
#include <conio.h>
#include <ctime>
#include <unistd.h>
#include <stdlib.h>
#include <cwchar>
#include <windows.h>
#include <wincon.h>


using namespace std;
#define boardSize 4
#define FIVE_HASHES "######"
#define ONE_LABEL "  #  "
#define BOUND_LABEL "#    #"
#define HASH_RIGHT_LABEL "     #"
int gameScore;
COORD coord={0,0}; // this is global variable
                                    //center of axis is set to the top left cornor of the screen
 void gotoxy(int x,int y)
 {
     coord.X=x;
     coord.Y=y;
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
 }
void endLogic();
void showGameScore();

void drawLayout()
{
    for(int i=0;i<42;i+=10)
    {
        for(int j=0;j<17;j++)
        {
            gotoxy(i,j);
            cout<<"||";
        }
    }

    for(int i=0;i<17;i+=4)
    {
        for(int j=0;j<42;j++)
        {
            gotoxy(j,i);
            cout<<"=";
        }
    }
}

void setTwoStartEntries(int arrayBoard[][boardSize])
{
    srand ( time(NULL) );
    int i=0;

    int firstPosX = (rand() % boardSize);
    int firstPosY = (rand() % boardSize);
    int secondPosX, secondPosY;
    do
    {
        secondPosX = (rand() % boardSize);
        secondPosY = (rand() % boardSize);
    }while((secondPosX == firstPosX) && (secondPosY == secondPosY));
    arrayBoard[firstPosX][firstPosY] = (rand() % 2) == 0 ? 2 : 4;
    arrayBoard[secondPosX][secondPosY] = (rand() % 2) == 0 ? 2 : 4;
}

void generateNewEntry(int arrayBoard[][boardSize])
{
    srand ( time(NULL) );
    int posX;
    int posY;
    do
    {
        posX = (rand() % boardSize);
        posY = (rand() % boardSize);
    }while(arrayBoard[posX][posY] != 0);

    arrayBoard[posX][posY] = (rand() % 2) == 0 ? 2 : 4;;
}
void initValues(int arrayBoard[][4])
{
    for(int i=0;i<boardSize;i++)
    {
        for(int j=0;j<boardSize;j++)
        {
            arrayBoard[i][j]=0;
        }
    }
    setTwoStartEntries(arrayBoard);
}

void setValues(int arrayBoard[][boardSize])
{
    for(int i=5, boardIndexRow = 0;i<42;i+=10, boardIndexRow++)
    {
        for(int j=2, boardIndexCol = 0;j<17;j+=4, boardIndexCol++)
        {
            gotoxy(i,j);
            cout<<"     ";
            gotoxy(i,j);
            cout<<arrayBoard[boardIndexRow][boardIndexCol];
        }
    }
}
void drawBoard(int arrayBoard[][boardSize])
{

    setValues(arrayBoard);
    endLogic();
}

void initBoard(int arrayBoard[][4])
{
    initValues(arrayBoard);
    drawLayout();
    drawBoard(arrayBoard);
    showGameScore();
}

void endLogic()
{
    gotoxy(0,21);
}
void gotoEndGame()
{
    gotoxy(0, 23);
}
void shiftRightBy1(int arrayBoard[][boardSize], int x, int y)
{
    while(x>0)
    {
        arrayBoard[x][y] = arrayBoard[x-1][y];
        x--;
    }
    arrayBoard[x][y] = 0;
}

void shiftLeftBy1(int arrayBoard[][boardSize], int x, int y)
{
    while(x<boardSize-1)
    {
        arrayBoard[x][y] = arrayBoard[x+1][y];
        x++;
    }
    arrayBoard[x][y] = 0;
}

void shiftUpBy1(int arrayBoard[][boardSize], int y, int x)
{
    while(y<boardSize-1)
    {
        arrayBoard[x][y] = arrayBoard[x][y+1];
        y++;
    }
    arrayBoard[x][y] = 0;
}

void shiftDownBy1(int arrayBoard[][boardSize], int y, int x)
{
    while(y>0)
    {
        arrayBoard[x][y] = arrayBoard[x][y-1];
        y--;
    }
    arrayBoard[x][y] = 0;
}
bool isZerosRow(int arrayBoard[][boardSize], int i, int y)
{
    while(i>=0)
        if(arrayBoard[i--][y]!=0)
            return false;
    return true;
}

bool isZerosRightOf(int arrayBoard[][boardSize], int i, int y)
{
    while(i<boardSize)
        if(arrayBoard[i++][y]!=0)
            return false;
    return true;
}

bool isZerosToUp(int arrayBoard[][boardSize], int i, int x)
{
    while(i>=0)
        if(arrayBoard[x][i--]!=0)
            return false;
    return true;
}

bool isZerosToDown(int arrayBoard[][boardSize], int i, int x)
{
    while(i<boardSize)
        if(arrayBoard[x][i++]!=0)
            return false;
    return true;
}

void rightCollapse(int arrayBoard[][boardSize], int y)
{
    for(int i = boardSize-1; i>0 ;i--)
    {
        if(arrayBoard[i][y]==0)
        {
            if(arrayBoard[i-1][y]==0 && !isZerosRow(arrayBoard, i-2,  y))
            {
                shiftRightBy1(arrayBoard, i, y);
                i++;
            }
            else
            {
                arrayBoard[i][y] = arrayBoard[i-1][y];
                arrayBoard[i-1][y] = 0;
                shiftRightBy1(arrayBoard, i-1, y);
            }
        }
    }
}

void leftCollapse(int arrayBoard[][boardSize], int y)
{
    for(int i = 0; i<boardSize-1 ;i++)
    {
        if(arrayBoard[i][y]==0)
        {
            if(arrayBoard[i+1][y]==0 && !isZerosRightOf(arrayBoard, i+2,  y))
            {
                shiftLeftBy1(arrayBoard, i, y);
                i--;
            }
            else
            {
                arrayBoard[i][y] = arrayBoard[i+1][y];
                arrayBoard[i+1][y] = 0;
                shiftLeftBy1(arrayBoard, i+1, y);
            }

        }
    }
}

void onRightKeyPressed(int arrayBoard[][boardSize])
{
    for(int y = 0; y < boardSize; y++)
    {
        rightCollapse(arrayBoard, y);

        for(int x = boardSize-1 ; x > 0 ; x--)
        {
            if(arrayBoard[x][y] == arrayBoard[x-1][y])
            {
                arrayBoard[x][y]+=arrayBoard[x-1][y];
                gameScore+=arrayBoard[x][y];
                shiftRightBy1(arrayBoard, x-1, y);
            }
        }
    }
}

void onLeftKeyPressed(int arrayBoard[][boardSize])
{
    for(int y = 0; y < boardSize; y++)
    {
        leftCollapse(arrayBoard, y);

        for(int x = 0; x < boardSize-1 ; x++)
        {
            if(arrayBoard[x][y] == arrayBoard[x+1][y])
            {
                arrayBoard[x][y]+=arrayBoard[x+1][y];
                gameScore+=arrayBoard[x][y];
                shiftLeftBy1(arrayBoard, x+1, y);
            }
        }
    }
}

void upCollapse(int arrayBoard[][boardSize], int x)
{
    for(int i = 0; i<boardSize-1 ;i++)
    {
        if(arrayBoard[x][i]==0)
        {
            if(arrayBoard[x][i+1]==0 && !isZerosToDown(arrayBoard, i+2, x))
            {
                shiftUpBy1(arrayBoard, i, x);
                i--;
            }
            else
            {
                arrayBoard[x][i] = arrayBoard[x][i+1];
                arrayBoard[x][i+1] = 0;
                shiftUpBy1(arrayBoard, i+1, x);
            }
        }
    }
}

void downCollapse(int arrayBoard[][boardSize], int x)
{
    for(int i = boardSize-1; i>0 ;i--)
    {
        if(arrayBoard[x][i]==0)
        {
            if(arrayBoard[x][i-1]==0 && !isZerosToUp(arrayBoard, i-2, x))
            {
                shiftDownBy1(arrayBoard, i, x);
                i++;
            }
            else
            {
                arrayBoard[x][i] = arrayBoard[x][i-1];
                arrayBoard[x][i-1] = 0;
                shiftDownBy1(arrayBoard, i-1, x);
            }
        }
    }
}

void onUpKeyPressed(int arrayBoard[][boardSize])
{
    for(int x = 0; x < boardSize; x++)
    {
        upCollapse(arrayBoard, x);
        for(int y = 0; y < boardSize-1 ; y++)
        {
            if(arrayBoard[x][y] == arrayBoard[x][y+1])
            {
                arrayBoard[x][y]+=arrayBoard[x][y+1];
                gameScore+=arrayBoard[x][y];
                shiftUpBy1(arrayBoard, y+1, x);
            }
        }
    }
}

void onDownKeyPressed(int arrayBoard[][boardSize])
{
    for(int x = 0; x < boardSize; x++)
    {
        downCollapse(arrayBoard, x);
        for(int y = boardSize-1; y > 0 ; y--)
        {
            if(arrayBoard[x][y] == arrayBoard[x][y-1])
            {
                arrayBoard[x][y]+=arrayBoard[x][y-1];
                gameScore+=arrayBoard[x][y];
                shiftDownBy1(arrayBoard, y-1, x);
            }
        }
    }
}

bool checkGameOver(int arrayBoard[][boardSize])
{
    for(int i=0;i<boardSize;i++)
        for(int j=0;j<boardSize;j++)
            if(arrayBoard[i][j]==0)
                return false;
    return true;
}

void showGameScore()
{
    endLogic();
    cout<<"SCORE: "<<gameScore;
}

void driveGame(int arrayBoard[][boardSize])
{
    int count = 0;
    bool lock = false;
    endLogic();
    while(1)
    {
        if (kbhit())
        {
            char key =_getch();
            if(lock)
            {
                switch(key)
                {
                  case 72:
                    onUpKeyPressed(arrayBoard);
                    break;
                  case 80:
                    onDownKeyPressed(arrayBoard);
                    break;
                  case 75:
                    onLeftKeyPressed(arrayBoard);
                    break;
                  case 77:
                    onRightKeyPressed(arrayBoard);
                    break;
                }
                if(checkGameOver(arrayBoard))
                {
                    drawBoard(arrayBoard);
                    break;
                }
                generateNewEntry(arrayBoard);
                drawBoard(arrayBoard);
                showGameScore();
                count++;
            }
            lock = !lock;
        }
        usleep(1000);

    }
}
/*
https://stackoverflow.com/a/18028927
*/
void showConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void displayGameName()
{
    int x,y; x = 14, y=17;
    //2
    for(int yTemp = y;yTemp<=y+4;yTemp++)
    {
        gotoxy(x,yTemp);
        if((yTemp%2)==1)
        {
            cout<<FIVE_HASHES;
        }
        else
        {
            cout<<((yTemp == (y+1)) ? HASH_RIGHT_LABEL : "#");
        }
    }

    x = x + 7;
    //0
    for(int yTemp = y;yTemp<=y+4;yTemp++)
    {
        if(yTemp == y || yTemp == (y+4))
        {
            gotoxy(x,yTemp);
            cout<<FIVE_HASHES;
        }
        else
        {
            gotoxy(x,yTemp);
            cout<<BOUND_LABEL;
        }
    }

    x = x + 7;
    //4
    for(int yTemp = y;yTemp<=y+1;yTemp++)
    {
        gotoxy(x,yTemp);
        cout<<BOUND_LABEL;
    }
    gotoxy(x,y+2);
    cout<<FIVE_HASHES;

    for(int yTemp = y+3;yTemp<=y+4;yTemp++)
    {
        gotoxy(x,yTemp);
        cout<<HASH_RIGHT_LABEL;
    }

    //8
    x = x + 7;
    for(int yTemp = y;yTemp<=y+4;yTemp++)
    {
        if((yTemp & 1) == 1)
        {
            gotoxy(x,yTemp);
            cout<<FIVE_HASHES;
        }
        else
        {
            gotoxy(x,yTemp);
            cout<<BOUND_LABEL;
        }
    }

}
int main()
{

    int arrayBoard[boardSize][boardSize];

    char ch='n';
    do
    {

        switch(ch)
        {
            case 'n':
                showConsoleCursor(false);
                system("cls");
                gameScore = 0;
                displayGameName();
                initBoard(arrayBoard);
                driveGame(arrayBoard);
                gotoEndGame();
                cout<<"Press n to restart, x to exit";
                break;
        }

        ch = getch();
    }while(ch!='x');
}
