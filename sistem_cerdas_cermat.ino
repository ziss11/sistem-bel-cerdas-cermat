#define pbt1 11
#define pbt2 10
#define pbt3 9
#define startLedPin 2
#define endLedPin 8

int pin = 0;
bool isLocked = false;

void setLedOuput(char inputString)
{
    switch (inputString)
    {
    case 'A':
        for (pin = startLedPin; pin <= endLedPin; pin++)
        {
            if (pin != 7)
                digitalWrite(pin, HIGH);
            else
                digitalWrite(pin, LOW);
        }
        break;
    case 'B':
        for (pin = startLedPin; pin <= endLedPin; pin++)
        {
            if (pin != 2 && pin != 3)
                digitalWrite(pin, HIGH);
            else
                digitalWrite(pin, LOW);
        }
        break;
    case 'C':
        for (pin = startLedPin; pin <= endLedPin; pin++)
        {
            if (pin != 3 && pin != 4 && pin != 8)
                digitalWrite(pin, HIGH);
            else
                digitalWrite(pin, LOW);
        }
        break;
    }
}
void setup()
{
    Serial.begin(9600);
    pinMode(pbt1, INPUT_PULLUP);
    pinMode(pbt2, INPUT_PULLUP);
    pinMode(pbt3, INPUT_PULLUP);

    for (pin = startLedPin; pin <= endLedPin; pin++)
    {
        pinMode(pin, OUTPUT);
    }
}

void loop()
{
    if (isLocked == false)
    {
        if (digitalRead(pbt1) == LOW)
        {
            setLedOuput('A');
        }
        if (digitalRead(pbt2) == LOW)
        {
            setLedOuput('B');
        }
        if (digitalRead(pbt3) == LOW)
        {
            setLedOuput('C');
        }
    }
}