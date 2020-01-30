import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import User 1.0

Page {
    title: qsTr("Check-In Information")
    property string firstName
    property string lastName

    Item {
        id: content
        anchors.centerIn: parent

        User {
            id: user
            name: firstName + " " + lastName;
            onNameChanged: {
                image.source = user.generateQR()
            }

        }

        RowLayout {
            anchors.bottom: infoLayout.top
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                id: label1
                text: qsTr("Title")
                Layout.columnSpan: 2
            }



            ComboBox {
                id: title
                Layout.columnSpan: 2
                Layout.fillWidth: true
                model: ["Mr.", "Ms."]
            }
        }

        GridLayout {
            id: infoLayout
            anchors.centerIn: parent
            columnSpacing: 8
            rowSpacing: 8
            columns: 1

            Image {
                id: image
            }
        }

        RowLayout {
            anchors.top: infoLayout.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10
            Rectangle{

            }

            Button {
                id: save
                text: qsTr("Print")
                onClicked: {
                    user.print()
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
