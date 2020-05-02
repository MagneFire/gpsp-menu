import QtQuick 2.0
import org.asteroid.controls 1.0
import org.asteroid.utils 1.0
import SdlGameController 1.0

Item {

    property string btn: ""
    property string ax: ""

    Component.onCompleted: SdlGameController.enable()
    Component.onDestruction: SdlGameController.disable()

    Connections {
        target: SdlGameController
        onAxisEvent: {
            console.log("onAxisEvent " + joyId + " " + axis + " " + value)
            ax = joyId + " " + axis + " " + value
        }
        onButtonEvent: {
            console.log("onButtonEvent " + joyId + " " + pressed + " " + button)
            console.log("HOEOE " + SdlGameController.GC_BUTTON_A);
            btn = joyId + " " + button + " " + pressed
            if (button == SdlGameController.GC_BUTTON_A) {
                console.log("HEIOWHAOWHDOIAHO");
            }
        }
    }
    Label {
        id: buttonPress
        //% "Button: %1"
        text: qsTrId("id-button").arg(btn)
        wrapMode: Text.WordWrap
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
    }
    Label {
        id: axisVal
        //% "Axis: %1"
        text: qsTrId("id-axis").arg(ax)
        wrapMode: Text.WordWrap
        anchors.top: buttonPress.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
    }
}