#ifndef SDLGAMECONTROLLER_H
#define SDLGAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QQmlEngine>
#include <QJSEngine>

#include <SDL2/SDL.h>

#include "Settings.h"

class SdlGameController : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SdlGameController)
    Q_PROPERTY(int count READ getCount NOTIFY countChanged)


    Q_PROPERTY(QString mapStr READ getCurrentKeyToMapString NOTIFY currentKeyToMapChanged)
    Q_PROPERTY(int map READ getCurrentKeyToMap NOTIFY currentKeyToMapChanged)
    Q_PROPERTY(int joyId READ getJoyId WRITE setJoyId NOTIFY joyIdChanged)
    Q_PROPERTY(int activeJoyId READ getActiveJoyId WRITE setActiveJoyId NOTIFY activeJoyIdChanged)
public:
    enum GpspButtons {
        GC_BUTTON_A = SDL_CONTROLLER_BUTTON_A,
        GC_BUTTON_B = SDL_CONTROLLER_BUTTON_B,
        GC_BUTTON_MENU = SDL_CONTROLLER_BUTTON_Y,
        GC_BUTTON_START = SDL_CONTROLLER_BUTTON_START,
        GC_BUTTON_SELECT = SDL_CONTROLLER_BUTTON_BACK,
        GC_BUTTON_L = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
        GC_BUTTON_R = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
        GC_BUTTON_DPAD_UP = SDL_CONTROLLER_BUTTON_DPAD_UP,
        GC_BUTTON_DPAD_DOWN = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
        GC_BUTTON_DPAD_LEFT = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
        GC_BUTTON_DPAD_RIGHT = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
        GC_BUTTON_MAX = SDL_CONTROLLER_BUTTON_MAX
    };
    Q_ENUM(GpspButtons)

    SdlGameController();
    ~SdlGameController();
public:
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
    static SdlGameController *getInstance();

    void setInterval(int delay);

    Q_INVOKABLE void enable(int joyId);
    Q_INVOKABLE void enable();
    Q_INVOKABLE void disable(int joyId);
    Q_INVOKABLE void disable();
    Q_INVOKABLE void disableBut(int joyId);

    int getCount();
    QString getCurrentKeyToMapString();
    int getCurrentKeyToMap();
    Q_INVOKABLE void setButtonToMap(int key);
    Q_INVOKABLE void setAxisToMap(int axis, int direction);

    Q_INVOKABLE void activateKeyMapping(int joyId);

    int getJoyId();
    void setJoyId(int id);

    int getActiveJoyId();
    void setActiveJoyId(int id);
signals:
    void event(SDL_Event event);

    void axisEvent(int joyId, int axis, int value);
    void buttonEvent(int joyId, int pressed, int button);

    void joyAxisEvent(int joyId, int axis, int value);
    void joyButtonEvent(int joyId, int pressed, int button);

    void countChanged();
    void currentKeyToMapChanged();
    void joyIdChanged();
    void activeJoyIdChanged();
private slots:
    void onEventTrigger();

private:
    static SdlGameController * instance;
    QTimer eventTimer;
    int joyId;
    int activeJoyId;
    int joyCount;
    int interval;
    int lastActiveMapping;
    QString buttonMap[GC_BUTTON_MAX];
    GpspButtons currentButtonToMap;

    Settings* settings;

    void enableTimer();
    void disableTimer();
    void setKeyToMap(QString value, int skip = 0);
    void writeMapping();
};

#endif