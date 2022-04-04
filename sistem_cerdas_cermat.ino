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
bool isStandBy = true;
bool isLocked = false;
bool isAnswered = false;

char currentTeamStand = 'A';

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
    countStandByClick++;
    delay(500);

    if (countStandByClick == 2)
    {
        isStandBy = false;
    }
    else
    {
        isStandBy = true;
        displayOutput(10, 11, 12);
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
        if (digitalRead(pbt1) == LOW)
        {
            currentTeamStand = 'A';
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
            isLocked = true;
            isAnswered = true;
        }
        else if (digitalRead(pbt2) == LOW)
        {
            currentTeamStand = 'B';
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
            isLocked = true;
            isAnswered = true;
        }
        else if (digitalRead(pbt3) == LOW)
        {
            currentTeamStand = 'C';
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
            if (currentTeamStand == 'A')
            {
                scoreTeamA += 2;
            }
            else if (currentTeamStand == 'B')
            {
                scoreTeamB += 2;
            }
            else if (currentTeamStand == 'C')
            {
                scoreTeamC += 2;
            }
            displayOutput(15, 15, 15);
            isAnswered = false;
        }
        else if (digitalRead(decScorePin) == LOW)
        {
            if (currentTeamStand == 'A')
            {
                scoreTeamA--;
                scoreTeamA = checkScore(scoreTeamA);
            }
            else if (currentTeamStand == 'B')
            {
                scoreTeamB--;
                scoreTeamB = checkScore(scoreTeamB);
            }
            else if (currentTeamStand == 'C')
            {
                scoreTeamC--;
                scoreTeamC = checkScore(scoreTeamC);
            }
            displayOutput(14, 14, 14);
            isAnswered = false;
            isLocked = false;
            participantAction();
        }
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
    pinMode(interupt, INPUT);
    attachInterrupt(digitalPinToInterrupt(interupt), participantAction, LOW);
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
    // participantAction();
    scoreMechanism();
}