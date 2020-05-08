import QtQuick 2.0
import org.asteroid.controls 1.0
import org.asteroid.utils 1.0
import RomManager 1.0
import SdlGameController 1.0

Item {
    property var path
    //% "No controller selected, press a button on the controller to continue or tap to skip."
    property var noControllerSelectedStr: qsTrId("id-warn-no-controller-selected")
    //% "Controller %1 selected. Loading ROM..."
    property var controllerSelectedStr: qsTrId("id-selected-loaded").arg(SdlGameController.activeJoyId)

    Component.onCompleted: SdlGameController.enable()
    Connections {
        target: SdlGameController
        onAxisEvent: SdlGameController.activeJoyId = joyId
        onButtonEvent: SdlGameController.activeJoyId = joyId
        onActiveJoyIdChanged: RomManager.run(path)
    }
    Label {
        id: controllerMessage
        text: (SdlGameController.activeJoyId == -1) ? noControllerSelectedStr : controllerSelectedStr
        wrapMode: Text.WordWrap
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width - (DeviceInfo.hasRoundScreen ? Dims.w(5) : 0)
        horizontalAlignment: Text.AlignHCenter
    }
    MouseArea {
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        onClicked: RomManager.run(path);
    }
}