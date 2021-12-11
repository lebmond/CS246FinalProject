#include "game.h"
#include <sstream>
#include <iostream>
#include "isChecked.h"

void Game::boardInit() {
    for ( int k = 0; k < 8; ++k ) {
        pieces.emplace_back( std::make_unique<Pawn>( 1 ) );
        board[k][1] = pieces.back().get();
    }
    pieces.emplace_back( std::make_unique<Rook>( 1 ) );
    board[0][0] = pieces.back().get();
    pieces.emplace_back( std::make_unique<Knight>( 1 ) );
    board[1][0] = pieces.back().get();
    pieces.emplace_back( std::make_unique<Bishop>( 1 ) );
    board[2][0] = pieces.back().get();
    pieces.emplace_back( std::make_unique<Queen>( 1 ) );
    board[3][0] = pieces.back().get();
    pieces.emplace_back( std::make_unique<King>( 1 ) );
    board[4][0] = pieces.back().get();
    pieces.emplace_back( std::make_unique<Bishop>( 1 ) );
    board[5][0] = pieces.back().get();
    pieces.emplace_back( std::make_unique<Knight>( 1 ) );
    board[6][0] = pieces.back().get();
    pieces.emplace_back( std::make_unique<Rook>( 1 ) );
    board[7][0] = pieces.back().get();
    for ( int k = 0; k < 8; ++k ) {
        pieces.emplace_back( std::make_unique<Pawn>( 2 ) );
        board[k][6] = pieces.back().get();
    }
    pieces.emplace_back( std::make_unique<Rook>( 2 ) );
    board[0][7] = pieces.back().get();
    pieces.emplace_back( std::make_unique<Knight>( 2 ) );
    board[1][7] = pieces.back().get();
    pieces.emplace_back( std::make_unique<Bishop>( 2 ) );
    board[2][7] = pieces.back().get();
    pieces.emplace_back( std::make_unique<Queen>( 2 ) );
    board[3][7] = pieces.back().get();
    pieces.emplace_back( std::make_unique<King>( 2 ) );
    board[4][7] = pieces.back().get();
    pieces.emplace_back( std::make_unique<Bishop>( 2 ) );
    board[5][7] = pieces.back().get();
    pieces.emplace_back( std::make_unique<Knight>( 2 ) );
    board[6][7] = pieces.back().get();
    pieces.emplace_back( std::make_unique<Rook>( 2 ) );
    board[7][7] = pieces.back().get();
    whiteStart = true;
}

Game::Game() :  mh{ std::make_unique<MoveHistory>() }, whiteStart{ true } {
    board.resize( 8, std::vector<Piece *>( 8, nullptr ) );
    boardInit();
}

std::string Game::move( const int &originalX, const int &originalY, 
    const int &endX, const int &endY ) {
    Piece *pc =  board[originalX][originalY];
    int side = board[originalX][originalY]->getSide();
    std::string retval = "";

    // The case when piece is going to kill another piece
    if ( board[endX][endY] != nullptr ) {
        deadPool.emplace_back( board[endX][endY] );
        board[originalX][originalY] = nullptr;
        board[endX][endY] = pc;
        retval = "k";
    } else {
        board[endX][endY] = pc;
        board[originalX][originalY] = nullptr;
        retval = "m";
    }

    // Checking pawn capture en passant
    if ( pc->getType() == 'p' && mh->hasMoved() ) {
        Move *lastMove = mh->lastMove();
        Posn *lastMoveEnd = lastMove->getEnd();
        Posn *lastMoveBegin = lastMove->getOriginal();
        Piece *lastPiece = board[lastMoveEnd->getX()][lastMoveEnd->getY()];
        if ( lastPiece->getType() == 'p' ) {
            if ( side == 1 ) {
                if ( endX == lastMoveEnd->getX() && 
                     endY - 1 == lastMoveEnd->getY() &&
                     endX == lastMoveBegin->getX() && 
                     endY + 1 == lastMoveBegin->getY() &&
                     lastPiece->getSide() != pc->getSide() ) {
                    retval = "k";
                }
            } else if ( side == 2 ) {
                if ( endX == lastMoveEnd->getX() && 
                     endY + 1 == lastMoveEnd->getY() &&
                     endX == lastMoveBegin->getX() && 
                     endY - 1 == lastMoveBegin->getY() &&
                     lastPiece->getSide() != pc->getSide() ) {
                    retval = "k";
                }
            }
        }
    }

    // Now checking if this is a promotion
    if ( pc->getType() == 'p' ) {
        if ( endY == 7 || endY == 0 ) {
            char promptTo = 'a';
            while ( true ) {
                std::cout << "Your pawn is being granted a promotion" << std::endl;
                std::cout << "You can change your pawn into:" << std::endl;
                std::cout << "(r)ook" << std::endl;
                std::cout << "k(n)ight" << std::endl;
                std::cout << "(b)ishop" << std::endl;
                std::cout << "(q)ueen" << std::endl;
                std::cin >> promptTo;
                if ( promptTo == 'r' ) {
                    pieces.emplace_back( std::make_unique<Rook>( pc->getSide() ) );
                    break;
                } else if ( promptTo == 'n' ) {
                    pieces.emplace_back( std::make_unique<Knight>( pc->getSide() ) );
                    break;
                } else if ( promptTo == 'b' ) {
                    pieces.emplace_back( std::make_unique<Bishop>( pc->getSide() ) );
                    break;
                } else if ( promptTo == 'q' ) {
                    pieces.emplace_back( std::make_unique<Queen>( pc->getSide() ) );
                    break;
                }
            }
            deadPool.emplace_back( pc );
            board[endX][endY] = pieces.back().get();
            pieces.back().get()->changeMoved( true );
            if ( retval == "k" ) {
                retval += "+p";
            } else {
                retval = "p";
            }
        }
    }

    // Now check for castling
    if ( pc->getType() == 'k' ) {
        int diffX = endX - originalX;
        if ( diffX == 2 || diffX == -2 ) {
            if ( diffX == 2 ) {
                board[endX][endY] = pc;
                board[originalX][originalY] = nullptr;
                board[endX - 1][endY] = board[7][endY];
                board[endX - 1][endY]->changeMoved( true );
                board[7][endY] = nullptr; 
            } else {
                board[endX][endY] = pc;
                board[originalX][originalY] = nullptr;
                board[endX + 1][endY] = board[0][endY];
                board[0][endY] = nullptr; 
                board[endX + 1][endY]->changeMoved( true );
            }
            retval = "c";
        }
    }

    return retval;
}

void Game::undo() {
    std::vector<Move *> undoHist = mh->undo();
    for ( auto hist : undoHist ) {
        Posn *end = hist->getEnd();
        Posn *begin = hist->getOriginal();
        int endX = end->getX();
        int endY = end->getY();
        int beginX = begin->getX();
        int beginY = begin->getY();
        if ( hist->getOperation() == "m" ) {
            board[beginX][beginY] = board[endX][endY];
            board[endX][endY] = nullptr;
            board[beginX][beginY]->changeMoved( hist->isFirstMove() );
        } else if ( hist->getOperation() == "c" ) {
            board[beginX][beginY] = board[endX][endY];
            board[endX][endY] = nullptr;
            board[beginX][beginY]->changeMoved( false );
            if ( endX > beginX ) {
                board[7][endY] = board[endX - 1][endY];
                board[7][endY]->changeMoved( false );
                board[endX + 1][endY] = nullptr;
            } else {
                board[0][endY] = board[endX + 1][endY];
                board[0][endY]->changeMoved( false );
                board[endX + 1][endY] = nullptr;
            }
        } else if ( hist->getOperation() == "k" ) {
            board[beginX][beginY] = board[endX][endY];
            board[endX][endY] = deadPool.back();
            deadPool.pop_back();
            board[beginX][beginY]->changeMoved( hist->isFirstMove() );
        } else if ( hist->getOperation() == "p" ) {
            board[endX][endY] = deadPool.back();
            deadPool.pop_back();
            board[beginX][beginY] = board[endX][endY];
            board[endX][endY] = nullptr;
            board[beginX][beginY]->changeMoved( hist->isFirstMove() );
        } else if ( hist->getOperation() == "k+p" ) {
            board[endX][endY] = deadPool.back();
            deadPool.pop_back();
            board[beginX][beginY] = board[endX][endY];
            board[endX][endY] = deadPool.back();
            deadPool.pop_back();
            board[beginX][beginY]->changeMoved( hist->isFirstMove() );
        }
    }
}

void Game::start() {
    boardInit();
    bool end = false;
    notifyObservers( *this );
    int diffI = 1;
    int diffK = 0;
    if ( !whiteStart ) {
        diffK = -1;
        diffI = 0;
    }
    while ( !end ) {
        for ( int i = 0, k = 1; i < 2 && k >= 0; ) {
            Player *player = nullptr;
            if ( whiteStart ) {
                player = players[i];
            } else {
                player = players[k];
            }
            if ( !player->hasAvaliableMove() && 
                !IsChecked::isChecked( player->getId(), board ) ) {
                std::cout << "Stalemate!" << std::endl;
                players[0]->getScore() += 0.5;
                players[1]->getScore() += 0.5;
                end = true;
                break;
            } else if ( !player->hasAvaliableMove() && 
                IsChecked::isChecked( player->getId(), board ) ) {
                if ( i == 0 ) {
                    std::cout << "Checkamte! Black wins!" << std::endl;
                    player[1].getScore()++;
                } else {
                    std::cout << "Checkamte! White wins!" << std::endl;
                    player[0].getScore()++;
                }
                end = true;
                break;
            } else if ( player->hasAvaliableMove() && 
                IsChecked::isChecked( player->getId(), board ) ) {
                if ( i == 0 ) {
                    std::cout << "White is in check." << std::endl;
                } else {
                    std::cout << "Black is in check." << std::endl;
                }
            }

            std::cerr << __LINE__ << std::endl;
            std::string cmd = player->cmd( *this );
            
            if ( cmd == "undo" ) {
                undo();
                player->usedUndo();
            } else if ( cmd == "resign" ) {
                if ( i == 0 ) {
                    std::cout << "Black wins!" << std::endl;
                    player[1].getScore()++;
                } else {
                    std::cout << "White wins!" << std::endl;
                    player[0].getScore()++;
                }
                end = true;
                break;
            } else if ( cmd == "" ) {
                end = true;
                break;
            } else {
                std::stringstream in{ cmd };
                std::string str = "";
                std::string iniPosn = "";
                std::string endPosn = "";
                in >> str;
                in >> iniPosn;
                in >> endPosn;
                int iniX = iniPosn[0] - 'a';
                int iniY = iniPosn[1] - '1';
                int endX = endPosn[0] - 'a';
                int endY = endPosn[1] - '1';
                std::string op = move( iniX, iniY, endX, endY );
                mh->add( iniX, iniY, endX, endY, player->getId(), op, 
                    board[endX][endY]->isMoved() );
                board[endX][endY]->changeMoved( true );
            }
            notifyObservers( *this );
            i += diffI; 
            k += diffK;
        }
    }
}

void errorMsg() {
    std::cout << "Bad input" << std::endl;
    std::cout << "You can try:" << std::endl;
    std::cout << "+ K e1" << std::endl;
    std::cout << "- e1" << std::endl;
    std::cout << "= white" << std::endl;
    std::cout << "done" << std::endl;
    std::cout << "Please enter command here: ";
}

void Game::setup() {
    std::string in = "";
    boardInit();
    board.resize( 8, std::vector<Piece *>( 8, nullptr ) );
    notifyObservers(*this);
    std::cout << "Please enter command here: ";
    while( std::getline( std::cin, in ) ) {
        std::string op = "";
        std::stringstream cmd{ in };
        cmd >> op;
        if ( op == "+" ) {
            char pc = '1';
            cmd >> pc;
            if ( pc == 'r' ) {
                pieces.emplace_back( std::make_unique<Rook>( 2 ) );
            } else if ( pc == 'R' ) {
                pieces.emplace_back( std::make_unique<Rook>( 1 ) );
            } else if ( pc == 'n' ) {
                pieces.emplace_back( std::make_unique<Knight>( 2 ) );
            } else if ( pc == 'N' ) {
                pieces.emplace_back( std::make_unique<Knight>( 1 ) );
            } else if ( pc == 'b' ) {
                pieces.emplace_back( std::make_unique<Bishop>( 2 ) );
            } else if ( pc == 'B' ) {
                pieces.emplace_back( std::make_unique<Bishop>( 1 ) );
            } else if ( pc == 'q' ) {
                pieces.emplace_back( std::make_unique<Queen>( 2 ) );
            } else if ( pc == 'Q' ) {
                pieces.emplace_back( std::make_unique<Queen>( 1 ) );
            } else if ( pc == 'k' ) {
                pieces.emplace_back( std::make_unique<King>( 2 ) );
            } else if ( pc == 'K' ) {
                pieces.emplace_back( std::make_unique<King>( 1 ) );
            } else if ( pc == 'p' ) {
                pieces.emplace_back( std::make_unique<Pawn>( 2 ) );
            } else if ( pc == 'P' ) {
                pieces.emplace_back( std::make_unique<Pawn>( 1 ) );
            } else {
                notifyObservers(*this);
                errorMsg();
                continue;
            }

            char x = 'a';
            int y = 0;
            cmd >> x;
            cmd >> y;
            if ( x < 'a' || x > 'h' || y < 1 || y > 8 ) {
                notifyObservers(*this);
                errorMsg();
                continue;
            }
            board[x - 'a'][y - 1] = pieces.back().get();
        } else if ( op == "-" ) {
            char x = 'a';
            int y = 0;
            cmd >> x;
            cmd >> y;
            if ( x < 'a' || x > 'h' || y < 1 || y > 8 ) {
                notifyObservers(*this);
                errorMsg();
                continue;
            }
            board[x - 'a'][y - 1] = nullptr;
        } else if ( op == "=" ) {
            std::string side = "";
            cmd >> side;
            if ( side == "white" ) {
                whiteStart = true;
            } else if ( side == "black" ) {
                whiteStart = false;
            } else {
                notifyObservers(*this);
                errorMsg();
                continue;
            }
        } else if ( op == "done" ) {
            int whiteKing = 0;
            int blackKing = 0;
            for ( int i = 0; i < 8; ++i ) {
                for ( int k = 0; k < 8; ++k ) {
                    if ( board[i][k] ) {
                        if ( board[i][k]->getType() == 'k' ) {
                            if ( board[i][k]->getSide() == 1 ) {
                                whiteKing++;
                            } else {
                                blackKing++;
                            }
                        }
                    }
                    
                }
            }

            if ( whiteKing != 1 && blackKing != 1 ) {
                notifyObservers(*this);
                std::cout << "There are more king than expected!" << std::endl;
                std::cout << "Please reconsider your setup." << std::endl;
                std::cout << "Please enter command here: ";
                continue;
            }

            for ( int i = 0; i < 7; ++i ) {
                if ( board[0][i] ) {
                    if ( board[0][i]->getType() == 'p' ) {
                        notifyObservers(*this);
                        std::cout << "Pawns are not allowed to be at the first or" 
                            << "last row of the board." << std::endl;
                        std::cout << "Please reconsider your setup." << std::endl;
                        std::cout << "Please enter command here: ";
                        continue;
                    }
                }
            }

            for ( int i = 0; i < 7; ++i ) {
                if ( board[7][i] ) {
                    if ( board[7][i]->getType() == 'p' ) {
                        notifyObservers(*this);
                        std::cout << "Pawns are not allowed to be at the first or" 
                            << "last row of the board." << std::endl;
                        std::cout << "Please reconsider your setup." << std::endl;
                        std::cout << "Please enter command here: ";
                        continue;
                    }
                }
            }

            if ( IsChecked::isChecked( 1, board ) ) {
                notifyObservers(*this);
                std::cout << "White king is being checked." << std::endl;
                std::cout << "Please reconsider your setup." << std::endl;
                std::cout << "Please enter command here: ";
                continue;
            } else if ( IsChecked::isChecked( 2, board ) ) {
                notifyObservers(*this);
                std::cout << "Black king is being checked." << std::endl;
                std::cout << "Please reconsider your setup." << std::endl;
                std::cout << "Please enter command here: ";
                continue;
            }

            break;
        }
        notifyObservers(*this);
        std::cout << "Please enter command here: ";
    }
    notifyObservers(*this);
}

std::vector<std::vector<Piece *>> &Game::getBoard() { return board; }

MoveHistory *Game::getMoveHistory() { return mh.get(); }

float Game::getScore( int idx ) { return players[idx]->getScore(); }

void Game::addPlayer( Player *player ) { 
    players.emplace_back( player ); 
    attach( player );
}