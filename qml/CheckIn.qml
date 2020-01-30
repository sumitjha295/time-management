import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    title: qsTr("Check In")
    property var checkInReasons : [
               { data : "Client" },
               { data : "Interview"},
               { data : "Business Meeting"},
               { data : "Repair and Maintenance"},
               { data : "Collections and Deliveries"},
               { data : "IT Team"},
               { data : "Others"}
            ];

    ListModel {
        id: checkInModel
    }

    Grid {
        anchors.centerIn: parent
        columns: 3
        spacing: 9
        Repeater{
            model: checkInModel
            delegate: Button{
                text: model.data
                onClicked: {
                    mainWindow.changeView("UserForm.qml", {"title": model.data})
                }
            }
        }
    }

    Component.onCompleted: {
        var count = checkInReasons.length;
        checkInModel.clear()
        for(var i=0; i < count; ++i) {
            checkInModel.append(checkInReasons[i])
        }
    }

}
