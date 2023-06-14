import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.VirtualKeyboard 2.4
import QtQuick.VirtualKeyboard.Settings 2.2

import CTcpClient 1.0

Window {
    id: window

    property bool m_bShow: true
    property var m_activeLocales : ["en_GB"]
    property var m_locale: "en_GB"

    visible: m_bShow
    width: 1024
    height: 768
    color: "#ECF5FC"

    function slotReceiveMessage(msg,type) {
        if(0 === type) {
            logGif.visible = false
            m_bShow = false
            console.log("上位机已启动,隐藏当前程序")
            CTcpClient.writeMessage("",0)
        } else if(1 === type){
            rawTextField.text = msg
            rawTextField.focus = true
            m_bShow = true
        }
    }

    Component.onCompleted: {
        CTcpClient.onCppSignaReceiveMsg.connect(slotReceiveMessage)

        var lan = CTcpClient.getLanguage()
        lan = 0
        console.log("当前语言:",lan)
        if(1 === lan) {
            m_activeLocales = ["en_GB"]
            m_locale = "en_GB"
        }
        else if(2 === lan) {
            m_activeLocales = ["fr_FR","en_GB"]
            m_locale = "fr_FR"
        }
        else if(3 === lan) {
            m_activeLocales = ["de_DE","en_GB"]
            m_locale = "de_DE"
        }
        else if(4 === lan) {
            m_activeLocales = ["th_TH","en_GB"]
            m_locale = "th_TH"
        }
        else if(5 === lan) {
            m_activeLocales = ["es_ES","en_GB"]
            m_locale = "es_ES"
        }
        else if(0 === lan) {
            m_activeLocales = ["zh_CN","en_GB"]
            m_locale = "en_GB"
        }

        VirtualKeyboardSettings.activeLocales = m_activeLocales
        VirtualKeyboardSettings.locale = m_locale
        VirtualKeyboardSettings.wordCandidateList.alwaysVisible = true

        inputPanel.setKeyboardBackgroundColor("#D2ECF5FC")
        inputPanel.setKeysBackgroundColor("white")
        inputPanel.setKeysTextColor("black")
    }

    Item {
        id: logGif
        width: window.width
        height: window.height
        visible: false
        z: 101
        AnimatedImage {
            anchors.fill: parent            
        }
    }


    TextField {
        id: rawTextField
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 2
        height: 60
        width: parent.width - 4
        font.pixelSize: 26
        color: "#000"
        font.family: "Source Han Sans CN"
        font.bold: false
        focus: true
        objectName: "keyboardTextField"
        background: Rectangle {
            radius: 10
            border.width: 1
            border.color: "#BDBDBD"
        }
    }

    Button {
        z: 101
        width: 115
        height: 60
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 45
        anchors.right: parent.right
        anchors.rightMargin: 50

        onClicked: {
            console.log("close btn")
            m_bShow = false
            CTcpClient.writeMessage(rawTextField.text)
            rawTextField.text = ""
        }

        background: Rectangle {
            color: "transparent"
            //border.width: 1
            //border.color: "red"
        }
    }

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height - inputPanel.height
        width: parent.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }

        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
