import QtQuick 2.0
import org.asteroid.controls 1.0
import org.asteroid.utils 1.0
import SdlGameController 1.0

Item {
    signal close

    Component.onCompleted: SdlGameController.enable()

    Connections {
        target: SdlGameController
        onJoyAxisEvent: {
            console.log("onAxisEvent " + joyId + " " + axis + " " + value)
            if (SdlGameController.joyId == -1) {
                SdlGameController.joyId = joyId
                SdlGameController.disableBut(SdlGameController.joyId)
            } else if (Math.abs(value) > 32000) {
                if (SdlGameController.map == SdlGameController.GC_BUTTON_MAX) {
                    close()
                } else {
                    SdlGameController.setAxisToMap(axis, value > 0)
                }
            }
        }
        onJoyButtonEvent: {
            console.log("onButtonEvent " + joyId + " " + pressed + " " + button)
            if (SdlGameController.joyId == -1) {
                SdlGameController.joyId = joyId
                SdlGameController.disableBut(SdlGameController.joyId)
            } else if (pressed) {
                if (SdlGameController.map == SdlGameController.GC_BUTTON_MAX) {
                    close()
                } else {
                    SdlGameController.setButtonToMap(button)
                }
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
        text: qsTrId("id-button-map").arg(SdlGameController.mapStr)
        wrapMode: Text.WordWrap
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        visible: (SdlGameController.joyId != -1) && (SdlGameController.map != SdlGameController.GC_BUTTON_MAX)
    }
    Label {
        //% "Controller mapped, press a button or tap to exit."
        text: qsTrId("id-controller-mapped")
        wrapMode: Text.WordWrap
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        visible: SdlGameController.map == SdlGameController.GC_BUTTON_MAX
    }

    MouseArea {
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        onClicked: {
            console.log(SdlGameController.map + " " + SdlGameController.GC_BUTTON_MAX)
            if (SdlGameController.map == SdlGameController.GC_BUTTON_MAX) {
                close()
            } else {
                SdlGameController.setButtonToMap(-1)
            }
        }
    }
}