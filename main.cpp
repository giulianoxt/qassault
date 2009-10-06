#include <QMetaType>
#include <QApplication>
#include <QTime>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <fstream>
#include "game.h"
#include "assaultmainwindow.h"
using namespace std;
#include "minimax.h"
#include "aiplayer.h"
#include <QSet>

int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);
    qRegisterMetaType<Move>("Move");
    
    srand(time(0));
    GameState::initZobrist();
    

    AssaultMainWindow w;
    w.show();
    
    a.setQuitOnLastWindowClosed(true);
    
    return a.exec();


    
/*
    PlayerType p;
    GameState st;
    st.init();
    st.insertDefensePiece(2,2);
    st.insertDefensePiece(1,3);
    
    QSet<GameState> vis;
    bool max = true, draw = false;
    
    AIPlayer* pAttack = new MinimaxPlayer(Attack, 5);
    AIPlayer* pDefense = new GreedyPlayer(Defense);
    
    while (!st.gameOver(p)) {
        
        if (vis.contains(st)) {
            draw = true;
            break;
        }
        else {
            vis.insert(st);
        }
        
        AIPlayer* ai = max ? pAttack : pDefense;
        
        ai->reset(&st);
        ai->start();
        ai->wait();
        
        Move m = ai->getMove();
        
        cout << m << endl;
        
        st.move(m);
        max = !max;
    }
  
    cout << endl;
    
    if (draw)
        cout << "Draw";
    else
        cout << playerTypeString(p).toStdString() << " wins!";
        
    cout << " in " << vis.size() << " turns" << endl;
*/
    
    
    
/*
    
    QTime t;
    Move m;
    GameState st, next;
    ifstream file("C:\\GiulianoXT\\workspace\\QAssault\\tests\\state.txt");
    file >> st;            
    
    cout << "Normal:" << endl;
    t.start();
    
    m = MinimaxAI(Attack, 8).searchTree(st);
    
    cout << QTime().addMSecs(t.elapsed()).toString("s.zzz").toStdString()
         << " secs" << endl
         << m << endl;
    
    st.copyAndMove(m, next);
    printf("eval = %.04lf\n", next.eval());
    
*/
    
//   system("pause");
    
}
