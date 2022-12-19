// Ejercicio1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <conio.h>
#include <windows.h>

struct TEntity;
typedef void (*funcEntity)(TEntity*);

constexpr int s_iMaxHeight = 30;
constexpr int s_iMaxWidth = 100;
unsigned int uKey;
int iDirection;
int iKills = 0;
bool bullet = false;
bool game = true;

// ***************************************************************************************
// ENTIDAD
// ***************************************************************************************
struct TEntity
{
  int m_ix;
  int m_iy;
  funcEntity m_move;
  funcEntity m_draw;
  TEntity(funcEntity move, funcEntity draw, int x, int y)
  {
    m_ix = x;
    m_iy = y;
    m_move = move;
    m_draw = draw;
  }
};

void Stay(TEntity* _pEntity)
{

}
void MoveRight(TEntity* _pEntity)
{
  _pEntity->m_ix++;
  if (_pEntity->m_ix > s_iMaxWidth)
  {
    _pEntity->m_ix = 0;
  }
}
void MoveLeft(TEntity* _pEntity)
{
  _pEntity->m_ix--;
  if (_pEntity->m_ix < 0)
  {
    _pEntity->m_ix = s_iMaxWidth;
  }
}

void MoveUp(TEntity* _pEntity)
{
  _pEntity->m_iy--;
  if (_pEntity->m_iy < 0)
  {
    _pEntity->m_iy = s_iMaxHeight;
  }
}
void MoveDown(TEntity* _pEntity)
{
  _pEntity->m_iy++;
  if (_pEntity->m_iy > s_iMaxHeight)
  {
    _pEntity->m_iy = 0;
  }
}
void gotoxy(int x, int y)
{
  COORD pos = { x, y };
  HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(output, pos);
}


void PaintPlayer(TEntity* _pEntity)
{
  gotoxy(_pEntity->m_ix, _pEntity->m_iy);
  printf("A");
}

void PaintEnemy(TEntity* _pEntity)
{
  gotoxy(_pEntity->m_ix, _pEntity->m_iy);
  printf("E");
}
void PaintBullet(TEntity* _pEntity)
{
  switch (iDirection)
  {
  case 1:
    gotoxy((_pEntity->m_ix) - 1, _pEntity->m_iy);
    break;
  case 2:
    gotoxy((_pEntity->m_ix) + 1, _pEntity->m_iy);
    break;
  case 3:
    gotoxy(_pEntity->m_ix, (_pEntity->m_iy) - 1);
    break;
  case 4:
    gotoxy(_pEntity->m_ix, (_pEntity->m_iy) + 1);
    break;
  default:
    gotoxy((_pEntity->m_ix) + 1, _pEntity->m_iy);
    break;
  }
  printf("O");
}


void clear()
{
  system("cls");
}


void CheckIfWillCollide(TEntity* entity1, TEntity* entity2)
{
  if (entity1->m_ix == entity2->m_ix && (entity1->m_iy == entity2->m_iy - 1 || entity1->m_iy == entity2->m_iy + 1))
  {
    bullet = false;
    /*delete(entity1);
    delete(entity2);*/
    iKills++;
  }
  else if ((entity1->m_ix == entity2->m_ix + 1 || entity1->m_ix == entity2->m_ix - 1) && entity1->m_iy == entity2->m_iy)
  {
    bullet = false;
    /*delete(entity1);
    delete(entity2);*/
    iKills++;
  }
  else if (entity1->m_ix == entity2->m_ix && entity1->m_iy == entity2->m_iy)
  {
    bullet = false;
    iKills++;
  }
}

void CheckIfWillCollideWithPlayer(TEntity* entity1, TEntity* entity2)
{
  if (entity1->m_ix == entity2->m_ix && (entity1->m_iy == entity2->m_iy - 1 || entity1->m_iy == entity2->m_iy + 1))
  {
    delete(entity1);
    delete(entity2);
    game = false;
  }
  else if ((entity1->m_ix == entity2->m_ix + 1 || entity1->m_ix == entity2->m_ix - 1) && entity1->m_iy == entity2->m_iy)
  {
    delete(entity1);
    delete(entity2);
    game = false;
  }
  else if (entity1->m_ix == entity2->m_ix && entity1->m_iy == entity2->m_iy)
  {
    delete(entity1);
    delete(entity2);
    game = false;
  }
}

// ***************************************************************************************
// MAIN
// ***************************************************************************************
int main(int argc, char* argv[])
{
  TEntity* tEntities[1];
  TEntity* tEntitiesEnemies[1];
  funcEntity tFuncsEnt0[2];
  tEntities[0] = new TEntity(&Stay, &PaintPlayer, 0, 0);
  tEntitiesEnemies[0] = new TEntity(&MoveDown, &PaintEnemy, 7, 3);

  TEntity* tEntityBullet[1];

  while (true)
  {
    clear();


    CheckIfWillCollideWithPlayer(tEntities[0], tEntitiesEnemies[0]);
    if (!game) break;

    //shootPlayer
    if (GetKeyState('F') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
    {
      if (!bullet)
      {
        iDirection = uKey;
        switch (iDirection)
        {
        case 1:
          tEntityBullet[0] = new TEntity(&MoveLeft, &PaintBullet, tEntities[0]->m_ix, tEntities[0]->m_iy);
          break;
        case 2:
          tEntityBullet[0] = new TEntity(&MoveRight, &PaintBullet, tEntities[0]->m_ix, tEntities[0]->m_iy);
          break;
        case 3:
          tEntityBullet[0] = new TEntity(&MoveUp, &PaintBullet, tEntities[0]->m_ix, tEntities[0]->m_iy);
          break;
        case 4:
          tEntityBullet[0] = new TEntity(&MoveDown, &PaintBullet, tEntities[0]->m_ix, tEntities[0]->m_iy);
          break;
        default:
          tEntityBullet[0] = new TEntity(&MoveRight, &PaintBullet, tEntities[0]->m_ix, tEntities[0]->m_iy);
          break;
        }
        bullet = true;

      }
    }

    //move bullet
    if (bullet)
    {
      tEntityBullet[0]->m_move(tEntityBullet[0]);
      tEntityBullet[0]->m_draw(tEntityBullet[0]);
      //destroy bullet
      if (tEntityBullet[0]->m_ix == 0 || tEntityBullet[0]->m_ix == s_iMaxWidth)
      {
        bullet = false;
        delete(tEntityBullet[0]);
      }
      if (tEntityBullet[0]->m_iy == 0 || tEntityBullet[0]->m_iy == s_iMaxHeight)
      {
        bullet = false;
        delete(tEntityBullet[0]);
      }
      CheckIfWillCollide(tEntityBullet[0], tEntitiesEnemies[0]);
    }

    //Movement player
    if (GetKeyState('A') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
    {
      uKey = 1;
      // Do stuff
      MoveLeft(tEntities[0]);
      tEntities[0]->m_move(tEntities[0]);
    }
    if (GetKeyState('D') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
    {
      uKey = 2;
      // Do stuff
      MoveRight(tEntities[0]);
      tEntities[0]->m_move(tEntities[0]);
    }
    if (GetKeyState('W') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
    {
      uKey = 3;
      MoveUp(tEntities[0]);
      tEntities[0]->m_move(tEntities[0]);
    }
    if (GetKeyState('S') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
    {
      uKey = 4;
      MoveDown(tEntities[0]);
      tEntities[0]->m_move(tEntities[0]);
    }
    tEntities[0]->m_draw(tEntities[0]);


    //move enemy
      tEntitiesEnemies[0]->m_move(tEntitiesEnemies[0]);
      tEntitiesEnemies[0]->m_draw(tEntitiesEnemies[0]);

    //print the kills
    gotoxy(s_iMaxWidth + 1, 0);
    printf("%d", iKills);

    Sleep(300);
  }
}
