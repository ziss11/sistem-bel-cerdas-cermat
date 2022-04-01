#define pbt1 13
#define pbt2 12
#define pbt3 11
#define standByBtnPin A0

#define data1 2
#define latch1 3
#define clock1 4

#define data2 5
#define latch2 6
#define clock2 7

#define data3 8
#define latch3 9
#define clock3 10

int countStandByClick = 0;
bool isStandBy = true;
bool isLocked = false;

int shift1[3] = {data1, latch1, clock1};
int shift2[3] = {data2, latch2, clock2};
int shift3[3] = {data3, latch3, clock3};

int n = sizeof(shift1) / sizeof(shift1[0]);

void setLedOuput(int *pinData, int pinDataSize, char inputString)
{
    switch (inputString)
    {
    case 'E':
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b00000000);
        digitalWrite(pinData[1], HIGH);
        break;
    case 'A':
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b11111010);
        digitalWrite(pinData[1], HIGH);
        break;
    case 'B':
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b00111110);
        digitalWrite(pinData[1], HIGH);
        break;
    case 'C':
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b10011100);
        digitalWrite(pinData[1], HIGH);
        break;
    case 0:
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b11111100);
        digitalWrite(pinData[1], HIGH);
        break;
    case 1:
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b01100000);
        digitalWrite(pinData[1], HIGH);
        break;
    case 2:
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b11011010);
        digitalWrite(pinData[1], HIGH);
        break;
    case 3:
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b11110010);
        digitalWrite(pinData[1], HIGH);
        break;
    case 4:
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b11110010);
        digitalWrite(pinData[1], HIGH);
        break;
    case 5:
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b10110110);
        digitalWrite(pinData[1], HIGH);
        break;
    case 6:
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b10111110);
        digitalWrite(pinData[1], HIGH);
        break;
    case 7:
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b11100000);
        digitalWrite(pinData[1], HIGH);
        break;
    case 8:
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b11111110);
        digitalWrite(pinData[1], HIGH);
        break;
    case 9:
        digitalWrite(pinData[1], LOW);
        shiftOut(pinData[0], pinData[2], LSBFIRST, 0b11110110);
        digitalWrite(pinData[1], HIGH);
        break;
    }
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
        setLedOuput(shift1, n, 'A');
        setLedOuput(shift2, n, 'B');
        setLedOuput(shift3, n, 'C');
    }
}

void setup()
{
    Serial.begin(9600);

    pinMode(pbt1, INPUT_PULLUP);
    pinMode(pbt2, INPUT_PULLUP);
    pinMode(pbt3, INPUT_PULLUP);
    pinMode(standByBtnPin, INPUT_PULLUP);

    pinMode(data1, OUTPUT);
    pinMode(latch1, OUTPUT);
    pinMode(clock1, OUTPUT);

    pinMode(data2, OUTPUT);
    pinMode(latch2, OUTPUT);
    pinMode(clock2, OUTPUT);

    pinMode(data3, OUTPUT);
    pinMode(latch3, OUTPUT);
    pinMode(clock3, OUTPUT);

    setLedOuput(shift1, n, 'E');
    setLedOuput(shift2, n, 'E');
    setLedOuput(shift3, n, 'E');
}

void loop()
{
    if (digitalRead(standByBtnPin) == LOW)
    {
        standByBtnAction();
    }
    if (!isStandBy)
    {
        setLedOuput(shift1, n, 'A');
        setLedOuput(shift2, n, 0);
        setLedOuput(shift3, n, 1);
        delay(2000);

        setLedOuput(shift1, n, 'B');
        setLedOuput(shift2, n, 0);
        setLedOuput(shift3, n, 2);
        delay(2000);

        setLedOuput(shift1, n, 'C');
        setLedOuput(shift2, n, 0);
        setLedOuput(shift3, n, 3);
        delay(2000);

        countStandByClick = 0;
        standByBtnAction();
    }
}