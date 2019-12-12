//
// Created by Alexander Powers on 12/12/19.
//

#ifndef TEMPERATUREDISPLAY_COMPONENT_HXX
#define TEMPERATUREDISPLAY_COMPONENT_HXX


class Component {
public:
    // constructor
    Component(int mx, int my, int Mx, int My, char *fname, TouchScreen *ts, Adafruit_ILI9341 *tft) {
        this->filename = fname;
        this->minX = mx;
        this->maxX = Mx;
        this->minY = my;
        this->maxY = My;
        this->tsPtr = ts;
        this->tftPtr = tft;
    }

    // state
    // position data
    int minX;
    int maxX;
    int minY;
    int maxY;
    // pointers to global variables
    TouchScreen *tsPtr;
    Adafruit_ILI9341 *tftPtr;
    // iamge status
    ImageReturnCode stat;
    // filename to read from
    char *filename;

    // functions
    void render() {
        Serial.print(F("Loading..."));
        Serial.print(this->filename);
        Serial.print("\nsize:");
        Serial.print(this->minX);
        Serial.print(",");
        Serial.print(this->maxX);
        Serial.print(",");
        Serial.print(this->minY);
        Serial.print(",");
        Serial.print(this->maxY);
        Serial.print("\n");

        Serial.print(F("..DONE"));
        this->stat = reader.drawBMP(this->filename, *(this->tftPtr), this->minX, this->minY);
        reader.printStatus(this->stat);
    }

    bool containsPoint(TSPoint p) {
        if ((p.x < this->maxX) and (p.x > this->minX)) {
            if ((p.y < this->maxY) and (p.y > this->minY)) {
                return true;
            }
        }
        return false;
    }
};


#endif //TEMPERATUREDISPLAY_COMPONENT_HXX
