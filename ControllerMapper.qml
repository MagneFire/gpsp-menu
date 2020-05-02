import QtQuick 2.0
import org.asteroid.controls 1.0
import org.asteroid.utils 1.0
import SdlGameController 1.0

Item {
    Component.onCompleted: SdlGameController.enable()
    Component.onDestruction: SdlGameController.disable()

    Connections {
        target: SdlGameController
        onJoyAxisEvent: {
            console.log("onAxisEvent " + joyId + " " + axis + " " + value)
            if (SdlGameController.joyId == -1) {
                SdlGameController.joyId = joyId
                SdlGameController.disableBut(SdlGameController.joyId)
            } else if (Math.abs(value) > 3200) {
                SdlGameController.setAxisToMap(axis, value > 0)
            }
        }
        onJoyButtonEvent: {
            console.log("onButtonEvent " + joyId + " " + pressed + " " + button)
            if (SdlGameController.joyId == -1) {
                SdlGameController.joyId = joyId
                SdlGameController.disableBut(SdlGameController.joyId)
            } else if (pressed) {
                SdlGameController.setButtonToMap(button)
            }
        }
    }
    Label {
        id: buttonPress
        //% "Press a button to start mapping"
        text: qsTrId("id-button-map-start")
        wrapMode: Text.WordWrap
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        visible: SdlGameController.joyId == -1
    }
    Label {
        //% "Press %1 button, tap to ignore"
        text: qsTrId("id-button-map").arg(SdlGameController.map)
        wrapMode: Text.WordWrap
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        visible: SdlGameController.joyId != -1
    }

    MouseArea {
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        onClicked: SdlGameController.setButtonToMap(-1)
    }
}