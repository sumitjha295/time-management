import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    title: qsTr("Check out")

    Label {
        id: inputLabel
        text: qsTr("<h1>Please Scan the BarCode from your Pass to checkout!</h1>
                  <h4>Hi, Good day! Help us know our guests</h4>
                  <h4>Please help yourself!</h4>")
        anchors.centerIn: parent
    }

    TextInput {
        id: textInput
        anchors.top: inputLabel.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 20
        focus: true
     }
}
