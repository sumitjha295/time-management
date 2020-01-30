import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    id: mainWindow
    function changeView(page, properties) {
        stackView.push(page, properties);
    }

    function goBack() {
        if (stackView.depth > 1) {
            stackView.pop()
        }
    }

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
                    goBack()
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
        height: mainWindow.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Check-in")
                width: parent.width
                onClicked: {
                    changeView("CheckIn.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Check-out")
                width: parent.width
                onClicked: {
                    changeView("CheckOut.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "Home.qml"
        anchors.fill: parent
    }
}
