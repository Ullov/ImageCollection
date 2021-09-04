var component;
var sprite;

function createPageObject(path) {
    if(sprite){
        console.log("sprite.destroy() ",typeof sprite);
        sprite.destroy();
        console.log("component.destroy() ",typeof component);
        component.destroy();
    }
    component = Qt.createComponent(path);
    if (component.status === Component.Ready)
        finishCreation();
    else
        component.statusChanged.connect(finishCreation);
}

function finishCreation() {
    if (component.status == Component.Ready) {
        sprite = component.createObject(root);
        if (sprite == null) {
            // Error Handling
            console.log("Error creating object");
        }
    } else{
        if (component.status === Component.Error) {
        // Error Handling
        console.log("Error loading component:", component.errorString());
        }else{
            console.log("Component status changed:", component.status);
        }
    }
}
