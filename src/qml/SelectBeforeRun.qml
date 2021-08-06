import QtQuick 2.0
import org.asteroid.controls 1.0
import org.asteroid.utils 1.0
import RomManager 1.0
import SdlGameController 1.0

Item {
    id: root
    property var pop

    property var path
    //% "No controller selected, do you wish to continue?"
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
        anchors.centerIn: parent
        width: parent.width - (DeviceInfo.hasRoundScreen ? Dims.w(5) : 0)
        horizontalAlignment: Text.AlignHCenter
    }

    IconButton {
        iconName: "ios-close-circle-outline"
        edge: undefinedEdge
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -Dims.w(15)
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Dims.h(15)
        onClicked: root.pop()
    }

    IconButton {
        iconName: "ios-checkmark-circle-outline"
        edge: undefinedEdge
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: Dims.w(15)
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Dims.h(15)
        onClicked: RomManager.run(path)
    }
}