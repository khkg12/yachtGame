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
    int diceArr[5] = { 0, }; // �ֻ��� �迭
    int singleArr[7] = { 0, }; // �̱����� �������� �迭    
    int score = 0; // ���� ����
    int nowSum = 0; // ������̽��� ��
    int singleSum = 0; // ���ʽ� ������ ���� �̱� �պ���        
    map<string, bool> m;
    map<string, string> scoreMap;
    HandRanking* rankArr[7];

    // ���� ����ü �迭 -> �迭�� ���� ����ü�� ����� ����ü�� �ֱ� -> ������ �� ����ü�� override�Լ����� 

    Dice();

    void Init() { // m�ʱ�ȭ
        for (int i = 1; i <= 13; i++) {
            m[to_string(i)] = false;
            scoreMap[to_string(i)] = "( )";
        }
    }

    void DiceRoll() {
        nowSum = 0; // sum�� ���������� �ʱ�ȭ
        for (int i = 0; i < 5; i++) {
            int num = rand() % 6 + 1;
            diceArr[i] = num;
        }
        nowSum = NowDiceSum();
    } // �ֻ��� ������

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

    bool NothingRank(bool arr[]) { // �ֻ����� ����� ���� �ƹ��� ������ �������� ���� ���              
        for (int i = 1; i <= 13; i++) {
            if (arr[i] == true) return false;
        }
        return true;
    }

    void PrintNumber() {
        cout << "����� �ֻ��� : ";
        for (int i = 0; i < 5; i++) {
            cout << diceArr[i] << " ";
        }
        cout << endl;
    } // �ֻ��� �ѹ� ���        

    int Single(int num) { // �̱� ���� üũ
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
            cout << "�̱����� ������ 63�̻��̶� ���ʽ� ���� 35���� ȹ���մϴ�!" << endl;
            score += 35;
        }
    }

    virtual void CheckRank() {
        cout << "����üũ" << endl;
    } // ����Ȯ��        
};

struct HandRanking {
    string name;
    int inputNum;
    virtual bool CheckHandRanking(Dice* dice) {
        cout << "����Ȯ��" << endl;
        return true;
    }
    virtual void PrintRank(Dice* dice) {
        cout << "���� ���� �� ���� ���" << endl;
    }
};

struct ThreeOfKindRanking : HandRanking {

    ThreeOfKindRanking() {
        name = "ThreeOfKind";
        inputNum = 7;
    }

    void PrintRank(Dice* dice) override {
        cout << "Three-Of-Kind ���� : " << dice->nowSum << "�� || �Է� -> " << inputNum << endl;
    }

    bool CheckHandRanking(Dice* dice) override {
        map<int, int> map;
        for (int i = 0; i < 5; i++) {
            map[dice->diceArr[i]] += 1;
        }
        for (auto i : map) {
            if (i.second >= 3 && dice->m["7"] == false) { // ���� ���� 3���̻��϶�
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
        cout << "Four-Of-Kind ���� : " << dice->nowSum << "�� || �Է� -> " << inputNum << endl;
    }

    bool CheckHandRanking(Dice* dice) override {
        map<int, int> map;
        for (int i = 0; i < 5; i++) {
            map[dice->diceArr[i]] += 1;
        }
        for (auto i : map) {
            if (i.second >= 4 && dice->m["8"] == false) { // ���� ���� 3���̻��϶�
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
        cout << "FullHouse ���� : 25�� || �Է� -> " << inputNum << endl;
    }

    bool CheckHandRanking(Dice* dice) override {
        map<int, int> map;
        for (int i = 0; i < 5; i++) {
            map[dice->diceArr[i]] += 1;
        }
        if (map.size() != 2 || dice->m["9"] == true) return false; // Ǯ�Ͽ콺�� 2���� �ֻ��������־����
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
        cout << "SmallStraight ���� : 30�� || �Է� -> " << inputNum << endl;
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
        cout << "LargeStraight ���� : 40�� || �Է� -> " << inputNum << endl;
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
        cout << "Chance ���� : " << dice->nowSum << "�� || �Է� -> " << inputNum << endl;
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
        cout << "Yahtzee ���� : 50�� || �Է� -> " << inputNum << endl;
    }

    bool CheckHandRanking(Dice* dice) override {
        map<int, int> map;
        for (int i = 0; i < 5; i++) {
            map[dice->diceArr[i]] += 1;
        }
        for (auto i : map) {
            if (i.second >= 5 && dice->m["13"] == false) { // ���� ���� 3���̻��϶�
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
            cout << "�ֻ����� �ٽ� �������� 1, �����ٸ� 2�� �����ּ���. : ";
            string input;
            cin >> input;
            if (input == "1") {
                cout << "�ٽ� ���� �ֻ����� �ε����� �Է����ּ���! (ex. 123 <- 1, 2, 3��° �ֻ��� �ٽô���) : ";
                string diceIndex;
                cin >> diceIndex;
                for (int j = 0; j < diceIndex.size(); j++) {
                    if (diceIndex[j] - '0' < 1 || diceIndex[j] - '0' > 5) { // �ε��� �ش� ���ڰ� �ƴϰų� �����϶�
                        cout << "�߸��� ���� �Է��ϼ̾��. �ش� �ֻ����� ��������ʽ��ϴ�!" << endl;                        
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
                cout << "�߸��� ���� �Է��߾��! �ٽ� �Է��ϼ���!" << endl;
                i--;
            }
        }
    }

    void CheckRank() override { // ����Ȯ��           
        bool trueCheckArr[14] = { false, }; // ���� ���� �� �ִ� �����ε����� true�� �ְ� true�� ���� ���� �ְ�
        string rankInput;

        cout << endl << "-------------�Է� ���� ����-------------" << endl << endl;

        for (int i = 1; i <= 6; i++) { // �̱�����Ȯ��
            singleArr[i] = Single(i);
            if (m[to_string(i)] == false) {                
                cout << rankNameArr[i] << " ���� : " << singleArr[i] << "�� || �Է� -> " << i << endl;
                trueCheckArr[i] = true;
            }
        }

        for (int i = 0; i < 7; i++) { //��������Ȯ��
            if (rankArr[i]->CheckHandRanking(this) && m[to_string(rankArr[i]->inputNum)] == false) {
                rankArr[i]->PrintRank(this);
                trueCheckArr[rankArr[i]->inputNum] = true;
            }
        }

        if (NothingRank(trueCheckArr)) { // �̺κе� trueCheckArr�� �����Ұ� false��� �̶�� �������� ���氡��
            cout << "ä�� �� �ִ� ������ ������ �����ϴ�! ���� ä������ ���� ������ �Է��Ͻø� �� ���� 0�� ä�����ϴ�" << endl;
            for (int i = 1; i <= 13; i++) {
                if (m[to_string(i)] == false) {
                    cout << i << " ";
                }
            }
            cout << "�� ������ ���� ���� �Էµ����ʾҽ��ϴ�! �� ���� ���� �Է��Ͽ� ä�켼��" << endl;
            while (1) {
                cin >> rankInput;
                if (!CheckInput(rankInput)) {
                    cout << "�ٽ� �Է��ϼ���" << endl;
                    continue;
                }
                else break;
            }            
            m[rankInput] = true;
            scoreMap[rankInput] = "0";
        }
        else {
            // ��� ���� ó�� �� �� �Է� ��
            while (1) {                
                cin >> rankInput;
                if (!CheckInput(rankInput)) {
                    cout << "�ٽ� �Է��ϼ���" << endl;
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
                        cout << "�̹� ä���� ���Դϴ�! �ٽ� �����ϼ���" << endl;
                        continue;
                    }

                    cout << "���� ������ �������� �ʾ� 0���� ä�����ϴ�! ���� ä��÷��� 1��, �ٽ� �����Ϸ��� 2�� �Է��ϼ���." << endl;
                    int input;
                    cin >> input;
                    if (input == 1) {
                        m[rankInput] = true;
                        scoreMap[rankInput] = "0";
                        break;
                    }
                    else {
                        cout << "�ٽ� �Է��ϼ���. �Ǽ��������ʽÿ�" << endl;
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
    cout << "Ones            : " << player->scoreMap["1"] << " / " << secondPlayer->scoreMap["1"] << "   �Է�Ű : 1" << endl;
    cout << "Twos            : " << player->scoreMap["2"] << " / " << secondPlayer->scoreMap["2"] << "   �Է�Ű : 2" << endl;
    cout << "Threes          : " << player->scoreMap["3"] << " / " << secondPlayer->scoreMap["3"] << "   �Է�Ű : 3" << endl;
    cout << "Fours           : " << player->scoreMap["4"] << " / " << secondPlayer->scoreMap["4"] << "   �Է�Ű : 4" << endl;
    cout << "Fives           : " << player->scoreMap["5"] << " / " << secondPlayer->scoreMap["5"] << "   �Է�Ű : 5" << endl;
    cout << "Sixes           : " << player->scoreMap["6"] << " / " << secondPlayer->scoreMap["6"] << "   �Է�Ű : 6" << endl;
    cout << "Sum             :   " << player->singleSum << " / " << secondPlayer->singleSum << endl;
    cout << "Three of a kind : " << player->scoreMap["7"] << " / " << secondPlayer->scoreMap["7"] << "   �Է�Ű : 7" << endl;
    cout << "Four of a kind  : " << player->scoreMap["8"] << " / " << secondPlayer->scoreMap["8"] << "   �Է�Ű : 8" << endl;
    cout << "Full House      : " << player->scoreMap["9"] << " / " << secondPlayer->scoreMap["9"] << "   �Է�Ű : 9" << endl;
    cout << "SmallStraight   : " << player->scoreMap["10"] << " / " << secondPlayer->scoreMap["10"] << "   �Է�Ű : 10" << endl;
    cout << "LargeStraight   : " << player->scoreMap["11"] << " / " << secondPlayer->scoreMap["11"] << "   �Է�Ű : 11" << endl;
    cout << "Chance          : " << player->scoreMap["12"] << " / " << secondPlayer->scoreMap["12"] << "   �Է�Ű : 12" << endl;
    cout << "YAHTZEE         : " << player->scoreMap["13"] << " / " << secondPlayer->scoreMap["13"] << "   �Է�Ű : 13" << endl;
    cout << "TOTAL SCORE     :   " << player->score << " / " << secondPlayer->score << endl;
}

void DiceBattle(PlayerDice* player, PlayerDice* secondPlayer) {
    PrintScoreTable(player, secondPlayer);
    cout << endl << "1p�� �����Դϴ�!" << endl;
    player->DiceRoll();
    player->PrintNumber();
    player->DiceReRoll();
    player->CheckRank();
    Sleep(1500);
    system("cls");
    PrintScoreTable(player, secondPlayer);
    cout << "2p�� �����Դϴ�!" << endl;
    secondPlayer->DiceRoll();
    secondPlayer->PrintNumber();
    secondPlayer->DiceReRoll();
    secondPlayer->CheckRank();
    Sleep(1500);
    system("cls");
}

void PrintResult(Dice* player, Dice* secondPlayer) {
    cout << "������ ����Ǿ����ϴ�!" << endl;
    player->SingleBonus();
    secondPlayer->SingleBonus();
    PrintScoreTable(player, secondPlayer);
    cout << "1P ���� : " << player->score << endl;
    cout << "2P ���� : " << secondPlayer->score << endl;
    if (secondPlayer->score > player->score) {
        cout << "2P�� �¸��Ͽ����ϴ�!" << endl;
    }
    else if (secondPlayer->score == player->score) {
        cout << "�����ϴ�!" << endl;
    }
    else {
        cout << "1P�� �¸��Ͽ����ϴ�!" << endl;
    }
}

void PrintStartScene() {
    cout << "����. ������ �����մϴ�. ���ӽ����� �ֹ��� �ܿ켼��." << endl;
    cout << "*     *    *       ****  *    *  *******   " << endl;
    cout << " *   *    * *     *      *    *     *      " << endl;
    cout << "  * *    *   *    *      ******     *      " << endl;
    cout << "   *    *******   *      *    *     *      " << endl;
    cout << "   *   *       *  *      *    *     *      " << endl;
    cout << "   *  *         *  ****  *    *     *      " << endl;
    cout << "���ӽ��� ";
    string start;
    cin >> start;
    if (start != "start") {
        cout << "��ȣ�� Ʋ�ȴ�! ���� ����!" << endl;
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
        if (secondPlayerDice.GameEnd()) { // 2P�� �����̹Ƿ�
            PrintResult(&playerDice, &secondPlayerDice);
            break;
        }
    }
}


