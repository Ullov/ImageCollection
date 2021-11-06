function returnSelectedFilesPaths()
{
    var selected = []
    for (var i = 0; i < modelItem.count; i++)
        if (modelItem.model.get(i).sel)
            selected.push(modelItem.model.get(i).filePath)
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
            it.triggered.connect(function(){fsExplorerHandle.slotCd(it.text, uuid)})
        }
        f(item)
    }
    driveMenu.open()
}

function formFilesList(elements, append) {
    if (elements["refresh"])
    {
        addressTextField.text = elements["currentDir"]
        modelItem.model.clear()
        var gi = 0
    }
    else
    {
        if (elements["currentDir"] !== addressTextField.text)
            return

        gi = modelItem.model.count
    }
    var jArr = elements["dir"]
    for (var i = 0; jArr[i]; i++, gi++)
    {
        jArr[i]["index"] = gi
        jArr[i]["sel"] = false
        jArr[i]["fileType"] = "Directory"
        modelItem.model.append(jArr[i])
    }

    jArr = elements["file"]
    for (i = 0; jArr[i]; i++, gi++)
    {
        jArr[i]["index"] = gi
        jArr[i]["sel"] = false
        modelItem.model.append(jArr[i])
    }
}
