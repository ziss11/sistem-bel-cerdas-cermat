#define pbt1 6
#define pbt2 7
#define pbt3 8
#define standByBtnPin 9
#define incScorePin 10
#define decScorePin 11

#define interupt 2

#define data 3
#define latch 4
#define clock 5

volatile boolean ledAisOn = false;
volatile boolean ledBisOn = false;
volatile boolean ledCisOn = false;

int countStandByClick = 0;
int numOfWrongAns = 0;
bool isStandBy = true;
bool isLocked = false;
bool isAnswered = false;
bool isAllTeamAnswered = false;
bool isCanAnswer[3] = {true, true, true};

char currentTeamStand;
char *answeredTeam = "";

char team1 = 'A';
char team2 = 'B';
char team3 = 'C';

int scoreTeamA = 0;
int scoreTeamB = 0;
int scoreTeamC = 0;

int digit[16]{
    0b11111100, // 0
    0b01100000, // 1
    0b11011010, // 2
    0b11110010, // 3
    0b01100110, // 4
    0b10110110, // 5
    0b10111110, // 6
    0b11100000, // 7
    0b11111110, // 8
    0b11110110, // 9
    0b11111010, // a
    0b00111110, // b
    0b10011100, // c
    0b00000000, // e
    0b01101110, // X salah
    0b00111010, // o benar
};

void displayOutput(int digit1, int digit2, int digit3)
{
    digitalWrite(latch, LOW);
    shiftOut(data, clock, LSBFIRST, digit[digit3]);
    shiftOut(data, clock, LSBFIRST, digit[digit2]);
    shiftOut(data, clock, LSBFIRST, digit[digit1]);
    digitalWrite(latch, HIGH);
}

void standByBtnAction()
{

    int digit[3] = {10, 11, 12};

    countStandByClick++;
    delay(500);

    if (countStandByClick == 2)
    {
        isStandBy = false;
    }
    else
    {
        isAllTeamAnswered = false;
        isStandBy = true;
        if (isAllTeamAnswered == false)
        {
            if (strchr(answeredTeam, team1) != NULL)
                digit[0] = 13;

            if (strchr(answeredTeam, team2) != NULL)
                digit[1] = 13;

            if (strchr(answeredTeam, team3) != NULL)
                digit[2] = 13;
        }
        else
        {
            digit[0] = 10;
            digit[1] = 11;
            digit[2] = 12;
            Serial.println("All team have answered");
        }

        displayOutput(digit[0], digit[1], digit[2]);
    }

    isLocked = false;
    ledAisOn = false;
    ledBisOn = false;
    ledCisOn = false;
}

void participantAction()
{
    if (countStandByClick == 1 && isLocked == false)
    {
        if (digitalRead(pbt1) == LOW && isCanAnswer[0])
        {
            currentTeamStand = team1;
            if (ledAisOn)
            {
                ledAisOn = false;
                displayOutput(13, 13, 13);
            }
            else
            {
                ledAisOn = true;
                displayOutput(10, 13, 13);
            }
            memset(answeredTeam, 0, 3);
            isLocked = true;
            isAnswered = true;
        }
        else if (digitalRead(pbt2) == LOW && isCanAnswer[1])
        {
            currentTeamStand = team2;
            if (ledBisOn)
            {
                ledBisOn = false;
                displayOutput(13, 13, 13);
            }
            else
            {
                ledBisOn = true;
                displayOutput(13, 11, 13);
            }
            memset(answeredTeam, 0, 3);
            isLocked = true;
            isAnswered = true;
        }
        else if (digitalRead(pbt3) == LOW && isCanAnswer[2])
        {
            currentTeamStand = team3;
            if (ledCisOn)
            {
                ledCisOn = false;
                displayOutput(13, 13, 13);
            }
            else
            {
                ledCisOn = true;
                displayOutput(13, 13, 12);
            }
            memset(answeredTeam, 0, 3);
            isLocked = true;
            isAnswered = true;
        }
    }
}

void scoreBoardAction()
{
    if (isStandBy == false)
    {
        displayOutput(10, floor(scoreTeamA / 10), scoreTeamA % 10);
        delay(2000);

        displayOutput(11, floor(scoreTeamB / 10), scoreTeamB % 10);
        delay(2000);

        displayOutput(12, floor(scoreTeamC / 10), scoreTeamC % 10);
        delay(2000);

        countStandByClick = 0;
        standByBtnAction();
    }
}

void scoreMechanism()
{
    if (isAnswered)
    {
        if (digitalRead(incScorePin) == LOW)
        {
            if (currentTeamStand == team1)
            {
                scoreTeamA += 2;
            }
            else if (currentTeamStand == team2)
            {
                scoreTeamB += 2;
            }
            else if (currentTeamStand == team3)
            {
                scoreTeamC += 2;
            }
            displayOutput(15, 15, 15);
            delay(1000);
            standByBtnAction();
            isAnswered = false;
        }
        else if (digitalRead(decScorePin) == LOW)
        {
            if (currentTeamStand == team1)
            {
                numOfWrongAns++;
                isCanAnswer[0] = false;
                scoreTeamA--;
                strncat(answeredTeam, &team1, 1);
                scoreTeamA = checkScore(scoreTeamA);
            }
            else if (currentTeamStand == team2)
            {
                numOfWrongAns++;
                isCanAnswer[1] = false;
                scoreTeamB--;
                strncat(answeredTeam, &team2, 1);
                scoreTeamB = checkScore(scoreTeamB);
            }
            else if (currentTeamStand == team3)
            {
                numOfWrongAns++;
                isCanAnswer[2] = false;
                scoreTeamC--;
                strncat(answeredTeam, &team3, 1);
                scoreTeamC = checkScore(scoreTeamC);
            }
            displayOutput(14, 14, 14);
            delay(1000);
            checkTeamAnswered();
            standByBtnAction();
            isAnswered = false;
            isLocked = false;
            participantAction();
        }
    }
    Serial.print("Team have answered: ");
    Serial.println(answeredTeam);
}

void checkTeamAnswered()
{
    if (numOfWrongAns == 3)
    {
        numOfWrongAns = 0;
        isAllTeamAnswered = true;
        memset(answeredTeam, 0, 3);

        for (int i = 0; i < 3; i++)
        {
            isCanAnswer[i] = true;
        }
        Serial.println("No more team can answered");
        Serial.println(strchr(answeredTeam, team1));
    }
}

int checkScore(int score)
{
    if (score < 0)
    {
        score = 0;
    }
    return score;
}

void setup()
{
    Serial.begin(9600);

    pinMode(pbt1, INPUT_PULLUP);
    pinMode(pbt2, INPUT_PULLUP);
    pinMode(pbt3, INPUT_PULLUP);
    pinMode(standByBtnPin, INPUT_PULLUP);
    // pinMode(interupt, INPUT);
    // attachInterrupt(digitalPinToInterrupt(interupt), participantAction, FALLING);
    pinMode(incScorePin, INPUT_PULLUP);
    pinMode(decScorePin, INPUT_PULLUP);
    pinMode(data, OUTPUT);
    pinMode(latch, OUTPUT);
    pinMode(clock, OUTPUT);

    displayOutput(13, 13, 13);
}

void loop()
{
    if (digitalRead(standByBtnPin) == LOW)
    {
        standByBtnAction();
    }
    scoreBoardAction();
    participantAction();
    scoreMechanism();
}