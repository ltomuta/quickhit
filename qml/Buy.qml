/**
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0
import "Game.js" as GameScript
import IAP 1.0 // In-App Purchase API, custom QML item


Item {
    id: container
    width: gameArea.width * 0.8
    height: gameArea.height * 0.6
    opacity: 0
    state: "hidden"

    property bool levelDownloaded: false
    property int levelCountBeforePurchase: 0

    /**
     * Hides the list of purchasable items.
     */
    function doBack()
    {
        messageProgress.hideOngoingMessage();
        container.state = "hidden";

        // Cancels only level sis package loading from backend
        //iap.cancel();
    }

    /**
     * Close view on error
     */
    function error()
    {
        console.log("Error, close view");
        gameArea.closeBuyView();
    }

    /**
     * If a level was downloaded, reloads the plugins (i.e. updates the list
     * of levels in the main menu) and closes the buy view.
     */
    function foreground()
    {
        // If application gets to foreground even when level is downloaded
        // and installing is ongoing or ended, the level list updated.
        if (levelDownloaded) {
            console.log("Level was loaded");
            gameArea.reloadPlugins();

            // Check is level already installed
            if (levelCountBeforePurchase != GameEngine.pluginList().length) {
                // Level was installed. Close Buy view and go to main menu
                console.log("Level was installed");
                gameArea.closeBuyView();
            }
        }
    }

    /**
     * Finds product IDs from the back-end server for the "quickhit" product.
     */
    function searchIapDataFromBackend()
    {
        console.log("searchIapDataFromBackend()");
        iap.getIapDataFromBackend("quickhit");
        // Response to onIapBackendDataReaded
    }

    /**
     * Finds product metadata from Nokia Store.
     */
    function searchProductMetaDataFromStore()
    {
        if (iap.iapready) {
            messageProgress.showMessageOngoing("Searching products from Store...");
            var count = iap.productDataCount();

            for (var i = 0; i < count; i++) {
                var product = iap.productDataAt(i);
                iap.getIapDataFromStore(product.id);
                // Response to onProductDataRead
            }
        }
    }

    /**
     * Adds a product to the list model containing the purchasable items.
     * @param product The product to add.
     */
    function setProductDataToModel(product)
    {
        // Get current level list
        var plugins = GameEngine.pluginList();
        levelCountBeforePurchase = plugins.length;

        // Do not show existing levels in the purchase list
        var levelExists = false;

        if (plugins.length > 0) {
            for (var i = 0; i < plugins.length; i++) {
                var level = plugins[i];

                if (level.iapid == product.id) {
                    levelExists = true;
                    break;
                }
            }
        }

        // Add project to model
        if (!levelExists) {
            // Fill model with Nokia Store Publishing data and
            // data from our own backend server
            // Level for sale
            listModel.append({ "iapId" : product.id,
                               "displayName" : product.title,
                               "price" : product.price,
                               "thumbnail" : product.thumbnail,
                               "valid" : "1" });
        }

        if (listModel.count < 1) {
            message.showMessage("No new levels available", 5000);
            noLevelsTimer.restart();
        }
    }

    /**
     * Remove purchased product from the list model.
     * @param productId The ID of the purchased product to remove.
     */
    function removeProductFromDataModel(productId)
    {
        for (var i = 0; i < listModel.count; i++) {
            if (listModel.get(i).iapId == productId) {
                listModel.remove(i);
                break;
            }
        }
    }

    Timer {
        id: noLevelsTimer
        interval: 4000
        repeat: false
        running: false

        onTriggered: {
            gameArea.closeBuyView();
        }
    }

    Rectangle {
        id: background
        color: "black"
        border.color: "white"
        border.width: 2
        radius: 8
        smooth: true
        anchors.fill: parent
        opacity: listView.opacity > 0 ? 0.7 : 0
    }

    IAPurchase {
        id: iap

        onIapCreatedAndReady: {
            console.log("onIapCreatedAndReady");
            messageProgress.showMessageOngoing("Reading product information");
            searchIapDataFromBackend();
        }
        onIapBackendDataReaded: {
            console.log("onIapBackendDataReaded");
            messageProgress.hideOngoingMessage();
            message.showMessage("Product information read", 5000);
            console.log("Backend product count is " + productDataCount());

            if (productDataCount() > 0) {
                searchProductMetaDataFromStore();
            }
        }
        onIapBackendDataReadError: {
            console.log("onIapBackendDataReadError");
            messageProgress.hideOngoingMessage();
            message.showErrorMessage("Failed to read product information", 5000);
            container.error();
        }
        onProductDataRead: {
            console.log("onProductDataRead: " + product.title);
            messageProgress.hideOngoingMessage();
            setProductDataToModel(product);

            if (listModel.count > 0) {
                container.state = "levelsShown";
            }
        }
        onProductDataReadError: {
            console.log("onProductDataReadError");
            messageProgress.hideOngoingMessage();
            message.showErrorMessage("Failed to read product information", 5000);
            container.error();
        }
        onProductDoesNotExists: {
            console.log("onProductDoesNotExists: " + productId);
            messageProgress.hideOngoingMessage();
            message.showErrorMessage("Product does not exist", 5000);
            container.error();
        }
        onProductPurchaseDone: {
            console.log("onProductPurchaseDone: " + productId);
            messageProgress.hideOngoingMessage();
            message.showMessage("Product purchased", 5000);
        }
        onProductPurchaseError: {
            console.log("onProductPurchaseError");
            messageProgress.hideOngoingMessage();
            message.showErrorMessage("Purchase cancelled", 5000);
            container.error();
        }
        onDownloadingProduct: {
            console.log("onDownloadingProduct: " + productId);
            messageProgress.showMessageOngoing("Downloading...");
        }
        onDownloadProgress: {
            messageProgress.showMessageProgress(bytesReceived, bytesTotal);
        }
        onDownloaded: {
            levelDownloaded = true;
            console.log("onDownloaded");
            messageProgress.hideOngoingMessage();
        }
        onDownloadingError: {
            levelDownloaded = false;
            console.log("onDownloadingError");
            messageProgress.hideOngoingMessage();
            message.showErrorMessage("Failed to download product", 5000);
            container.error();
        }
        onInstallingProduct: {
            console.log("onInstallingProduct");
            messageProgress.hideOngoingMessage();
        }
        onBusy: {
            console.log("onBusy");
            messageProgress.hideOngoingMessage();
            message.showErrorMessage("In-App Payment is busy", 5000);
        }
    }

    Component {
        id: listDelegate

        Item {
            id: listItem
            height: 180
            width: listView.width
            property bool isValid: valid

            Rectangle {
                radius: 4
                border.color: itemMouseArea.pressed ? "red": "white"
                border.width: itemMouseArea.pressed ? 4: 2
                anchors.fill: parent
                color: "transparent"

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 0.7; color: "black" }
                    GradientStop { position: 0.9; color: "black" }
                    GradientStop { position: 1.0; color: "#ee01e6" }
                }
            }

            Row {
                anchors.fill: listItem.paddingItem
                anchors.verticalCenter: parent.verticalCenter
                Image {
                    smooth: true
                    source: thumbnail
                    fillMode: Image.PreserveAspectFit
                    height: listItem.height - 25
                    width: listItem.height - 25
                }
                Column {
                    id: column
                    anchors.verticalCenter: parent.verticalCenter
                    Image {
                        smooth: true
                        source: "qrc:/gfx/shopping_cart.png"
                        fillMode: Image.PreserveAspectFit
                    }
                    Text {
                        smooth: true
                        font.pixelSize: 18
                        color: "white"
                        text: displayName
                        width: column.width * 0.5
                        wrapMode: Text.WordWrap
                    }
                    Text {
                        smooth: true
                        font.pixelSize: 18
                        color: "white"
                        text: isValid ? "Buy " + price : ""
                        wrapMode: Text.WordWrap
                    }
                }
            }

            MouseArea {
                id: itemMouseArea
                anchors.fill: parent

                onClicked: {
                    var currentItem = listModel.get(index);
                    if (currentItem.valid) {
                        iap.buyProduct(currentItem.iapId);
                    }
                    else {
                        message.showErrorMessage("No products", 4000);
                    }
                }
            }
        }
    }

    ListModel {
        id: listModel
    }

    ListView {
        id: listView
        anchors.centerIn: parent
        width: parent.width * 0.9
        height: parent.height * 0.9
        clip: true
        delegate: listDelegate
        model: listModel
        spacing: 15
    }

    Rectangle {
        id: scrollbar
        clip: true
        anchors.left: listView.right
        anchors.margins: 5
        opacity: 0.4
        y: listView.visibleArea.yPosition * listView.height  + listView.y
        width: 5
        height: listView.visibleArea.heightRatio * listView.height
        color: "gray"
    }

    states: [
        State {
            name: "hidden"
        },
        State {
            name: "levelsShown"
        }
    ]

    transitions: [
        Transition {
            from: "*"; to: "hidden"
            ParallelAnimation {
                NumberAnimation { target: container; property: "x"; to: container.width * -0.8;
                                  duration: 500; easing.type: Easing.InOutBack }
                PropertyAnimation { target: container; property: "opacity"; to: 0; duration:500 }
            }
        },
        Transition {
            from: "*"; to: "levelsShown"
            SequentialAnimation {
                SequentialAnimation {
                    PropertyAction { target: container; property: "x"; value: container.width * -1.2 }
                    PropertyAction { target: container; property: "y";
                                     value: (gameArea.height - container.height) *0.7 }
                    PropertyAction { target: container; property: "opacity"; value: 1 }
                    NumberAnimation { target: container; property: "x";
                                      to: (gameArea.width - container.width) *0.5;
                                      duration: 500; easing.type: Easing.InOutBack }
                }
            }
        }
    ]
}
