import QtQuick 2.0
import QtQuick.Controls 2.2
import "../../../js/Tools.js" as Tools

Rectangle {
    id: downloadBarRoot
    width: parent.width
    property double downloadedTotal: 0
    Rectangle {
        id: infoRectangle
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottomMargin: 10
        height: 25
        color: "Black"
        Text {
            id: downloadedTotalTextBox
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 5
            color: "White"
            text: "Total: "
            font.pixelSize: 14
        }
    }

    Rectangle {
        id: pixivDownloadRectangle
        anchors.top: infoRectangle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        height: 40
        color: "#292225"
        radius: 6
        Label {
            id: pixivDownloadTitleLabel
            anchors.verticalCenter: parent.verticalCenter
            width: 120
            text: "Pixiv"
            color: "White"
            font.pixelSize: 14
            x: 5
        }
        TextField {
            id: pixivTextField
            anchors.left: pixivDownloadTitleLabel.right
            anchors.right: pixivStatusTextBox.left
            anchors.margins: 5
            anchors.verticalCenter: parent.verticalCenter
            height: 30
            placeholderText: "Enter user ID..."
        }
        Text {
            id: pixivStatusTextBox
            anchors.right: pixivStartDownloadButton.left
            anchors.margins: 5
            anchors.verticalCenter: parent.verticalCenter
            width: 145
            font.pixelSize: 14
            color: "White"
            horizontalAlignment: Text.AlignRight
            //text: "(1000/1000) 200 Mb/s 00:00:00"
        }
        KawaiButton {
            id: pixivStartDownloadButton
            heightC: 30
            labelText: "Download"
            anchors.right: pixivStatusRectangle.left
            anchors.margins: 5
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                var param = [pixivTextField.text]
                var mode = ["pixiv"]
                apiHandler.universalStartDownloading(param, mode)
            }
        }
        Rectangle {
            id: pixivStatusRectangle
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.margins: 5
            color: "#00ff08"
            height: 20
            width: 20
        }
    }

    Rectangle {
        id: mangarockDownloadRectangle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: pixivDownloadRectangle.bottom
        anchors.margins: 5
        height: 40
        color: "#292225"
        radius: 6
        Label {
            id: mangarockDownloadTitleLabel
            anchors.verticalCenter: parent.verticalCenter
            width: 120
            text: "MangaRock"
            color: "White"
            font.pixelSize: 14
            x: 5
        }
        TextField {
            id: mangarockTextField
            anchors.left: mangarockDownloadTitleLabel.right
            anchors.right: mangarockStatusTextBox.left
            anchors.margins: 5
            anchors.verticalCenter: parent.verticalCenter
            height: 30
            placeholderText: "Enter manga OID..."
        }
        Text {
            id: mangarockStatusTextBox
            anchors.right: mangarockStartDownloadButton.left
            anchors.margins: 5
            anchors.verticalCenter: parent.verticalCenter
            width: 145
            font.pixelSize: 14
            color: "White"
            horizontalAlignment: Text.AlignRight
        }
        KawaiButton {
            id: mangarockStartDownloadButton
            heightC: 30
            labelText: "Download"
            anchors.right: mangarockStatusRectangle.left
            anchors.margins: 5
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                var param = [mangarockTextField.text]
                var mode = ["mangarock"]
                apiHandler.universalStartDownloading(param, mode)
            }
        }
        Rectangle {
            id: mangarockStatusRectangle
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.margins: 5
            color: "#00ff08"
            height: 20
            width: 20
        }
    }

    Rectangle {
        id: exhentaiDownloadRectangle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: mangarockDownloadRectangle.bottom
        anchors.margins: 5
        height: 40
        radius: 6
        color: "#292225"
        Label {
            id: exhentaiLabel
            text: "ExHentai"
            width: 120
            anchors.verticalCenter: parent.verticalCenter
            color: "White"
            font.pixelSize: 14
            x: 5
        }
        TextField {
            id: galleryUrl
            anchors.left: exhentaiLabel.right
            anchors.right: exhentaiStatusTextBox.left
            anchors.margins: 5
            height: 30
            placeholderText: "Enter gallery URL..."
            anchors.verticalCenter: parent.verticalCenter
        }
        Text {
            id: exhentaiStatusTextBox
            anchors.right: exhentaiButton.left
            anchors.margins: 5
            anchors.verticalCenter: parent.verticalCenter
            width: 145
            font.pixelSize: 14
            color: "White"
            horizontalAlignment: Text.AlignRight
        }
        KawaiButton {
            id: exhentaiButton
            heightC: 30
            labelText: "Download"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: exhentaiStatusRectangle.left
            anchors.margins: 5
            onClicked: {
                var param = [galleryUrl.text]
                var mode = ["exhentai", "download"]
                apiHandler.universalStartDownloading(param, mode)
            }
        }

        Rectangle {
            id: exhentaiStatusRectangle
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.margins: 5
            color: "#00ff08"
            height: 20
            width: 20
        }
    }

    Rectangle {
        id: mangadexDownloadRectangle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: exhentaiDownloadRectangle.bottom
        anchors.margins: 5
        height: 40
        radius: 6
        color: "#292225"
        Label {
            id: mangadexLabel
            text: "MangaDex"
            width: 120
            anchors.verticalCenter: parent.verticalCenter
            color: "White"
            font.pixelSize: 14
            x: 5
        }
        TextField {
            id: mangadexMangaId
            anchors.left: mangadexLabel.right
            anchors.right: checkBoxesRectangle.left
            anchors.margins: 5
            height: 30
            placeholderText: "Enter manga ID..."
            anchors.verticalCenter: parent.verticalCenter
        }
        Rectangle {
            id: checkBoxesRectangle
            anchors.right: mangadexStatusTextBox.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 5
            height: 30
            width: 200
            CheckBox {
                id: enLangChekbox
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                text: "EN"
            }
            CheckBox {
                id: ruLangChekbox
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: enLangChekbox.right
                text: "RU"
            }
            CheckBox {
                id: otherLangChekbox
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: ruLangChekbox.right
                text: "Other"
            }
        }
        Text {
            id: mangadexStatusTextBox
            anchors.right: mangadexButton.left
            anchors.margins: 5
            anchors.verticalCenter: parent.verticalCenter
            width: 145
            font.pixelSize: 14
            color: "White"
            horizontalAlignment: Text.AlignRight
        }
        KawaiButton {
            id: mangadexButton
            heightC: 30
            labelText: "Download"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: mangadexStatusRectangle.left
            anchors.margins: 5
            onClicked: {
                var param = [mangadexMangaId.text, enLangChekbox.checked, ruLangChekbox.checked, otherLangChekbox.checked]
                var mode = ["mangadex", "download"]
                apiHandler.universalStartDownloading(param, mode)
            }
        }

        Rectangle {
            id: mangadexStatusRectangle
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.margins: 5
            color: "#00ff08"
            height: 20
            width: 20
        }
    }

    Rectangle {
        id: vkDownloadRectangle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: mangadexDownloadRectangle.bottom
        anchors.margins: 5
        height: 40
        radius: 6
        color: "#292225"
        Label {
            id: vkLabel
            text: "Vk"
            width: 120
            anchors.verticalCenter: parent.verticalCenter
            color: "White"
            font.pixelSize: 14
            x: 5
        }
        TextField {
            id: vkMangaId
            anchors.left: vkLabel.right
            anchors.right: vkStatusTextBox.left
            anchors.margins: 5
            height: 30
            placeholderText: "Enter manga ID..."
            anchors.verticalCenter: parent.verticalCenter
        }
        Text {
            id: vkStatusTextBox
            anchors.right: vkButton.left
            anchors.margins: 5
            anchors.verticalCenter: parent.verticalCenter
            width: 145
            font.pixelSize: 14
            color: "White"
            horizontalAlignment: Text.AlignRight
        }
        KawaiButton {
            id: vkButton
            heightC: 30
            labelText: "Download"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: vkStatusRectangle.left
            anchors.margins: 5
            onClicked: {
                var param = [mangadexMangaId.text, enLangChekbox.checked, ruLangChekbox.checked, otherLangChekbox.checked]
                var mode = ["vk", "downloadPost"]
                apiHandler.universalStartDownloading(param, mode)
            }
        }

        Rectangle {
            id: vkStatusRectangle
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.margins: 5
            color: "#00ff08"
            height: 20
            width: 20
        }
    }

    Rectangle {
        id: mangairoDownloadRectangle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: vkDownloadRectangle.bottom
        anchors.margins: 5
        height: 40
        radius: 6
        color: "#292225"
        Label {
            id: mangairoLabel
            text: "MangaIro"
            width: 120
            anchors.verticalCenter: parent.verticalCenter
            color: "White"
            font.pixelSize: 14
            x: 5
        }
        TextField {
            id: mangairoTextField
            anchors.left: mangairoLabel.right
            anchors.right: mangairoStatusTextBox.left
            anchors.margins: 5
            height: 30
            placeholderText: "Enter manga URL..."
            anchors.verticalCenter: parent.verticalCenter
        }
        Text {
            id: mangairoStatusTextBox
            anchors.right: mangairoButton.left
            anchors.margins: 5
            anchors.verticalCenter: parent.verticalCenter
            width: 145
            font.pixelSize: 14
            color: "White"
            horizontalAlignment: Text.AlignRight
        }
        KawaiButton {
            id: mangairoButton
            heightC: 30
            labelText: "Download"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: mangairoStatusRectangle.left
            anchors.margins: 5
            onClicked: {
                var param = [mangairoTextField.text]
                var mode = ["mangairo", "download"]
                apiHandler.universalStartDownloading(param, mode)
            }
        }

        Rectangle {
            id: mangairoStatusRectangle
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.margins: 5
            color: "#00ff08"
            height: 20
            width: 20
        }
    }

    Rectangle {
        id: youtubeDownloadRectangle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: mangairoDownloadRectangle.bottom
        anchors.margins: 5
        height: 40
        radius: 6
        color: "#292225"
        Label {
            id: youtubeLabel
            text: "YouTube"
            width: 120
            anchors.verticalCenter: parent.verticalCenter
            color: "White"
            font.pixelSize: 14
            x: 5
        }
        TextField {
            id: youtubeTextField
            anchors.left: youtubeLabel.right
            anchors.right: youtubeStatusTextBox.left
            anchors.margins: 5
            height: 30
            placeholderText: "Enter video URL..."
            anchors.verticalCenter: parent.verticalCenter
        }
        Text {
            id: youtubeStatusTextBox
            anchors.right: youtubeButton.left
            anchors.margins: 5
            anchors.verticalCenter: parent.verticalCenter
            width: 145
            font.pixelSize: 14
            color: "White"
            horizontalAlignment: Text.AlignRight
        }
        KawaiButton {
            id: youtubeButton
            heightC: 30
            labelText: "Download"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: youtubeStatusRectangle.left
            anchors.margins: 5
            onClicked: {
                var param = [youtubeTextField.text]
                var mode = ["youtube", "download"]
                apiHandler.universalStartDownloading(param, mode)
            }
        }

        Rectangle {
            id: youtubeStatusRectangle
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.margins: 5
            color: "#00ff08"
            height: 20
            width: 20
        }
    }

    Rectangle {
        id: twitterDownloadRectangle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: youtubeDownloadRectangle.bottom
        anchors.margins: 5
        height: 40
        radius: 6
        color: "#292225"
        Label {
            id: twitterLabel
            text: "Twitter"
            width: 120
            anchors.verticalCenter: parent.verticalCenter
            color: "White"
            font.pixelSize: 14
            x: 5
        }
        TextField {
            id: twitterTextField
            anchors.left: twitterLabel.right
            anchors.right: twitterStatusTextBox.left
            anchors.margins: 5
            height: 30
            placeholderText: "Enter user name..."
            anchors.verticalCenter: parent.verticalCenter
        }
        Text {
            id: twitterStatusTextBox
            anchors.right: twitterButton.left
            anchors.margins: 5
            anchors.verticalCenter: parent.verticalCenter
            width: 145
            font.pixelSize: 14
            color: "White"
            horizontalAlignment: Text.AlignRight
        }
        KawaiButton {
            id: twitterButton
            heightC: 30
            labelText: "Download"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: twitterStatusRectangle.left
            anchors.margins: 5
            onClicked: {
                var param = [twitterTextField.text]
                var mode = ["twitter", "download"]
                apiHandler.universalStartDownloading(param, mode)
            }
        }

        Rectangle {
            id: twitterStatusRectangle
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.margins: 5
            color: "#00ff08"
            height: 20
            width: 20
        }
    }

    property double downloadedPixiv: 0
    property double downloadedExhentai: 0
    property double downloadedMangadex: 0
    property double downloadedVk: 0
    property double downloadedMangairo: 0
    property double downloadedYoutube: 0
    Connections {
        target: apiHandler
        onUniversalDownloadingStarted: {
            if (mode[0] == "exhentai")
                exhentaiStatusRectangle.color = "#ff0000"
            else if (mode[0] == "mangarock")
                mangarockStatusRectangle.color = "#ff0000"
            else if (mode[0] == "pixiv")
                pixivStatusRectangle.color = "#ff0000"
            else if (mode[0] == "mangadex")
                mangadexStatusRectangle.color = "#ff0000"
            else if (mode[0] == "vk")
                vkStatusRectangle.color = "#ff0000"
            else if (mode[0] == "mangairo")
                mangairoStatusRectangle.color = "#ff0000"
            else if (mode[0] == "youtube")
                youtubeStatusRectangle.color = "#ff0000"
        }
        onUniversalDownloadingFinished: {
            if (mode[0] == "exhentai" && mode[1] == "void")
                exhentaiStatusRectangle.color = "#00ff08"
            else if (mode[0] == "mangarock")
                mangarockStatusRectangle.color = "#00ff08"
            else if (mode[0] == "pixiv")
                pixivStatusRectangle.color = "#00ff08"
            else if (mode[0] == "mangadex")
                mangadexStatusRectangle.color = "#00ff08"
            else if (mode[0] == "vk")
                vkStatusRectangle.color = "#00ff08"
            else if (mode[0] == "mangairo")
                mangairoStatusRectangle.color = "#00ff08"
            else if (mode[0] == "youtube")
                youtubeStatusRectangle.color = "#00ff08"
        }
        onDownloadStatusSignal: {
            // (1000/1000) 200 Mb/s 00:00:00
            // 20428081
            downloadedTotal += list[0]
            if (downloaderType == "pixiv")
                downloadedPixiv += list[0];
            else if (downloaderType == "exhentai")
                downloadedExhentai += list[0];
            else if (downloaderType == "mangadex")
                downloadedMangadex += list[0];
            else if (downloaderType == "vk")
                downloadedVk += list[0];
            else if (downloaderType == "mangairo")
                downloadedMangairo += list[0];
            else if (downloaderType == "youtube")
                downloadedYoutube += list[0];
        }
    }

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            var tmp = downloadedPixiv
            downloadedPixiv = 0
            pixivStatusTextBox.text = Tools.bytesToHumanReadable(tmp)

            tmp = downloadedExhentai
            downloadedExhentai = 0
            exhentaiStatusTextBox.text = Tools.bytesToHumanReadable(tmp)

            tmp = downloadedMangadex
            downloadedMangadex = 0
            mangadexStatusTextBox.text = Tools.bytesToHumanReadable(tmp)

            tmp = downloadedVk
            downloadedVk = 0
            vkStatusTextBox.text = Tools.bytesToHumanReadable(tmp)

            tmp = downloadedMangairo
            downloadedMangairo = 0
            mangairoStatusTextBox.text = Tools.bytesToHumanReadable(tmp)

            tmp = downloadedYoutube
            downloadedYoutube = 0
            youtubeStatusTextBox.text = Tools.bytesToHumanReadable(tmp)

            downloadedTotalTextBox.text = "Total: " + Tools.bytesToHumanReadable(downloadedTotal).replace("/s", "")
        }
    }
}


