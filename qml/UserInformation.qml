import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
Page {
    title: qsTr("Check-In Information")
    Item {
        id: content
        anchors.centerIn: parent


        RowLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10

            Button {
                id: save
                text: qsTr("Print")
                onClicked: {
                    console.log("print")
                }
            }

            Button {
                id: cancel
                text: qsTr("Cancel")
                onClicked: {
                    mainWindow.goBack()
                }
            }
        }
    }
}
