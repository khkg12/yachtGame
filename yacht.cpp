#include <iostream>
#include <string>
#include <map>
#include<cstdlib>
#include<ctime>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <set>
#include<iomanip> 

using namespace std;

const string rankNameArr[14] = { "not", "Ones", "Twos" , "Threes" , "Fours" , "Fives" , "sixes" , "ThreeOfKind" , "FourOfKind" , "FullHouse" , "SmallStraight", "LargeStraight" , "Chance" , "Yathzee" };

bool cmp(const pair<int, int>& a, const pair<int, int>& b) {
    return a.second > b.second;
}

struct HandRanking;

struct Dice {
    int diceArr[5] = { 0, }; // 주사위 배열
    int singleArr[7] = { 0, }; // 싱글족보 점수저장 배열    
    int score = 0; // 총합 점수
    int nowSum = 0; // 현재다이스의 합
    int singleSum = 0; // 보너스 점수를 위한 싱글 합변수        
    map<string, bool> m;
    map<string, string> scoreMap;
    HandRanking* rankArr[7];

    // 족보 구조체 배열 -> 배열에 족보 구조체를 상속한 구조체들 넣기 -> 루프로 각 구조체의 override함수실행 

    Dice();

    void Init() { // m초기화
        for (int i = 1; i <= 13; i++) {
            m[to_string(i)] = false;
            scoreMap[to_string(i)] = "( )";
        }
    }

    void DiceRoll() {
        nowSum = 0; // sum값 굴릴때마다 초기화
        for (int i = 0; i < 5; i++) {
            int num = rand() % 6 + 1;
            diceArr[i] = num;
        }
        nowSum = NowDiceSum();
    } // 주사위 굴리기

    int NowDiceSum() {
        int sum = 0;
        for (int i = 0; i < 5; i++) {
            sum += diceArr[i];
        }
        return sum;
    }

    bool GameEnd() {
        for (int i = 1; i <= 13; i++) {
            if (m[to_string(i)] == false) return false;
        }
        return true;
    }

    bool NothingRank(bool arr[]) { // 주사위의 경우의 수가 아무런 족보도 존재하지 않을 경우              
        for (int i = 1; i <= 13; i++) {
            if (arr[i] == true) return false;
        }
        return true;
    }

    void PrintNumber() {
        cout << "당신의 주사위 : ";
        for (int i = 0; i < 5; i++) {
            cout << diceArr[i] << " ";
        }
        cout << endl;
    } // 주사위 넘버 출력        

    int Single(int num) { // 싱글 족보 체크
        int sum = 0;
        for (int i = 0; i < 5; i++) {
            if (diceArr[i] == num) {
                sum += diceArr[i];
            }
        }
        return sum;
    }

    void SingleBonus() {
        if (singleSum >= 63) {
            cout << "싱글점수 총합이 63이상이라 보너스 점수 35점을 획득합니다!" << endl;
            score += 35;
        }
    }

    virtual void CheckRank() {
        cout << "족보체크" << endl;
    } // 족보확인        
};

struct HandRanking {
    string name;
    int inputNum;
    virtual bool CheckHandRanking(Dice* dice) {
        cout << "족보확인" << endl;
        return true;
    }
    virtual void PrintRank(Dice* dice) {
        cout << "족보 만족 시 문장 출력" << endl;
    }
};

struct ThreeOfKindRanking : HandRanking {

    ThreeOfKindRanking() {
        name = "ThreeOfKind";
        inputNum = 7;
    }

    void PrintRank(Dice* dice) override {
        cout << "Three-Of-Kind 점수 : " << dice->nowSum << "점 || 입력 -> " << inputNum << endl;
    }

    bool CheckHandRanking(Dice* dice) override {
        map<int, int> map;
        for (int i = 0; i < 5; i++) {
            map[dice->diceArr[i]] += 1;
        }
        for (auto i : map) {
            if (i.second >= 3 && dice->m["7"] == false) { // 같은 값이 3개이상일때
                return true;
            }
        }
        return false;
    }
};

struct FourOfKindRanking : HandRanking {

    FourOfKindRanking() {
        name = "FourOfKind";
        inputNum = 8;
    }

    void PrintRank(Dice* dice) override {
        cout << "Four-Of-Kind 점수 : " << dice->nowSum << "점 || 입력 -> " << inputNum << endl;
    }

    bool CheckHandRanking(Dice* dice) override {
        map<int, int> map;
        for (int i = 0; i < 5; i++) {
            map[dice->diceArr[i]] += 1;
        }
        for (auto i : map) {
            if (i.second >= 4 && dice->m["8"] == false) { // 같은 값이 3개이상일때
                return true;
            }
        }
        return false;
    }
};

struct FullHouseRanking : HandRanking {

    FullHouseRanking() {
        name = "FullHouse";
        inputNum = 9;
    }

    void PrintRank(Dice* dice) override {
        cout << "FullHouse 점수 : 25점 || 입력 -> " << inputNum << endl;
    }

    bool CheckHandRanking(Dice* dice) override {
        map<int, int> map;
        for (int i = 0; i < 5; i++) {
            map[dice->diceArr[i]] += 1;
        }
        if (map.size() != 2 || dice->m["9"] == true) return false; // 풀하우스는 2개의 주사위값만있어야함
        for (auto i : map) {
            if (i.second != 3 && i.second != 2) {
                return false;
            }
        }
        return true;
    }
};

struct SmallStraightRanking : HandRanking {

    SmallStraightRanking() {
        name = "SmallStraight";
        inputNum = 10;
    }

    void PrintRank(Dice* dice) override {
        cout << "SmallStraight 점수 : 30점 || 입력 -> " << inputNum << endl;
    }

    bool CheckHandRanking(Dice* dice) override {
        set<int> s(dice->diceArr, dice->diceArr + 5);
        int preValue = -1;
        int cnt = 0;
        if (s.size() < 4) return false;
        else {
            for (auto i : s) { // 1234 12456        
                if (preValue + 1 == i) cnt++;
                else cnt = 0;
                preValue = i;
            }
        }
        if (cnt >= 3 && dice->m["10"] == false) return true;
        else return false;
    }
};

struct LargeStraightRanking : HandRanking {

    LargeStraightRanking() {
        name = "LargeStraight";
        inputNum = 11;
    }

    void PrintRank(Dice* dice) override {
        cout << "LargeStraight 점수 : 40점 || 입력 -> " << inputNum << endl;
    }


    bool CheckHandRanking(Dice* dice) override {
        set<int> s(dice->diceArr, dice->diceArr + 5);
        int preValue = -1;
        int cnt = 0;
        if (s.size() < 5) return false;
        else {
            for (auto i : s) { // 1234 12456        
                if (preValue + 1 == i) cnt++;
                else cnt = 0;
                preValue = i;
            }
        }
        if (cnt >= 4 && dice->m["11"] == false) return true;
        else return false;
    }
};

struct ChanceRanking : HandRanking {

    ChanceRanking() {
        name = "Chance";
        inputNum = 12;
    }

    void PrintRank(Dice* dice) override {
        cout << "Chance 점수 : " << dice->nowSum << "점 || 입력 -> " << inputNum << endl;
    }

    bool CheckHandRanking(Dice* dice) override {
        if (dice->m["12"] == false) return true;
        else return false;
    }
};

struct YahtzeeRanking : HandRanking {

    YahtzeeRanking() {
        name = "Yahtzee";
        inputNum = 13;
    }

    void PrintRank(Dice* dice) override {
        cout << "Yahtzee 점수 : 50점 || 입력 -> " << inputNum << endl;
    }

    bool CheckHandRanking(Dice* dice) override {
        map<int, int> map;
        for (int i = 0; i < 5; i++) {
            map[dice->diceArr[i]] += 1;
        }
        for (auto i : map) {
            if (i.second >= 5 && dice->m["13"] == false) { // 같은 값이 3개이상일때
                return true;
            }
        }
        return false;
    }
};

Dice::Dice() {
    Init();
    rankArr[0] = new ThreeOfKindRanking;
    rankArr[1] = new FourOfKindRanking;
    rankArr[2] = new FullHouseRanking;
    rankArr[3] = new SmallStraightRanking;
    rankArr[4] = new LargeStraightRanking;
    rankArr[5] = new ChanceRanking;
    rankArr[6] = new YahtzeeRanking;
}

struct PlayerDice : Dice {

    bool CheckInput(string input) {
        for (int i = 1; i <= 13; i++) {
            if (to_string(i) == input) {
                return true;
            }
        }
        return false;
    }

    ~PlayerDice() {
        for (int i = 0; i < 7; i++) {
            delete rankArr[i];
        }
    }

    void DiceReRoll() {
        for (int i = 0; i < 2; i++) {
            cout << "주사위를 다시 돌리려면 1, 괜찮다면 2를 눌러주세요. : ";
            string input;
            cin >> input;
            if (input == "1") {
                cout << "다시 돌릴 주사위의 인덱스를 입력해주세요! (ex. 123 <- 1, 2, 3번째 주사위 다시던짐) : ";
                string diceIndex;
                cin >> diceIndex;
                for (int j = 0; j < diceIndex.size(); j++) {
                    if (diceIndex[j] - '0' < 1 || diceIndex[j] - '0' > 5) { // 인덱스 해당 숫자가 아니거나 문자일때
                        cout << "잘못된 값을 입력하셨어요. 해당 주사위는 변경되지않습니다!" << endl;                        
                    }
                    else {
                        int num = diceIndex[j] - '0' - 1;
                        diceArr[num] = rand() % 6 + 1;
                    }                    
                }
                nowSum = NowDiceSum();
                PrintNumber();
            }
            else if (input == "2") {
                return;
            }
            else {
                cout << "잘못된 값을 입력했어요! 다시 입력하세요!" << endl;
                i--;
            }
        }
    }

    void CheckRank() override { // 족보확인           
        bool trueCheckArr[14] = { false, }; // 값을 넣을 수 있는 족보인덱스에 true를 넣고 true일 때만 값을 넣게
        string rankInput;

        cout << endl << "-------------입력 가능 족보-------------" << endl << endl;

        for (int i = 1; i <= 6; i++) { // 싱글족보확인
            singleArr[i] = Single(i);
            if (m[to_string(i)] == false) {                
                cout << rankNameArr[i] << " 점수 : " << singleArr[i] << "점 || 입력 -> " << i << endl;
                trueCheckArr[i] = true;
            }
        }

        for (int i = 0; i < 7; i++) { //메인족보확인
            if (rankArr[i]->CheckHandRanking(this) && m[to_string(rankArr[i]->inputNum)] == false) {
                rankArr[i]->PrintRank(this);
                trueCheckArr[rankArr[i]->inputNum] = true;
            }
        }

        if (NothingRank(trueCheckArr)) { // 이부분도 trueCheckArr의 모든원소가 false라면 이라는 조건으로 변경가능
            cout << "채울 수 있는 조건의 족보가 없습니다! 아직 채워지지 않은 족보를 입력하시면 그 곳에 0이 채워집니다" << endl;
            for (int i = 1; i <= 13; i++) {
                if (m[to_string(i)] == false) {
                    cout << i << " ";
                }
            }
            cout << "의 족보는 아직 값이 입력되지않았습니다! 이 곳중 값을 입력하여 채우세요" << endl;
            while (1) {
                cin >> rankInput;
                if (!CheckInput(rankInput)) {
                    cout << "다시 입력하세요" << endl;
                    continue;
                }
                else break;
            }            
            m[rankInput] = true;
            scoreMap[rankInput] = "0";
        }
        else {
            // 모든 조건 처리 후 값 입력 시
            while (1) {                
                cin >> rankInput;
                if (!CheckInput(rankInput)) {
                    cout << "다시 입력하세요" << endl;
                    continue;
                }

                if (trueCheckArr[stoi(rankInput)] == true) {
                    int inputValue = stoi(rankInput);
                    switch (inputValue) {
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        m[rankInput] = true;
                        scoreMap[rankInput] = to_string(singleArr[inputValue]);
                        score += singleArr[inputValue];
                        singleSum += singleArr[inputValue];
                        break;
                    case 7:
                    case 8:
                    case 12:
                        m[rankInput] = true;
                        scoreMap[rankInput] = to_string(nowSum);
                        score += nowSum;
                        break;
                    case 9:
                        m[rankInput] = true;
                        scoreMap[rankInput] = "25";
                        score += 25;
                        break;
                    case 10:
                        m[rankInput] = true;
                        scoreMap[rankInput] = "30";
                        score += 30;
                        break;
                    case 11:
                        m[rankInput] = true;
                        scoreMap[rankInput] = "40";
                        score += 40;
                        break;
                    case 13:
                        m[rankInput] = true;
                        scoreMap[rankInput] = "50";
                        score += 50;
                        break;
                    }
                }
                else {
                    if (m[rankInput] == true) {
                        cout << "이미 채워진 값입니다! 다시 선택하세요" << endl;
                        continue;
                    }

                    cout << "현재 조건을 만족하지 않아 0으로 채워집니다! 정말 채우시려면 1을, 다시 선택하려면 2를 입력하세요." << endl;
                    int input;
                    cin >> input;
                    if (input == 1) {
                        m[rankInput] = true;
                        scoreMap[rankInput] = "0";
                        break;
                    }
                    else {
                        cout << "다시 입력하세요. 실수하지마십시오" << endl;
                        continue;
                    }
                }
                break;
            }
        }
    }

};

void PrintScoreTable(Dice* player, Dice* secondPlayer) {
    cout << "------------------ 1p / 2p" << endl;
    cout << "Ones            : " << player->scoreMap["1"] << " / " << secondPlayer->scoreMap["1"] << "   입력키 : 1" << endl;
    cout << "Twos            : " << player->scoreMap["2"] << " / " << secondPlayer->scoreMap["2"] << "   입력키 : 2" << endl;
    cout << "Threes          : " << player->scoreMap["3"] << " / " << secondPlayer->scoreMap["3"] << "   입력키 : 3" << endl;
    cout << "Fours           : " << player->scoreMap["4"] << " / " << secondPlayer->scoreMap["4"] << "   입력키 : 4" << endl;
    cout << "Fives           : " << player->scoreMap["5"] << " / " << secondPlayer->scoreMap["5"] << "   입력키 : 5" << endl;
    cout << "Sixes           : " << player->scoreMap["6"] << " / " << secondPlayer->scoreMap["6"] << "   입력키 : 6" << endl;
    cout << "Sum             :   " << player->singleSum << " / " << secondPlayer->singleSum << endl;
    cout << "Three of a kind : " << player->scoreMap["7"] << " / " << secondPlayer->scoreMap["7"] << "   입력키 : 7" << endl;
    cout << "Four of a kind  : " << player->scoreMap["8"] << " / " << secondPlayer->scoreMap["8"] << "   입력키 : 8" << endl;
    cout << "Full House      : " << player->scoreMap["9"] << " / " << secondPlayer->scoreMap["9"] << "   입력키 : 9" << endl;
    cout << "SmallStraight   : " << player->scoreMap["10"] << " / " << secondPlayer->scoreMap["10"] << "   입력키 : 10" << endl;
    cout << "LargeStraight   : " << player->scoreMap["11"] << " / " << secondPlayer->scoreMap["11"] << "   입력키 : 11" << endl;
    cout << "Chance          : " << player->scoreMap["12"] << " / " << secondPlayer->scoreMap["12"] << "   입력키 : 12" << endl;
    cout << "YAHTZEE         : " << player->scoreMap["13"] << " / " << secondPlayer->scoreMap["13"] << "   입력키 : 13" << endl;
    cout << "TOTAL SCORE     :   " << player->score << " / " << secondPlayer->score << endl;
}

void DiceBattle(PlayerDice* player, PlayerDice* secondPlayer) {
    PrintScoreTable(player, secondPlayer);
    cout << endl << "1p의 차례입니다!" << endl;
    player->DiceRoll();
    player->PrintNumber();
    player->DiceReRoll();
    player->CheckRank();
    Sleep(1500);
    system("cls");
    PrintScoreTable(player, secondPlayer);
    cout << "2p의 차례입니다!" << endl;
    secondPlayer->DiceRoll();
    secondPlayer->PrintNumber();
    secondPlayer->DiceReRoll();
    secondPlayer->CheckRank();
    Sleep(1500);
    system("cls");
}

void PrintResult(Dice* player, Dice* secondPlayer) {
    cout << "게임이 종료되었습니다!" << endl;
    player->SingleBonus();
    secondPlayer->SingleBonus();
    PrintScoreTable(player, secondPlayer);
    cout << "1P 점수 : " << player->score << endl;
    cout << "2P 점수 : " << secondPlayer->score << endl;
    if (secondPlayer->score > player->score) {
        cout << "2P가 승리하였습니다!" << endl;
    }
    else if (secondPlayer->score == player->score) {
        cout << "비겼습니다!" << endl;
    }
    else {
        cout << "1P가 승리하였습니다!" << endl;
    }
}

void PrintStartScene() {
    cout << "야추. 게임을 시작합니다. 게임시작의 주문을 외우세요." << endl;
    cout << "*     *    *       ****  *    *  *******   " << endl;
    cout << " *   *    * *     *      *    *     *      " << endl;
    cout << "  * *    *   *    *      ******     *      " << endl;
    cout << "   *    *******   *      *    *     *      " << endl;
    cout << "   *   *       *  *      *    *     *      " << endl;
    cout << "   *  *         *  ****  *    *     *      " << endl;
    cout << "게임시작 ";
    string start;
    cin >> start;
    if (start != "start") {
        cout << "암호가 틀렸다! 당장 나가!" << endl;
        Sleep(1000);
        system("cls");
        PrintStartScene();
    }
    else {
        Sleep(1000);
        system("cls");
    }
}

int main()
{
    srand(time(0));
    PlayerDice playerDice;
    PlayerDice secondPlayerDice;
    PrintStartScene();
    while (1) {
        DiceBattle(&playerDice, &secondPlayerDice);
        if (secondPlayerDice.GameEnd()) { // 2P가 후턴이므로
            PrintResult(&playerDice, &secondPlayerDice);
            break;
        }
    }
}


