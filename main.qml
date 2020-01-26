import QtQuick 2.0
import org.asteroid.controls 1.0
import org.asteroid.utils 1.0

Application {
    id: app

    centerColor: "#dfb103"
    outerColor: "#be4e0e"
    property color overlayColor: "#b07414"

    LayerStack {
        id: layerStack
        firstPage: firstPageComponent
    }

    Component {
        id: firstPageComponent

        Item {
            Item {
                id: paddingTop
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.left: parent.left
                width: parent.width
                height: DeviceInfo.hasRoundScreen ? Dims.h(10) : 0
            }
            Item {
                id: lisView
                height: parent.height - (DeviceInfo.hasRoundScreen ? Dims.h(20) : 0)
                width: parent.width
                anchors.top: paddingTop.bottom
                ListView {
                    id: romBrowser
                    anchors.fill: parent
                    height: parent.height

                    model: myModel
                    delegate: Item {
                        height: Dims.h(16)
                        width: parent.width

                        Rectangle {
                            width: parent.width
                            height: parent.height
                            color: "#ffffff"
                            opacity: ma.pressed ? 0.2 : 0.0
                        }
                        Label {
                            anchors.left: parent.left
                            anchors.leftMargin: Dims.w(15)
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: Dims.l(6)
                            text: title
                            width: parent.width
                        }
                        MouseArea {
                            id: ma
                            width: parent.width
                            height: parent.height
                            anchors.centerIn: parent
                            onClicked: myModel.run(path);
                        }
                    }
                }
            }
        }
    }
}
