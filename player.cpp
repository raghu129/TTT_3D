#include "player.hpp"
#include <cstdlib>


namespace TICTACTOE3D
{

int rotateDeg(const GameState &pState)
{
}

int zobristHash(const GameState &pState)
{

}

bool checkIfSameState(const GameState &aState, const GameState &bState)
{
    std::string strA = aState.toMessage();
    std::string strB = bState.toMessage();
    std::string boardAStatus = strA.substr(0, 64);
    std::string boardBStatus = strB.substr(0, 64);

    return false;

}

int score(const GameState &pState, const Deadline &pDue)
{
    Deadline currentTime=pDue.now();
    //std::cerr << "Reached endState" << std::endl;
    //std::cerr << "Time remaining " << currentTime.getSeconds() << std::endl;

    int stateScores[11];
    for (int i=0;i<11;i++)
    {
        stateScores[i]=0;
    }
    for (int i=0;i<4;i++)
    {
        for (int j=0;j<4;j++)
        {
            for (int k=0;k<4;k++)
            {
                if (pState.at((16*i)+(4*j)+k)&CELL_X)
                {
                    stateScores[i]++;
                    stateScores[4+j]++;
                }
                else if (pState.at((16*i)+(4*j)+k)&CELL_O)
                {
                    stateScores[i]--;
                    stateScores[4+j]--;
                }
                if(i==j)
                {
                    if (pState.at((16*i)+(4*j)+k)&CELL_X)
                    {
                        stateScores[8]++;
                    }
                    else if (pState.at((16*i)+(4*j)+k)&CELL_O)
                    {
                        stateScores[8]--;
                    }
                }
                if(i+j==3)
                {
                    if (pState.at((16*i)+(4*j)+k)&CELL_X)
                    {
                        stateScores[9]++;
                    }
                    else if (pState.at((16*i)+(4*j)+k)&CELL_O)
                    {
                        stateScores[9]--;
                    }
                }
                if((i==0&&j==0)||(i==0&&j==3)||(i==3&&j==0)||(i==3&&j==3))
                {
                    if (pState.at((4*i)+j)&CELL_X)
                    {
                        stateScores[10]++;
                    }
                    else if (pState.at((4*i)+j)&CELL_O)
                    {
                        stateScores[10]--;
                    }
                }
            }
        }
    }
    int scoreRet=0;
    for (int i=0;i<11;i++)
    {
        scoreRet=scoreRet+(abs(stateScores[i])*stateScores[i]);
    }
    //scoreRet=scoreRet+
    //std::cerr << "Score at endState is " << scoreRet << std::endl;
    return scoreRet;
}

int minimax(const GameState &pState,Cell pPlayer,const Deadline &pDue)
{
    //std::cerr << "Trying to reach a lower state" << std::endl;
    std::vector<GameState> childStates;
    pState.findPossibleMoves(childStates);
    //int bestPossibleMove=0;

    if (childStates.size() == 0)
    {
        return score(pState,pDue);
    }
    else
    {
        if (pPlayer==CELL_X)
        {
            int bestPossible=-9999;
            int childScore=-9999;
            for(int i=0; i<childStates.size();i++)
            {
                //int myMove=childStates[i];
                childScore=minimax(childStates[i],CELL_O,pDue);
                if(bestPossible<childScore)
                {
                    bestPossible=childScore;
                    //bestPossibleMove=childStates[i];
                }

            }
            return bestPossible;
        }
        else
        {
            int bestPossible=9999;
            int childScore=9999;
            for(int i=0; i<childStates.size();i++)
            {
                //int myMove=childStates[i];
                childScore=minimax(childStates[i],CELL_X,pDue);
                if(bestPossible>childScore)
                {
                    bestPossible=childScore;
                    //bestPossibleMove=childStates[i];
                }

            }
            return bestPossible;
        }


    }
    //return bestPossible;
}

int minimax_alphabeta(const GameState &pState, int depth, int alpha, int beta, Cell pPlayer, const Deadline &pDue)
{
    Deadline currentTime=pDue.now();
    //std::cerr << "Time remaining " << currentTime.getSeconds() << std::endl;
    //std::cerr << "Trying to reach a lower state" << std::endl;
    std::vector<GameState> childStates;
    pState.findPossibleMoves(childStates);
    int bestPossible;

    if ((depth==0)||(childStates.size() == 0))
    {
        return score(pState,pDue);
    }
    else
    {
        if (pPlayer==CELL_X)
        {
            bestPossible=-9999;
            int childScore=-9999;
            for(int i=0; i<childStates.size();i++)
            {
                //int myMove=childStates[i];
                childScore=minimax_alphabeta(childStates[i],depth-1,alpha,beta,CELL_O,pDue);
                /*if(bestPossible<childScore)
                {
                    bestPossible=childScore;
                    //bestPossibleMove=childStates[i];
                }*/
                if(alpha<childScore)
                {
                    alpha=childScore;
                }
                if(beta<=alpha)
                {
                    break;
                }
            }
            return alpha;
        }
        else
        {
            bestPossible=9999;
            int childScore=9999;
            for(int i=0; i<childStates.size();i++)
            {
                //int myMove=childStates[i];
                childScore=minimax_alphabeta(childStates[i],depth-1,alpha,beta,CELL_X,pDue);
                /*if(bestPossible>childScore)
                {
                    bestPossible=childScore;
                    //bestPossibleMove=childStates[i];
                }*/
                if(beta>childScore)
                {
                    beta=childScore;
                }
                if(beta<=alpha)
                {
                    break;
                }

            }
            return beta;
        }

    }
    //return bestPossible;
}

GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    //std::cerr << "Processing " << pState.toMessage() << std::endl;
    std::cerr << "Time remaining " << pDue.getSeconds() << std::endl;
    //std::cerr << "Processing " << boardStatus << std::endl;
    std::vector<GameState> lNextStates;

    //check all possible moves
    pState.findPossibleMoves(lNextStates);
    Deadline currentTime=pDue.now();
    //std::cerr << "Time remaining " << currentTime.getSeconds() << std::endl;
    //calculate score for all possible moves

    GameState uniqueStates[lNextStates.size()];
    //int uniqueStatesScore[lNextStates.size()];
    GameState bestNextState=lNextStates[0];

    if (lNextStates.size() == 0)
    {
        return GameState(pState, Move());
    }
    else
    {
        int bestEvaluationValue=0;

        for (int i=0;i<lNextStates.size();i++)
        {
            currentTime=pDue.now();
            std::cerr << "Current Time " << currentTime.getSeconds() << std::endl;

            std::string currGameState=lNextStates[i].toMessage();
            std::string currBoardState=currGameState.substr(0,64);

            if (scoreMap.find(currBoardState) == scoreMap.end())
            {
              // not found

                //scoreMap;
                int currentStateScore=0;

                //Check for repeated states
                bool repeatedState=false;
                for (int j=0;j<lNextStates.size();j++)
                {
                    //repeatedState=checkIfSameState(uniqueStates[j],lNextStates[i]);
                    if(repeatedState==true)
                    {
                        break;
                    }
                }
                //uniqueStates[i]=lNextStates[i];
                if(repeatedState==false)
                {
                    currentTime=pDue.now();
                    std::cerr << "Current Time start alphabeta " << currentTime.getSeconds() << std::endl;

                    int alphabetaNextI=minimax_alphabeta(lNextStates[i],2,-9999,9999,CELL_X,pDue);
                    //int alphabetaNextI=minimax(lNextStates[i],CELL_X);
                    if (alphabetaNextI>bestEvaluationValue){
                        bestEvaluationValue=alphabetaNextI;
                        bestNextState=lNextStates[i];
                    }
                    currentStateScore=alphabetaNextI;

                    currentTime=pDue.now();
                    std::cerr << "Current Time end alphabeta " << currentTime.getSeconds() << std::endl;

                }
                            //std::cerr << "This State is " << currBoardState << " " << currentStateScore << std::endl;
                scoreMap[currBoardState]=currentStateScore;
            }
            else
            {
                if(scoreMap[currBoardState]>bestEvaluationValue)
                {
                    bestEvaluationValue=scoreMap[currBoardState];
                    bestNextState=lNextStates[i];
                }
            }

        }
    }

    /*
     * Here you should write your clever algorithms to get the best next move, ie the best
     * next state. This skeleton returns a random move instead.
     */
    //std::cerr << "Best possible score for this state: " << minimax(pState,CELL_X) << std::endl;

    //return lNextStates[rand() % lNextStates.size()];
    return bestNextState;
}

/*namespace TICTACTOE3D*/ }
