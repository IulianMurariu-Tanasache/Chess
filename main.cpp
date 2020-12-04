#include <SFML/Graphics.hpp>
#include<iostream>
#include<conio.h>
#include<windows.h>
#include <ctime>
#include <stdlib.h>
#define PAWN 7
#define ROOK 2
#define KNIGHT 5
#define BISHOP 3
#define KING 99
#define QUEEN 6
#define _B 10
#define _W 14
#define Log(x) cout<<x<<endl;

using namespace sf;
using namespace std;

struct coord{int x,y;}a;//pozitia piesei selectate

bool turn,stop;//turn-> a cui este tura//stop->daca jocul s-a incheiat

int chesspieces[8][8]= //tabla si piesele
                            {
                                {ROOK,KNIGHT,BISHOP,KING,QUEEN,BISHOP,KNIGHT,ROOK},
                                {PAWN,PAWN,PAWN,PAWN,PAWN,PAWN,PAWN,PAWN},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {PAWN,PAWN,PAWN,PAWN,PAWN,PAWN,PAWN,PAWN},
                                {ROOK,KNIGHT,BISHOP,KING,QUEEN,BISHOP,KNIGHT,ROOK}
                            };

int chesspiecescolor[8][8]= //culoarea fiecarei piese
                                {
                                    {_B,_B,_B,_B,_B,_B,_B,_B},
                                    {_B,_B,_B,_B,_B,_B,_B,_B},
                                    {0,0,0,0,0,0,0,0},
                                    {0,0,0,0,0,0,0,0},
                                    {0,0,0,0,0,0,0,0},
                                    {0,0,0,0,0,0,0,0},
                                    {_W,_W,_W,_W,_W,_W,_W,_W},
                                    {_W,_W,_W,_W,_W,_W,_W,_W}
                                };

bool verfiMutare(int x,int y,int xdir,int ydir,int piece)//verifica daca este o mutare corecta in fucntie de piesa aleasa
{
    switch(piece)
    {
    case ROOK:
        {if(!(xdir==x||ydir==y))
            return false;
        return true;}
    case BISHOP:
        {if(abs(xdir-x)!=abs(ydir-y))
            return false;
        return true;}
    case KNIGHT:
        {if((abs(xdir-x)!=2||abs(ydir-y)!=1)&&(abs(xdir-x)!=1||abs(ydir-y)!=2))
            return false;
        return true;}
    case QUEEN:
    {bool ok1 = verfiMutare(x,y,xdir,ydir,ROOK);
    bool ok2 = verfiMutare(x,y,xdir,ydir,BISHOP);
    if(ok1==false&&ok2==false)
        return false;
    return true;}
    case KING:
    {
    if(abs(xdir-x)>1||abs(ydir-y)>1)
        return false;
    bool okey = verfiMutare(x,y,xdir,ydir,QUEEN);
    if(okey==false)
        return false;
    return true;
    }
    }
    return false;
}

bool verificare(int x,int y,int xdir,int ydir,int piece)//verifica daca in drumul piesei nu se afal alte piese peste care nu poate sari
{
    int i;
    bool ok = verfiMutare(x,y,xdir,ydir,piece);
    if(ok==false)
        return false;
    if((turn==false&&chesspiecescolor[xdir][ydir]==_W)||(turn==true&&chesspiecescolor[xdir][ydir]==_B))
        return false;
    if(piece==KNIGHT)
        return true;
    else{
    if(x==xdir)
    {
        if(ydir>y)
        {for(i=y+1;i<ydir;i++)
            if(chesspieces[x][i]!=0)
                return false;
        return true;}
        else
        {
         for(i=y-1;i>ydir;i--)
            if(chesspieces[x][i]!=0)
                return false;
        return true;
        }
    }
    else if(y==ydir)
    {
         if(xdir>x)
        {for(i=x+1;i<xdir;i++)
            if(chesspieces[i][y]!=0)
            return false;
        return true;}
        else
        {
         for(i=x-1;i>xdir;i--)
            if(chesspieces[i][y]!=0)
            return false;
        return true;
        }
    }
    else
    {
        int c=abs(xdir-x);
        if(xdir>x&&ydir>y)
        {for(i=1;i<c;i++)
            if(chesspieces[x+i][y+i]!=0)
            return false;
        return true;}
        else if(xdir<x&&ydir<y)
        {
         for(i=1;i<c;i++)
            if(chesspieces[x-i][y-i]!=0)
            return false;
        return true;
        }
        else if(xdir>x&&ydir<y)
        {
            for(i=1;i<c;i++)
            if(chesspieces[x+i][y-i]!=0)
            return false;
        return true;
        }
        else
        {
            for(i=1;i<c;i++)
            if(chesspieces[x-i][y+i]!=0)
            return false;
        return true;
        }
    }
    }
}

void muta(int x,int y,int xdir, int ydir,int piece)//muta piesa aleasa si schimba tura
{
    chesspieces[xdir][ydir]=piece;
    chesspieces[x][y]=0;
    if(turn==false)
    chesspiecescolor[xdir][ydir]=_W;
    else
    chesspiecescolor[xdir][ydir]=_B;
    chesspiecescolor[x][y]=0;
    turn = !turn;

}

int checkWin()//verifica daca mai exista ambii regi//daca un rege este mancat, atunci jocul se opreste
{
    int i,j;
    bool white=false,black=false;
    for(i=0;i<8;i++)
        for(j=0;j<8;j++)
            if(chesspieces[i][j]==KING)
        {
            if(chesspiecescolor[i][j]==_W)
                white=true;
            else
                black=true;
        }
    if(white==false)
        return 1;
    else if(black==false)
        return 2;
    else if(white==true&&black==true)
        return 0;

}

void promote()//promoveaza pionii care ajung in capatul tablei in regine
{
    for(int i = 0;i < 8;i++)
        if(chesspieces[0][i]==PAWN && chesspiecescolor[0][i]==_W)chesspieces[0][i]=QUEEN;
        else if(chesspieces[7][i]==PAWN && chesspiecescolor[7][i]==_B)chesspieces[7][i]=QUEEN;
}

int main()
{
    // Create the main window
    RenderWindow app(VideoMode(600, 600), "CHESS",Style::Close|Style::Titlebar);

    int colt=36,mX,mY,d_patrat=66;//colt->pozitia coltului stanga sus al table//mx,my->pozitia mouselui in matricea tabla//d_patrat->dimensiunea unui patra de pe tabla

    // incarcam texturi
    Texture board,pieces,green;
    board.loadFromFile("board.png");
    pieces.loadFromFile("pieces.png");
    green.loadFromFile("selected.png");

    //cream sprite-urile/imaginile

    Sprite background,piece,selected;
    background.setTexture(board);
    background.scale(Vector2f(0.8333,0.8333));
    piece.setTexture(pieces);
    piece.scale(Vector2f(0.32,0.32));
    selected.setTexture(green);
    selected.setScale(Vector2f(0,0));

    Font font;
    font.loadFromFile("font.otf");
    Text text;
    text.setFont(font);
    text.setCharacterSize(150);
    text.setPosition(65,150);

    int i,j,offsetX=25,offsetY=10,isPicked=0;//offsetX/offsetY->muta totul la dreapta pentru a lasa loc pentru rama tablei
    Vector2i mousePos;//pozitia mouse-ului

    app.setFramerateLimit(15);
	// Start the game loop
    while (app.isOpen())
    {
        // Process events
        Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == Event::Closed)
                app.close();
            if((event.type == Event::KeyPressed||event.type == Event::MouseButtonPressed)&&stop==true)//inchide jocul dupa ce se opreste
            {
                app.close();
            }
        }

        if(Keyboard::isKeyPressed(Keyboard::R))//reseteaza piesa aleasa//in caz de orice
        {
            isPicked=0;
        }

        if(isPicked==0)//daca nu avem piesa selectata, patratul verde dispare
            selected.scale(0,0);

        if(Mouse::isButtonPressed(Mouse::Left))//daca se apasa click stanga
        {
            mousePos= Mouse::getPosition(app);//obtinem pozitia mouse-ului care trebuie transformata in coordonate pentru matricea tabla
            for(i=0;i<=7;i++)
                if(mousePos.x>colt+i*d_patrat&&mousePos.x<colt+(i+1)*d_patrat)
                    mX=i;
            for(i=0;i<=7;i++)
                if(mousePos.y>colt+i*d_patrat&&mousePos.y<colt+(i+1)*d_patrat)
                    mY=i;
            //Log(chesspieces[mY][mX]);
            //verficam daca putem alege piesa selectata, in functie de culoarea ei si a cui este tura//daca o putem alege, o marcam cu un patrat verde sa nu uitam ce am ales
            if(turn==false&&chesspiecescolor[mY][mX]==_W)
                {isPicked=chesspieces[mY][mX];a.x=mX;a.y=mY;selected.setScale(Vector2f(0.829,0.829));selected.setPosition(mX*67+offsetX+3,mY*67+28);}
            else if(turn==true&&chesspiecescolor[mY][mX]==_B)
                {isPicked=chesspieces[mY][mX];a.x=mX;a.y=mY;selected.setScale(Vector2f(0.829,0.829));selected.setPosition(mX*67+offsetX+3,mY*67+28);}
            else if(isPicked!=0&&(chesspieces[mY][mX]==0||((turn==false&&chesspiecescolor[mY][mX]==_B)||(turn==true&&chesspiecescolor[mY][mX]==_W))))
                {
                   // Log(isPicked);Log(mX);Log(mY);Log(a.x);Log(a.y);
                   //daca vrem sa mutam o piesa, trebuie sa verficam daca mutarea este valida, in functie de piesa
                    switch(isPicked)
            {
            case PAWN:
                if(turn==false)
                {if((mX==a.x&&mY==a.y-1&&chesspieces[mY][mX]==0)||(mX==a.x&&mY==a.y-2&&chesspieces[mY][mX]==0&&a.y==6))
                {
                    chesspieces[mY][mX]=PAWN;
                    chesspieces[a.y][a.x]=0;
                    chesspiecescolor[mY][mX]=_W;
                    chesspiecescolor[a.y][a.x]=0;
                    isPicked=0;
                    turn = true;
                }
                if(mY==a.y-1&&(mX==a.x+1||mX==a.x-1)&&chesspiecescolor[mY][mX]==_B&&chesspieces[mY][mX]!=KING)
                   {
                    chesspieces[mY][mX]=PAWN;
                    chesspieces[a.y][a.x]=0;
                    chesspiecescolor[mY][mX]=_W;
                    chesspiecescolor[a.y][a.x]=0;
                    isPicked=0;
                    turn = true;
                    }}
                else
                {if((mX==a.x&&mY==a.y+1&&chesspieces[mY][mX]==0)||(mX==a.x&&mY==a.y+2&&chesspieces[mY][mX]==0&&a.y==1))
                {
                    chesspieces[mY][mX]=PAWN;
                    chesspieces[a.y][a.x]=0;
                    chesspiecescolor[mY][mX]=_B;
                    chesspiecescolor[a.y][a.x]=0;
                    isPicked=0;
                    turn = false;
                }
                if(mY==a.y+1&&(mX==a.x+1||mX==a.x-1)&&chesspiecescolor[mY][mX]==_W)
                   {
                    chesspieces[mY][mX]=PAWN;
                    chesspieces[a.y][a.x]=0;
                    chesspiecescolor[mY][mX]=_B;
                    chesspiecescolor[a.y][a.x]=0;
                    isPicked=0;
                    turn = false;
                    }

                }
                break;
            default:
                {bool ok;
                ok = verificare(a.y,a.x,mY,mX,isPicked);
                if(ok==false)
                    break;
                muta(a.y,a.x,mY,mX,isPicked);
                isPicked=0;
                break;}
            }

        }}

        promote();//verficam daca trebuie sa promovam vreun pion

        // Clear screen
        app.clear();

        // Draw the sprite
        app.draw(background);
        app.draw(selected);


        //desenam fiecare piesa
        for(i=0;i<8;i++)
            for(j=0;j<8;j++)
            switch(chesspieces[i][j])
        {
        default:
            break;
        case PAWN:
            if(chesspiecescolor[i][j]==_W)
            piece.setTextureRect(IntRect(1,1,200,320));
            else
            piece.setTextureRect(IntRect(990,310,220,320));
            piece.setPosition(j*67.5+offsetX,i*67.5-offsetY);
            app.draw(piece);
            break;
        case ROOK:
            if(chesspiecescolor[i][j]==_W)
            piece.setTextureRect(IntRect(190,1,200,320));
            else
            piece.setTextureRect(IntRect(782,310,200,320));
            piece.setPosition(j*67.5+offsetX,i*67.5-offsetY);
            app.draw(piece);
            break;
         case KNIGHT:
            if(chesspiecescolor[i][j]==_W)
            piece.setTextureRect(IntRect(390,1,200,320));
            else
            piece.setTextureRect(IntRect(582,310,200,320));
            piece.setPosition(j*67.5+offsetX,i*67.5-offsetY);
            app.draw(piece);
            break;
         case BISHOP:
            if(chesspiecescolor[i][j]==_W)
            piece.setTextureRect(IntRect(585,1,200,320));
            else
            piece.setTextureRect(IntRect(395,310,200,320));
            piece.setPosition(j*67.5+offsetX,i*67.5-offsetY);
            app.draw(piece);
            break;
         case QUEEN:
            if(chesspiecescolor[i][j]==_W)
            piece.setTextureRect(IntRect(765,1,200,320));
            else
            piece.setTextureRect(IntRect(195,310,200,320));
            piece.setPosition(j*67.5+offsetX,i*67.5-offsetY);
            app.draw(piece);
            break;
         case KING:
            if(chesspiecescolor[i][j]==_W)
            piece.setTextureRect(IntRect(970,1,200,320));
            else
            piece.setTextureRect(IntRect(1,310,200,320));
            piece.setPosition(j*67.5+offsetX,i*67.5-offsetY);
            app.draw(piece);
            break;
        }
        if(turn==false)
            cout<<"white"<<endl;
        else
            cout<<"black"<<endl;

        //Log(isPicked);
        app.draw(piece);

        //daca avem un castigator, oprim jocul si afisam text

        if(checkWin()==2)
        {
            stop=true;
            char s[] = "White  Wins" ;
            text.setString(s);
            //text.setColor(Color::White);
            app.draw(text);
        }
        else if(checkWin()==1)
        {
            stop = true;
            char s[] = "Black  Wins" ;
            text.setString(s);
            //text.setColor(Color::Black);
            app.draw(text);
        }

         // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
