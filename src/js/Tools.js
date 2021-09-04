function bytesToHumanReadable(bytes, mode)
{
    var ext = "/s"
    if (mode !== null)
        ext = ""
    if (bytes === 0)
        return 0 + " Bit" + ext

    var tmp = 0
    if (bytes < 1024)
    {
        tmp = bytes + " Byte" + ext
    }
    else
    {
        tmp = (bytes / 1024);
        if (tmp < 1024)
        {
            tmp = Math.round(tmp * 1000) / 1000
            tmp += " Kb" + ext
        }
        else
        {
            tmp = tmp / 1024
            if (tmp < 1024)
            {
                tmp = Math.round(tmp * 1000) / 1000
                tmp += " Mb" + ext
            }
            else
            {
                tmp = tmp / 1024;
                tmp = Math.round(tmp * 1000) / 1000
                tmp += " Gb" + ext
            }
        }
    }
    return tmp
}

function cutNumberByPercents(number, percents)
{
    return (number / 100) * percents;
}

function boolSwich(boolVariable) {
    if (boolVariable)
        boolVariable = false
    else
        boolVariable = true
    return boolVariable
}
