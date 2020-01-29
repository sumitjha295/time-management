import QtQuick 2.12
import QtQuick.Controls 2.5
import "."

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Time Management")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
    }

    Drawer {
        id: drawer
        width: 150
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Check-in")
                width: parent.width
                onClicked: {
                    stackView.push("CheckIn.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Check-out")
                width: parent.width
                onClicked: {
                    stackView.push("CheckOut.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: homepage
        anchors.fill: parent
    }

    Home {
        id : homepage
        onChangeView : {
            stackView.push(page)
        }
    }
}
