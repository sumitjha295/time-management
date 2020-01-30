import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    title: qsTr("Home")

    property var centerOffset: 100
    Button {
        id: checkInButton
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -centerOffset
        text: qsTr("Check-in")
        onClicked: mainWindow.changeView("CheckIn.qml")
    }

    Button {
        id: checkOutButton
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: centerOffset
        text: qsTr("Check-out")
        onClicked: mainWindow.changeView("CheckOut.qml")
    }
}
