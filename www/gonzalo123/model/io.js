sap.ui.define([], function () {
    "use strict";

    var socket,
        status;

    return {
        disconnect: function (onDisconnection) {
            if (socket) {
                console.log('socket.disconnect');
                socket.disconnect();
                socket = undefined;
                if (onDisconnection) onDisconnection();
            }
        },
        connect: function (uri, onConnection) {
            if (!socket) {
                console.log('socket.connect', uri);
                socket = io(uri, {query: '_=' + (new Date()).valueOf(), 'transports': ['polling', 'websocket', 'htmlfile', 'xhr-polling']});
                socket.on('connect', function () {
                    status = "Conected";
                    if (onConnection) onConnection();
                });

                socket.on('connect_error', function (e) {
                    status = "Connect error: " + JSON.stringify(e);
                    console.log('webssocket connect_error:' + JSON.stringify(e));
                });

                socket.on('disconnect', function () {
                    status = "disconnect ";
                    console.log('webssocket disconnect');
                });
            }
        },

        removeAllListeners: function () {
            socket.removeAllListeners();
        },

        on: function (eventName, callback) {
            socket.on(eventName, function (message) {
                callback(message);
            });
        },

        emit: function (eventName, data, callback) {
            socket.emit(eventName, data, function () {
                if (callback) {
                    callback.apply(socket, arguments);
                }
            });
        }
    };
});
