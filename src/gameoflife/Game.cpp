#include "Game.h"
#include "CellMap.h"

#include <QFile>

Game::Game(std:: size_t width, std::size_t height) : generation(0) {
  map = new CellMap(this, width, height);
}

Game::~Game() { }

std::size_t Game::getGeneration() {
  return generation;
}

CellMap* Game::getMap() {
  return map;
}

void Game::nextGeneration() {
  ++generation;
  map->nextGeneration();
}

//TODO: Expand world size if necessary
void Game::loadRLE(QString filename){

  map->clear();
  generation = 0;

  QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
      return;
    int width = 0;
    int height = 0;
    int nb = 0;
    QByteArray data("");
    while(!file.atEnd())
    {
      QByteArray line = file.readLine();
      if(line[0] == '#'){
        continue;
      }
      if(line[0] == 'x'){
        QList<QByteArray> list = line.split(',');
        int width = ((list[0].split('='))[1].simplified()).toInt();
        int height = ((list[1].split('='))[1].simplified()).toInt();
        continue;
      }
      data.append(line);
    }
    data = data.simplified();
    int init_x = map->getWidth()/2 - (int) (width/2);
    int init_y = map->getHeight()/2 - (int) (height/2);

    int curr_x = init_x;
    int curr_y = init_y;

    QByteArray qs("");
    for(int i = 0; i < data.length(); ++i)
    {
      if(data[i] == '\0')
        continue;
      int q;
      if(data[i] == '$'){
        q = qs.isEmpty() ? 1 : qs.toInt();
        curr_y += q;
        curr_x = init_x;
        qs.clear();
      }
      if(data[i] >= '0' && data[i] <= '9'){
        qs.append(data[i]);
      }
      if(data[i] == 'o' || data[i] == 'b'){
        q = qs.isEmpty() ? 1 : qs.toInt();
        for (int n = 0; n < q; n++) {
          map->changeCellState(curr_x,curr_y,(data[i] == 'o'));
          curr_x++;
        }
        qs.clear();
      }
      if(data[i] == '!')
        break;
    }

}

