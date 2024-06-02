
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

import app


Rectangle {
    Layout.fillHeight: true
    Layout.fillWidth: true
    color: "white"

    ColumnLayout {
        anchors.centerIn: parent
        width: parent.width * 0.8

        Label {
            text: qsTr("Вход")
            font.pointSize: 24
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignCenter
        }

        TextField {
            id: usernameInput
            placeholderText: qsTr("Имя пользователя")
            Layout.fillWidth: true
            Material.accent: Material.Blue
        }

        TextField {
            id: passwordInput
            placeholderText: qsTr("Пароль")
            Layout.fillWidth: true
            echoMode: TextInput.Password
            Material.accent: Material.Blue
        }

        Button {
            id: registerButton
            text: qsTr("Войти")
            Material.accent: Material.Blue
            Layout.fillWidth: true
            onClicked: {
                AppBackend.signIn(usernameInput.text, passwordInput.text)
            }
        }
    }

    Button {
        id: loginButton
        text: qsTr("Зарегестрироваться")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        Material.elevation: 2
        background: Rectangle {
            radius: 20
            color: Material.BlueGrey
        }
        contentItem: Text {
            text: loginButton.text
            color: "white"
            font: loginButton.font
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        onClicked: {
            changePage("SignUpPage.qml")
        }
    }

    Connections {
        target: AppBackend
        function onSignInDone() {
            changePage("PersonalAreaPage.qml")
        }
    }
}
