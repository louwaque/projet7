import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import Projet7 1.0

Item {
    id: item
    readonly property string title: app.currentGame !== null ? app.currentGame.name : ""

    Dialog {
        id: dialogError
        x: (item.width - width) / 2
        y: (item.height - height) / 2
        title: qsTr("An error occurred")
        standardButtons: Dialog.Ok
        onClosed: app.closeCurrentGame()

        Label {
            id: labelError
        }
    }

    Connections {
        target: app.currentGame
        onError: {
            labelError.text = errorString
            dialogError.open()
        }
    }

    Component {
        id: serverGame

        ColumnLayout {
            TextField {
                Layout.fillWidth: true
                text: app.currentGame.name
                onAccepted: app.currentGame.name = text
            }
        }
    }

    Component {
        id: clientGame

        ColumnLayout {
            Label {
                Layout.fillWidth: true
                text: app.currentGame.name
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 6

        Loader {
            Layout.fillWidth: true
            sourceComponent: app.currentGame.type === Game.ServerGame ? serverGame : clientGame
        }

        TextField {
            Layout.fillWidth: true
            visible: app.currentGame !== null && app.currentGame.type === Game.ClientGame
            text: Projet7.userName
            onAccepted: Projet7.userName = text
        }

        Label {
            Layout.fillWidth: true
            text: app.currentGame !== null ? "Adresse ip : " + app.currentGame.ipAddress + ":" + app.currentGame.port : ""
        }

        Button {
            Layout.fillWidth: true
            text: qsTr("Open factions")
            onClicked: stack.push("qrc:///FactionsView.qml")
        }

        Button {
            Layout.fillWidth: true
            text: qsTr("Open players")
            onClicked: stack.push("qrc:///PlayersView.qml")
        }

        Button {
            Layout.fillWidth: true
            text: qsTr("Open map")
            onClicked: stack.push("qrc:///MapView.qml")
        }

        Button {
            Layout.fillWidth: true
            text: qsTr("Roll a die")
            onClicked: stack.push("qrc:///DieView.qml")
        }

        Button {
            Layout.fillWidth: true
            text: qsTr("Quit the game")
            onClicked: app.closeCurrentGame()
        }
    }
}
