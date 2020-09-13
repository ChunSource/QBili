import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14

Window {
    id: mrect
    visible: true
    width: 350
    height: 650
    title: qsTr("QBili 弹幕窗口")
    opacity: 1
    property int listviewwidth: 400
    property int listviewheigth: 400

    Component.onCompleted:
        {
        hide()
        console.log("初始化 qml")
        //function addMessage(mcolor,title,level,userlevelcolor,ulevel,message,name,showtitle)
        //addMessage("#5896de","no","15","#969696","15","测试弹幕","ChunSource",true)
        //addMessage("#5896de","我是勋章","15","#969696","15","测试弹幕","ChunSource",false)
        }
    ListModel
    {
        id : dmode
    }
    
    ListView {
        id : listview;
        x:10
        width: mrect.width-10
        height: mrect.height
        model: dmode

        delegate: Rectangle
        {
            anchors.topMargin:100
            height : danmuRectangle.height+5;
            width : listview.width
            Rectangle{//粉丝牌
                id:mid
                width: one.width+two.width+1
                height: 20
                radius: 2
                visible:showTitle
                anchors.left: parent.left
                Rectangle{//徽章
                    id:one
                    color: mcolor
                    width: textDanmu.width+10
                    height: mid.height
                    radius: 2
                    anchors.right: two.left
                    anchors.left: parent.left
                    Text {
                        anchors.centerIn: parent
                        id:textDanmu
                        //height: parent.height
                        lineHeight : Text.FixedHeight
                        horizontalAlignment : Text.AlignHCenter
                        verticalAlignment : Text.AlignVCenter
                        font.pixelSize: 10
                        text: title
                        color: "white"
                        font.family: "微软雅黑" 
                        font.bold: true
                    }
                }
                Rectangle{ //等级
                    border.color: mcolor
                    border.width: 1
                    
                    id:two
                    color: "white"
                    height: 20
                    width: levelText.contentWidth+7
                    anchors.left: one.right
                    //    anchors.centerIn: parent
                    Text {
                        anchors.centerIn: parent
                        horizontalAlignment : Text.AlignHCenter
                        verticalAlignment : Text.AlignVCenter
                        id: levelText
                        text: level
                        color: mcolor
                        font.family: "微软雅黑" 
                        
                    }
                }
                
                //     }
                
            }
            
            Rectangle{//用户等级
                id:userLevelRectangle
                width: userLevel.contentWidth+5+5
                height: 20
                anchors.left: (mid.visible)?mid.right:parent.left
                Rectangle{
                    height: 20
                    width: userLevel.contentWidth+5
                    anchors.centerIn: parent
                    border.width: 1
                    border.color: userLevelColor
                    Text {
                        id:userLevel
                        color: userLevelColor
                        horizontalAlignment : Text.AlignHCenter
                        verticalAlignment : Text.AlignVCenter
                        font.pixelSize: 10
                        anchors.centerIn: parent
                        font.family: "微软雅黑" 
                        text: "UL "+ulevel
                        
                    }
                }
                
                
            }
            
            Rectangle{//用户名
                id:userNameRectangle
                height: 20
                width: username.contentWidth+5
                anchors.left: userLevelRectangle.right
                Text {
                    color: "#23ade5" 
                    id:username
                    text: userName
                    font.family: "微软雅黑" 
                    font.pixelSize: 11
                }
            }
            
            Rectangle{ //弹幕
                id:danmuRectangle
                color: "white"
                height: danmuText.contentHeight+5
                width: 25
                anchors.left: userNameRectangle.right
                Text {
                    width: mrect.width-mid.width-userLevelRectangle.width-10
                    id:danmuText
                    color: "#646c7a"
                    font.family: "微软雅黑" 
                    text: danmu
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                }
            }
        
            
        }
    }
    
    ScrollBar {
        id: vbar
        visible: false
        hoverEnabled: true
        active: hovered || pressed
        orientation: Qt.Vertical
        size: mrect.height/listview.height
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        policy: ScrollBar.AlwaysOn
    }
    ScrollBar {
        id: hbar
        visible: false
        hoverEnabled: true
        active: hovered || pressed
        orientation: Qt.Horizontal
        size: mrect.width/listview.width
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
    
    function addMessage(mcolor,title,level,userlevelcolor,ulevel,message,name,showtitle)
    {
        addElement({"mcolor":mcolor,
                       "title":title,
                       "level":level,
                       "userLevelColor":userlevelcolor,
                       "ulevel":ulevel,
                       "danmu":message,
                       "userName":name,
                       "showTitle":showtitle})
        listview.positionViewAtEnd()
        
    }

    function addElement(ele)
    {
        dmode.append(ele)
    }
    function clearData()
    {
        dmode.clear();
    }
    function hide()
    {
        mrect.visible = false;
        console.log("qml hide ");
    }
    function show()
    {
        mrect.visible = true;
        console.log("qml show ");
    }
    function setOpeatly(s)
    {
        mrect.opacity = s
    }
}
