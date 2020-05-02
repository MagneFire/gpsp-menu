import QtQuick 2.0
import org.asteroid.controls 1.0
import org.asteroid.utils 1.0
import RomManager 1.0
import SdlGameController 1.0

Item {

    signal controllerTestClicked
    signal controllerMapClicked
    signal controllerSelectorClicked

    LayerStack {
        id: layerStack
        firstPage: firstPageComponent
    }

    Component {
        id: firstPageComponent
        Item {
            Component {
                id: lvHeader
                Item {
                    anchors.right: parent.right
                    anchors.left: parent.left
                    width: parent.width
                    height: (DeviceInfo.hasRoundScreen ? app.height/12 : 0)
                }
            }
            Component {
                id: lvFooter
                Item {
                    anchors.right: parent.right
                    anchors.left: parent.left
                    width: parent.width
                    height: (DeviceInfo.hasRoundScreen ? app.height/12 : 0) + (app.height/6)
                }
            }
            ListView {
                id: romBrowser
                anchors.fill: parent
                // Add a header to the top and the bottom
                // The paddingTopBottom will adjust the height based on whether the screen is round.
                header: lvHeader
                footer: lvFooter

                model: entries
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
                            if (DeviceInfo.hasRoundScreen) {
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
                            emitSignal()
                        }
                    }
                }
            }


            ListModel {
                id: entries

                ListElement {
                    //% "Test controller mapping"
                    title: qsTrId("id-settings-map-test")
                    property var emitSignal: function(){ controllerTestClicked(); }
                }
                ListElement {
                    //% "Map a controller"
                    title: qsTrId("id-settings-map")
                    property var emitSignal: function(){ controllerMapClicked(); }
                }
                ListElement {
                    //% "Select a controller"
                    title: qsTrId("id-settings-select")
                    property var emitSignal: function(){ controllerSelectorClicked(); }
                }
            }
        }
    }
}