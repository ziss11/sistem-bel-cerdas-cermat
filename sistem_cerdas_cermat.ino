#define pbt1 5
#define pbt2 6
#define pbt3 7
#define standByBtnPin 8
#define incScorePin 9
#define decScorePin 10

#define interupt A0

#define data 2
#define latch 3
#define clock 4

int countStandByClick = 0;
bool isStandBy = true;
bool isLocked = false;

char currentTeamStand = 'A';

int scoreTeamA = 0;
int scoreTeamB = 0;
int scoreTeamC = 0;

int digit[14]{
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
}

void participantAction()
{
    if (countStandByClick == 1 && isLocked == false)
    {
        if (digitalRead(pbt1) == LOW)
        {
            displayOutput(10, 13, 13);
            isLocked = true;
        }
        else if (digitalRead(pbt2) == LOW)
        {
            displayOutput(13, 11, 13);
            isLocked = true;
        }
        else if (digitalRead(pbt3) == LOW)
        {
            displayOutput(13, 13, 12);
            isLocked = true;
        }
    }
}

void scoreBoardAction()
{
    if (isStandBy == false)
    {
        displayOutput(10, scoreTeamA, scoreTeamA);
        delay(2000);

        displayOutput(11, scoreTeamB, scoreTeamB);
        delay(2000);

        displayOutput(12, scoreTeamC, scoreTeamC);
        delay(2000);

        countStandByClick = 0;
        standByBtnAction();
    }
}

// void scoreMechanism()
// {
//     if (digitalRead(incScorePin) == LOW)
//     {
//         if (currentTeamStand == 'A')
//         {
//             scoreTeamA++;
//         }
//         else if (currentTeamStand == 'B')
//         {
//             scoreTeamB++;
//         }
//         else if (currentTeamStand == 'C')
//         {
//             scoreTeamC++;
//         }
//     }
//     else if (digitalRead(decScorePin) == LOW)
//     {
//         if (currentTeamStand == 'A')
//         {
//             scoreTeamA--;
//         }
//         else if (currentTeamStand == 'B')
//         {
//             scoreTeamB--;
//         }
//         else if (currentTeamStand == 'C')
//         {
//             scoreTeamC--;
//         }
//     }
// }

void setup()
{
    Serial.begin(9600);

    pinMode(pbt1, INPUT_PULLUP);
    pinMode(pbt2, INPUT_PULLUP);
    pinMode(pbt3, INPUT_PULLUP);
    pinMode(standByBtnPin, INPUT_PULLUP);

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
    // scoreMechanism();
}