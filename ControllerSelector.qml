import QtQuick 2.0
import org.asteroid.controls 1.0
import org.asteroid.utils 1.0
import SdlGameController 1.0

Item {
    //% "Press a button on the controller to select it."
    property var pressButtonStr: qsTrId("id-gc-selection")
    //% "Controller %1 selected."
    property var controllerSelectedStr: qsTrId("id-gc-selected").arg(SdlGameController.activeJoyId)

    Component.onCompleted: SdlGameController.enable()
    Component.onDestruction: SdlGameController.disable()
    Connections {
        target: SdlGameController
        onAxisEvent: {
            console.log("onAxisEvent " + joyId + " " + axis + " " + value)
            SdlGameController.activeJoyId = joyId
        }
        onButtonEvent: {
            console.log("onButtonEvent " + joyId + " " + pressed + " " + button)
            SdlGameController.activeJoyId = joyId
        }
    }
    Label {
        id: buttonPress
        text: (SdlGameController.activeJoyId == -1) ? pressButtonStr : controllerSelectedStr
        wrapMode: Text.WordWrap
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
    }
}