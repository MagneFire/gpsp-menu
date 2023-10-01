#include "SdlGameController.h"

SdlGameController * SdlGameController::instance;

SdlGameController::SdlGameController()
  : joyId(-1)
  , activeJoyId(-1)
  , interval(100)
  , lastActiveMapping(-1)
  , currentButtonToMap(GC_BUTTON_A)
{

  int ret;
  ret = SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
  if (ret != 0) {
    fprintf(stderr, "SDL_Init failed: %s\r\n", SDL_GetError());
    exit(1);
  }
  SDL_JoystickEventState(SDL_ENABLE);
  SDL_GameControllerEventState(SDL_ENABLE);

  joyCount = SDL_NumJoysticks();
  printf("Number of joysticks found: %d\r\n", joyCount);

  settings = Settings::getInstance();

  QObject::connect(&eventTimer, &QTimer::timeout, this, &SdlGameController::onEventTrigger);
}

SdlGameController::~SdlGameController() {

}

QObject * SdlGameController::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return getInstance();
}

SdlGameController * SdlGameController::getInstance() {
    if (instance == nullptr)
        instance = new SdlGameController();

    return instance;
}

void SdlGameController::setInterval(int delay) {
  interval = delay;
}

void SdlGameController::enableTimer() {
  if (!eventTimer.isActive()) {
    eventTimer.start(interval);
  }
}
void SdlGameController::disableTimer() {
  eventTimer.stop();
}

void SdlGameController::enable() {
  for (int i=0; i < getCount(); ++i) {
    enable(i);
  }
}

void SdlGameController::enable(int joyId) {
    SDL_Joystick *joystick;
    SDL_GameController *gameController;
    SDL_JoystickGUID guid;
    char *mapping;
		char* guidStr = new char[64];
    if (SDL_IsGameController(joyId)) {
      gameController = SDL_GameControllerOpen(joyId);
      mapping = SDL_GameControllerMapping(gameController);
      joystick = SDL_GameControllerGetJoystick(gameController);
      printf("Gamecontroller Name: %s\r\n", SDL_GameControllerName(gameController));
      printf("Gamecontroller Mapping: %s\r\n", mapping);
    } else {
      joystick = SDL_JoystickOpen(joyId);
      printf("Joystick Name: %s\r\n", SDL_JoystickName(joystick));
    }
    guid = SDL_JoystickGetGUID(joystick);
    SDL_JoystickGetGUIDString(guid, guidStr, 64);

    printf("Gamecontroller/Joystick GUID: %s\r\n", guidStr);
    enableTimer();
}
void SdlGameController::disable() {
  for (int i=0; i<getCount(); ++i) {
    disable(i);
  }
  disableTimer();
}

void SdlGameController::select(int id) {
  if (joyId == id) return;
  setJoyId(id);
  for (int i=0; i<getCount(); ++i) {
    if (i != id) {
      disable(i);
    }
  }
}
void SdlGameController::disable(int joyId) {
  SDL_GameControllerClose(SDL_GameControllerFromInstanceID(joyId));
}
int SdlGameController::getCount() {
    return joyCount;
}
void SdlGameController::onEventTrigger() {
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_JOYAXISMOTION:
        //printf("SDL_JOYAXISMOTION: %d %d\r\n", event.jaxis.axis, event.jaxis.value);
        emit joyAxisEvent(event.jaxis.which, event.jaxis.axis, event.jaxis.value);
        break;
      case SDL_JOYHATMOTION:
        //printf("SDL_JOYHATMOTION: %d %d\r\n", event.jhat.hat, event.jhat.value);
        emit joyHatEvent(event.jhat.which, event.jhat.hat, event.jhat.value);
        break;
      case SDL_JOYBUTTONDOWN:
        //printf("SDL_JOYBUTTONDOWN: %d\r\n", event.jbutton.button);
        emit joyButtonEvent(event.jbutton.which, event.jbutton.state == SDL_PRESSED, event.jbutton.button);
        break;
      case SDL_JOYBUTTONUP:
        //printf("SDL_JOYBUTTONUP: %d\r\n", event.jbutton.button);
        emit joyButtonEvent(event.jbutton.which, event.jbutton.state == SDL_PRESSED, event.jbutton.button);
        break;
      case SDL_CONTROLLERAXISMOTION:
        //printf("SDL_CONTROLLERAXISMOTION: %d %d\r\n", event.caxis.axis, event.caxis.value);
        emit axisEvent(event.caxis.which, event.caxis.axis, event.caxis.value);
        break;
      case SDL_CONTROLLERBUTTONDOWN:
        //printf("SDL_CONTROLLERBUTTONDOWN: %d\r\n", event.cbutton.button);
        emit buttonEvent(event.cbutton.which, event.cbutton.state == SDL_PRESSED, event.cbutton.button);
        break;
      case SDL_CONTROLLERBUTTONUP:
        //printf("SDL_CONTROLLERBUTTONUP: %d\r\n", event.cbutton.button);
        emit buttonEvent(event.cbutton.which, event.cbutton.state == SDL_PRESSED, event.cbutton.button);
        break;
      default:
        //printf("onEventTrigger: UNKNOWN! %d\r\n", event.type);
        break;
    }
  }
}

QString SdlGameController::getCurrentKeyToMapString() {
    switch(currentButtonToMap) {
      case GC_BUTTON_A:
        return "A";
      case GC_BUTTON_B:
        return "B";
      case GC_BUTTON_MENU:
        return "MENU";
      case GC_BUTTON_START:
        return "START";
      case GC_BUTTON_SELECT:
        return "SELECT";
      case GC_BUTTON_L:
        return "L";
      case GC_BUTTON_R:
        return "R";
      case GC_BUTTON_DPAD_UP:
        return "DPAD UP";
      case GC_BUTTON_DPAD_DOWN:
        return "DPAD DOWN";
      case GC_BUTTON_DPAD_LEFT:
        return "DPAD LEFT";
      case GC_BUTTON_DPAD_RIGHT:
        return "DPAD RIGHT";
      default:
        break;
    }
  return QVariant::fromValue(currentButtonToMap).toString();
}

int SdlGameController::getCurrentKeyToMap() {
  return currentButtonToMap;
}

void SdlGameController::resetMapper() {
  currentButtonToMap = GC_BUTTON_A;
}

void SdlGameController::setKeyToMap(QString value, int skip) {
  if (currentButtonToMap == GC_BUTTON_MAX) return;

  QString* alreadyMapped = std::find(std::begin(buttonMap), std::end(buttonMap), value);
  if ((alreadyMapped == std::end(buttonMap)) || skip) {
    buttonMap[currentButtonToMap] = value;

    switch(currentButtonToMap) {
      case GC_BUTTON_A:
        currentButtonToMap = GC_BUTTON_B;
        break;
      case GC_BUTTON_B:
        currentButtonToMap = GC_BUTTON_MENU;
        break;
      case GC_BUTTON_MENU:
        currentButtonToMap = GC_BUTTON_START;
        break;
      case GC_BUTTON_START:
        currentButtonToMap = GC_BUTTON_SELECT;
        break;
      case GC_BUTTON_SELECT:
        currentButtonToMap = GC_BUTTON_L;
        break;
      case GC_BUTTON_L:
        currentButtonToMap = GC_BUTTON_R;
        break;
      case GC_BUTTON_R:
        currentButtonToMap = GC_BUTTON_DPAD_UP;
        break;
      case GC_BUTTON_DPAD_UP:
        currentButtonToMap = GC_BUTTON_DPAD_DOWN;
        break;
      case GC_BUTTON_DPAD_DOWN:
        currentButtonToMap = GC_BUTTON_DPAD_LEFT;
        break;
      case GC_BUTTON_DPAD_LEFT:
        currentButtonToMap = GC_BUTTON_DPAD_RIGHT;
        break;
      case GC_BUTTON_DPAD_RIGHT:
        currentButtonToMap = GC_BUTTON_MAX;
        break;
      default:
        break;
    }
    emit currentKeyToMapChanged();
  }

  if (currentButtonToMap == GC_BUTTON_MAX) {
    writeMapping();
  }
}

void SdlGameController::setButtonToMap(int key) {
  if (joyId == -1) return;
  //printf("setButtonToMap: %d\r\n", key);
  QString value = QString("b");
  value.append(QString::number(key));
  setKeyToMap(value, (key == -1));
}


void SdlGameController::setHatToMap(int hat, int val) {
  if (joyId == -1) return;
  //printf("setHatToMap: %d %d\r\n", hat, val);
  QString value = QString("h");
  value.append(QString::number(hat));
  value.append(".");
  value.append(QString::number(val));
  setKeyToMap(value);
}

void SdlGameController::setAxisToMap(int axis, int direction) {
  if (joyId == -1) return;
  //printf("setAxisToMap: %d %d\r\n", axis, direction);
  QString value = QString("a");
  value.append(QString::number(axis));
  value.prepend(direction ? "+" : "-");
  setKeyToMap(value);
}

void SdlGameController::activateMappings() {
  const auto mappingsPath = settings->getMappingsPath();
  int ret = SDL_GameControllerAddMappingsFromFile(mappingsPath.toStdString().c_str());
  if (ret == -1) {
    printf("Failure adding new mappings: %s\n", SDL_GetError());
  } else {
    printf("Added %d mappings.\n", ret);
  }
}

int SdlGameController::getJoyId() {
  return joyId;
}

void SdlGameController::setJoyId(int id) {
  if (joyId == id) return;
  joyId = id;
  emit joyIdChanged();
}

int SdlGameController::getActiveJoyId() {
  return activeJoyId;
}

void SdlGameController::setActiveJoyId(int id) {
  if (activeJoyId == id) return;
  activeJoyId = id;
  settings->setActiveGameController(activeJoyId);
  emit activeJoyIdChanged();
}
void SdlGameController::writeMapping() {
  QString map;
  SDL_JoystickGUID guid;
  char* guidStr = new char[64];
  guid = SDL_JoystickGetDeviceGUID(joyId);
  SDL_JoystickGetGUIDString(guid, guidStr, 64);
  map.append(guidStr);
  map.append(",");
  map.append(SDL_JoystickNameForIndex(joyId));
  for (int i = 0; i < GC_BUTTON_MAX; ++i) {
    if (buttonMap[i] == "b-1")
      continue;

    switch(i) {
        case GC_BUTTON_B:
          map.append(",b:");
          break;
        case GC_BUTTON_A:
          map.append(",a:");
          break;
        case GC_BUTTON_MENU:
          map.append(",y:");
          break;
        case GC_BUTTON_START:
          map.append(",start:");
          break;
        case GC_BUTTON_SELECT:
          map.append(",back:");
          break;
        case GC_BUTTON_L:
          map.append(",leftshoulder:");
          break;
        case GC_BUTTON_R:
          map.append(",rightshoulder:");
          break;
        case GC_BUTTON_DPAD_UP:
          map.append(",dpup:");
          break;
        case GC_BUTTON_DPAD_DOWN:
          map.append(",dpdown:");
          break;
        case GC_BUTTON_DPAD_LEFT:
          map.append(",dpleft:");
          break;
        case GC_BUTTON_DPAD_RIGHT:
          map.append(",dpright:");
          break;
    }
    map.append(buttonMap[i]);
  }
  map.append(",platform:Linux,");
  printf("%s\r\n", map.toStdString().c_str());
  settings->addMapping(guidStr, map.toStdString().c_str());
  settings->saveMappings();
}