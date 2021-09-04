function returnSelectedFilesPaths()
{
    var selected = []
    for (var i = 0; i < modelItem.count; i++)
        if (modelItem.get(i).sel)
            selected.push(modelItem.get(i).filePath)
    return selected
}

function createDrivesMenu(items, actions, xPos, yPos) {
    while (driveMenu.count > 0)
    {
        driveMenu.removeItem(0)
    }
    driveMenu.x = xPos
    driveMenu.y = yPos
    for (var i = 0; items[i]; i++)
    {
        var item = Qt.createQmlObject('import QtQuick 2.13; import QtQuick.Controls 2.13; MenuItem {}', driveMenu)
        item.text = items[i]
        driveMenu.addItem(item)
        var f = function(it) {
            it.triggered.connect(function(){fsExplorerHandle.slotCd(it.text)})
        }
        f(item)
    }
    driveMenu.open()
}

function formFilesList(elements) {
    modelItem.clear()
    var gi = 0
    var jArr = elements["dir"]
    for (var i = 0; jArr[i]; i++, gi++) {
        modelItem.append(
                    {
                        index: gi,
                        itemName: jArr[i]["fileName"],
                        iconPath: jArr[i]["iconPath"],
                        filePath: jArr[i]["filePath"],
                        isDir: true,
                        fileType: "Folder",
                        size: 0,
                        sel: false,
                        hov: false
                    })
    }

    jArr = elements["file"]
    for (i = 0; jArr[i]; i++, gi++) {
        modelItem.append(
                    {
                        index: gi,
                        itemName: jArr[i]["fileName"],
                        iconPath: jArr[i]["iconPath"],
                        filePath: jArr[i]["filePath"],
                        isDir: false,
                        fileType: jArr[i]["fileType"],
                        size: jArr[i]["size"],
                        sel: false,
                        hov: false
                    })
    }
}
