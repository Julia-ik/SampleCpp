#include <iostream>
#include <conio.h>
#include "windows.h"
#include <thread>
#include <algorithm>
#include <list>
using namespace std;
class TRoad
{
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
public:

    int Length;
    int Width;
    char** GraphRoad;
    char RoadSymb;
    TRoad(){}
    TRoad(int length0, int width0)
    {
        RoadSymb='_';
        if(length0>0)
        {
            Length=length0;
        }

        else Length=1;
        if(width0>0)
        {
            Width=width0;
        }
        else Width=1;

        GraphRoad = new char*[Width];
        for(int i=0;i<Width;i++)
        {
             GraphRoad[i]= new char [Length];
        }
        for(int i=0;i<Width;i++)
            {
                for(int j=0;j<Length;j++)
                    {
                        GraphRoad[i][j]=RoadSymb;
                    }
            }
    }
    void PrintRoad(int color=7)
    {
        for(int i=0;i<Width;i++)
        {
           for(int j=0;j<Length;j++)
           {
              if(GraphRoad[i][j]=='o')
              {
                SetConsoleTextAttribute(hConsole,color);
              }
             else
              {
                SetConsoleTextAttribute(hConsole,7);
              }


               cout<<GraphRoad[i][j];

           }
           cout<<endl;

        }
    }

};
class TCar
{
public:
    int X,V;//так как используем граф изображение нужно было флоат заменить на инт
    int P;
    TRoad *Road;
    char GraphCar;
    int NearestTrafLight;
    TCar()
    {

    }
    TCar ( TRoad *road0, int p0, int v0 )
    {
        GraphCar='*';
        Road = road0;
        P = p0;
        V = v0;
        X = 0;
        NearestTrafLight=Road->Length+1;
    }
    void move()
    {
        Road->GraphRoad[P-1][X]=Road->RoadSymb;
        X = X + V;
        if(Road->GraphRoad[P-1][X]!=Road->RoadSymb)
           {
              X=X-1;
           }
        if ( X > Road->Length )
            {
                X = 0;
            }

        Road->GraphRoad[P-1][X]=GraphCar;

    }
    int FindTrafficLight(char light)
    {
        for(int i=X;i<Road->Length;i++)
        {
            if (Road->GraphRoad[P-1][i]==light)
            {
                return i;
            }
        }
        return Road->Length+1;

    }
};
class TrafLight
{
public:

    int Color;
    int X;
    int P;
    char GraphTrafLight;
    TRoad *Road;

    TrafLight(){}
    TrafLight(TRoad *road0, int x0, int p0)
    {
        Road=road0;
        X=x0;
        P=p0;
        Color=2;
        GraphTrafLight='o';
        SetOnRoad();
    }
    void SetColor()
    {
        for(;;)
        {
            Color=2;
             this_thread::sleep_for(chrono::milliseconds(5000));
            Color=14;
             this_thread::sleep_for(chrono::milliseconds(3000));
            Color=4;
             this_thread::sleep_for(chrono::milliseconds(5000));
        }
    }
    void SetOnRoad()
    {
        Road->GraphRoad[P-1][X]=GraphTrafLight;
    }
};

int main()
{
   TRoad road(60,3);
   list<TrafLight*> svetofori;
   TrafLight *svetofor1 = new TrafLight(&road, 15, 1);
   TrafLight *svetofor4 = new TrafLight(&road, 38, 1);
   TrafLight *svetofor2 = new TrafLight(&road, 22, 2);
   TrafLight *svetofor3 = new TrafLight(&road, 26, 3);
   svetofori.push_front(svetofor1);
   svetofori.push_front(svetofor2);
   svetofori.push_front(svetofor3);
   svetofori.push_front(svetofor4);
   road.PrintRoad(svetofor1->Color);
   thread t1(&TrafLight::SetColor,svetofor1);
   thread t2(&TrafLight::SetColor,svetofor2);
   thread t3(&TrafLight::SetColor,svetofor3);
   thread t4(&TrafLight::SetColor,svetofor4);
   const int N=3;
   TCar *cars= new TCar[N];
   for(int i=0;i<N;i++)
        {
            cars[i]= *(new TCar(&road, i+1,2*(i+1)));
        }
   do
    {   this_thread::sleep_for(chrono::milliseconds(500));


        for(int i=0;i<N;i++)
        {
            TrafLight* gdeSvetofor;

                cars[i].NearestTrafLight=cars[i].FindTrafficLight(svetofor1->GraphTrafLight);

                for (TrafLight* svet : svetofori)
                {
                    if(svet->P==cars[i].P&&svet->X==cars[i].NearestTrafLight)
                    {
                        gdeSvetofor=svet;
                     }
                }
            if(gdeSvetofor->Color==2||(gdeSvetofor->X-cars[i].X)>cars[i].V||gdeSvetofor->X<cars[i].X)
                {
                    cars[i].move();
                }


        system("cls");
        road.PrintRoad(svetofor1->Color);

        }
     }
    while(!kbhit());
    t1.detach();
    t2.detach();
    t3.detach();
    t4.detach();
    return 0;
}

