#include "SparkFun-Spark-Phant.h"

const char server[] = "data.sparkfun.com";
const char publicKey[] = "DJjNowwjgxFR9ogvr45Q";
const char privateKey[] = "P4eKwGGek5tJVz9Ar84n";
Phant phant(server, publicKey, privateKey);

const int POST_RATE = 30000; // Time between posts, in ms.
unsigned long lastPost = 0;

void setup()
{
	Serial.begin(9600);
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
	pinMode(A3, INPUT);
	pinMode(A4, INPUT);
	pinMode(A5, INPUT);
}

void loop()
{
    if (lastPost + POST_RATE < millis())
    {
        if (postToPhant() > 0)
        {
            lastPost = millis();
        }
    }
	delay(1000);
}

int postToPhant()
{
    phant.add("analog0", analogRead(A0));
    phant.add("analog1", analogRead(A1));
    phant.add("analog2", analogRead(A2));
    phant.add("analog3", analogRead(A3));
    phant.add("analog4", analogRead(A4));
    phant.add("analog5", analogRead(A5));
	
    TCPClient client;
    char response[512];
    int i = 0;
    int retVal = 0;
    
    if (client.connect(server, 80))
    {
        Serial.println("Posting!");
        client.print(phant.post());
        delay(1000);
        while (client.available())
        {
            char c = client.read();
            //Serial.print(c);
            if (i < 512)
                response[i++] = c;
        }
        if (strstr(response, "200 OK"))
        {
            Serial.println("Post success!");
            retVal = 1;
        }
        else if (strstr(response, "400 Bad Request"))
        {
            Serial.println("Bad request");
            retVal = -1;
        }
        else
        {
            retVal = -2;
        }
    }
    else
    {
        Serial.println("connection failed");
        retVal = -3;
    }
    client.stop();
    return retVal;	
}