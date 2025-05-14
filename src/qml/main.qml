import QtQuick 2.0
import org.asteroid.controls 1.0
import org.asteroid.utils 1.0
import Nemo.KeepAlive 1.1
import RomManager 1.0
import SdlGameController 1.0

Application {
    id: app

    centerColor: "#dfb103"
    outerColor: "#be4e0e"
    // The item to highlight. -1 indicates that nothing is highlighted.
    // Highlighting is only used when a controller button is pressed.
    property int romSelectedIndex: -1
    property int romMaxIndex: 0
    property bool mainVisible: false

    Component.onCompleted: {
        DisplayBlanking.preventBlanking = true
        SdlGameController.enable()
        SdlGameController.activateMappings()
    }

    Connections {
        target: SdlGameController
        function onButtonEvent(joyId, pressed, button) {
            if (!mainVisible) return
            if (!pressed) return

            if (button == SdlGameController.GC_BUTTON_DPAD_UP) {
                romSelectedIndex = romSelectedIndex > 1 ? romSelectedIndex - 1 : 0
            } else if (button == SdlGameController.GC_BUTTON_DPAD_DOWN) {
                romSelectedIndex = romSelectedIndex < romMaxIndex ? romSelectedIndex + 1 : romSelectedIndex;
            } else if (button == SdlGameController.GC_BUTTON_A) {
                if (romSelectedIndex > 0 && romSelectedIndex < romMaxIndex) {
                    RomManager.run(romSelectedIndex);
                } else if (romSelectedIndex == romMaxIndex) {
                    layerStack.push(settingsLayer)
                }
            }
        }
    }

    Component { id: controllerTestLayer;        ControllerTest        { } }
    Component { id: controllerMapLayer;         ControllerMapper      { } }
    Component { id: controllerSelectorLayer;    ControllerSelector    { } }
    Component {
        id: settingsLayer
        Settings {
            onControllerTestClicked: layerStack.push(controllerTestLayer)
            onControllerMapClicked: layerStack.push(controllerMapLayer)
            onControllerSelectorClicked: layerStack.push(controllerSelectorLayer)
        }
    }

    LayerStack {
        id: layerStack
        firstPage: firstPageComponent
        // main is the first layer (layer 0).
        // other layer may disable the controller
        onLayersChanged: {
            mainVisible = layers.length == 0
            if (!mainVisible) return
            SdlGameController.activateMappings()
        }
    }

    Component {
        id: firstPageComponent
        Item {
            Label {
                //% "No ROMs found please add them to the %1 folder"
                text: qsTrId("id-no-roms").arg(RomManager.root)
                wrapMode: Text.WordWrap
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                visible: romBrowser.count == 0
            }
            ListView {
                id: romBrowser
                anchors.fill: parent
                currentIndex: romSelectedIndex
                highlight: Rectangle { color: "white";  opacity: 0.2}
                // Centers currently highlighted item.
                preferredHighlightBegin: romBrowser.height/2 - app.height/12
                preferredHighlightEnd: romBrowser.height/2 + app.height/12
                highlightRangeMode: ListView.StrictlyEnforceRange

                model: RomManager.getModel()
                delegate: Item {
                    // The item height is 1/6 of the screen height, resulting in 5 items on screen.
                    height: app.height/6
                    width: parent.width

                    Rectangle {
                        width: parent.width
                        height: parent.height
                        color: "#ffffff"
                        opacity: ma.pressed ? 0.2 : 0.0
                    }
                    Label {
                        width: parent.width
                        anchors.left: parent.left
                        //anchors.leftMargin: app.height/2 - Math.sqrt(Math.pow(app.height/2, 2) - Math.pow(romBrowser.contentY,2))
                        //anchors.leftMargin: romBrowser.height/2 - Math.sqrt(Math.pow(romBrowser.height/2, 2) - Math.pow((romBrowser.height/2 + (parent.height * (romBrowser.contentY/parent.height - index)-parent.height/2)),2))
                        // We want items to move to the left when an item is near the middle of the screen:
                        //  / 1
                        // | 2
                        //  \ 3
                        // To achieve this we need to know the current y location of the element. This is provided by the FileModel.
                        // Using the index of the item and the current location of the top of the listview(contentY) we can find the location of a specific item.
                        // Next we use the Pythagoras rule (x^2+y^2=r^) to align the item around the left edge.
                        // Rewriting Pythagoras rule: sqrt(r^2 - y^2) => sqrt(listview_height/2^2 - location_item_y^2)
                        // Finally we add a small padding (Dims.w(5)) so that the item is not touching the left 'bezel'.
                        anchors.leftMargin: {
                            if (DeviceSpecs.hasRoundScreen) {
                                var itemLocationY = (parent.height * (romBrowser.contentY/parent.height - index) - parent.height/2);
                                var screenRadius = romBrowser.height/2;
                                screenRadius - Math.sqrt(Math.pow(screenRadius, 2) - Math.pow((screenRadius + itemLocationY),2)) + Dims.w(5);
                            } else {
                                0;
                            }
                        }
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: Dims.l(6)
                        text: title
                    }
                    MouseArea {
                        id: ma
                        width: parent.width
                        height: parent.height
                        anchors.centerIn: parent
                        onClicked: {
                            RomManager.run(path);
                        }
                    }
                }
                onCountChanged: romMaxIndex = romBrowser.count
            }

            IconButton {
                id: add
                visible: romBrowser.atYEnd
                enabled: opacity == 1.0
                opacity: visible ? (romSelectedIndex == romMaxIndex ? 0.8 : 1.0) : 0.0
                Behavior on opacity { NumberAnimation { duration: 200 } }
                iconName:  "ios-settings-outline"
                anchors { 
                    bottom: parent.bottom
                    horizontalCenter: parent.horizontalCenter
                    bottomMargin: Dims.iconButtonMargin
                }
                onClicked: layerStack.push(settingsLayer)
            }
        }
    }
}
